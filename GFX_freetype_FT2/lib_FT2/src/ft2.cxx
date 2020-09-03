
#include "dgb.h"

#include "ft2.h"

using namespace FT2;

 ft2 :: ft2() {
 	if(!init()) throw "FT2_INIT";
 }

 bool ft2 :: init() {
 	int err = FT_Init_FreeType( &library );  	
	if(err) {
		return FAIL("FT_Init_FreeType(&libr)");
	}
	PASS("FT_Init_FreeType");
	return true;
 }

 bool ft2 :: face1_load_font( STR0 filename ) {
	static const int idx_0 = 0; // first font in file
 	int err = FT_New_Face( library, filename, idx_0, &face1 );  	
	if(err) {
		return FAIL("FT_New_Face()");
	}
	PASS("FT_new_face(%s)", filename );
	return true;
  // TODO:
  // load font over network or ...
  // FT_New_Memory_Face( library, font_file_data, font_file_size, idx, &face )
 }



 bool ft2 :: test1() {
 	STR0 font_file = 
	"/usr/share/fonts/truetype/liberation2/LiberationMono-Regular.ttf";

 	if (!face1_load_font( font_file )) return FAIL_FAILED();
	return true;
 }


