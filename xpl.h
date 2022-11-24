#ifndef XPL_H
#define XPL_H

#include <sys/queue.h>
#include <stdint.h>
#include "buf.h"

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
typedef uint32_t xpl_timestamp_t;
typedef uint8_t xpl_bool_t;
typedef uint16_t xpl_keybutmask_t;

enum xpl_bit_gravity
{
	XPL_BIT_GRAVITY_FORGET    = 0,
	XPL_BIT_GRAVITY_NORTHWEST = 1,
	XPL_BIT_GRAVITY_NORTH     = 2,
	XPL_BIT_GRAVITY_NORTHEAST = 3,
	XPL_BIT_GRAVITY_WEST      = 4,
	XPL_BIT_GRAVITY_CENTER    = 5,
	XPL_BIT_GRAVITY_EAST      = 6,
	XPL_BIT_GRAVITY_SOUTHWEST = 7,
	XPL_BIT_GRAVITY_SOUTH     = 8,
	XPL_BIT_GRAVITY_SOUTHEAST = 9,
	XPL_BIT_GRAVITY_STATIC    = 10,
};

enum xpl_win_gravity
{
	XPL_WIN_GRAVITY_UNMAP     = 0,
	XPL_WIN_GRAVITY_NORTHWEST = 1,
	XPL_WIN_GRAVITY_NORTH     = 2,
	XPL_WIN_GRAVITY_NORTHEAST = 3,
	XPL_WIN_GRAVITY_WEST      = 4,
	XPL_WIN_GRAVITY_CENTER    = 5,
	XPL_WIN_GRAVITY_EAST      = 6,
	XPL_WIN_GRAVITY_SOUTHWEST = 7,
	XPL_WIN_GRAVITY_SOUTH     = 8,
	XPL_WIN_GRAVITY_SOUTHEAST = 9,
	XPL_WIN_GRAVITY_STATIC    = 10,
};

enum xpl_bool
{
	XPL_FALSE,
	XPL_TRUE,
};

enum xpl_event_type
{
	XPL_EVENT_KEY_PRESS         = 2,
	XPL_EVENT_KEY_RELEASE       = 3,
	XPL_EVENT_BUTTON_PRESS      = 4,
	XPL_EVENT_BUTTON_RELEASE    = 5,
	XPL_EVENT_MOTION_NOTIFY     = 6,
	XPL_EVENT_ENTER_NOTIFY      = 7,
	XPL_EVENT_LEAVE_NOTIFY      = 8,
	XPL_EVENT_FOCUS_IN          = 9,
	XPL_EVENT_FOCUS_OUT         = 10,
	XPL_EVENT_KEYMAP_NOTIFY     = 11,
	XPL_EVENT_EXPOSE            = 12,
	XPL_EVENT_GRAPHICS_EXPOSURE = 13,
	XPL_EVENT_NO_EXPOSURE       = 14,
	XPL_EVENT_VISIBILITY_NOTIFY = 15,
	XPL_EVENT_CREATE_NOTIFY     = 16,
	XPL_EVENT_DESTROY_NOTIFY    = 17,
	XPL_EVENT_UNMAP_NOTIFY      = 18,
	XPL_EVENT_MAP_NOTIFY        = 19,
	XPL_EVENT_MAP_REQUEST       = 20,
	XPL_EVENT_REPARENT_NOTIFY   = 21,
	XPL_EVENT_CONFIGURE_NOTIFY  = 22,
	XPL_EVENT_CONFIGURE_REQUEST = 23,
	XPL_EVENT_GRAVITY_NOTIFY    = 24,
	XPL_EVENT_RESIZE_REQUEST    = 25,
	XPL_EVENT_CIRCULATE_NOTIFY  = 26,
	XPL_EVENT_CIRCULATE_REQUEST = 27,
	XPL_EVENT_PROPERTY_NOTIFY   = 28,
	XPL_EVENT_SELECTION_CLEAR   = 29,
	XPL_EVENT_SELECTION_REQUEST = 30,
	XPL_EVENT_SELECTION_NOTIFY  = 31,
	XPL_EVENT_COLORMAP_NOTIFY   = 32,
	XPL_EVENT_CLIENT_MESSAGE    = 33,
	XPL_EVENT_MAPPING_NOTIFY    = 34,
};

