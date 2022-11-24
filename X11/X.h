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

#define CWX           (1 << 0)
#define CWY           (1 << 1)
#define CWWidth       (1 << 2)
#define CWHeight      (1 << 3)
#define CWBorderWidth (1 << 4)
#define CWSibling     (1 << 5)
#define CWStackMode   (1 << 6)

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

#define Button1Mask (1 << 8)
#define Button2Mask (1 << 9)
#define Button3Mask (1 << 10)
#define Button4Mask (1 << 11)
#define Button5Mask (1 << 12)

#define Button1 1
#define Button2 2
#define Button3 3
#define Button4 4
#define Button5 5

#define ShiftMask   (1 << 0)
#define LockMask    (1 << 1)
#define ControlMask (1 << 2)
#define Mod1Mask    (1 << 3)
#define Mod2Mask    (1 << 4)
#define Mod3Mask    (1 << 5)
#define Mod4Mask    (1 << 6)
#define Mod5Mask    (1 << 7)

#define KeyPress         2
#define KeyRelease       3
#define ButtonPress      4
#define ButtonRelease    5
#define MotionNotify     6
#define EnterNotify      7
#define LeaveNotify      8
#define FocusIn          9
#define FocusOut         10
#define KeymapNotify     11
#define Expose           12
#define GraphicsExposure 13
#define NoExposure       14
#define VisibilityNotify 15
#define CreateNotify     16
#define DestroyNotify    17
#define UnmapNotify      18
#define MapNotify        19
#define MapRequest       20
#define ReparentNotify   21
#define ConfigureNotify  22
#define ConfigureRequest 23
#define GravityNotify    24
#define ResizeRequest    25
#define CirculateNotify  26
#define CirculateRequest 27
#define PropertyNotify   28
#define SelectionClear   29
#define SelectionRequest 30
#define SelectionNotify  31
#define ColormapNotify   32
#define ClientMessage    33
#define MappingNotify    34

#define RaiseLowest  0
#define LowerHighest 1

#endif
