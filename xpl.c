#include "xpl.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

static inline uint32_t xpl_pad(uint32_t length)
{
	uint32_t n = length % 4;
	if (!n)
		return 0;
	return 4 - n;
}

struct xpl_conn *xpl_conn_new(const char *display_name)
{
	if (!display_name)
	{
		display_name = getenv("DISPLAY");
		if (!display_name)
			return NULL;
	}
	const char *semi = strchr(display_name, ':');
	if (!semi)
		return NULL;
	if (strchr(semi, '/'))
		return NULL;
	struct sockaddr_un sun;
	struct xpl_conn *conn = malloc(sizeof(*conn));
	if (!conn)
		return NULL;
	conn->setup = NULL;
	conn->fd = -1;
	buf_init(&conn->wbuf);
	buf_init(&conn->rbuf);
	if (!buf_reserve(&conn->rbuf, 4096 * 4))
		goto err;
	conn->fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (conn->fd < 0)
		goto err;
	sun.sun_family = AF_UNIX;
	snprintf(sun.sun_path, sizeof(sun.sun_path), "/tmp/.X11-unix/X%s", semi + 1);
	if (connect(conn->fd, (struct sockaddr*)&sun, sizeof(sun)) == -1)
		goto err;
	return conn;

err:
	xpl_conn_delete(conn);
	return NULL;
}

static void setup_free(struct xpl_setup *setup)
{
	if (!setup)
		return;
	if (setup->screens)
	{
		for (size_t i = 0; i < setup->screens_count; ++i)
		{
			struct xpl_screen *screen = &setup->screens[i];
			if (screen->depths)
			{
				for (size_t i = 0; i < screen->depths_count; ++i)
					free(screen->depths[i].visualtypes);
				free(screen->depths);
			}
		}
		free(setup->screens);
	}
	free(setup->formats);
	free(setup->vendor);
	free(setup);
}

void xpl_conn_delete(struct xpl_conn *conn)
{
	if (!conn)
		return;
	setup_free(conn->setup);
	close(conn->fd);
	buf_destroy(&conn->rbuf);
	buf_destroy(&conn->wbuf);
	free(conn);
}

uint32_t xplc_generate_id(struct xpl_conn *conn)
{
	/* XXX implement bitmap ? */
	return conn->res_id++;
}

int xpl_send(struct xpl_conn *conn)
{
	return buf_send(&conn->wbuf, conn->fd);
}

int xpl_recv(struct xpl_conn *conn)
{
	int ret = buf_recv(&conn->rbuf, conn->fd);
	if (ret > 0)
	{
		for (int i = 0; i < ret; i += 4)
			printf("%08x\n", *(uint32_t*)&conn->rbuf.data[conn->rbuf.pos + i]);
	}
	return ret;
}

int xpl_flush(struct xpl_conn *conn)
{
	while (buf_remaining(&conn->wbuf))
	{
		int ret = xpl_send(conn);
		printf("send %d\n", ret);
		if (ret == -1)
		{
			if (errno == EAGAIN || errno == EINTR)
				continue;
			return 0;
		}
	}
	return 1;
}

int xplc_conn_init(struct xpl_conn *conn, int8_t endian, uint16_t major,
                   uint16_t minor, const char *protoname,
                   const char *protodata)
{
	size_t protonamelen = strlen(protoname);
	size_t protodatalen = strlen(protodata);
	return buf_wi8(&conn->wbuf, endian)
	    && buf_wi8(&conn->wbuf, 0)
	    && buf_wu16(&conn->wbuf, major)
	    && buf_wu16(&conn->wbuf, minor)
	    && buf_wu16(&conn->wbuf, protonamelen)
	    && buf_wu16(&conn->wbuf, protodatalen)
	    && buf_wu16(&conn->wbuf, 0)
	    && buf_wstr(&conn->wbuf, protoname, protonamelen)
	    && buf_wpad(&conn->wbuf)
	    && buf_wstr(&conn->wbuf, protodata, protodatalen)
	    && buf_wpad(&conn->wbuf);
}

