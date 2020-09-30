
#include "dgb.h"

#include <math.h> // sin cos
#include "ft2.h"

// file:///home/gps/2020/src_build_2020/DTP/freetype-2.10.2/docs/reference/ft2-basic_types.html#ft_bitmap

// https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_glyphslotrec

// ft2demos-2.10.2/graph/x11/grx11.c
// some BLIT Image X11



// #define WIDTH   640
// #define HEIGHT  480

#define WIDTH    70
#define HEIGHT   50


/* origin is the upper left corner */

struct homebrew_pixmap_grey_t
{
	unsigned char image_bytes[HEIGHT][WIDTH];

	void set_xy_byte(
	  FT_Int x,
	  FT_Int y,
	  u8 val
	) {
	      image_bytes[y][x] |= val;
	}

	u8 get_xy_byte(
	  FT_Int x,
	  FT_Int y
	) {
		u8 grey = image_bytes[y][x];
		return grey;
	}

} image;


/* Replace this function with something useful. */

void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;


  /* for simplicity, we assume that `bitmap->pixel_mode' */
  /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0      || j < 0       ||
           i >= WIDTH || j >= HEIGHT )
        continue;

	// COPYING FT_Bitmap to HOMEBREW_t image_bytes
	image.set_xy_byte( i, j, 
	      bitmap->buffer[q * bitmap->width + p ]
	);
    }
  }
}

void
show_image( void )
{
  INFO("HERE");
  int  i, j;
  const char * hexes = " 123456789ABCDEF";
//  hexes = "    ----++++****";

  for ( i = 0; i < HEIGHT; i++ )
  {
    for ( j = 0; j < WIDTH; j++ ) {
      u8 grey = image.get_xy_byte( j, i );
      putchar( hexes[ grey >> 4  ] );
    }
    putchar( '\n' );
  }
  fflush(0);
}



using namespace FT2;

 // static var and init
 bool ft2:: init_done = false;

 ft2 :: ft2() {
 	if(!init()) throw "FT2_INIT";
 }

 ft2 :: ~ft2() {
 	if(!done()) FAIL_FAILED();
 //	if(!done()) throw "FT2_DONE"; // dont throw // 
 }

 bool ft2 :: init() {
 	if(init_done) {
		WARN("already init_done");
		return true; // but OK
	}
	// LOCK HERE?
	if(!FT2_OK( FT_Init_FreeType( &library ))) {
		return FAIL("FT_Init_FreeType(&libr)");
	}
	init_done = true;
	PASS("FT_Init_FreeType");
	return true;
 }

 bool ft2 :: done() {
 	if(!init_done) {
		WARN("already init_done false");
		return true; // unusual but OK
	}
	// LOCK
	init_done = false;
	if(!FT2_OK( FT_Done_FreeType( library ))) {
		return FAIL("FT_Done_FreeType(&libr)");
	}
	PASS("FT_Done_FreeType");
	return true;
 }

 bool ft2:: FT2_OK( int errr )
 {
  if( errr ) {
  	error = errr;
  	return WARN("FT2 %d - %s", errr, FT_Error_String( errr ));
  } else {
  	return true;
  }
 }

 bool ft2 :: face1_load_font( STR0 filename ) {
	static const int idx_0 = 0; // first font in file
 	if( !FT2_OK( FT_New_Face( library, filename, idx_0, &face ))) {
		INFO("file %s", filename );
		return FAIL("FT_New_Face()");
	}
	PASS("FT_new_face(%s)", filename );
	return true;
  // TODO:
  // load font over network or ...
  // FT_New_Memory_Face( library, font_file_data, font_file_size, idx_0, &face )
 }


