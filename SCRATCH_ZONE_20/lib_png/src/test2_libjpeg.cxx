/*
 * Inspired by tjexample.c of the TurboJPEG C API
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <turbojpeg.h>
#include "str0.h"
#include "buffer1.h"
#include "fmt_system.h" // system(...) += sprintf

#include "tbl_enum.h"
#include "tbl_enum_MACROS.h"
#include "TJ_blk.h"
#include "TJ_scale_factor.h"
#include "TJ_values.h" // quality_t subsamp_t
#include "TJ_FB_image_t.h" //
#include "TJ_instance.h"

using namespace TJ;

/*
	TJ has the magic to transform an image without looking at it

	the individual blocks do not need to be decompressed

	I want a much simpler setup,
	to load a JPEG to FB_image
	to save a JPEG from FB_image

	I want to preserve/edit comments - but cant see how

	I want to use the same quality levels, subsamp, ? or do I

	I want to use TJ scale as it is probably sound,
	my resamplig would be clumsy

	SO that gives

		ARGV parser
		IN loader
		IN settings
		THRU bridge
		OUT settings
		OUT saver
	
	and
		ximage from loaded img, from file
		ximage into saver img, from file
	
	why not

		allow SQL BLOB to hold the FILE
		let caller do the FS IO
*/



 ENUM_TBL( "TJSAMP_", tbl_samp, "output resample", rows_samp, { 

	ITEM2( TJSAMP_444,  "4:4:4" )
	ITEM2( TJSAMP_422,  "4:2:2" )
	ITEM2( TJSAMP_420,  "4:2:0" )
	ITEM2( TJSAMP_GRAY, "Grayscale" )
	ITEM2( TJSAMP_440,  "4:4:0" )
	ITEM2( TJSAMP_411,  "4:1:1" )

 })
	// #define TJ_NUMSAMP  6

 ENUM_TBL( "TJPF_", tbl_PF, "pixel format", rows_PF, { // #define TJ_NUMPF  12

	ITEM3( TJPF_RGB,  "RGB", "descr" )
	ITEM3( TJPF_BGR,  "BGR", "descr" )
	ITEM3( TJPF_RGBX, "RGBX", "RGB X bytes order ABCD lohi" )
	ITEM3( TJPF_BGRX, "BGRX", "descr" )
	ITEM3( TJPF_XBGR, "XBGR", "descr" )
	ITEM3( TJPF_XRGB, "XRGB", "descr" )
	ITEM3( TJPF_GRAY, "GRAY", "descr" )
	ITEM3( TJPF_RGBA, "RGBA", "RGB A==0xFF ABCD lohi" )
	ITEM3( TJPF_BGRA, "BGRA", "descr" )
	ITEM3( TJPF_ABGR, "ABGR", "descr" )
	ITEM3( TJPF_ARGB, "ARGB", "descr" )
	ITEM3( TJPF_CMYK, "CMYK", "descr" )
	ITEM3( TJPF_UNKNOWN,"unset", "default for accept INPUT" )

 })


 // CS color space
 ENUM_TBL( "TJCS_", tbl_CS, "pixel format", rows_CS, { // #define TJ_NUMCS  5

	ITEM3( TJCS_RGB,   "rgb",   "descr" )
	ITEM3( TJCS_YCbCr, "ycbcr", "descr" )
	ITEM3( TJCS_GRAY,  "gray",  "descr" )
	ITEM3( TJCS_CMYK,  "cmyk",  "descr" )
	ITEM3( TJCS_YCCK,  "ycck",  "descr" )

 } )

/*
	DCT filter example.  This produces a negative of the image.
	TODO test use how ?
*/
static int customFilter(
	short *coeffs,
	tjregion arrayRegion,
	tjregion planeRegion,
	int componentIndex,
	int transformIndex,
	tjtransform *transform
)
{
  int i;

  for (i = 0; i < arrayRegion.w * arrayRegion.h; i++)
    coeffs[i] = -coeffs[i];

  return 0;
}