enum xpl_status xplc_conn_init_reply(struct xpl_conn *conn)
{
	uint8_t err;
	uint16_t major;
	uint16_t minor;
	uint16_t length;
	uint16_t vendor_length;
	struct xpl_setup *setup = NULL;
	if (!buf_ru8(&conn->rbuf, &err))
		return XPL_AGAIN;
	switch (err)
	{
		case 0:
			printf("failed\n");
			return XPL_OK;
		case 2:
			printf("authenticate\n");
			return XPL_OK;
		default:
			printf("unknown: %d\n", err);
			return XPL_OK;
		case 1:
			break;
	}
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !buf_ru16(&conn->rbuf, &major)
	 || !buf_ru16(&conn->rbuf, &minor)
	 || !buf_ru16(&conn->rbuf, &length))
		return XPL_INTERNAL;
	if (length * 4 > buf_remaining(&conn->rbuf))
	{
		conn->rbuf.pos -= 8;
		return XPL_AGAIN;
	}
	setup = calloc(sizeof(*setup), 1);
	if (!setup)
		return XPL_INTERNAL;
	if (!buf_ru32(&conn->rbuf, &setup->release_number)
	 || !buf_ru32(&conn->rbuf, &setup->resource_id_base)
	 || !buf_ru32(&conn->rbuf, &setup->resource_id_mask)
	 || !buf_ru32(&conn->rbuf, &setup->motion_buffer_size)
	 || !buf_ru16(&conn->rbuf, &vendor_length)
	 || !buf_ru16(&conn->rbuf, &setup->maximum_request_length)
	 || !buf_ru8(&conn->rbuf, &setup->screens_count)
	 || !buf_ru8(&conn->rbuf, &setup->formats_count)
	 || !buf_ru8(&conn->rbuf, &setup->image_byte_order)
	 || !buf_ru8(&conn->rbuf, &setup->bitmap_bit_order)
	 || !buf_ru8(&conn->rbuf, &setup->bitmap_scanline_unit)
	 || !buf_ru8(&conn->rbuf, &setup->bitmap_scanline_pad)
	 || !buf_ru8(&conn->rbuf, &setup->min_keycode)
	 || !buf_ru8(&conn->rbuf, &setup->max_keycode)
	 || !buf_ru32(&conn->rbuf, NULL))
		goto err;
	setup->vendor = malloc(vendor_length + 1);
	if (!setup->vendor)
		goto err;
	if (!buf_read(&conn->rbuf, setup->vendor, vendor_length))
		goto err;
	setup->vendor[vendor_length] = 0;
	if (!buf_read(&conn->rbuf, NULL, xpl_pad(vendor_length)))
		goto err;
	setup->formats = malloc(sizeof(*setup->formats) * setup->formats_count);
	if (!setup->formats)
		goto err;
	for (size_t i = 0; i < setup->formats_count; ++i)
	{
		struct xpl_format *format = &setup->formats[i];
		if (!buf_ru8(&conn->rbuf, &format->depth)
		 || !buf_ru8(&conn->rbuf, &format->bpp)
		 || !buf_ru8(&conn->rbuf, &format->scanline_pad)
		 || !buf_read(&conn->rbuf, NULL, 5))
			goto err;
	}
	setup->screens = calloc(sizeof(*setup->screens) * setup->screens_count, 1);
	if (!setup->screens)
		goto err;
	for (size_t i = 0; i < setup->screens_count; ++i)
	{
		struct xpl_screen *screen = &setup->screens[i];
		if (!buf_ru32(&conn->rbuf, &screen->root)
		 || !buf_ru32(&conn->rbuf, &screen->default_colormap)
		 || !buf_ru32(&conn->rbuf, &screen->white_pixel)
		 || !buf_ru32(&conn->rbuf, &screen->black_pixel)
		 || !buf_ru32(&conn->rbuf, &screen->current_input_mask)
		 || !buf_ru16(&conn->rbuf, &screen->width_pixels)
		 || !buf_ru16(&conn->rbuf, &screen->height_pixels)
		 || !buf_ru16(&conn->rbuf, &screen->width_millimeters)
		 || !buf_ru16(&conn->rbuf, &screen->height_millimeters)
		 || !buf_ru16(&conn->rbuf, &screen->min_installed_maps)
		 || !buf_ru16(&conn->rbuf, &screen->max_installed_maps)
		 || !buf_ru32(&conn->rbuf, &screen->root_visual)
		 || !buf_ru8(&conn->rbuf, &screen->backing_stores)
		 || !buf_ru8(&conn->rbuf, &screen->save_unders)
		 || !buf_ru8(&conn->rbuf, &screen->root_depth)
		 || !buf_ru8(&conn->rbuf, &screen->depths_count))
			goto err;
		screen->depths = calloc(sizeof(*screen->depths) * screen->depths_count, 1);
		if (!screen->depths)
			goto err;
		for (size_t j = 0; j < screen->depths_count; ++j)
		{
			struct xpl_depth *depth = &screen->depths[j];
			if (!buf_ru8(&conn->rbuf, &depth->depth)
			 || !buf_ru8(&conn->rbuf, NULL)
			 || !buf_ru16(&conn->rbuf, &depth->visualtypes_count)
			 || !buf_ru32(&conn->rbuf, NULL))
				goto err;
			depth->visualtypes = malloc(sizeof(*depth->visualtypes)
			                          * depth->visualtypes_count);
			if (!depth->visualtypes)
				goto err;
			for (size_t k = 0; k < depth->visualtypes_count; ++k)
			{
				struct xpl_visualtype *visualtype = &depth->visualtypes[k];
				if (!buf_ru32(&conn->rbuf, &visualtype->visual_id)
				 || !buf_ru8(&conn->rbuf, &visualtype->class)
				 || !buf_ru8(&conn->rbuf, &visualtype->bits_per_rgb_value)
				 || !buf_ru16(&conn->rbuf, &visualtype->colormap_entries)
				 || !buf_ru32(&conn->rbuf, &visualtype->red_mask)
				 || !buf_ru32(&conn->rbuf, &visualtype->green_mask)
				 || !buf_ru32(&conn->rbuf, &visualtype->blue_mask)
				 || !buf_ru32(&conn->rbuf, NULL))
					goto err;
			}
		}
	}
	conn->setup = setup;
	conn->res_id = conn->setup->resource_id_base;
	return XPL_OK;

