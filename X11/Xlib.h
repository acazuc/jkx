#ifndef XLIB_H
#define XLIB_H

#include "X.h"
#include <stdint.h>

#define False 0
#define True  1

struct xpl_conn;

struct _Display
{
	struct xpl_conn *conn;
};

typedef struct _Display Display;
typedef uint32_t Atom;
typedef uint32_t Window;
typedef uint32_t Pixmap;
typedef uint32_t Cursor;
typedef uint32_t Colormap;
typedef uint32_t Time;
typedef uint32_t Window;
typedef uint32_t Drawable;
typedef uint32_t XID;
typedef uint32_t VisualID;
typedef struct xpl_visualtype Visual;
typedef void *XPointer;
typedef char Bool;

typedef struct
{
	Pixmap background_pixmap;
	unsigned long background_pixel;
	Pixmap border_pixmap;
	unsigned long border_pixel;
	int bit_gravity;
	int win_gravity;
	int backing_store;
	unsigned long backing_planes;
	unsigned long backing_pixel;
	Bool save_under;
	long event_mask;
	long do_not_propagate_mask;
	Bool override_redirect;
	Colormap colormap;
	Cursor cursor;
} XSetWindowAttributes;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
} XAnyEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	Window root;
	Window subwindow;
	Time time;
	int x;
	int y;
	int x_root;
	int y_root;
	unsigned state;
	unsigned keycode;
	Bool same_screen;
} XKeyEvent;

typedef XKeyEvent XKeyPressedEvent;
typedef XKeyEvent XKeyReleasedEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	Window root;
	Window subwindow;
	Time time;
	int x;
	int y;
	int x_root;
	int y_root;
	unsigned state;
	unsigned button;
	Bool same_screen;
} XButtonEvent;

typedef XButtonEvent XButtonPressedEvent;
typedef XButtonEvent XButtonReleasedEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	Window root;
	Window subwindow;
	Time time;
	int x;
	int y;
	int x_root;
	int y_root;
	unsigned state;
	char is_hiny;
	Bool same_screen;
} XMotionEvent;

typedef XMotionEvent XPointerMovedEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	Window root;
	Window subwindow;
	Time time;
	int x;
	int y;
	int x_root;
	int y_root;
	int mode;
	int detail;
	Bool same_screen;
	Bool focus;
	unsigned state;
} XCrossingEvent;

typedef XCrossingEvent XEnterWindowEvent;
typedef XCrossingEvent XLeaveWindowEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	int mode;
	int detail;
} XFocusChangeEvent;

typedef XFocusChangeEvent XFocusInEvent;
typedef XFocusChangeEvent XFocusOutEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	int x;
	int y;
	int width;
	int height;
	int count;
} XExposeEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Drawable drawable;
	int x;
	int y;
	int width;
	int height;
	int count;
	int major_code;
	int minor_code;
} XGraphicsExposeEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Drawable drawable;
	int major_code;
	int minor_code;
} XNoExposeEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	int state;
} XVisibilityEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window parent;
	Window window;
	int x;
	int y;
	int width;
	int height;
	int border_width;
	Bool override_redirect;
} XCreateWindowEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window event;
	Window window;
} XDestroyWindowEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window event;
	Window window;
	Bool from_configure;
} XUnmapEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window event;
	Window window;
	Bool override_redirect;
} XMapEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window parent;
	Window window;
} XMapRequestEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window event;
	Window window;
	Window parent;
	int x;
	int y;
	Bool override_redirect;
} XReparentEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window event;
	Window window;
	int x;
	int y;
	int width;
	int height;
	int border_width;
	Window above;
	Bool override_redirect;
} XConfigureEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window event;
	Window window;
	int x;
	int y;
} XGravityEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	int width;
	int height;
} XResizeRequestEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window parent;
	Window window;
	int x;
	int y;
	int width;
	int height;
	int border_width;
	Window above;
	int detail;
	unsigned long value_mask;
} XConfigureRequestEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window event;
	Window window;
	int place;
} XCirculateEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window parent;
	Window window;
	int place;
} XCirculateRequestEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	Atom atom;
	Time time;
	int state;
} XPropertyEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	Atom selection;
	Time time;
} XSelectionClearEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window owner;
	Window requestor;
	Atom selection;
	Atom target;
	Atom property;
	Time time;
} XSelectionRequestEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window requestor;
	Atom selection;
	Atom target;
	Atom property;
	Time time;
} XSelectionEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool sned_event;
	Display *display;
	Window window;
	Colormap colormap;
	Bool new;
	int sate;
} XColormapEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	Atom message_type;
	int format;
	union
	{
		char b[20];
		short s[10];
		long l[5];
	} data;
} XClientMessageEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	int request;
	int first_keycode;
	int count;
} XMappingEvent;

