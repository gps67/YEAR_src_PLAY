
// : public XPoint
// : XRectangle

#ifndef X_STUBS_H
#define X_STUBS_H

// stdarg - see e_print.h
// typedef char * va_list;

struct buffer1;


// X11/X.h defines some basis things // ok if duplicated
typedef long unsigned int XID;
typedef XID Window;
struct _XGC;
typedef struct _XGC * GC ;

struct _XDisplay;
typedef struct _XDisplay Display;

namespace WAX {
	struct A_Point;
	struct A_XY;
	struct A_WH;
	struct A_Rectangle;
	struct X_Colours;
	struct X_Display_One;
	struct X_Display;
	struct X_Draw;
	struct X_Window;
	struct X_Pixmap;
	struct X_Image;
};

#endif
