#ifndef XYWH_16_H
#define XYWH_16_H

// can I have a template namespace XYWH_%s "16" type XYWH_t XYWH<i16>
// can I have a template namespace XYWH_%s "32" type XYWH_t XYWH<i32>
// can I have a template namespace XYWH_%s "B3" type XYWH_t XYWH<u8_u24_hilo>
// can I have a template namespace XYWH_%s "64" type XYWH_t XYWH<u48_u16_hilo>
// PTR = PAYLOAD_64 >> 16; // fill_zero drop_u16_OPCODE u8_u8
// OFFS = PAYLOAD_32_HI || PAYLOAD_48_shifted_to_zero // _baseline
// INDX == IDX = u16_lo || EXPR_u48 DECODED_u48 // += CTXT
// VIEW += CTXT // CTXT += DECODE_u48 // DECODER = .
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

  X_t X_first() { return XY.X; }
  Y_t Y_first() { return XY.Y; }

  X_t X_next() { return XY.X + WH.W; }
  Y_t Y_next() { return XY.Y + WH.H; }

  X_t X_last() { return XY.X + WH.W - 1; }
  Y_t Y_last() { return XY.Y + WH.H - 1; }
 };

}; // namespace
#endif