enum xpl_event_mask
{
	XPL_EVENT_MASK_KEY_PRESS             = (1 << 0),
	XPL_EVENT_MASK_KEY_RELEASE           = (1 << 1),
	XPL_EVENT_MASK_BUTTON_PRESS          = (1 << 2),
	XPL_EVENT_MASK_BUTTON_RELEASE        = (1 << 3),
	XPL_EVENT_MASK_ENTER_WINDOW          = (1 << 4),
	XPL_EVENT_MASK_LEAVE_WINDOW          = (1 << 5),
	XPL_EVENT_MASK_POINTER_MOTION        = (1 << 6),
	XPL_EVENT_MASK_POINTER_MOTION_HINT   = (1 << 7),
	XPL_EVENT_MASK_BUTTON1_MOTION        = (1 << 8),
	XPL_EVENT_MASK_BUTTON2_MOTION        = (1 << 9),
	XPL_EVENT_MASK_BUTTON3_MOTION        = (1 << 10),
	XPL_EVENT_MASK_BUTTON4_MOTION        = (1 << 11),
	XPL_EVENT_MASK_BUTTON5_MOTION        = (1 << 12),
	XPL_EVENT_MASK_BUTTON_MOTION         = (1 << 13),
	XPL_EVENT_MASK_KEYMAP_STATE          = (1 << 14),
	XPL_EVENT_MASK_EXPOSURE              = (1 << 15),
	XPL_EVENT_MASK_VISIBILITY_CHANGE     = (1 << 16),
	XPL_EVENT_MASK_STRUCTURE_NOTIFY      = (1 << 17),
	XPL_EVENT_MASK_RESIZE_REDIRECT       = (1 << 18),
	XPL_EVENT_MASK_SUBSTRUCTURE_NOTIFY   = (1 << 19),
	XPL_EVENT_MASK_SUBSTRUCTURE_REDIRECT = (1 << 20),
	XPL_EVENT_MASK_FOCUS_CHANGE          = (1 << 21),
	XPL_EVENT_MASK_PROPERTY_CHANGE       = (1 << 22),
	XPL_EVENT_MASK_COLORMAP_CHANGE       = (1 << 23),
	XPL_EVENT_MASK_OWNER_GRAB_BUTTON     = (1 << 24),
};

enum xpl_keybutmask
{
	XPL_KEYBUTMASK_SHIFT   = (1 << 0),
	XPL_KEYBUTMASK_LOCK    = (1 << 1),
	XPL_KEYBUTMASK_CONTROL = (1 << 2),
	XPL_KEYBUTMASK_MOD1    = (1 << 3),
	XPL_KEYBUTMASK_MOD2    = (1 << 4),
	XPL_KEYBUTMASK_MOD3    = (1 << 5),
	XPL_KEYBUTMASK_MOD4    = (1 << 6),
	XPL_KEYBUTMASK_MOD5    = (1 << 7),
	XPL_KEYBUTMASK_BUTTON1 = (1 << 8),
	XPL_KEYBUTMASK_BUTTON2 = (1 << 9),
	XPL_KEYBUTMASK_BUTTON3 = (1 << 10),
	XPL_KEYBUTMASK_BUTTON4 = (1 << 11),
	XPL_KEYBUTMASK_BUTTON5 = (1 << 12),
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
	XPL_ERR_REQUEST        = 1,
	XPL_ERR_VALUE          = 2,
	XPL_ERR_WINDOW         = 3,
	XPL_ERR_PIXMAP         = 4,
	XPL_ERR_ATOM           = 5,
	XPL_ERR_CURSOR         = 6,
	XPL_ERR_FONT           = 7,
	XPL_ERR_MATCH          = 8,
	XPL_ERR_DRAWABLE       = 9,
	XPL_ERR_ACCESS         = 10,
	XPL_ERR_ALLOC          = 11,
	XPL_ERR_COLORMAP       = 12,
	XPL_ERR_GCONTEXT       = 13,
	XPL_ERR_IDCHOICE       = 14,
	XPL_ERR_NAME           = 15,
	XPL_ERR_LENGTH         = 16,
	XPL_ERR_IMPLEMENTATION = 17,
};