err:
	setup_free(setup);
	return XPL_INTERNAL;
}

static enum xpl_status xplc_parse_err(struct xpl_conn *conn,
                                      struct xpl_err *err)
{
	if (buf_remaining(&conn->rbuf) < 31)
	{
		conn->rbuf.pos--;
		return XPL_AGAIN;
	}
	uint8_t code;
	if (!buf_ru8(&conn->rbuf, &code)
	 || !buf_ru16(&conn->rbuf, &err->sequence_number)
	 || !buf_ru32(&conn->rbuf, &err->bad_id)
	 || !buf_ru16(&conn->rbuf, &err->minor_opcode)
	 || !buf_ru8(&conn->rbuf, &err->major_opcode)
	 || !buf_read(&conn->rbuf, NULL, 21))
		return XPL_INTERNAL;
	err->code = code;
	return XPL_ERR;
}

#define EVENT_RI8(n)  buf_ri8(&conn->rbuf, &e->n)
#define EVENT_RU8(n)  buf_ru8(&conn->rbuf, &e->n)
#define EVENT_RI16(n) buf_ri16(&conn->rbuf, &e->n)
#define EVENT_RU16(n) buf_ru16(&conn->rbuf, &e->n)
#define EVENT_RI32(n) buf_ri32(&conn->rbuf, &e->n)
#define EVENT_RU32(n) buf_ru32(&conn->rbuf, &e->n)