typedef struct
{
	int type;
	Display *display;
	XID resourceid;
	unsigned long serial;
	unsigned char error_cpde;
	unsigned char request_code;
	unsigned char minor_code;
} XErrorEvent;

typedef struct
{
	int type;
	unsigned long serial;
	Bool send_event;
	Display *display;
	Window window;
	char key_vector[32];
} XKeymapEvent;

typedef union _XEvent
{
	int type;
	XAnyEvent xany;
	XKeyEvent xkey;
	XButtonEvent xbutton;
	XMotionEvent xmotion;
	XCrossingEvent xcrossing;
	XFocusChangeEvent xfocus;
	XExposeEvent xexpose;
	XGraphicsExposeEvent xgraphicsexpose;
	XNoExposeEvent xnoexpose;
	XVisibilityEvent xvisibility;
	XCreateWindowEvent xcreatewindow;
	XDestroyWindowEvent xdestroywindow;
	XUnmapEvent xunmap;
	XMapEvent xmap;
	XMapRequestEvent xmaprequest;
	XReparentEvent xreparent;
	XConfigureEvent xconfigure;
	XGravityEvent xgravity;
	XResizeRequestEvent xresizerequest;
	XConfigureRequestEvent xconfigurerequest;
	XCirculateEvent xcirculate;
	XCirculateRequestEvent xcirculaterequest;
	XPropertyEvent xproperty;
	XSelectionClearEvent xselectionclear;
	XSelectionRequestEvent xselectionrequest;
	XSelectionEvent xselection;
	XColormapEvent xcolormap;
	XClientMessageEvent xclientmessage;
	XMappingEvent xmapping;
	XErrorEvent xerror;
	XKeymapEvent xkeymap;
	long pad[24];
} XEvent;

typedef struct
{
	int x;
	int y;
	int width;
	int height;
	int border_width;
	Window sibling;
	int stack_mode;
} XWindowChanges;

Display *XOpenDisplay(const char *display_name);

int XConnectionNumber(Display *display);
int XDefaultScreen(Display *display);
Window XRootWindow(Display *display, int screen_number);
int XScreenCount(Display *display);

int XFlush(Display *display);
int XPending(Display *display);
Bool XCheckIfEvent(Display *display, XEvent *event_return, Bool (*predicate)(),
                   XPointer arg);
int XNextEvent(Display *display, XEvent *event_return);
int XPeekEvent(Display *display, XEvent *event_return);

Window XCreateWindow(Display *display, Window parent, int x, int y,
                     unsigned width, unsigned height, unsigned border_width,
                     int depth, unsigned _class, Visual *visual,
                     unsigned valuemask, XSetWindowAttributes *attributes);
Window XCreateSimpleWindow(Display *display, Window parent, int x, int y,
                           unsigned width, unsigned height,
                           unsigned border_width, unsigned long border,
                           unsigned long background);
int XMapWindow(Display *display, Window w);
int XUnmapWindow(Display *display, Window w);
Atom XInternAtom(Display *display, char *atom_name, Bool only_if_exists);
int XChangeProperty(Display *display, Window w, Atom property, Atom type,
                    int format, int mode, const uint8_t *data, int nelements);
int XConfigureWindow(Display *display, Window w, unsigned value_mask,
                     XWindowChanges *values);
int XMoveWindow(Display *display, Window w, int x, int y);
int XResizeWindow(Display *display, Window w, unsigned width, unsigned height);
int XMoveResizeWindow(Display *display, Window w, int x, int y, unsigned width,
                      unsigned height);
int XSetWindowBorderWidth(Display *display, Window w, unsigned width);
int XRaiseWindow(Display *display, Window w);
int XLowerWindow(Display *display, Window w);
int XChangeWindowAttributes(Display *display, Window w,
                            unsigned long valuemask,
                            XSetWindowAttributes *attributes);
int XSetWindowBackground(Display *display, Window w,
                         unsigned long background_pixel);
int XSetWindowBackgroundPixmap(Display *display, Window w,
                               Pixmap background_pixmap);
int XSetWindowBorder(Display *display, Window w, unsigned long border_pixel);
int XSetWindowBorderPixmap(Display *display, Window w, Pixmap border_pixmap);
int XSetWindowColormap(Display *display, Window w, Colormap colormap);
int XDefineCursor(Display *display, Window w, Cursor cursor);
int XUndefineCursor(Display *display, Window w);

#endif
