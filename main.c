#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include "xpl.h"
#include "buf.h"

enum parse_status
{
	PARSE_OK,
	PARSE_ERR,
	PARSE_AGAIN,
};

static int conn_init(struct buf *buf, int8_t endian, uint16_t major, uint16_t minor, const char *protoname, const char *protodata)
{
	size_t protonamelen = strlen(protoname);
	size_t protodatalen = strlen(protodata);
	return buf_wi8(buf, endian)
	    && buf_wi8(buf, 0)
	    && buf_wu16(buf, major)
	    && buf_wu16(buf, minor)
	    && buf_wu16(buf, protonamelen)
	    && buf_wu16(buf, protodatalen)
	    && buf_wu16(buf, 0)
	    && buf_wstr(buf, protoname, protonamelen)
	    && buf_wpad(buf)
	    && buf_wstr(buf, protodata, protodatalen)
	    && buf_wpad(buf);
}

static enum parse_status parse_setup(struct buf *buf, struct xpl_setup **setupp)
{
	uint8_t err;
	uint16_t major;
	uint16_t minor;
	uint16_t length;
	uint16_t vendor_length;
	struct xpl_setup *setup = NULL;
	if (!buf_ru8(buf, &err))
		return 0;
	switch (err)
	{
		case 0:
			printf("failed\n");
			return PARSE_OK;
		case 2:
			printf("authenticate\n");
			return PARSE_OK;
		default:
			printf("unknown: %d\n", err);
			return PARSE_OK;
		case 1:
			break;
	}
	if (!buf_ru8(buf, NULL)
	 || !buf_ru16(buf, &major)
	 || !buf_ru16(buf, &minor)
	 || !buf_ru16(buf, &length))
	{
		fprintf(stderr, "buf read failed\n");
		return PARSE_ERR;
	}
	if (length * 4 > buf_remaining(buf))
	{
		buf->pos -= 8;
		return PARSE_AGAIN;
	}
	setup = calloc(sizeof(*setup), 1);
	if (!setup)
	{
		perror("malloc");
		return PARSE_ERR;
	}
	if (!buf_ru32(buf, &setup->release_number)
	 || !buf_ru32(buf, &setup->resource_id_base)
	 || !buf_ru32(buf, &setup->resource_id_mask)
	 || !buf_ru32(buf, &setup->motion_buffer_size)
	 || !buf_ru16(buf, &vendor_length)
	 || !buf_ru16(buf, &setup->maximum_request_length)
	 || !buf_ru8(buf, &setup->screens_count)
	 || !buf_ru8(buf, &setup->formats_count)
	 || !buf_ru8(buf, &setup->image_byte_order)
	 || !buf_ru8(buf, &setup->bitmap_bit_order)
	 || !buf_ru8(buf, &setup->bitmap_scanline_unit)
	 || !buf_ru8(buf, &setup->bitmap_scanline_pad)
	 || !buf_ru8(buf, &setup->min_keycode)
	 || !buf_ru8(buf, &setup->max_keycode)
	 || !buf_ru32(buf, NULL))
	{
		fprintf(stderr, "failed to read\n");
		goto err;
	}
	setup->vendor = malloc(vendor_length + 1);
	if (!setup->vendor)
	{
		perror("malloc");
		goto err;
	}
	if (!buf_read(buf, setup->vendor, vendor_length))
	{
		fprintf(stderr, "vendor read failed\n");
		goto err;
	}
	setup->vendor[vendor_length] = 0;
	if (!buf_read(buf, NULL, xpl_pad(vendor_length)))
	{
		fprintf(stderr, "vendor pad read failed\n");
		goto err;
	}
	setup->formats = malloc(sizeof(*setup->formats) * setup->formats_count);
	if (!setup->formats)
	{
		perror("malloc");
		goto err;
	}
	for (size_t i = 0; i < setup->formats_count; ++i)
	{
		struct xpl_format *format = &setup->formats[i];
		if (!buf_ru8(buf, &format->depth)
		 || !buf_ru8(buf, &format->bpp)
		 || !buf_ru8(buf, &format->scanline_pad)
		 || !buf_read(buf, NULL, 5))
		{
			fprintf(stderr, "failed to read format\n");
			goto err;
		}
	}
	setup->screens = calloc(sizeof(*setup->screens) * setup->screens_count, 1);
	if (!setup->screens)
	{
		perror("malloc");
		goto err;
	}
	for (size_t i = 0; i < setup->screens_count; ++i)
	{
		struct xpl_screen *screen = &setup->screens[i];
		if (!buf_ru32(buf, &screen->root)
		 || !buf_ru32(buf, &screen->default_colormap)
		 || !buf_ru32(buf, &screen->white_pixel)
		 || !buf_ru32(buf, &screen->black_pixel)
		 || !buf_ru32(buf, &screen->current_input_mask)
		 || !buf_ru16(buf, &screen->width_pixels)
		 || !buf_ru16(buf, &screen->height_pixels)
		 || !buf_ru16(buf, &screen->width_millimeters)
		 || !buf_ru16(buf, &screen->height_millimeters)
		 || !buf_ru16(buf, &screen->min_installed_maps)
		 || !buf_ru16(buf, &screen->max_installed_maps)
		 || !buf_ru32(buf, &screen->root_visual)
		 || !buf_ru8(buf, &screen->backing_stores)
		 || !buf_ru8(buf, &screen->save_unders)
		 || !buf_ru8(buf, &screen->root_depth)
		 || !buf_ru8(buf, &screen->depths_count))
		{
			fprintf(stderr, "failed to read screen\n");
			goto err;
		}
		screen->depths = calloc(sizeof(*screen->depths) * screen->depths_count, 1);
		if (!screen->depths)
		{
			perror("malloc");
			goto err;
		}
		for (size_t j = 0; j < screen->depths_count; ++j)
		{
			struct xpl_depth *depth = &screen->depths[i];
			if (!buf_ru8(buf, &depth->depth)
			 || !buf_ru8(buf, NULL)
			 || !buf_ru16(buf, &depth->visualtypes_count)
			 || !buf_ru32(buf, NULL))
			{
				fprintf(stderr, "failed to read depth\n");
				goto err;
			}
			depth->visualtypes = malloc(sizeof(*depth->visualtypes) * depth->visualtypes_count);
			if (!depth->visualtypes)
			{
				perror("malloc");
				goto err;
			}
			for (size_t k = 0; k < depth->visualtypes_count; ++k)
			{
				struct xpl_visualtype *visualtype = &depth->visualtypes[k];
				if (!buf_ru32(buf, &visualtype->visual_id)
				 || !buf_ru8(buf, &visualtype->class_type)
				 || !buf_ru8(buf, &visualtype->bits_per_rgb_value)
				 || !buf_ru16(buf, &visualtype->colormap_entries)
				 || !buf_ru32(buf, &visualtype->red_mask)
				 || !buf_ru32(buf, &visualtype->green_mask)
				 || !buf_ru32(buf, &visualtype->blue_mask)
				 || !buf_ru32(buf, NULL))
				{
					fprintf(stderr, "failed to read visualtype\n");
					goto err;
				}
			}
		}
	}
	return PARSE_OK;

err:
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
	return PARSE_ERR;
}