static enum xpl_status parse_event_key_press(struct xpl_conn *conn,
                                             struct xpl_event *event)
{
	struct xpl_event_key_press *e = &event->key_press;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(time)
	 || !EVENT_RU32(root)
	 || !EVENT_RU32(event)
	 || !EVENT_RU32(child)
	 || !EVENT_RI16(root_x)
	 || !EVENT_RI16(root_y)
	 || !EVENT_RI16(event_x)
	 || !EVENT_RI16(event_y)
	 || !EVENT_RU16(state)
	 || !EVENT_RU8(same_screen)
	 || !buf_read(&conn->rbuf, NULL, 1))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_key_release(struct xpl_conn *conn,
                                               struct xpl_event *event)
{
	struct xpl_event_key_release *e = &event->key_release;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(time)
	 || !EVENT_RU32(root)
	 || !EVENT_RU32(event)
	 || !EVENT_RU32(child)
	 || !EVENT_RI16(root_x)
	 || !EVENT_RI16(root_y)
	 || !EVENT_RI16(event_x)
	 || !EVENT_RI16(event_y)
	 || !EVENT_RU16(state)
	 || !EVENT_RU8(same_screen)
	 || !buf_read(&conn->rbuf, NULL, 1))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_button_press(struct xpl_conn *conn,
                                                struct xpl_event *event)
{
	struct xpl_event_button_press *e = &event->button_press;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(time)
	 || !EVENT_RU32(root)
	 || !EVENT_RU32(event)
	 || !EVENT_RU32(child)
	 || !EVENT_RI16(root_x)
	 || !EVENT_RI16(root_y)
	 || !EVENT_RI16(event_x)
	 || !EVENT_RI16(event_y)
	 || !EVENT_RU16(state)
	 || !EVENT_RU8(same_screen)
	 || !buf_read(&conn->rbuf, NULL, 1))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_button_release(struct xpl_conn *conn,
                                                  struct xpl_event *event)
{
	struct xpl_event_button_release *e = &event->button_release;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(time)
	 || !EVENT_RU32(root)
	 || !EVENT_RU32(event)
	 || !EVENT_RU32(child)
	 || !EVENT_RI16(root_x)
	 || !EVENT_RI16(root_y)
	 || !EVENT_RI16(event_x)
	 || !EVENT_RI16(event_y)
	 || !EVENT_RU16(state)
	 || !EVENT_RU8(same_screen)
	 || !buf_read(&conn->rbuf, NULL, 1))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_motion_notify(struct xpl_conn *conn,
                                                 struct xpl_event *event)
{
	struct xpl_event_motion_notify *e = &event->motion_notify;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(time)
	 || !EVENT_RU32(root)
	 || !EVENT_RU32(event)
	 || !EVENT_RU32(child)
	 || !EVENT_RU16(root_x)
	 || !EVENT_RU16(root_y)
	 || !EVENT_RU16(event_x)
	 || !EVENT_RU16(event_y)
	 || !EVENT_RU16(state)
	 || !EVENT_RU8(same_screen)
	 || !buf_read(&conn->rbuf, NULL, 1))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_enter_notify(struct xpl_conn *conn,
                                                struct xpl_event *event)
{
	struct xpl_event_enter_notify *e = &event->enter_notify;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(time)
	 || !EVENT_RU32(root)
	 || !EVENT_RU32(event)
	 || !EVENT_RU32(child)
	 || !EVENT_RI16(root_x)
	 || !EVENT_RI16(root_y)
	 || !EVENT_RI16(event_x)
	 || !EVENT_RI16(event_y)
	 || !EVENT_RU16(state)
	 || !EVENT_RU8(mode)
	 || !EVENT_RU8(same_screen_focus))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_leave_notify(struct xpl_conn *conn,
                                                struct xpl_event *event)
{
	struct xpl_event_leave_notify *e = &event->leave_notify;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(time)
	 || !EVENT_RU32(root)
	 || !EVENT_RU32(event)
	 || !EVENT_RU32(child)
	 || !EVENT_RI16(root_x)
	 || !EVENT_RI16(root_y)
	 || !EVENT_RI16(event_x)
	 || !EVENT_RI16(event_y)
	 || !EVENT_RU16(state)
	 || !EVENT_RU8(mode)
	 || !EVENT_RU8(same_screen_focus))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_focus_in(struct xpl_conn *conn,
                                            struct xpl_event *event)
{
	struct xpl_event_focus_in *e = &event->focus_in;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(event)
	 || !EVENT_RU8(mode)
	 || !buf_read(&conn->rbuf, NULL, 23))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_focus_out(struct xpl_conn *conn,
                                             struct xpl_event *event)
{
	struct xpl_event_focus_out *e = &event->focus_out;
	if (!EVENT_RU8(detail)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(event)
	 || !EVENT_RU8(mode)
	 || !buf_read(&conn->rbuf, NULL, 23))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_keymap_notify(struct xpl_conn *conn,
                                                 struct xpl_event *event)
{
	struct xpl_event_keymap_notify *e = &event->keymap_notify;
	if (!buf_read(&conn->rbuf, &e->keys, 31))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_expose(struct xpl_conn *conn,
                                          struct xpl_event *event)
{
	struct xpl_event_expose *e = &event->expose;
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(window)
	 || !EVENT_RU16(x)
	 || !EVENT_RU16(y)
	 || !EVENT_RU16(width)
	 || !EVENT_RU16(height)
	 || !EVENT_RU16(count)
	 || !buf_read(&conn->rbuf, NULL, 14))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_graphics_exposure(struct xpl_conn *conn,
                                                     struct xpl_event *event)
{
	struct xpl_event_graphics_exposure *e = &event->graphics_exposure;
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(drawable)
	 || !EVENT_RU16(x)
	 || !EVENT_RU16(y)
	 || !EVENT_RU16(width)
	 || !EVENT_RU16(height)
	 || !EVENT_RU16(minor_opcode)
	 || !EVENT_RU16(count)
	 || !EVENT_RU8(major_opcode)
	 || !buf_read(&conn->rbuf, NULL, 11))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_no_exposure(struct xpl_conn *conn,
                                               struct xpl_event *event)
{
	struct xpl_event_no_exposure *e = &event->no_exposure;
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(drawable)
	 || !EVENT_RU16(minor_opcode)
	 || !EVENT_RU8(major_opcode)
	 || !buf_read(&conn->rbuf, NULL, 21))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_visibility_notify(struct xpl_conn *conn,
                                                     struct xpl_event *event)
{
	struct xpl_event_visibility_notify *e = &event->visibility_notify;
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(window)
	 || !EVENT_RU8(state)
	 || !buf_read(&conn->rbuf, NULL, 23))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_create_notify(struct xpl_conn *conn,
                                                 struct xpl_event *event)
{
	struct xpl_event_create_notify *e = &event->create_notify;
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(parent)
	 || !EVENT_RU32(window)
	 || !EVENT_RI16(x)
	 || !EVENT_RI16(y)
	 || !EVENT_RU16(width)
	 || !EVENT_RU16(height)
	 || !EVENT_RU16(border_width)
	 || !EVENT_RU8(override_redirect)
	 || !buf_read(&conn->rbuf, NULL, 9))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_configure_notify(struct xpl_conn *conn,
                                                    struct xpl_event *event)
{
	struct xpl_event_configure_notify *e = &event->configure_notify;
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(event)
	 || !EVENT_RU32(window)
	 || !EVENT_RU32(above_sibling)
	 || !EVENT_RI16(x)
	 || !EVENT_RI16(y)
	 || !EVENT_RU16(width)
	 || !EVENT_RU16(height)
	 || !EVENT_RU16(border_width)
	 || !EVENT_RU8(override_redirect)
	 || !buf_read(&conn->rbuf, NULL, 5))
		return XPL_INTERNAL;
	return XPL_OK;
}

static enum xpl_status parse_event_property_notify(struct xpl_conn *conn,
                                                   struct xpl_event *event)
{
	struct xpl_event_property_notify *e = &event->property_notify;
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !EVENT_RU16(sequence_number)
	 || !EVENT_RU32(window)
	 || !EVENT_RU32(atom)
	 || !EVENT_RU32(time)
	 || !EVENT_RU8(state)
	 || !buf_read(&conn->rbuf, NULL, 15))
		return XPL_INTERNAL;
	return XPL_OK;
}

