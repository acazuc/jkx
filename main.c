#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "xpl.h"

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

static int get_atom(struct xpl_conn *conn, const char *name, xpl_atom_t *atom)
{
	if (!xplc_intern_atom(conn, 0, name))
	{
		fprintf(stderr, "intern atom\n");
		return 0;
	}
	if (!xpl_flush(conn))
	{
		fprintf(stderr, "flush failed\n");
		return 0;
	}
	enum xpl_status parse_status;
	struct xpl_err xpl_err;
	do
	{
		int ret = xpl_recv(conn);
		printf("recv %d (%d)\n", ret, errno);
		parse_status = xplc_intern_atom_reply(conn, atom, &xpl_err);
	} while (parse_status == XPL_AGAIN);
	return parse_status == XPL_OK;
}

int main()
{
	struct xpl_conn *conn;
	conn = xpl_conn_new();
	if (!conn)
	{
		fprintf(stderr, "failed to create conn\n");
		return EXIT_FAILURE;
	}
	if (!xplc_conn_init(conn, 'l', 11, 0, "", ""))
	{
		fprintf(stderr, "conn_init failed\n");
		return EXIT_FAILURE;
	}
	if (!xpl_flush(conn))
	{
		fprintf(stderr, "flush failed\n");
		return EXIT_FAILURE;
	}
	enum xpl_status parse_status;
	do
	{
		int ret = xpl_recv(conn);
		printf("recv %d\n", ret);
		parse_status = xplc_conn_init_reply(conn);
	} while (parse_status == XPL_AGAIN);
	if (parse_status == XPL_ERR)
	{
		fprintf(stderr, "failed to parse setup\n");
		return EXIT_FAILURE;
	}
	print_setup(conn->setup);
	xpl_window_t window = xplc_generate_id(conn);
	struct xpl_screen *screen = &conn->setup->screens[0];
	xpl_window_t parent = screen->root;
	uint32_t mask = XPL_CREATE_WINDOW_BACKING_PIXEL
	              | XPL_CREATE_WINDOW_EVENT_MASK;
	uint32_t mask_values[2];
	mask_values[0] = screen->white_pixel;
	mask_values[1] = XPL_EVENT_MASK_EXPOSURE | XPL_EVENT_MASK_KEY_PRESS;
	if (!xplc_create_window(conn, 0, window, parent, 0, 0, 640, 480, 0, 1,
	                        screen->root_visual, mask, mask_values))
	{
		fprintf(stderr, "create window\n");
		return EXIT_FAILURE;
	}
	if (!xplc_map_window(conn, window))
	{
		fprintf(stderr, "map window\n");
		return EXIT_FAILURE;
	}
	if (!xpl_flush(conn))
	{
		fprintf(stderr, "flush failed\n");
		return EXIT_FAILURE;
	}
	do
	{
		int ret = xpl_recv(conn);
		printf("recv %d\n", ret);
	} while (buf_remaining(&conn->rbuf) < 32);
	uint32_t err;
	while (buf_ru32(&conn->rbuf, &err))
		printf("%08x\n", err);
	xpl_atom_t atom_net_wm_pid;
	xpl_atom_t atom_net_wm_name;
	xpl_atom_t atom_utf8_string;
	if (!get_atom(conn, "_NET_WM_PID", &atom_net_wm_pid))
	{
		fprintf(stderr, "failed to get pid\n");
		return EXIT_FAILURE;
	}
	printf("_NET_WM_PID: %u\n", atom_net_wm_pid);
	if (!get_atom(conn, "_NET_WM_NAME", &atom_net_wm_name))
	{
		fprintf(stderr, "failed to get name\n");
		return EXIT_FAILURE;
	}
	printf("_NET_WM_NAME: %u\n", atom_net_wm_name);
	if (!get_atom(conn, "UTF8_STRING", &atom_utf8_string))
	{
		fprintf(stderr, "failed to get utf8 string\n");
		return EXIT_FAILURE;
	}
	printf("UTF8_STRING: %u\n", atom_utf8_string);
	if (!xplc_change_property(conn, XPL_PROPERTY_REPLACE, window, atom_net_wm_name, atom_utf8_string, 8, 4, "test"))
	{
		fprintf(stderr, "change property\n");
		return EXIT_FAILURE;
	}
	if (!xpl_flush(conn))
	{
		fprintf(stderr, "flush failed\n");
		return EXIT_FAILURE;
	}
	while (1);
	return EXIT_SUCCESS;
}