static void usage(char *programName)
{
  int i;

  e_print("\nUSAGE: %s filename_input_image filename_output image [options]\n\n",
         programName);

  e_print("Input and output images are usually JPEG format. .ext == .jpg \n");
  e_print("Images may also be Windows BMP or PBMPLUS (PPM/PGM).\n");
  e_print("This example program uses the TurboJPEG API\n");
  e_print("to compress or decompress the image.\n\n");

  e_print("Compression Options (when the output image is JPEG)\n");
  e_print("--------------------------------------------------------------\n\n");

  e_print("-subsamp SUBSAMP\n");
  e_print("     chrominance subsampling when compressing\n");
  e_print("     The default is to use the same as the input JPEG\n");
  e_print("     or grayscale if the input image is a grayscale non-JPEG\n");
  e_print("     image, else use DEFAULT %s \n\n",
	tbl_samp.name_of_value ( subsamp_t::DEFAULT_SUBSAMP )
  );
  e_print("     SUBSAMP must be same spelling as C constant name or alias)\n");
  e_print("     SUBSAMP is case sensitive\n");
  e_print("     SUBSAMP can skip the common prefix 'TJSAMP_' \n");
  e_print("\n");
  e_print("     	'TJSAMP_444' \n");
  e_print("     	'TJSAMP_422' \n");
  e_print("     	'TJSAMP_420' \n");
  e_print("     	'TJSAMP_GRAY' \n");
  e_print("     	'TJSAMP_440' \n");
  e_print("     	'TJSAMP_411' \n");
  e_print("\n");


  e_print("-q QUALITY\n");
  e_print("     QUALITY is number 1 .. 100 \n");
  e_print("     Compress output with JPEG quality level\n");
  e_print("     (default = %d).\n\n", quality_t::DEFAULT_QUALITY);

  e_print("Decompression Options (when input image is JPEG)\n");
  e_print("---------------------------------------------------------------\n\n");

  e_print("-scale M/N = Scale the input image, must be 1/8 .. 16/8 .\n");
  buffer1 buf;
  TJ::scale_factors_t factors;
  e_print("(M/N = %s)\n", factors.list_scaling_factors( buf ));  
  e_print("\n");

  e_print("-hflip,      = mirror flip sideways\n");
  e_print("-vflip,      = mirror flip vertically\n");
  e_print("-transpose,  = TL\\BR flip along diagonal \n");
  e_print("-transverse, = BL/tR flip along diagonal \n");
  e_print("-rot90,      = rotate 90 right (nb clockwise is mathematically -ve )\n");
  e_print("-rot270      = rotate 270 right == 90 left \n");
  e_print("     lossless transformation\n");
  e_print("     done prior to decompressing the input image\n");
  e_print("\n");

  e_print("-grayscale\n");
  e_print("     lossless grayscale conversion of input image\n");
  e_print("     done prior to decompressing the input image\n");
  e_print("\n");

  e_print("-crop WxH+X+Y\n");
  e_print("     crop the input image prior to decompressing it\n");
  e_print("     X and Y specify the upper left corner\n");
  e_print("     W and H specify the width and height.\n");
  e_print("     X and Y must be evenly divible by the MCU block size of the input\n");
  e_print("     (8x8 if compressed using no subsampling or grayscale),\n");
  e_print("     (16x8 if was compressed using 4:2:2 subsampling\n");
  e_print("     (16x16 if was compressed using 4:2:0 subsampling.)\n");
  e_print("\n");

  e_print("General Options\n");
  e_print("---------------\n\n");

  e_print("-fastupsample\n");
  e_print("     Use the fastest chrominance upsampling algorithm available\n");
  e_print("     in the underlying codec.\n");
  e_print("\n");

  e_print("-fastdct\n");
  e_print("      Use the fastest DCT/IDCT algorithms available\n");
  e_print("      in the underlying codec.\n");
  e_print("\n");

  e_print("-accuratedct\n");
  e_print("     Use the most accurate DCT/IDCT algorithms available in the\n");
  e_print("     in the underlying codec.\n");
  e_print("\n");

  exit(1);
}

