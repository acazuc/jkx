#ifndef XPL_H
#define XPL_H

#include <stdint.h>

typedef uint32_t xpl_window_t;
typedef uint32_t xpl_pixmap_t;
typedef uint32_t xpl_cursor_t;
typedef uint32_t xpl_font_t;
typedef uint32_t xpl_gcontext_t;
typedef uint32_t xpl_colormap_t;
typedef uint32_t xpl_drawable_t;
typedef uint32_t xpl_fontable_t;
typedef uint32_t xpl_atom_t;
typedef uint32_t xpl_visualid_t;
typedef uint32_t xpl_keysym_t;
typedef uint8_t xpl_keycode_t;
typedef uint8_t xpl_button_t;

enum xpl_gravity
{
	XPL_GRAVITY_FORGET,
	XPL_GRAVITY_UNMAP,
	XPL_GRAVITY_STATIC,
	XPL_GRAVITY_NORTHWEST,
	XPL_GRAVITY_NORTH,
	XPL_GRAVITY_NORTHEAST,
	XPL_GRAVITY_WEST,
	XPL_GRAVITY_CENTER,
	XPL_GRAVITY_EAST,
	XPL_GRAVITY_SOUTHWEST,
	XPL_GRAVITY_SOUTH,
	XPL_GRAVITY_SOUTHEAST,
};

enum xpl_bool
{
	XPL_FALSE,
	XPL_TRUE,
};

enum xpl_event
{
	XPL_KEY_PRESS,
	XPL_KEY_RELEASE,
	XPL_OWNER_GRAB_BUTTON,
	XPL_BUTTON_PRESS,
	XPL_BUTTON_RELEASE,
	XPL_ENTER_WINDOW,
	XPL_LEAVE_WINDOW,
	XPL_POINTER_MOTION,
	XPL_POINTER_MOTION_HINT,
	XPL_BUTTON1_MOTION,
	XPL_BUTTON2_MOTION,
	XPL_BUTTON3_MOTION,
	XPL_BUTTON4_MOTION,
	XPL_BUTTON5_MOTION,
	XPL_BUTTON_MOTION,
	XPL_EXPOSURE,
	XPL_VISIBILITY_CHANGE,
	XPL_STRUCTURE_NOTIFY,
	XPL_RESIZE_REDIRECT,
	XPL_SUBSTRUCTURE_NOTIFY,
	XPL_SUBSTRUCTURE_REDIRECT,
	XPL_FOCUS_CHANGE,
	XPL_PROPERTY_CHANGE,
	XPL_COLORMAP_CHANGE,
	XPL_KEYMAP_STATE,
};

enum xpl_keymask
{
	XPL_KEYM_SHIFT   = (1 << 0),
	XPL_KEYM_LOCK    = (1 << 1),
	XPL_KEYM_CONTROL = (1 << 2),
	XPL_KEYM_MOD1    = (1 << 3),
	XPL_KEYM_MOD2    = (1 << 4),
	XPL_KEYM_MOD3    = (1 << 5),
	XPL_KEYM_MOD4    = (1 << 6),
	XPL_KEYM_MOD5    = (1 << 7),
};

enum xpl_butmask
{
	XPL_BUTM_BUTTON1 = (1 << 0),
	XPL_BUTM_BUTTON2 = (1 << 1),
	XPL_BUTM_BUTTON3 = (1 << 2),
	XPL_BUTM_BUTTON4 = (1 << 3),
	XPL_BUTM_BUTTON5 = (1 << 4),
};

struct xpl_point
{
	int16_t x;
	int16_t y;
};

struct xpl_rectangle
{
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
};

struct xpl_arc
{
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
	int16_t angle1;
	int16_t angle2;
};

enum xpl_error
{
	XPL_ERR_ACCESS,
	XPL_ERR_ALLOC,
	XPL_ERR_ATOM,
	XPL_ERR_COLORMAP,
	XPL_ERR_CURSOR,
	XPL_ERR_DRAWABLE,
	XPL_ERR_FONT,
	XPL_ERR_GCONTEXT,
	XPL_ERR_IDCHOICE,
	XPL_ERR_IMPLEMENTATION,
	XPL_ERR_LENGTH,
	XPL_ERR_MATCH,
	XPL_ERR_NAME,
	XPL_ERR_PIXMAP,
	XPL_ERR_REQUEST,
	XPL_ERR_VALUE,
	XPL_ERR_WINDOW,
};

struct xpl_format
{
	uint8_t depth;
	uint8_t bpp;
	uint8_t scanline_pad;
};

struct xpl_visualtype
{
	xpl_visualid_t visual_id;
	uint8_t class_type;
	uint8_t bits_per_rgb_value;
	uint16_t colormap_entries;
	uint32_t red_mask;
	uint32_t green_mask;
	uint32_t blue_mask;
};

struct xpl_depth
{
	uint8_t depth;
	uint16_t visualtypes_count;
	struct xpl_visualtype *visualtypes;
};

struct xpl_screen
{
	xpl_window_t root;
	xpl_colormap_t default_colormap;
	uint32_t white_pixel;
	uint32_t black_pixel;
	uint32_t current_input_mask;
	uint16_t width_pixels;
	uint16_t height_pixels;
	uint16_t width_millimeters;
	uint16_t height_millimeters;
	uint16_t min_installed_maps;
	uint16_t max_installed_maps;
	xpl_visualid_t root_visual;
	uint8_t backing_stores;
	uint8_t save_unders;
	uint8_t root_depth;
	uint8_t depths_count;
	struct xpl_depth *depths;
};

struct xpl_setup
{
	uint32_t release_number;
	uint32_t resource_id_base;
	uint32_t resource_id_mask;
	uint32_t motion_buffer_size;
	uint16_t maximum_request_length;
	uint8_t screens_count;
	uint8_t formats_count;
	uint8_t image_byte_order;
	uint8_t bitmap_bit_order;
	uint8_t bitmap_scanline_unit;
	uint8_t bitmap_scanline_pad;
	xpl_keycode_t min_keycode;
	xpl_keycode_t max_keycode;
	char *vendor;
	struct xpl_format *formats;
	struct xpl_screen *screens;
};

static inline uint32_t xpl_pad(uint32_t length)
{
	uint32_t n = length % 4;
	if (!n)
		return 0;
	return 4 - n;
}

#endif