#undef EVENT_RI8
#undef EVENT_RU8
#undef EVENT_RI16
#undef EVENT_RU16
#undef EVENT_RI32
#undef EVENT_RU32

static const enum xpl_status (*parse_fns[])(struct xpl_conn *conn,
                                            struct xpl_event *event) =
{
	[XPL_EVENT_KEY_PRESS] = parse_event_key_press,
	[XPL_EVENT_KEY_RELEASE] = parse_event_key_release,
	[XPL_EVENT_BUTTON_PRESS] = parse_event_button_press,
	[XPL_EVENT_BUTTON_RELEASE] = parse_event_button_release,
	[XPL_EVENT_MOTION_NOTIFY] = parse_event_motion_notify,
	[XPL_EVENT_ENTER_NOTIFY] = parse_event_enter_notify,
	[XPL_EVENT_LEAVE_NOTIFY] = parse_event_leave_notify,
	[XPL_EVENT_FOCUS_IN] = parse_event_focus_in,
	[XPL_EVENT_FOCUS_OUT] = parse_event_focus_out,
	[XPL_EVENT_KEYMAP_NOTIFY] = parse_event_keymap_notify,
	[XPL_EVENT_EXPOSE] = parse_event_expose,
	[XPL_EVENT_GRAPHICS_EXPOSURE] = parse_event_graphics_exposure,
	[XPL_EVENT_NO_EXPOSURE] = parse_event_no_exposure,
	[XPL_EVENT_VISIBILITY_NOTIFY] = parse_event_visibility_notify,
	[XPL_EVENT_CREATE_NOTIFY] = parse_event_create_notify,

