#ifndef TJ_loader_H
#define TJ_loader_H

#include "TJ_STUBS.h" // TJ::
#include "TJ_instance.h" // base class
// Y // DONE // #include <turbojpeg.h> // tjhandle tjtransform
#include "TJ_values.h"
#include "TJ_blk.h"
#include "TJ_FB_image.h"
#include "str1.h"

namespace TJ {

 struct TJ_loader_t : public TJ_instance_t {

 	int K_max; // camera typically 1.3 MB // JWST 5.5 // 20 //

	bool set_M_max( int M ) { return set_M_max( (float) M ); }
	bool set_M_max( float M );

	tjtransform xform;

	TJ_blk_t jpeg_file_in_data; // jpeg file loaded into memory // tmp_
	TJ_blk_t tmp_transformed; // transform mallocs and write into here
	TJ_FB_image_t FB_image;
	int hdr_ColourSpace;

	TJ::subsamp_t hdr_subsamp; // as found in input file // copy to _out
	int hdr_FB_width;
	int hdr_FB_height;
	int req_FB_pixel;
	TJ::scale_factor_t req_scale_factor; // from argv


	TJ_loader_t();
	bool show_info_one(const char * msg);
	bool do_do_transform(); // looks at { tjtransform & xform; }
	bool call_transform_and_decompress( int flags );
	bool skip_transform_and_call_decompress( int flags );
	bool call_transform_and_move_back( int flags );
	bool call_DecompressHeaders3_and_get_WH();
	bool call_Decompress2( int flags );

	str1 filename_was;
	bool load_file( const char * filename );

 }; // class
}; // namespace
#endif

