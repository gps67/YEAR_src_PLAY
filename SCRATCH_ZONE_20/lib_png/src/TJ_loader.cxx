#include <turbojpeg.h>
#include "TJ_blk.h"
#include "TJ_FB_image.h"
#include "TJ_loader.h"

using namespace TJ;

 TJ_loader_t:: 

	TJ_loader_t:: TJ_loader_t() {
		memset(&xform, 0, sizeof(tjtransform));
		hdr_ColourSpace = 0; // set by tjDecompressHeader3
		hdr_FB_width = 0; // set by tjDecompressHeader3
		hdr_FB_height = 0; // set by tjDecompressHeader3
		req_FB_pixel = 0; // set by caller
	}

	bool TJ_loader_t:: show_info_one(const char * msg) {
	  e_print("%s Image: WxH %dx%d subsampling %s colorspace %s scale %5.3lf \n",
		msg,
		FB_image.width,
		FB_image.height,
		hdr_subsamp.get_name(),
		tbl_CS.name_of_value( hdr_ColourSpace  ),
		req_scale_factor.get_factor()
	  );
	  return true;
	}

	bool TJ_loader_t:: tjInstance_Init_Transform() {

		if(! set_instance_check( tjInitTransform() ))
			return FAIL ("tjInitTransform ERROR %s", get_error_str() );
		return true;
	}

	bool TJ_loader_t:: tjInstance_Init_Decompress() {
		if( tjInstance ) {
			WARN("expected NULL tjInstance");
		}

		// no transform, just decompress
		// or other forward would be here // transparent thru
		if(!set_instance_check( tjInitDecompress() ))
			return FAIL("tjInitDecompress ERROR %s", get_error_str() );
		return true;
	}

	bool TJ_loader_t:: call_transform_and_decompress(
		int flags
	) {
		// flip crop rotate  // AND THEN decompress
		INFO("doing transform");

		if(! tjInstance_Init_Transform())
			return FAIL_FAILED();

		if(! call_transform_and_move_back( flags ))
			return FAIL_FAILED();

		if(! call_DecompressHeaders3_and_get_WH())
			return FAIL_FAILED();

		/*
			NOW WxH is available so could compute scale_factor
			the command line tool has the entire plan from argv
			we might pause, adjust and repeat, or calc, _cb_
		*/

		if(!FB_image.prep_FB_RGBA(
			req_scale_factor,	// from caller
			hdr_FB_width,	// from tjDecompressHeader3
			hdr_FB_height,	// from tjDecompressHeader3
			req_FB_pixel	// from caller
		 ))
			return FAIL_FAILED();

		if(! call_Decompress2( flags ))
			return FAIL_FAILED();

		return FAIL("TODO");
	}

	bool TJ_loader_t:: skip_transform_and_call_decompress(
		int flags
	) {
		INFO("CALL");
		if( tjInstance ) {
			WARN("tjInstance already exists");
			// decompress can use same as transform
			// presuming it was the same original file
		} else {
			if(! tjInstance_Init_Decompress())
				return FAIL_FAILED();
		}

		req_FB_pixel = TJPF_BGRX; // FB request pixelType
		if(! call_DecompressHeaders3_and_get_WH())
			return FAIL_FAILED();

		// NB we dont have pixelFormat of the original file
		// We do get W,H from the headers
		// WE tell TJ what we want ... using ...
		if(!FB_image.prep_FB_RGBA(
			req_scale_factor,	// from caller
			hdr_FB_width,	// from tjDecompressHeader3
			hdr_FB_height,	// from tjDecompressHeader3
			req_FB_pixel	// from caller
		 ))
			return FAIL_FAILED();

		if(! call_Decompress2( flags )) // scale resample here
			return FAIL_FAILED();

		set_instance_NULL(); // spent

		return PASS("TODO");
	}


	bool TJ_loader_t:: call_transform_and_move_back(
		int flags
	) {
		// discard any partial MCU blocks that cant be processed
		// this would have triggered do_call_Transform so set late
		xform.options |= TJXOPT_TRIM;

		int gen_n_images = 1;

		if (tjTransform(
			tjInstance,
			jpeg_file_in_data.buff, // jpeg,
			jpeg_file_in_data.size, // jpeg,
			gen_n_images,		// how many images to generate
			&tmp_transformed.buff,	// tj mallocs this 
			&tmp_transformed.size,	// we manually move it back
			&xform,			// list [1] of transforms
			flags		// propagate this
		) < 0) {
			return FAIL("tjTransform() returned -1 ERROR %s", get_error_str() );
		}

		// move transformed data back // or other forward?
		if(! tmp_transformed.move_to_blk( jpeg_file_in_data ))
			return FAIL_FAILED(); // malloc error
			// continue with same tjInstance //

		return true;
	}

	bool TJ_loader_t:: call_DecompressHeaders3_and_get_WH()
	{
		if (tjDecompressHeader3(
			tjInstance,		// some fields
			jpeg_file_in_data.buff,	// jpegBuf,
			jpeg_file_in_data.size, // jpegSize,
			&hdr_FB_width,		// kept aside
			&hdr_FB_height,		// kept aside
			&hdr_subsamp.val,	// pointer to val
			&hdr_ColourSpace	// kept from hdr_
		) < 0) {
			return FAIL ("decompressing JPEG headers ERROR %s", get_error_str() );
		}

		return true;
	}

	bool TJ_loader_t:: call_Decompress2( int flags ) {

		// you must already have { width height pixelFormat }
		// you should have allocated space - I think
		// all in this->FB_image where FB pixels are written

		// scale_factor is NOT a parameter but happens here
		// must be WxH tht makes it happen
		INFO("WxH %d %d", FB_image.width, FB_image.height );

		if (tjDecompress2(
			tjInstance,
			jpeg_file_in_data.buff,	// data to decompress
			jpeg_file_in_data.size, // data to decompress
			FB_image.img_buf.buff,	// FB_image from data	// pre-allocated
			FB_image.width,		// caller must set from headers
			0,		// bytes_per_row() // autocalc 0 //
			FB_image.height,	// caller must set from headers
			FB_image.pixelFormat,	// caller CAN set OR from headers
			flags			// from argv
		) < 0) 
			return FAIL("decompressing JPEG image ERROR %s", get_error_str() );
		return true;
	}