	[XPL_EVENT_CONFIGURE_NOTIFY] = parse_event_configure_notify,

	[XPL_EVENT_PROPERTY_NOTIFY] = parse_event_property_notify,
};

static enum xpl_status xplc_parse_event(struct xpl_conn *conn,
                                        uint8_t code)
{
	if (buf_remaining(&conn->rbuf) < 31)
	{
		conn->rbuf.pos--;
		return XPL_AGAIN;
	}
	if (code >= sizeof(parse_fns) / sizeof(*parse_fns) || !parse_fns[code])
		return XPL_INTERNAL;
	struct xpl_event_queue *evt = malloc(sizeof(*evt));
	if (!evt)
		return XPL_INTERNAL;
	evt->event.type = code;
	enum xpl_status status = parse_fns[code](conn, &evt->event);
	if (status != XPL_OK)
	{
		free(evt);
		return status;
	}

	return XPL_AGAIN;
}

int xplc_create_window(struct xpl_conn *conn, uint8_t depth, xpl_window_t id,
                       xpl_window_t parent, int16_t x, int16_t y,
                       uint16_t width, uint16_t height, uint16_t border_width,
                       uint16_t class_type, xpl_visualid_t visual,
                       uint32_t value_mask, uint32_t *values)
{
	size_t n = 0;
	for (size_t i = 0; i < 15; ++i)
	{
		if (value_mask & (1 << i))
			n++;
	}
	if (!buf_wi8(&conn->wbuf, XPL_REQUEST_CREATE_WINDOW)
	 || !buf_wu8(&conn->wbuf, depth)
	 || !buf_wu16(&conn->wbuf, 8 + n)
	 || !buf_wu32(&conn->wbuf, id)
	 || !buf_wu32(&conn->wbuf, parent)
	 || !buf_wi16(&conn->wbuf, x)
	 || !buf_wi16(&conn->wbuf, y)
	 || !buf_wu16(&conn->wbuf, width)
	 || !buf_wu16(&conn->wbuf, height)
	 || !buf_wu16(&conn->wbuf, border_width)
	 || !buf_wu16(&conn->wbuf, class_type)
	 || !buf_wu32(&conn->wbuf, visual)
	 || !buf_wu32(&conn->wbuf, value_mask))
		return 0;
	n = 0;
	for (size_t i = 0; i < 15; ++i)
	{
		if (!(value_mask & (1 << i)))
			continue;
		if (!buf_wu32(&conn->wbuf, values[n]))
			return 0;
	}
	return 1;
}