enum xpl_create_window_value_mask
{
	XPL_CREATE_WINDOW_BACKGROUND_PIXMAP     = (1 << 0),
	XPL_CREATE_WINDOW_BACKGROUND_PIXEL      = (1 << 1),
	XPL_CREATE_WINDOW_BORDER_PIXMAP         = (1 << 2),
	XPL_CREATE_WINDOW_BORDER_PIXEL          = (1 << 3),
	XPL_CREATE_WINDOW_BIT_GRAVITY           = (1 << 4),
	XPL_CREATE_WINDOW_WIN_GRAVITY           = (1 << 5),
	XPL_CREATE_WINDOW_BACKING_STORE         = (1 << 6),
	XPL_CREATE_WINDOW_BACKING_PLANES        = (1 << 7),
	XPL_CREATE_WINDOW_BACKING_PIXEL         = (1 << 8),
	XPL_CREATE_WINDOW_OVERRIDE_REDIRECT     = (1 << 9),
	XPL_CREATE_WINDOW_SAVE_UNDER            = (1 << 10),
	XPL_CREATE_WINDOW_EVENT_MASK            = (1 << 11),
	XPL_CREATE_WINDOW_DO_NOT_PROPAGATE_MASK = (1 << 12),
	XPL_CREATE_WINDOW_COLORMAP              = (1 << 13),
	XPL_CREATE_WINDOW_CURSOR                = (1 << 14),
};

enum xpl_property_mode
{
	XPL_PROPERTY_REPLACE = 0,
	XPL_PROPERTY_PREPEND = 1,
	XPL_PROPERTY_APPEND  = 2,
};

