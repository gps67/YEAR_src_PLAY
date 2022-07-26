#ifndef TJ_FB_image_t_H
#define TJ_FB_image_t_H
namespace TJ {

 struct TJ_FB_image_t {

	TJ_blk_t img_buf; // FB data

	int width;
	int height;
	int pixelFormat;

 TJ_FB_image_t()
 : img_buf()
 , width(0)
 , height(0)
 , pixelFormat( TJPF_UNKNOWN )
 {
 }

 bool allocate_img_buf() {
	if(!img_buf.tj_alloc( bytes_for_image() )) 
		return FAIL("allocate image FB buffer ERROR %s",
		 "TODO find instance for get_error_str()");
 	return true;
 }

 int bytes_per_pixel() {
 	// other pixel formats are close to being supported by me
	// and needs a few samples and tests, until then it is u32_RGBA only
	// or other 4-byte values
	// eg when 1 byte bytes_per_row is = TJPAD( width ) // mult of 4

	int t = tjPixelSize[pixelFormat] ;
	if( t != 4 ) {
		FAIL("expected 4 got %d", t );
	}
	return t;
 }

 int bytes_per_row() { // includes padding
 	int pitch1 = width * bytes_per_pixel();
 	int pitch4 = TJPAD( pitch1 );
	if( pitch4 == pitch1 ) {
		INFO("pitch1 already aligned %d", pitch1 );
	} else {
		INFO("pitch4 <> pitch1 %d != %d ", pitch4, pitch1 );
	}
 	return TJPAD( width * bytes_per_pixel());
 }

 int bytes_for_image() {
	// INFO("WxH %d %d", width, height );
	return height * bytes_per_row();
	return width * height * tjPixelSize[pixelFormat];
 }

	bool prep_FB_RGBA( 
		scale_factor_t & scale_factor,
		int _width,
		int _height,
		int pix_fmt_RGBA = TJPF_BGRX
	) {
		if( pix_fmt_RGBA != TJPF_BGRX ) {
			WARN("expected %d got %d", TJPF_BGRX, pixelFormat );
			WARN("bytes_per_pixel %d", bytes_per_pixel() );
		}
		return prep_FB( scale_factor, _width, _height, pix_fmt_RGBA );
	}

	bool prep_FB( 
		scale_factor_t & scale_factor,
		int _width,
		int _height,
		int pix_fmt_RGBA
	) {
		// multiply 
		scale_factor.apply_to_W_H( _width, _height );
		width = _width;
		height = _height;
		pixelFormat = pix_fmt_RGBA; // TJPF_BGRX;
	//	INFO("WxH %d %d", width, height );
	//	INFO("scale %lf", scale_factor.get_factor() );

		// preparing _FB means allocate buffer or size
		if(!allocate_img_buf())
			return FAIL("allocate image FB buffer");

		// seems that scale_factor ONLY works by setting WxH
		// as it is never passed as a parameter

		return true;
	}

 /*
 	no point writing EA_pixel EA_row etc
	as ximage can do that
	as Pixmap can do all drawing things that X11 can

	some point in getting img_buf to ximage
	copy or directly
	temp or lasting

	see JPEG as a single function WRITE_IMAGE_TO_JPEG_BUFFER

	use SCALE to nearest 1/8 2/18 .. 16/8 - probably clever stuff
	beware CROP to multiple of 8 or 16

	LOST comments in jpeg, completely !! tiff had that?
 */

 }; // class
}; // namespace
#endif