#define filename_1 "/home/gps/YEAR/G_PHOTOS/Photos_2022/2022-07-0X_Readipop/2022-07-08_Readipop/IMG_20210101_222232.jpg"
#define filename_2 "/tmp/IMG_OUT.jpg"



namespace TJ {


bool display_FB_image( TJ_FB_image_t & FB_image )
{
	return FAIL("TODO");
}

#define THIS_TJ_FAIL( ONESTR ) FAIL("%s ERROR %s", ONESTR, get_error_str() ) 

 struct TJ_loader_t : public TJ_instance_t {

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


	TJ_loader_t() {
		memset(&xform, 0, sizeof(tjtransform));
		hdr_ColourSpace = 0; // set by tjDecompressHeader3
		hdr_FB_width = 0; // set by tjDecompressHeader3
		hdr_FB_height = 0; // set by tjDecompressHeader3
		req_FB_pixel = 0; // set by caller
	}

	bool show_info_one(const char * msg) {
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

	bool tjInstance_Init_Transform() {

		if(! set_instance_check( tjInitTransform() ))
			return THIS_TJ_FAIL("tjInitTransform");
		return true;
	}

	bool tjInstance_Init_Decompress() {
		if( tjInstance ) {
			WARN("expected NULL tjInstance");
		}

		// no transform, just decompress
		// or other forward would be here // transparent thru
		if(!set_instance_check( tjInitDecompress() ))
			return THIS_TJ_FAIL("tjInitDecompress");
		return true;
	}

	bool call_transform_and_decompress(
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

		return THIS_TJ_FAIL("TODO");
	}

	bool skip_transform_and_call_decompress(
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


	bool call_transform_and_move_back(
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
			return THIS_TJ_FAIL("tjTransform() returned -1");
		}

		// move transformed data back // or other forward?
		if(! tmp_transformed.move_to_blk( jpeg_file_in_data ))
			return FAIL_FAILED(); // malloc error
			// continue with same tjInstance //

		return true;
	}

	bool call_DecompressHeaders3_and_get_WH()
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
			return THIS_TJ_FAIL ("decompressing JPEG headers");
		}

		return true;
	}

	bool call_Decompress2( int flags ) {

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
			THIS_TJ_FAIL("decompressing JPEG image");
		return true;
	}

 };

 struct TJ_saver_t : public TJ_instance_t {

	TJ::subsamp_t out_subsamp; // default 444
	quality_t out_quality;
	TJ_blk_t jpeg_file_out_data; // jpeg data to write to file

 bool call_Compress2(
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
		// THIS_TJ_FAIL("initializing compressor");

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
		// THIS_TJ_FAIL("compressing image");
	}
	return PASS("data prepared");
  }
 };

bool do_do_transform( tjtransform & xform ) {
	// the transform call must be made // or not
	// if ANY of these are true
  return
	xform.op != TJXOP_NONE || 	// flip rotate ...
	xform.options != 0 ||		// crop conv-gray
	xform.customFilter != NULL ;	// custom per-pixel
	// a function because there is no xform class
}

}; // namespace TJ

