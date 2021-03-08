#include <stdlib.h>
#include "lex_yacc_LEX_TOKEN_precedence.h"

using namespace PSG;

// using namespace EXPRS;


	void EXPRS:: operator_precedence_t::
	set_flags_str( str0 flags ) //! parse flags into self, eg "L3" "L1"
	{
		if(!flags) return;
		char * P = (char *) flags; // unconst
		while( u8 c = (u8) *P ) {
		 P++;
		 switch(c) {
		  case 'L' :
		  	set_left();
		  break;
		  case 'R' :
		  	set_right();
		  break;
		  case ' ': // skip space
		  case '\t':
		  break;
		  	// BAIL out at EOLN
		  case '\n':
		  case '\r':
		  case '#': // comment
			 P = (str0) ""; // break while
		  break;
		  case '1' ... '9': // digit
		  	P--;
			int val = strtol( P, &P, 0 );
			set_precedence( val );
		 }
		}
	}

