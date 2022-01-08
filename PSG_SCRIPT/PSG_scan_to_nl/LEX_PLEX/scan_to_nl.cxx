#include "scan_to_nl.h"

bool scan_to_nl::GAP_after_LEX()
{
	GAP_after_LEX_found = false;;
	P_start_lex = P_start_next;	// old end WAS new beginning
	P_start_gap = P;		// this is where we are
	// HERE is the start of the GAP
	// derived class would parse gap here ...
	// see scan_nl_C_LEX
	while(1) {
		u8 c = *P;
		switch(c) {
		 case ' ':
		 case '\t':
		 case '\r':
			P++;
			continue;
		 case '\n':
			if( scan_nl() ) continue;
		}
		break;
	}
	P_start_next = P;		// just in case
	if( P_start_gap < P_start_next )
		GAP_after_LEX_found = true;
	return GAP_after_LEX_found;
}

/*
	XXX_expect section
*/

bool	scan_to_nl::scan_char_expect( u8 literal )
{
	bool ok = scan_char( literal );
	if(ok) return ok;
	report_FAIL( "expected '%c'", literal );
	return false;
}

bool	scan_to_nl::scan_word_expect( const u8 * word )
{
	bool ok = scan_word( word );
	if(ok) return ok;
	report_FAIL( "expected '%s'", word );
	return false;
}