bool bool_main(int argc, char **argv)
{

	TJ::TJ_loader_t tj_loader;
	TJ::TJ_saver_t  tj_saver;

	TJ::scale_factors_t scale_factors;

	// C++ reference // items in saver or loader appear local // rename

	TJ::scale_factor_t & argv_scale_factor = tj_loader.req_scale_factor;;
	tjtransform & argv_xform = tj_loader.xform; // route argv to here
	quality_t & out_quality = tj_saver.out_quality;

	// flags can be shared in/out but docs says use different opts
	int flags = 0;	// used by both loader and saver // diff choices tho

	if (argc < 3) {
		FAIL("expected args: cmd filein fileout argc == %d", argc);
		usage(argv[0]);
	}

	char * in_filename = argv[1];
	char * out_filename = argv[2];

	/* Parse arguments. */
	int i = 3;
	while( i < argc ) { 
	 str0 arg = argv[i++];

	 if( arg == "-scale" ) {
		if(!scale_factors.parse_scale_value(
			argv[i++],
			argv_scale_factor
		)) {
			return FAIL_FAILED();
		}

	 } else if ( arg == "-subsamp" ) {
		if(!tj_saver.out_subsamp.parse_subsamp_value( argv[i++] ) ) {
			return FAIL_FAILED();
		}

	 } else if(( arg == "-q" )||( arg == "-quality")) { // -quality
	 	if(!out_quality.parse_quality_value( argv[i++] ))
			return FAIL_FAILED();

	 } else if( arg == "-grayscale" ) // lossless grayscale conversion
		argv_xform.options |= TJXOPT_GRAY;
	 else if( arg == "-hflip" )
		argv_xform.op = TJXOP_HFLIP;
	 else if( arg == "-vflip" )
		argv_xform.op = TJXOP_VFLIP;
	 else if( arg == "-transpose" )
		argv_xform.op = TJXOP_TRANSPOSE;
	 else if( arg == "-transverse" )
		argv_xform.op = TJXOP_TRANSVERSE;
	 else if( arg == "-rot90" ) // maths says +ve is anti-clockwise, no!
		argv_xform.op = TJXOP_ROT90;
	 else if( arg == "-rot180" ) // TJ uses clockwise +ve
		argv_xform.op = TJXOP_ROT180;
	 else if( arg == "-rot270" )
		argv_xform.op = TJXOP_ROT270; // so this is left turn == 3 x right
	 else if( arg == "-custom" )
		argv_xform.customFilter = customFilter;

	 else if( arg == "-crop" ) {
		if( sscanf(argv[i++], "%dx%d+%d+%d",
		      &argv_xform.r.w,
		      &argv_xform.r.h,
		      &argv_xform.r.x,
		      &argv_xform.r.y ) < 4
		) {
		 return FAIL("-crop expected WxH+X+Y got '%s'", (STR0) arg );
		}
		if( argv_xform.r.x < 0 ||
		    argv_xform.r.y < 0 ||
		    argv_xform.r.w < 1 ||
		    argv_xform.r.h < 1   )
		 return FAIL("-crop WxH+X+Y got NEGATIVE or ZERO '%s'", (STR0) arg );
		argv_xform.options |= TJXOPT_CROP;

	 } else if( arg == "-fastupsample" ) {
		e_print("Using fast upsampling code\n");
		flags |= TJFLAG_FASTUPSAMPLE;

	 } else if( arg == "-fastdct" ) {
		e_print("Using fastest DCT/IDCT algorithm\n");
		flags |= TJFLAG_FASTDCT;

	 } else if( arg == "-accuratedct" ) {
		e_print("Using most accurate DCT/IDCT algorithm\n");
		flags |= TJFLAG_ACCURATEDCT;

	 } else {
		FAIL("bad command line option, got '%s'", (STR0) arg );
		usage(argv[0]);
	 }
	} // loop over argv

	// Determine input and output image formats based on file extensions. 
	// .ext must exist and be 3+ letters
	char *  in_ext = strrchr( in_filename, '.');
	char * out_ext = strrchr(out_filename, '.');
	if(  in_ext == NULL ||
	    out_ext == NULL ||
	    strlen( in_ext) < 2 ||
	    strlen(out_ext) < 2 )
		return FAIL("expected in.jpg out,jpg got %s %s",
			in_filename, out_filename );
	 in_ext =  &in_ext[1]; // skip past dot
	out_ext = &out_ext[1];
	bool outFormat_is_jpg = !strcasecmp(out_ext, "jpg");
	bool  inFormat_is_jpg = !strcasecmp( in_ext, "jpg");
	// do we call Transform or not
	bool do_call_Transform = do_do_transform( argv_xform );

	if( inFormat_is_jpg ) {
	/* Input image is a JPEG image.  Decompress and/or transform it. */
	// unsigned long jpegSize;

	 // maximum input JPEG size ...
	 int KMAX = 1024 * 200; // 200 MB
	// copy data from file to memory
	 if(!tj_loader.jpeg_file_in_data.read_entire_file( in_filename, KMAX ))
		return FAIL_FAILED();

	 if (do_call_Transform) {
		if(!tj_loader.call_transform_and_decompress( flags ))
			return FAIL_FAILED();
	 } else {
		if(!tj_loader.skip_transform_and_call_decompress( flags ))
			return FAIL_FAILED();
	 }
	}


	// transformed total, but not decompressed blocks
	// scaling has NOT been done
	// decompress the headers to get WxH etc

	const char * msg = do_call_Transform ? "Transforme Image" : "Input Image";
	// "Image:  %d x %d pixels, %s subsampling, %s colorspace\n",
	tj_loader.show_info_one(msg);

	// OPTION save after some xform but no compression into JPEG
	// I dont really want that, but it is an option
	if(
		outFormat_is_jpg &&
		do_call_Transform &&
		argv_scale_factor.scale_is_1() &&
		tj_saver.out_subsamp.is_UNSET() &&
		out_quality.is_UNSET()
	) {
	/* Input image has been transformed, and no re-compression options
	 have been selected.  Write the transformed image to disk and exit. */
		if( tj_loader.jpeg_file_in_data.write_as_file(
			out_filename
		)) // mask uid gid
			return PASS("wrote PBM orplain file %s", out_filename );
		else
			return FAIL_FAILED();
	}

	// if subsample unset by ARGV copy value from in_file
	if (tj_saver.out_subsamp.is_UNSET())
		tj_saver.out_subsamp = tj_loader.hdr_subsamp;
	
	if(!display_FB_image( tj_loader.FB_image )) WARN("xdisplay failed");

	if( outFormat_is_jpg ) {
		/* Compress the uncompressed input FB_image. */
		if(! tj_saver.call_Compress2(
			tj_loader.FB_image,
			flags
		)) {
			return FAIL_FAILED();
		}
		/* Write the JPEG image to disk. */
		if(!tj_saver.jpeg_file_out_data
			.write_as_file( out_filename )) // mask uid gid
				return FAIL_FAILED();

	} else {
		/* Write the PBM image to disk. */
		if(!tj_loader.FB_image.img_buf
			.write_as_file( out_filename )) // mask uid gid
				return FAIL_FAILED();
	}

	fmt_system("m '%s'", filename_2);
	return PASS("OK");
 } 

 int main(int argc, char **argv) {

	const char * argv_2[] = {
		"test2_libjpeg.elf",

		filename_1,
		filename_2,
		"-subsamp", "444",
		"-q", "100",

		//	"-scale", "1/8",
		"-scale", "0.4",
		// 16/8 down to 1/8 or normalised eg 1/2 from 4/8

		// only 1 of these // or 0 
		//	"-hflip",
		//	"-vflip",
		//	"-transpose",
		//	"-transverse",
		//	"-rot90",	// 90 right
		//	"-rot180",	// 270 right == 90 left
		//	"-rot270",

		//	"-greyscale",

		//	"-crop", "200x100+50+50",

		//	"-fastupsample",
		//	"-fastdct",
		//	"-accuratedct",

		NULL
	};
	int argc_2 = N_ARY( argv_2 ) - 1; // because of added NULL

	// untried
	// Argv_scanner argv_scanner( argc, argv );
	// argv_scanner.register_program_name();

	// this converted the core dump into a gdb invoke
	// gdb_sigaction( argv[0] ); // sets progname_argv0
	// Segmentation fault (core dumped) // message from libc? shell ?
	// exit code 139 

	if( argc == 1 ) {
		argv_2[0] = argv[0]; // progname
		argc = argc_2;
		argv = (char **) argv_2;
	}
	// INFO("argc == %d", argc );
	// above gives us a default ARGV
	if( bool_main( argc, argv ) ) {
		if(0) usage(argv[0]); // test anyway
		PASS("return 0");
		return 0;
	} else {
		if(errno) return errno;
		return 22;
	}
 }
