#ifndef tty_curses_UDLR_H
#define tty_curses_UDLR_H
#include "XYWH_16.h" // for XY_t XY; for JB.XY
using namespace XYWH_16;

namespace TTY_CURSES {

	  static int char_of_udlr[16];
	// these are my box_chars // ? char_attr_t
	  void init_char_of_udlr();

	 enum enum_UDLR
	 {
	 	UDLR_____  =   0,
	 	UDLR_U___  = 0x8,
	 	UDLR__D__  = 0x4,
	 	UDLR___L_  = 0x2,
	 	UDLR____R  = 0x1,
	 	UDLR_UDLR  = 0xF,

	 	UDLR_UD__  = UDLR_U___  + UDLR__D__,
	 	UDLR___LR  = UDLR___L_  + UDLR____R,

	 	UDLR_U_L_  = UDLR_U___  + UDLR___L_,
	 	UDLR_U__R  = UDLR_U___  + UDLR____R,
	 	UDLR__DL_  = UDLR__D__  + UDLR___L_,
	 	UDLR__D_R  = UDLR__D__  + UDLR____R,

	 	UDLR__DLR  = UDLR_UDLR  - UDLR_U___,
	 	UDLR_U_LR  = UDLR_UDLR  - UDLR__D__,
	 	UDLR_UD_R  = UDLR_UDLR  - UDLR___L_,
	 	UDLR_UDL_  = UDLR_UDLR  - UDLR____R
	 };

	/*!
		UDLR udlr; // holds Up Down Left Right Flags
	*/
	struct UDLR {

	  enum_UDLR udlr;
	  UDLR( enum_UDLR _udlr = UDLR_____ ) : udlr( _udlr) {};

	  void clear() { udlr = UDLR_____ ; }
	  void udlr_clear() { clear(); } // inherit makes clear() muddy

	  // default zer0 UDLR() 


	  int get_acs_char();
#if 0
	  int get_acs_char() {
	    // init on first use // use item0 instead of extra bool var;
	    if(0 == char_of_udlr[ 0 ] )
	      init_char_of_udlr();
	    return char_of_udlr[ (int) udlr ];
	   }
#endif

	 void OR_VAL(enum_UDLR rhs) {
		 udlr = enum_UDLR(((int) udlr ) | (int) rhs );
	 }
	 void OR_VAL(UDLR rhs) {
	 	OR_VAL( rhs.udlr );
	 }
	 void set_U() { OR_VAL( UDLR_U___); }
	 void set_D() { OR_VAL( UDLR__D__); }
	 void set_L() { OR_VAL( UDLR___L_); }
	 void set_R() { OR_VAL( UDLR____R); }

	// CC can figure this, local int scope
	  void set_UDLR (
	   bool U,
	   bool D,
	   bool L,
	   bool R
	  ) {
		int     iUDLR   = (int) UDLR_____;
		if( U ) iUDLR  += (int) UDLR_U___;
		if( D ) iUDLR  += (int) UDLR__D__;
		if( L ) iUDLR  += (int) UDLR___L_;
		if( R ) iUDLR  += (int) UDLR____R;
		enum_UDLR UDLR = ( enum_UDLR ) iUDLR;
	 }

  }; // class

  /*!
  	Junction Box


  */
  struct JB_t : public UDLR {
   XY_t XY;

   JB_t() {} // udlr.clear() XY=00
   JB_t( XY_t _XY ) : UDLR(), XY(_XY) {}
   operator XY_t () { return XY;  }
  };

}; // namespace
#endif
