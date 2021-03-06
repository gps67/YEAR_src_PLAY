#pragma implementation "tty_curses_UDLR.h"
#include "tty_curses_UDLR.h"
#include "tty_curses_CSR.h" // wrong


namespace TTY_CURSES {

#define UDLR_CHAR( BITS, VAL ) char_of_udlr[ (int) BITS ] = (int)(VAL);
	 void init_char_of_udlr()
	 {
	 	// was in BYTE_u4 order for ITEM_t item == VECT_DATA[ u4 ]
		 UDLR_CHAR( UDLR_____, 'X' )
		 UDLR_CHAR( UDLR____R, ACS_HLINE ) // HALF LINE not avail
		 UDLR_CHAR( UDLR___L_, ACS_HLINE ) // HALF LINE not avail
		 UDLR_CHAR( UDLR___LR, ACS_HLINE )
		 UDLR_CHAR( UDLR__D__, ACS_VLINE ) // HALF LINE not avail
		 UDLR_CHAR( UDLR__D_R, ACS_ULCORNER )
		 UDLR_CHAR( UDLR__DL_, ACS_URCORNER )
		 UDLR_CHAR( UDLR__DLR, ACS_TTEE ) // TT not UT
		 UDLR_CHAR( UDLR_U___, ACS_VLINE ) // HALF LINE not avail
		 UDLR_CHAR( UDLR_U__R, ACS_LLCORNER ) 
		 UDLR_CHAR( UDLR_U_L_, ACS_LRCORNER )
		 UDLR_CHAR( UDLR_U_LR, ACS_BTEE )
		 UDLR_CHAR( UDLR_UD__, ACS_VLINE )
		 UDLR_CHAR( UDLR_UD_R, ACS_LTEE )
		 UDLR_CHAR( UDLR_UDL_, ACS_RTEE )
		 UDLR_CHAR( UDLR_UDLR, ACS_PLUS )
	};

	  int UDLR:: get_acs_char() {
	    // init on first use // use item0 instead of extra bool var;
	    if(0 == char_of_udlr[ 0 ] )
	      init_char_of_udlr();
	    return char_of_udlr[ (int) udlr ];
	   }

}; // namespace