static void print_setup(const struct xpl_setup *setup)
{
	printf("min keycode: %d\n", setup->min_keycode);
	printf("max keycode: %d\n", setup->max_keycode);
	printf("id base: %08x\n", setup->resource_id_base);
	printf("id mask: %08x\n", setup->resource_id_mask);
	printf("vendor: %s\n", setup->vendor);
	for (size_t i = 0; i < setup->screens_count; ++i)
	{
		struct xpl_screen *screen = &setup->screens[i];
		printf("screen:\n");
		printf("\troot: %u\n", screen->root);
		for (size_t j = 0; j < screen->depths_count; ++j)
		{
			struct xpl_depth *depth = &screen->depths[i];
			printf("\tdepth:\n");
			printf("\t\tdepth: %u\n", depth->depth);
			for (size_t k = 0; k < depth->visualtypes_count; ++k)
			{
				struct xpl_visualtype *visualtype = &depth->visualtypes[k];
				printf("\t\tvisualtype:\n");
				printf("\t\t\tvisual_id: %u\n", visualtype->visual_id);
				printf("\t\t\tclass_type: %u\n", visualtype->class_type);
				printf("\t\t\tbpr_value: %u\n", visualtype->bits_per_rgb_value);
				printf("\t\t\tred_mask: %x\n", visualtype->red_mask);
				printf("\t\t\tgreen_mask: %x\n", visualtype->green_mask);
				printf("\t\t\tblue_mask: %x\n", visualtype->blue_mask);
			}
		}
	}
}

int main()
{
	struct buf wbuf;
	struct buf rbuf;
	struct sockaddr_un sun;
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("socket");
		return EXIT_FAILURE;
	}
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, "/tmp/.X11-unix/X0");
	if (connect(fd, (struct sockaddr*)&sun, sizeof(sun)) == -1)
	{
		perror("connect");
		return EXIT_FAILURE;
	}
	buf_init(&wbuf);
	buf_init(&rbuf);
	buf_reserve(&rbuf, 4096 * 4);
	if (!conn_init(&wbuf, 'l', 11, 0, "", ""))
	{
		perror("conn_init");
		return EXIT_FAILURE;
	}
	if (buf_send(&wbuf, fd) < 0)
	{
		perror("send");
		return EXIT_FAILURE;
	}
	struct xpl_setup *setup;
	enum parse_status parse_status;
	do
	{
		int ret = buf_recv(&rbuf, fd);
		parse_status = parse_setup(&rbuf, &setup);
	} while (parse_status == PARSE_AGAIN);
	if (parse_status == PARSE_ERR)
	{
		fprintf(stderr, "failed to parse setup\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
