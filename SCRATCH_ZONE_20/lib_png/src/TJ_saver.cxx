#include "TJ_saver.h"
#include "X_Image.h"
#include "fmt_system.h"

using namespace TJ;

 TJ_saver_t:: TJ_saver_t() {
 	// out_quality // own CTOR
 	flags = 0;
	// errm - maybe
	flags |= TJFLAG_FASTUPSAMPLE;
	flags |= TJFLAG_FASTDCT;
	flags |= TJFLAG_ACCURATEDCT;
 }

 bool TJ_saver_t:: call_Compress2(
		TJ_FB_image_t & FB_image
 ) {
	// out_quality -1 means unset on command line, or input
	int quality = out_quality.get_val(); // conv -1 to DEFAULT

	e_print(", %s subsampling, quality = %d\n",
		out_subsamp.get_name(),
		out_quality.get_val()
	);

	e_print("Output Image (JPEG):  %d x %d pixels",
		FB_image.width,
		FB_image.height
	);
 
	if(! set_instance_InitCompress() )
		return FAIL_FAILED();

	if (tjCompress2(
		tjInstance,
		FB_image.img_buf.buff, // imgBuf, // input to func
		FB_image.width,
		0,		// autocalc // pitch == width * bytes_per_pixel 
		FB_image.height,
		FB_image.pixel_format_tj,
		&jpeg_file_out_data.buff,	// jpegBuf,
		&jpeg_file_out_data.size, // jpegSize,
		out_subsamp.get_val(),
		out_quality.get_val(),
		flags) < 0
	) {
		FAIL("compressing image ERROR %s", get_error_str() );
	}
	return PASS("data prepared");
  }


bool TJ_saver_t:: get_pixmap_save_as_filename(
	X_Display & disp, 
	X_Pixmap & pixmap,
	const char * filename 
) {
	X_Image img;
	if(! img.get_ximage_from_pixmap( disp, pixmap ))
		return FAIL_FAILED();
	// NOTE img holds memory that img_tj uses

	TJ:: TJ_FB_image_t img_tj;
	img.put_ximage_into_TJ_IMG( img_tj );

	// default quality and flags
	if(! call_Compress2( img_tj )) return FAIL_FAILED();
	// can now release ximage // holder of data

	if(! jpeg_file_out_data.write_as_file( filename ))
		return FAIL_FAILED();

	fmt_system("m %s", filename );
	return PASS("where is %s", filename);

}

