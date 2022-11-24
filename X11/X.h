#ifndef X_H
#define X_H

#define None 0

#define Success           0
#define BadRequest        1
#define BadValue          2
#define BadWindow         3
#define BadPixmap         4
#define BadAtom           5
#define BadCursor         6
#define BadFont           7
#define BadMatch          8
#define BadDrawable       9
#define BadAccess         10
#define BadAlloc          11
#define BadColor          12
#define BadGC             13
#define BadIDChoice       14
#define BadName           15
#define BadLength         16
#define BadImplementation 17

#define CWBackPixmap       (1 << 0)
#define CWBackPixel        (1 << 1)
#define CWBorderPixmap     (1 << 2)
#define CWBorderPixel      (1 << 3)
#define CWBitGravity       (1 << 4)
#define CWWinGravity       (1 << 5)
#define CWBackingStore     (1 << 6)
#define CWBackingPlanes    (1 << 7)
#define CWBackingPixel     (1 << 8)
#define CWOverrideRedirect (1 << 9)
#define CWSaveUnder        (1 << 10)
#define CWEventMask        (1 << 11)
#define CWDontPropagate    (1 << 12)
#define CWColormap         (1 << 13)
#define CWCursor           (1 << 14)

#define NoEventMask               0
#define KeyPressMask             (1 << 0)
#define KeyReleaseMask           (1 << 1)
#define ButtonPressMask          (1 << 2)
#define ButtonReleaseMask        (1 << 3)
#define EnterWindowMask          (1 << 4)
#define LeaveWindowMask          (1 << 5)
#define PointerMotionMask        (1 << 6)
#define PointerMotionHintMask    (1 << 7)
#define Button1MotionMask        (1 << 8)
#define Button2MotionMask        (1 << 9)
#define Button3MotionMask        (1 << 10)
#define Button4MotionMask        (1 << 11)
#define Button5MotionMask        (1 << 12)
#define ButtonMotionMask         (1 << 13)
#define KeymapStateMask          (1 << 14)
#define ExposureMask             (1 << 15)
#define VisibilityChangeMask     (1 << 16)
#define StrutureNotifyMask       (1 << 17)
#define ResizeRedirectMask       (1 << 18)
#define SubstructureMotifyMask   (1 << 19)
#define SubstructureRedirectMask (1 << 20)
#define FocusChangeMask          (1 << 21)
#define PropertyChangeMask       (1 << 22)
#define ColormapChangeMask       (1 << 23)
#define OwnerGrabButtonMask      (1 << 24)

#define CopyFromParent 0

#define InputOutput 1
#define InputOnly   2

#define PropModeReplace 0
#define PropModePrepend 1
#define PropModeAppend  2

#endif
