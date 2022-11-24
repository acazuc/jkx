#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "Xatom.h"
#include "Xutil.h"
#include "Xlib.h"
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
			struct xpl_depth *depth = &screen->depths[j];
			printf("\tdepth:\n");
			printf("\t\tdepth: %u\n", depth->depth);
			for (size_t k = 0; k < depth->visualtypes_count; ++k)
			{
				struct xpl_visualtype *visualtype = &depth->visualtypes[k];
				printf("\t\tvisualtype:\n");
				printf("\t\t\tvisual_id: %u\n", visualtype->visual_id);
				printf("\t\t\tclass: %u\n", visualtype->class);
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

static XVisualInfo *get_vi(Display *display)
{
	int nitems;
	long mask = VisualRedMaskMask | VisualGreenMaskMask | VisualBlueMaskMask
	          | VisualBitsPerRGBMask | VisualDepthMask | VisualScreenMask;
	XVisualInfo tmpl;
	tmpl.screen = 0;
	tmpl.depth = 24;
	tmpl.red_mask = 0xFF0000;
	tmpl.green_mask = 0xFF00;
	tmpl.blue_mask = 0xFF;
	tmpl.bits_per_rgb = 8;
	return XGetVisualInfo(display, mask, &tmpl, &nitems);
}

int main()
{
	Display *display = XOpenDisplay(NULL);
	Window root = XRootWindow(display, 0);
	XVisualInfo *vi = get_vi(display);
	if (!vi)
	{
		fprintf(stderr, "failed to get vi\n");
		return EXIT_FAILURE;
	}
	XSetWindowAttributes swa;
	swa.event_mask = ExposureMask | KeyPressMask;
	unsigned mask = CWEventMask;
	Window window = XCreateWindow(display, root, 0, 0, 640, 480, 0, vi->depth,
	                              InputOutput, vi->visual, mask, &swa);
	XMapWindow(display, window);
	XFlush(display);
	Atom atom_net_wm_name = XInternAtom(display, "_NET_WM_NAME", 0);
	printf("_NET_WM_NAME: %u\n", atom_net_wm_name);
	Atom atom_net_wm_pid = XInternAtom(display, "_NET_WM_PID", 0);
	printf("_NET_WM_PID: %u\n", atom_net_wm_pid);
	Atom atom_utf8_string = XInternAtom(display, "UTF8_STRING", 0);
	printf("UTF8_STRING: %u\n", atom_utf8_string);
	Atom atom_net_wm_icon_name = XInternAtom(display, "_NET_WM_ICON_NAME", 0);
	printf("_NET_WM_ICON_NAME: %u\n", atom_net_wm_icon_name);
	XChangeProperty(display, window, atom_net_wm_name, atom_utf8_string, 8,
	                PropModeReplace, "test", 4);
	XChangeProperty(display, window, atom_net_wm_icon_name, atom_utf8_string, 8,
	                PropModeReplace, "test", 4);
	int pid = getpid();
	XChangeProperty(display, window, atom_net_wm_pid, XA_CARDINAL, 32,
	                PropModeReplace, (uint8_t*)&pid, 1);
	XFlush(display);
	sleep(2);
	XUnmapWindow(display, window);
	XFlush(display);
#if 0
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
#endif
	while (1);
	return EXIT_SUCCESS;
}
