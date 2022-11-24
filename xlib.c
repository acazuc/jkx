#include "Xutil.h"
#include "Xlib.h"
#include "xpl.h"
#include <stdlib.h>
#include <stdio.h>

Display *XOpenDisplay(const char *display_name)
{
	Display *display = malloc(sizeof(*display));
	if (!display)
		return NULL;
	display->conn = xpl_conn_new();
	if (!display->conn)
	{
		free(display);
		return NULL;
	}
	if (!xplc_conn_init(display->conn, 'l', 11, 0, "", ""))
		goto err;
	if (!xpl_flush(display->conn))
		goto err;
	enum xpl_status parse_status;
	do
	{
		int ret = xpl_recv(display->conn);
		parse_status = xplc_conn_init_reply(display->conn);
	} while (parse_status == XPL_AGAIN);
	if (parse_status != XPL_OK)
		goto err;
	return display;

err:
	xpl_conn_delete(display->conn);
	free(display);
	return NULL;
}

int XConnectionNumber(Display *display)
{
	return display->conn->fd;
}

int XDefaultScreen(Display *display)
{
	return 0;
}

Window XRootWindow(Display *display, int screen_number)
{
	if (screen_number < 0
	 || screen_number >= display->conn->setup->screens_count)
		return None;
	return display->conn->setup->screens[screen_number].root;
}

int XScreenCount(Display *display)
{
	return display->conn->setup->screens_count;
}

XVisualInfo *XGetVisualInfo(Display *display, long vinfo_mask,
                            XVisualInfo *vinfo_template, int *nitems_return)
{
	*nitems_return = 0;
	XVisualInfo *vi = NULL;
	for (size_t i = 0; i < display->conn->setup->screens_count; ++i)
	{
		struct xpl_screen *screen = &display->conn->setup->screens[i];
		if ((vinfo_mask & VisualScreenMask) && vinfo_template->screen != i)
			continue;
		for (size_t j = 0; j < screen->depths_count; ++j)
		{
			struct xpl_depth *depth = &screen->depths[j];
			if ((vinfo_mask & VisualDepthMask)
			 && depth->depth != vinfo_template->depth)
				continue;
			for (size_t k = 0; k < depth->visualtypes_count; ++k)
			{
				struct xpl_visualtype *visualtype = &depth->visualtypes[k];
				if ((vinfo_mask & VisualClassMask)
				 && visualtype->class != vinfo_template->class)
					continue;
				if ((vinfo_mask & VisualRedMaskMask)
				 && visualtype->red_mask != vinfo_template->red_mask)
					continue;
				if ((vinfo_mask & VisualRedMaskMask)
				 && visualtype->green_mask != vinfo_template->green_mask)
					continue;
				if ((vinfo_mask & VisualRedMaskMask)
				 && visualtype->blue_mask != vinfo_template->blue_mask)
					continue;
				if ((vinfo_mask & VisualColormapSizeMask)
				 && visualtype->colormap_entries != vinfo_template->colormap_size)
					continue;
				if ((vinfo_mask & VisualBitsPerRGBMask)
				 && visualtype->bits_per_rgb_value != vinfo_template->bits_per_rgb)
					continue;
				XVisualInfo *tmp = realloc(vi, sizeof(*vi) * (*nitems_return + 1));
				if (!tmp)
				{
					free(vi);
					*nitems_return = 0;
					return NULL;
				}
				vi = tmp;
				tmp = &vi[(*nitems_return)++];
				tmp->visual = visualtype;
				tmp->visualid = visualtype->visual_id;
				tmp->screen = i;
				tmp->depth = depth->depth;
				tmp->class = visualtype->class;
				tmp->red_mask = visualtype->red_mask;
				tmp->green_mask = visualtype->green_mask;
				tmp->blue_mask = visualtype->blue_mask;
				tmp->colormap_size = visualtype->colormap_entries;
				tmp->bits_per_rgb = visualtype->bits_per_rgb_value;
			}
		}
	}
	return vi;
}

int XFlush(Display *display)
{
	if (!xpl_flush(display->conn))
		return BadRequest;
	return Success;
}

int XPending(Display *display)
{
	return 0;
}

Bool XCheckIfEvent(Display *display, XEvent *event_return, Bool (*predicate)(),
                   XPointer arg)
{
	return False;
}

int XNextEvent(Display *display, XEvent *event_return)
{
	return Success;
}

int XPeekEvent(Display *display, XEvent *event_return)
{
	return Success;
}

Window XCreateWindow(Display *display, Window parent, int x, int y,
                     unsigned width, unsigned height, unsigned border_width,
                     int depth, unsigned _class, Visual *visual,
                     unsigned valuemask, XSetWindowAttributes *attributes)
{
	uint32_t values[15];
	Window id = xplc_generate_id(display->conn);
	size_t n = 0;
	if (valuemask & CWBackPixmap)
		values[n++] = attributes->background_pixmap;
	if (valuemask & CWBackPixel)
		values[n++] = attributes->background_pixel;
	if (valuemask & CWBorderPixmap)
		values[n++] = attributes->border_pixmap;
	if (valuemask & CWBorderPixel)
		values[n++] = attributes->border_pixel;
	if (valuemask & CWBitGravity)
		values[n++] = attributes->bit_gravity;
	if (valuemask & CWWinGravity)
		values[n++] = attributes->win_gravity;
	if (valuemask & CWBackingStore)
		values[n++] = attributes->backing_store;
	if (valuemask & CWBackingPlanes)
		values[n++] = attributes->backing_planes;
	if (valuemask & CWBackingPixel)
		values[n++] = attributes->backing_pixel;
	if (valuemask & CWOverrideRedirect)
		values[n++] = attributes->override_redirect;
	if (valuemask & CWSaveUnder)
		values[n++] = attributes->save_under;
	if (valuemask & CWEventMask)
		values[n++] = attributes->event_mask;
	if (valuemask & CWDontPropagate)
		values[n++] = attributes->do_not_propagate_mask;
	if (valuemask & CWColormap)
		values[n++] = attributes->colormap;
	if (valuemask & CWCursor)
		values[n++] = attributes->cursor;
	if (!xplc_create_window(display->conn, depth, id, parent, x, y, width,
	                        height, border_width, _class, visual ? visual->visual_id : 0,
	                        valuemask, values))
		return BadAlloc;
	return id;
}

int XMapWindow(Display *display, Window w)
{
	if (!xplc_map_window(display->conn, w))
		return BadAlloc;
	return Success;
}

int XUnmapWindow(Display *display, Window w)
{
	if (!xplc_unmap_window(display->conn, w))
		return BadAlloc;
	return Success;
}

Atom XInternAtom(Display *display, char *atom_name, Bool only_if_exists)
{
	if (!xplc_intern_atom(display->conn, only_if_exists, atom_name))
		return BadAlloc;
	if (!xpl_flush(display->conn))
		return BadAlloc;
	enum xpl_status parse_status;
	struct xpl_err xpl_err;
	Atom atom;
	do
	{
		int ret = xpl_recv(display->conn);
		parse_status = xplc_intern_atom_reply(display->conn, &atom, &xpl_err);
	} while (parse_status == XPL_AGAIN);
	if (parse_status == XPL_ERR)
		return xpl_err.code;
	return atom;
}

int XChangeProperty(Display *display, Window w, Atom property, Atom type,
                    int format, int mode, const uint8_t *data, int nelements)
{
	if (!xplc_change_property(display->conn, mode, w, property, type, format,
	                          nelements, data))
		return BadAlloc;
	return Success;
}
