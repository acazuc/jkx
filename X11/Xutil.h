#ifndef XUTIL_H
#define XUTIL_H

#include "Xlib.h"

#define VisualNoMask            0
#define VisualIDMask           (1 << 0)
#define VisualScreenMask       (1 << 1)
#define VisualDepthMask        (1 << 2)
#define VisualClassMask        (1 << 3)
#define VisualRedMaskMask      (1 << 4)
#define VisualGreenMaskMask    (1 << 5)
#define VisualBlueMaskMask     (1 << 6)
#define VisualColormapSizeMask (1 << 7)
#define VisualBitsPerRGBMask   (1 << 8)
#define VisualAllMask           0x1FF

typedef struct
{
	Visual *visual;
	VisualID visualid;
	int screen;
	unsigned depth;
	int class;
	unsigned long red_mask;
	unsigned long green_mask;
	unsigned long blue_mask;
	int colormap_size;
	int bits_per_rgb;
} XVisualInfo;

XVisualInfo *XGetVisualInfo(Display *display, long vinfo_mask,
                            XVisualInfo *vinfo_template, int *nitems_return);

#endif
