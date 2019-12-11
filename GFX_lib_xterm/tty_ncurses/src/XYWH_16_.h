#ifndef XYWH_16_H
#define XYWH_16_H

// can I have a template namespace XYWH_%s "16" type XYWH_t XYWH<i16>
// can I have a template namespace XYWH_%s "32" type XYWH_t XYWH<i32>
// can I have a template namespace XYWH_%s "B3" type XYWH_t XYWH<u8_u24_hilo>
// then item is ROM code for VAR in STO via CACHE and gcc_register ITEM_t * PTR
// that requires PTR = get_EA_of_VAR_lookup("anystr");
// that enables TOKEN_for_anystr = CT_lookup("anystr"); // compile run tracer
// tracer magically make itself look like an u32_decoded u32_unpacked
// u32_unpacked == u16_u8_u8_hilo

namespace XYWH_16 {

typedef short i16 ;
typedef i16 X_t ;
typedef i16 Y_t ;

 struct XY_t {
  X_t X;
  Y_t Y;

  XY_t(
  	X_t _X,
  	Y_t _Y
  )
  : X( _X )
  , Y( _Y )
  {
  }

  XY_t() { X=Y=0; }

  bool operator== ( const XY_t & rhs )
  {
  	bool expr = ( (X == rhs.X) && (Y == rhs.Y) );
	return expr; // debugging is easier with these unfolded terms
	// macro_compiler knows gcc will fold constants, EA direct access,
	// inline works
	// which means we have to make it work for us
	// first tho, emulate how JIT PREBUILT adds to SEGMENT.bss .data .malloc
	// get GCC compiler (clang, etc), almost proper C++,
  }
  bool operator!= ( const XY_t & rhs ) { return !(*this == rhs); }
 };

 struct WH_t {
  X_t W;
  Y_t H;
 };

 struct XYWH_t {
  XY_t XY;
  WH_t WH;
 };

}; // namespace
#endif

