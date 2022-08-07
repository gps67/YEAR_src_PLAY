#ifndef IMG_loader_H
#define IMG_loader_H

#include "X_STUBS.h"

 namespace TJ {
	 struct TJ_FB_image_t;
 };


 namespace WAX {
	bool show_image_data( XImage * ximage );
	bool call_create_ximage_from_png( X_Display & disp, XImage * & ximage, png_one & png );
	bool call_put_image_to_pixmap( X_Display & disp, GC gc, XImage & ximage, X_Pixmap & pixmap ); // to pixmap // to drawable might be Pixmap
	bool create_pixmap_from_ximage( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, XImage & ximage );
	bool create_pixmap_from_png( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, png_one & png );

	bool call_create_ximage_from_TJ_IMG( X_Display & disp, XImage * & ximage, TJ:: TJ_FB_image_t & img );
	bool create_pixmap_from_TJ_IMG( X_Display & disp, GC gc, Drawable drawable, X_Pixmap & pixmap, TJ:: TJ_FB_image_t & img );
 }; // WAX

#endif
