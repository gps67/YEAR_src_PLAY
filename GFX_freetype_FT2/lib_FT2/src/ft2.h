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

	FT_F2Dot14 // i16_p14  -3[-2.9 .. +1.9[2
	FT_UnitVector // i16_p14 XY  near 1.x 0.x -1.x -2.x 

	FT_F26Dot6 // i32_p6 += 32M.1/64

	FT_Matrix { xx xy yx yy }
#endif


namespace FT2 {

 /*!
 	one instance per multi-thread multi-cursor (IDK)

	ref_counted so DTOR does work
 */
 class ft2_library_t : public obj_ref {

 	// The LIBR needs a singleton to init
	// we then have one instance per document CSR
	// so init_done is a static


  	ft2_library_t();
  	~ft2_library_t();
 };

 class ft2 : public obj_ref {

	static bool init_done; // init syntax // bool ft2:: init_done = false;

	FT_Library library;

	obj_hold<ft2_library_t> ft2_library;

	FT_Face face;

	FT_GlyphSlot  slot; // = face->gylph
	FT_Matrix     matrix; // fixed_16p16

	FT_Vector     pen;  // pen_pos XY fixed_26p6
	FT_Error      error; // not cleared ...

  public:
	// PATCH // SEGMENT.ON_INIT.set_ROM_init_done_bool_false // or SIMPLER
	// OPTION // set_rom_varname_type_initval ARGV //
	// ARGV // create_on_first_mention

  	ft2();
  	~ft2();
	bool FT2_OK( int err ); // 0 means OK // convert to bool // report
	bool init();
	bool done();
	bool face1_load_font( STR0 filename );

	// start new users as SELF.field or THAT.field
	// so if it becomes generic reused code in this class
	// it is already there
	// common extra might be derive class
	// then add {" bool test1() ; "); PARSE_using simplistic BYTE expect
	// BYTE EXPECT "SP '(' ')' SP SEMICOLON SP Q2 // X == 'C' // C == lookup
	// BYTE EXPECT "SP { %s test1 } SP " // CODE_ASIF SP_MERGE GAP_DETECT
	// FSM uses almost identical STEP( " SP " ) // MERGE_SP will catch XS
	// LEX uses LEX_EDGE_GAP_LEX_EDGE where PARSE(" SP ") // " SP " compiled
	// CACHE in CXX and in ES6 // upgrade "{ STR0_t STR0 }" "ALIAS" VAR_NAME
	// PARSE ALLOW GAP
	// PARSE EXPECT GAP _empty_GAP_is_often_OK as is more_than_SP_GAP
	// PARSE ALLOW GAP to be EMPTY_GAP NO_GAP
	// PARSE EXPECT GAP _incl_ABSENT_GAP // ZLEN_GAP_FOUND // INFER_GAP
	// happens at cident99 GAP LPAR_RPAR  // LPAR_RPAR auto expands
	// LPAR_RPAR "()" PARSE(" GAP LPAR GAP ZONE RPAR GAP ") then DECL_ZONE
	// DECL_ZONE gets a MEDULE already started with PARSE("test1()"); // ";"
	// DECL_MODE AUTO_ADD_SEMICOLONS // long list of VIEW switches in MODULE
	// VIEW_SWITCHES // AUTO_ADD("_LIST += AUTO_ADD PARSE_OPTIONAL_SEMICOLON
	// UNPARSE forks CSR into next_token_pre_parsing += DIAG += STO
	// XPOS_CSR // a CSR that uses "{ FILE OFFS }" or "{ XPOS }" // FOLD_USED_DECL
	// USED += AVAR("VAR_NAME")
	// DECL += "{ AVAR VAR_NAME }" // presume "{ VAR_NAME "VAR_NAME" }"
	// EXPR += "{ DECL += EXPR }"
	// ITEM += DECL USED XPOS // more this XPOS are available via EXPR
	//
	// MATCH("test1()"
	bool test1();

 };

};

#endif