#define int_from_fixed_6(x) ((x+32)>>6) // +32 rounds to 64
// #define FT_Fixed_from_float(f)( (FT_Fixed)( (f) * (float) 0x10000L )) // << 16
#define FT_Fixed_from_float(f)( (FT_Fixed)( (f) * 0x10000L )) // << 16
#define PI 3.1415926
#define rads_from_degress( degs ) ( 2.0 * PI * degs / 360.0 ) //

  // angle above horizontal, anti-clockwise
  // -ve below horizontal, clockwise
  // float angle = rads_from_degress( -23.0 );

 bool ft2 :: test1() {
 	STR0 font_file = 
	"/usr/share/fonts/truetype/liberation2/LiberationMono-Regular.ttf";

 	if (!face1_load_font( font_file )) return FAIL_FAILED();
	// VALID FT_Face face // face

	// 64 fixed point <<6 // 16 ??
	// FT_Set_Char_Size( face, w0, h_16x64, dpi_h_300, dpi_v_300 )
	// ppem // pixels per em // 16 is 12pt ?
	int w0 = 0; // same as h
	int point_size = 36;
	int h_16x64 = point_size * 64;
	int dpi_x_300 = 100;
	int dpi_y_300 = 100;
	dpi_x_300 = 35;
	dpi_y_300 = 20;
	if(!FT2_OK( FT_Set_Char_Size( face, w0, h_16x64, dpi_x_300, dpi_y_300 ))) {
		return FAIL("FT_set_Char_Size()");
	}

	// FT_Set_Pixel_Sizes( face, w0, h16 )
	// face->fixed_sizes // lists allowed

	int charcode = '@'; // 32 bit utf-32
	int glyph_index = FT_Get_Char_Index( face, charcode );

	slot = face->glyph;

  STR0 text          = "text";
  float angle = rads_from_degress( -23.0 );
  angle = rads_from_degress( -3.0 );


  // rotatoe counter clockwise by angle around (0,0)
  // ( cos -sin ) ( v1.x ) = (V2.X)
  // ( sin  cos ) ( V1.y ) = (V2.y)
  //
  // M V1 == V2
  //
  // counter clockwise: Y-axis from X-axis rotated 90 left counter-clockwise
  // "right handed coordinate system"
  // for moving VECTORS on axes, not moving the AXES themselves ?

  matrix.xx = FT_Fixed_from_float( cos( angle ) );
  matrix.xy = FT_Fixed_from_float(-sin( angle ) );
  matrix.yx = FT_Fixed_from_float( sin( angle ) );
  matrix.yy = FT_Fixed_from_float( cos( angle ) );

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
  pen.x = 3 * 64;
  pen.y = ( HEIGHT - 2 ) * 64;
  pen.y = 20 * 64;

  int n = strlen( text );
  for( int i = 0; i < n; i++ )
  {
    int chr = text[i];

    // VOID
    FT_Set_Transform( face, &matrix, &pen );

    /* load glyph image into the slot (erase previous one) */
    if( !FT2_OK(
      FT_Load_Char( face, chr, FT_LOAD_RENDER )
    )) {
    	// error absent char
    	continue;
    }

    // draw glyph bitmap to zone bitmap
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 HEIGHT - slot->bitmap_top );

    /* pen advance by rotated slot advance */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  show_image();

  FT_Done_Face( face );


#if 0
	int load_flags =  FT_LOAD_DEFAULT; // 0 // with bitmap
	FT2_err = FT_Load_Glyph(
		face,
		glyph_index,
		load_flags
	);
	if(FT2_err) return FAIL("FT_Load_Glyph()");

#define face_glyph_metrics face->glyph->metrics
// SEARCH REPLACE OPTION Optimising_Instance common_expr

//DIALECT
// FT_Pos face_glyph_metrics.%s field_name "horiBearingX";
//
//	EXPR = face->glyph->metrics;
//

	FT_Pos left  =        face->glyph->metrics.horiBearingX;
	FT_Pos right = left + face->glyph->metrics.width;
	FT_Pos top   =        face->glyph->metrics.horiBearingY;
	FT_Pos bottom = top - face->glyph->metrics.height;


	int glyph_rect_w = int_from_fixed_6(right - left) + 1;
	int glyph_rect_h = int_from_fixed_6(top - bottom) + 1;

	#if 0
	setFixedSize(
	 glyph_rect_w,
	 glyph_rect_h
	);
	m_glyphRect.width(),
	m_glyphRect.height());
	#endif
#endif

	return PASS("OK");
	return true;
 }


