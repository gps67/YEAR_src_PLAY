#include "TJ_saver.h"

using namespace TJ;

 bool TJ_saver_t:: call_Compress2(
		TJ_FB_image_t & FB_image,
		int flags
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
		FB_image.pixelFormat,
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

