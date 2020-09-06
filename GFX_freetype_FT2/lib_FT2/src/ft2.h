#ifndef FT2__H
#define FT2__H

#include "FT2_STUBS.h"

#include "obj_ref.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#if 0
file:///nfs/NAS2/mnt/HD/HD_b2/2020/src_build_2020/DTP/freetype-2.10.2/docs/reference/ft2-basic_types.html

	typedef signed long FT_Pos;
	FT_Vector { x, y } // FT_Pos
	FT_BBOX { xMin yMin xMax yMax }
	FT_Fixed i32_fixed_p16

	FT_F2Dot14 // i16_p14
	FT_UnitVector // near 1.x 0.x -1.x -2.x

	FT_F26Dot6 // i32_p6 

	FT_Matrix { xx xy yx yy }
#endif


namespace FT2 {

 class ft2 : public obj_ref {
	// we need this to be a singleton
	// so move to a static
	FT_Library library;
	FT_Face face;

	FT_GlyphSlot  slot; // = face->gylph
	FT_Matrix     matrix;

	FT_Vector     pen;  
	FT_Error      error;

  public:
	static bool init_done; // move to private // init how // syntax
	// static bool init_done = false; // C++ says cant set default init here
	// PATCH // SEGMENT.ON_INIT.set_ROM_init_done_bool_false // or SIMPLER
	// OPTION // set_rom_varname_type_initval ARGV //
	// ARGV // create_on_first_mention

  	ft2();
  	~ft2();
	bool FT2_OK( int err ); // 0 means OK // convert to bool // report
	bool init();
	bool done();
	bool face1_load_font( STR0 filename );
	bool test1();

 };

};

#endif