enum xpl_request
{
	XPL_REQUEST_CREATE_WINDOW              = 1,
	XPL_REQUEST_CHANGE_WINDOW_ATTRIBUTES   = 2,
	XPL_REQUEST_GET_WINDOW_ATTRIBUTES      = 3,
	XPL_REQUEST_DESTROY_WINDOW             = 4,
	XPL_REQUEST_DESTROY_SUBWINDOWS         = 5,
	XPL_REQUEST_CHANGE_SAVE_SET            = 6,
	XPL_REQUEST_REPARENT_WINDOW            = 7,
	XPL_REQUEST_MAP_WINDOW                 = 8,
	XPL_REQUEST_MAP_SUBWINDOWS             = 9,
	XPL_REQUEST_UNMAP_WINDOW               = 10,
	XPL_REQUEST_UNMAP_SUBWINDOWS           = 11,
	XPL_REQUEST_CONFIGURE_WINDOW           = 12,
	XPL_REQUEST_CIRCULATE_WINDOW           = 13,
	XPL_REQUEST_GET_GEOMETRY               = 14,
	XPL_REQUEST_QUERY_TREE                 = 15,
	XPL_REQUEST_INTERN_ATOM                = 16,
	XPL_REQUEST_GET_ATOM_NAME              = 17,
	XPL_REQUEST_CHANGE_PROPERTY            = 18,
	XPL_REQUEST_DELETE_PROPERTY            = 19,
	XPL_REQUEST_GET_PROPERTY               = 20,
	XPL_REQUEST_LIST_PROPERTIES            = 21,
	XPL_REQUEST_SET_SELECTION_OWNER        = 22,
	XPL_REQUEST_GET_SELECTION_OWNER        = 23,
	XPL_REQUEST_CONVERT_SELECTION          = 24,
	XPL_REQUEST_SEND_EVENT                 = 25,
	XPL_REQUEST_GRAB_POINTER               = 26,
	XPL_REQUEST_UNGRAB_POINTER             = 27,
	XPL_REQUEST_GRAB_BUTTON                = 28,
	XPL_REQUEST_UNGRAB_BUTTON              = 29,
	XPL_REQUEST_CHANGE_ACTIVE_POINTER_GRAB = 30,
	XPL_REQUEST_GRAB_KEYBOARD              = 31,
	XPL_REQUEST_UNGRAB_KEYBOARD            = 32,
	XPL_REQUEST_GRAB_KEY                   = 33,
	XPL_REQUEST_UNGRAB_KEY                 = 34,
	XPL_REQUEST_ALLOW_EVENTS               = 35,
	XPL_REQUEST_GRAB_SERVER                = 36,
	XPL_REQUEST_UNGRAB_SERVER              = 37,
	XPL_REQUEST_QUERY_POINTER              = 38,
	XPL_REQUEST_GET_MOTION_EVENTS          = 39,
	XPL_REQUEST_TRANSLATE_COORDINATES      = 40,
	XPL_REQUEST_WARP_POINTER               = 41,
	XPL_REQUEST_SET_INPUT_FOCUS            = 42,
	XPL_REQUEST_GET_INPUT_FOCUS            = 43,
	XPL_REQUEST_QUERY_KEYMAP               = 44,
	XPL_REQUEST_OPEN_FONT                  = 45,
	XPL_REQUEST_CLOSE_FONT                 = 46,
	XPL_REQUEST_QUERY_FONT                 = 47,
	XPL_REQUEST_QUERY_TEXT_EXTENTS         = 48,
	XPL_REQUEST_LIST_FONTS                 = 49,
	XPL_REQUEST_LIST_FONTS_WITH_INFO       = 50,
	XPL_REQUEST_SET_FONT_PATH              = 51,
	XPL_REQUEST_GET_FONT_PATH              = 52,
	XPL_REQUEST_CREATE_PIXMAP              = 53,
	XPL_REQUEST_FREE_PIXMAP                = 54,
	XPL_REQUEST_CREATE_GC                  = 55,
	XPL_REQUEST_CHANGE_GC                  = 56,
	XPL_REQUEST_COPY_GC                    = 57,
	XPL_REQUEST_SET_DASHES                 = 58,
	XPL_REQUEST_SET_CLIP_RECTANGLES        = 59,
	XPL_REQUEST_FREE_GC                    = 60,
	XPL_REQUEST_CLEAR_AREA                 = 61,
	XPL_REQUEST_COPY_AREA                  = 62,
	XPL_REQUEST_COPY_PLANE                 = 63,
	XPL_REQUEST_POLY_POINT                 = 64,
	XPL_REQUEST_POLY_LINE                  = 65,
	XPL_REQUEST_POLY_SEGMENT               = 66,
	XPL_REQUEST_POLY_RECTANGLE             = 67,
	XPL_REQUEST_POLY_ARC                   = 68,
	XPL_REQUEST_FILL_POLY                  = 69,
	XPL_REQUEST_POLY_FILL_RECTANGLE        = 70,
	XPL_REQUEST_POLY_FILL_ARC              = 71,
	XPL_REQUEST_PUT_IMAGE                  = 72,
	XPL_REQUEST_GET_IMAGE                  = 73,
	XPL_REQUEST_POLY_TEXT_8                = 74,
	XPL_REQUEST_POLY_TEXT_16               = 75,
	XPL_REQUEST_IMAGE_TEXT_8               = 76,
	XPL_REQUEST_IMAGE_TEXT_16              = 77,
	XPL_REQUEST_CREATE_COLORMAP            = 78,
	XPL_REQUEST_FREE_COLORMAP              = 79,
	XPL_REQUEST_COPY_COLORMAP_AND_FREE     = 80,
	XPL_REQUEST_INSTALL_COLORMAP           = 81,
	XPL_REQUEST_UNINSTALL_COLORMAP         = 82,
	XPL_REQUEST_LIST_INSTALLED_COLORMAPS   = 83,
	XPL_REQUEST_ALLOC_COLOR                = 84,
	XPL_REQUEST_ALLOC_NAMED_COLOR          = 85,
	XPL_REQUEST_ALLOC_COLOR_CELLS          = 86,
	XPL_REQUEST_ALLOC_COLOR_PLANES         = 87,
	XPL_REQUEST_FREE_COLORS                = 88,
	XPL_REQUEST_STORE_COLORS               = 89,
	XPL_REQUEST_STORE_NAMED_COLOR          = 90,
	XPL_REQUEST_QUERY_COLORS               = 91,
	XPL_REQUEST_LOOKUP_COLOR               = 92,
	XPL_REQUEST_CREATE_CURSOR              = 93,
	XPL_REQUEST_CREATE_GLYPH_CURSOR        = 94,
	XPL_REQUEST_FREE_CURSOR                = 95,
	XPL_REQUEST_RECOLOR_CURSOR             = 96,
	XPL_REQUEST_QUERY_BEST_SIZE            = 97,
	XPL_REQUEST_QUERY_EXTENSION            = 98,
	XPL_REQUEST_LIST_EXTENSIONS            = 99,
	XPL_REQUEST_CHANGE_KEYBOARD_MAPPING    = 100,
	XPL_REQUEST_GET_KEYBOARD_MAPPING       = 101,
	XPL_REQUEST_CHANGE_KEYBOARD_CONTROL    = 102,
	XPL_REQUEST_GET_KEYBOARD_CONTROL       = 103,
	XPL_REQUEST_BELL                       = 104,
	XPL_REQUEST_CHANGE_POINTER_CONTROL     = 105,
	XPL_REQUEST_GET_POINTER_CONTROL        = 106,
	XPL_REQUEST_SET_SCREEN_SAVER           = 107,
	XPL_REQUEST_GET_SCREEN_SAVER           = 108,
	XPL_REQUEST_CHANGE_HOSTS               = 109,
	XPL_REQUEST_LIST_HOSTS                 = 110,
	XPL_REQUEST_SET_ACCESS_CONTROL         = 111,
	XPL_REQUEST_SET_CLOSE_DOWN_MODE        = 112,
	XPL_REQUEST_KILL_CLIENT                = 113,
	XPL_REQUEST_ROTATE_PROPERTIES          = 114,
	XPL_REQUEST_FORCE_SCREEN_SAVER         = 115,
	XPL_REQUEST_SET_POINTER_MAPPING        = 116,
	XPL_REQUEST_GET_POINTER_MAPPING        = 117,
	XPL_REQUEST_SET_MODIFIER_MAPPING       = 118,
	XPL_REQUEST_GET_MODIFIER_MAPPING       = 119,
	XPL_REQUEST_NO_OPERATION               = 127,
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
	uint8_t class;
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

enum xpl_status
{
	XPL_OK,       /* message has been fetched */
	XPL_ERR,      /* received an error */
	XPL_AGAIN,    /* not enough data */
	XPL_INTERNAL, /* internal error / malformed message */
};

struct xpl_err
{
	enum xpl_error code;
	uint16_t sequence_number;
	uint16_t minor_opcode;
	uint8_t major_opcode;
	union
	{
		struct
		{
			uint32_t value;
		} value;
		struct
		{
			uint32_t bad_resource_id;
		} window;
		struct
		{
			uint32_t bad_resource_id;
		} pixmap;
		struct
		{
			uint32_t bad_atom_id;
		} atom;
		struct
		{
			uint32_t bad_resource_id;
		} cursor;
		struct
		{
			uint32_t bad_resource_id;
		} font;
		struct
		{
			uint32_t bad_resource_id;
		} drawable;
		struct
		{
			uint32_t bad_resource_id;
		} colormap;
		struct
		{
			uint32_t bad_resource_id;
		} gcontext;
		struct
		{
			uint32_t bad_resource_id;
		} id_choice;
		uint32_t bad_id;
	};
};

struct xpl_event_key_press
{
	xpl_keycode_t detail;
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t root;
	xpl_window_t event;
	xpl_window_t child;
	int16_t root_x;
	int16_t root_y;
	int16_t event_x;
	int16_t event_y;
	xpl_keybutmask_t state;
	uint8_t same_screen;
};

struct xpl_event_key_release
{
	xpl_keycode_t detail;
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t root;
	xpl_window_t event;
	xpl_window_t child;
	int16_t root_x;
	int16_t root_y;
	int16_t event_x;
	int16_t event_y;
	xpl_keybutmask_t state;
	uint8_t same_screen;
};

struct xpl_event_button_press
{
	xpl_button_t detail;
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t root;
	xpl_window_t event;
	xpl_window_t child;
	int16_t root_x;
	int16_t root_y;
	int16_t event_x;
	int16_t event_y;
	xpl_keybutmask_t state;
	uint8_t same_screen;
};

struct xpl_event_button_release
{
	xpl_button_t detail;
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t root;
	xpl_window_t event;
	xpl_window_t child;
	int16_t root_x;
	int16_t root_y;
	int16_t event_x;
	int16_t event_y;
	xpl_keybutmask_t state;
	uint8_t same_screen;
};

struct xpl_event_motion_notify
{
	uint8_t detail;
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t root;
	xpl_window_t event;
	xpl_window_t child;
	int16_t root_x;
	int16_t root_y;
	int16_t event_x;
	int16_t event_y;
	xpl_keybutmask_t state;
	uint8_t same_screen;
};

struct xpl_event_enter_notify
{
	uint8_t detail;
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t root;
	xpl_window_t event;
	xpl_window_t child;
	int16_t root_x;
	int16_t root_y;
	int16_t event_x;
	int16_t event_y;
	xpl_keybutmask_t state;
	uint8_t mode;
	uint8_t same_screen_focus;
};

struct xpl_event_leave_notify
{
	uint8_t detail;
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t root;
	xpl_window_t event;
	xpl_window_t child;
	int16_t root_x;
	int16_t root_y;
	int16_t event_x;
	int16_t event_y;
	xpl_keybutmask_t state;
	uint8_t mode;
	uint8_t same_screen_focus;
};

struct xpl_event_focus_in
{
	uint8_t detail;
	uint16_t sequence_number;
	xpl_window_t event;
	uint8_t mode;
};

struct xpl_event_focus_out
{
	uint8_t detail;
	uint16_t sequence_number;
	xpl_window_t event;
	uint8_t mode;
};

struct xpl_event_keymap_notify
{
	uint8_t keys[31];
};

struct xpl_event_expose
{
	uint16_t sequence_number;
	xpl_window_t window;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t count;
};

struct xpl_event_graphics_exposure
{
	uint16_t sequence_number;
	xpl_drawable_t drawable;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t minor_opcode;
	uint16_t count;
	uint8_t major_opcode;
};

struct xpl_event_no_exposure
{
	uint16_t sequence_number;
	xpl_drawable_t drawable;
	uint16_t minor_opcode;
	uint8_t major_opcode;
};

struct xpl_event_visibility_notify
{
	uint16_t sequence_number;
	xpl_window_t window;
	uint8_t state;
};

struct xpl_event_create_notify
{
	uint16_t sequence_number;
	xpl_window_t parent;
	xpl_window_t window;
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t border_width;
	xpl_bool_t override_redirect;
};

struct xpl_event_destroy_notify
{
	uint16_t sequence_number;
	xpl_window_t event;
	xpl_window_t window;
};

struct xpl_event_unmap_notify
{
	uint16_t sequence_number;
	xpl_window_t event;
	xpl_window_t window;
	xpl_bool_t from_configure;
};

struct xpl_event_map_notify
{
	uint16_t sequence_number;
	xpl_window_t event;
	xpl_window_t window;
	xpl_bool_t override_redirect;
};

struct xpl_event_map_request
{
	uint16_t sequence_number;
	xpl_window_t parent;
	xpl_window_t window;
};

struct xpl_event_reparent_notify
{
	uint16_t sequence_number;
	xpl_window_t event;
	xpl_window_t window;
	xpl_window_t parent;
	int16_t x;
	int16_t y;
	xpl_bool_t override_redirect;
};

struct xpl_event_configure_notify
{
	uint16_t sequence_number;
	xpl_window_t event;
	xpl_window_t window;
	xpl_window_t above_sibling;
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t border_width;
	xpl_bool_t override_redirect;
};

struct xpl_event_configure_request
{
	uint8_t stack_mode;
	uint16_t sequence_number;
	xpl_window_t parent;
	xpl_window_t window;
	xpl_window_t sibling;
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t border_width;
	uint16_t value_mask;
};

struct xpl_event_gravity_notify
{
	uint16_t sequence_number;
	xpl_window_t event;
	xpl_window_t window;
	int16_t x;
	int16_t y;
};

struct xpl_event_resize_request
{
	uint16_t sequence_number;
	xpl_window_t window;
	uint16_t width;
	uint16_t height;
};

struct xpl_event_circulate_notify
{
	uint16_t sequence_number;
	xpl_window_t event;
	xpl_window_t window;
	uint8_t place;
};

struct xpl_event_circulate_request
{
	uint16_t sequence_number;
	xpl_window_t parent;
	xpl_window_t window;
	uint8_t place;
};

struct xpl_event_property_notify
{
	uint16_t sequence_number;
	xpl_window_t window;
	xpl_atom_t atom;
	xpl_timestamp_t time;
	uint8_t state;
};

struct xpl_event_selection_clear
{
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t owner;
	xpl_atom_t selection;
};

struct xpl_event_selection_request
{
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t owner;
	xpl_window_t requestor;
	xpl_atom_t selection;
	xpl_atom_t target;
	xpl_atom_t property;
};

struct xpl_event_selection_notify
{
	uint16_t sequence_number;
	xpl_timestamp_t time;
	xpl_window_t requestor;
	xpl_atom_t selection;
	xpl_atom_t target;
	xpl_atom_t property;
};

struct xpl_event_colormap_notify
{
	uint16_t sequence_number;
	xpl_window_t window;
	xpl_colormap_t colormap;
	xpl_bool_t _new;
	uint8_t state;
};

struct xpl_event_client_message
{
	uint16_t sequence_number;
	xpl_window_t window;
	xpl_atom_t type;
};

struct xpl_event_mapping_notify
{
	uint16_t sequence_number;
	uint8_t request;
	xpl_keycode_t first_keycode;
	uint8_t count;
};

struct xpl_event
{
	enum xpl_event_type type;
	union
	{
		struct xpl_event_key_press key_press;
		struct xpl_event_key_release key_release;
		struct xpl_event_button_press button_press;
		struct xpl_event_button_release button_release;
		struct xpl_event_motion_notify motion_notify;
		struct xpl_event_enter_notify enter_notify;
		struct xpl_event_leave_notify leave_notify;
		struct xpl_event_focus_in focus_in;
		struct xpl_event_focus_out focus_out;
		struct xpl_event_keymap_notify keymap_notify;
		struct xpl_event_expose expose;
		struct xpl_event_graphics_exposure graphics_exposure;
		struct xpl_event_no_exposure no_exposure;
		struct xpl_event_visibility_notify visibility_notify;
		struct xpl_event_create_notify create_notify;
		struct xpl_event_destroy_notify destroy_notify;
		struct xpl_event_unmap_notify unmap_notify;
		struct xpl_event_map_notify map_notify;
		struct xpl_event_map_request map_request;
		struct xpl_event_reparent_notify reparent_notify;
		struct xpl_event_configure_notify configure_notify;
		struct xpl_event_configure_request configure_request;
		struct xpl_event_gravity_notify gravity_notify;
		struct xpl_event_resize_request resize_request;
		struct xpl_event_circulate_notify circulate_notify;
		struct xpl_event_circulate_request circulate_request;
		struct xpl_event_property_notify property_notify;
		struct xpl_event_selection_clear selection_clear;
		struct xpl_event_selection_request selection_request;
		struct xpl_event_selection_notify selection_notify;
		struct xpl_event_colormap_notify colormap_notify;
		struct xpl_event_client_message client_message;
		struct xpl_event_mapping_notify mapping_notify;
	};
};

struct xpl_event_queue
{
	struct xpl_event event;
	SLIST_ENTRY(xpl_event_queue) chain;
};

struct xpl_conn
{
	int fd;
	struct buf rbuf;
	struct buf wbuf;
	struct xpl_setup *setup;
	uint32_t res_id;
	SLIST_HEAD(, xpl_event_queue) events;
};

struct xpl_conn *xpl_conn_new(const char *display_name);
void xpl_conn_delete(struct xpl_conn *conn);

int xpl_send(struct xpl_conn *conn);
int xpl_recv(struct xpl_conn *conn);
int xpl_flush(struct xpl_conn *conn);

uint32_t xplc_generate_id(struct xpl_conn *conn);

int xplc_conn_init(struct xpl_conn *conn, int8_t endian, uint16_t major,
                   uint16_t minor, const char *protoname,
                   const char *protodata);

enum xpl_status xplc_conn_init_reply(struct xpl_conn *conn);

int xplc_create_window(struct xpl_conn *conn, uint8_t depth, xpl_window_t id,
                       xpl_window_t parent, int16_t x, int16_t y,
                       uint16_t width, uint16_t height, uint16_t border_width,
                       uint16_t class_type, xpl_visualid_t visual,
                       uint32_t value_mask, uint32_t *values);

int xplc_map_window(struct xpl_conn *conn, xpl_window_t window);
int xplc_unmap_window(struct xpl_conn *conn, xpl_window_t window);

int xplc_intern_atom(struct xpl_conn *conn, uint8_t only_if_exists,
                     const char *name);

enum xpl_status xplc_intern_atom_reply(struct xpl_conn *conn,
                                       xpl_atom_t *atom,
                                       struct xpl_err *err);

int xplc_change_property(struct xpl_conn *conn, enum xpl_property_mode mode,
                         xpl_window_t window, xpl_atom_t property,
                         xpl_atom_t type, uint8_t format, uint32_t length,
                         const void *data);

#endif