int xplc_map_window(struct xpl_conn *conn, xpl_window_t window)
{
	return buf_wu8(&conn->wbuf, XPL_REQUEST_MAP_WINDOW)
	    && buf_wu8(&conn->wbuf, 0)
	    && buf_wu16(&conn->wbuf, 2)
	    && buf_wu32(&conn->wbuf, window);
}

int xplc_unmap_window(struct xpl_conn *conn, xpl_window_t window)
{
	return buf_wu8(&conn->wbuf, XPL_REQUEST_UNMAP_WINDOW)
	    && buf_wu8(&conn->wbuf, 0)
	    && buf_wu16(&conn->wbuf, 2)
	    && buf_wu32(&conn->wbuf, window);
}

int xplc_intern_atom(struct xpl_conn *conn, uint8_t only_if_exists,
                     const char *name)
{
	size_t name_len = strlen(name);
	return buf_wu8(&conn->wbuf, XPL_REQUEST_INTERN_ATOM)
	    && buf_wu8(&conn->wbuf, only_if_exists)
	    && buf_wu16(&conn->wbuf, 2 + (name_len + 3) / 4)
	    && buf_wu16(&conn->wbuf, name_len)
	    && buf_wu16(&conn->wbuf, 0)
	    && buf_write(&conn->wbuf, name, name_len)
	    && buf_wpad(&conn->wbuf);
}

enum xpl_status xplc_intern_atom_reply(struct xpl_conn *conn,
                                       xpl_atom_t *atom,
                                       struct xpl_err *err)
{
	if (buf_remaining(&conn->rbuf) < 32)
		return XPL_AGAIN;
	uint8_t err_code;
	if (!buf_ru8(&conn->rbuf, &err_code))
		return XPL_INTERNAL;
	if (!err_code)
		return xplc_parse_err(conn, err);
	if (err_code != 1)
		return xplc_parse_event(conn, err_code);
	if (!buf_ru8(&conn->rbuf, NULL)
	 || !buf_ru16(&conn->rbuf, NULL)
	 || !buf_ru32(&conn->rbuf, NULL)
	 || !buf_ru32(&conn->rbuf, atom)
	 || !buf_read(&conn->rbuf, NULL, 20))
		return XPL_INTERNAL;
	return XPL_OK;
}

int xplc_change_property(struct xpl_conn *conn, enum xpl_property_mode mode,
                         xpl_window_t window, xpl_atom_t property,
                         xpl_atom_t type, uint8_t format, uint32_t length,
                         const void *data)
{
	uint32_t l = format / 8;
	if (length % l)
		return 0;
	uint32_t p = xpl_pad(length);
	return buf_wu8(&conn->wbuf, XPL_REQUEST_CHANGE_PROPERTY)
	    && buf_wu8(&conn->wbuf, mode)
	    && buf_wu16(&conn->wbuf, 6 + (length + p) / 4)
	    && buf_wu32(&conn->wbuf, window)
	    && buf_wu32(&conn->wbuf, property)
	    && buf_wu32(&conn->wbuf, type)
	    && buf_wu8(&conn->wbuf, format)
	    && buf_write(&conn->wbuf, NULL, 3)
	    && buf_wu32(&conn->wbuf, length / l)
	    && buf_write(&conn->wbuf, data, length)
	    && buf_write(&conn->wbuf, NULL, p);
}
