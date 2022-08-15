#ifndef TJ_saver_H
#define TJ_saver_H

#include "TJ_STUBS.h" // TJ::
#include "TJ_instance.h"
#include "TJ_values.h"
#include "TJ_FB_image.h"
#include "TJ_STUBS.h"

namespace TJ {

 struct TJ_saver_t : public TJ_instance_t {

	TJ::subsamp_t out_subsamp; // default 444
	quality_t out_quality;
	int flags;
	TJ_blk_t jpeg_file_out_data; // jpeg data to write to file

	TJ_saver_t();

	bool call_Compress2(
		TJ_FB_image_t & FB_image
	);

	bool get_pixmap_save_as_filename(
		X_Display & disp, 
		X_Pixmap & pixmap,
		const char * filename 
	);

 }; // class
}; // namespace
#endif


