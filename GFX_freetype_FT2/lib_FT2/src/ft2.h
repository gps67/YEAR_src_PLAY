#ifndef FT2__H
#define FT2__H

#include "FT2_STUBS.h"

#include "obj_ref.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace FT2 {

 class ft2 : public obj_ref {
	FT_Library library;
	FT_Face face1;
  public:
  	ft2();
	bool init();
	bool face1_load_font( STR0 filename );
	bool test1();

 };

};

#endif

