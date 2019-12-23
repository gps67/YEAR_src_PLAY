#include <stdlib.h>
#include "Tree_PSG_LEX_TOKEN_precedence.h"


// using namespace EXPRS;


	void EXPRS:: operator_precedence_t::
	set_flags_str( str0 flags ) // move to not inline
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

