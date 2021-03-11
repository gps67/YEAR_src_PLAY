#include <stdlib.h>
#include "lex_yacc_LEX_TOKEN_precedence.h"

using namespace PSG;

// using namespace EXPRS;

/*
	NOTE unary minus is assoc_none
	but it is confusing with binary minus (assoc left)
	LEX/YACC can resolve this by NOT having a LEX_TOKEN for unary_minus
	but then having an (RTFM) %assoc_none in the YACC rule
	so I'm not sure what this is, maybe retro fitting a second minus
	but that would need a flag, do not regen what was never there
*/


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
		  case 'N' : // or is this daft ?
		  	set_none();
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

