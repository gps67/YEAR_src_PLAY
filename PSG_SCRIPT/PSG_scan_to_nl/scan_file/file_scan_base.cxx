#include "file_scan_base.h"	// includeds the following

file_scan_base::file_scan_base()
{
}

/*
	CALLER specifies _Jan ( int & ret_val ) // retval == mm // [01 12] // 

	Here we Parse for the UK _Jan
	AUTO_RUN iterate once over "XXX" 

	FILTER

		DIVERT 
		or ALSO_AND_THEN

		Match _Jan LANG_DIALECT_Jan( int & mm ) // 12 month names 

		array[ mm ] of FITS_in_four_bytes

			"ABCD"

			D == NUL 

			"ABC"

		AUTO_GEN_VAR D ABCD ABC0 ABCD AB CD A B C D ABC 
		AUTO_GEN_WORD dbca d==0 cba ba b a // cb == WORD >> 8 
		AUTO_GEN ba i16 u16 u16_idx // ALIAS uses familiar list

		fast_match u32_word == 'Feb' // u32_lohi // required

		any 3 byte str with own NUL // ABC D==NUL "ABC" dcba_CBA

			use cpu BYTE_A
			use cpu BYTE_B
			use cpu BYTE_PAIR { B A }
			use cpu BYTE_WORD { dbca } // D == NUL
			// u24_idx // RAW_INT // CPU_VAR_FIT // with ROOOM

	METAPHOR

		idx = N++

		u8_idx
		u16_idx
		u24_idx
		u32_idx
		u32_lohi
		u64_lohi

		WORD _t VAL // DIALECT rhymes with VAR // VAR VAL //
		// reserved word var // var_

	METAPHOR

		idx = lookup NAME // POOL 

		NAME = int_val_dcba 

		NAME = NAME_ABC

			"ABC"	// D == NUL // u24_lohi // WORD_cba // 
			"xFF"	// [ u8_idx ] // find STR3 in WORD_LIST

		NAME = RECODED u24 // non nul bytes or nul

			truncating prefix
			ABCD	// CHECK internal NUL A_CD "A" "
			ABC	// USED by xFF STR3 CIDENT 
			AB	// PLAIN STR3 STR2 STR1 empty_str
			A
			""
	MECHANISM

		idx = N++

		ABC_WORD += xFF_WORD / "xFF" // 

		int N_STR4 == 12 // or 83

		u32_WORD ARY[N] // u32_N

		PARSE 
		 literal "x"
		 UPPERHEX hi
		 UPPERHEX lo

*/
bool file_scan_base::scan_Jan( int & mm )
{
	if(LEX.scan_word("Jan")) { mm = 1 ; return TRUE; } else
	if(LEX.scan_word("Feb")) { mm = 2 ; return TRUE; } else
	if(LEX.scan_word("Mar")) { mm = 3 ; return TRUE; } else
	if(LEX.scan_word("Apr")) { mm = 4 ; return TRUE; } else
	if(LEX.scan_word("May")) { mm = 5 ; return TRUE; } else
	if(LEX.scan_word("Jun")) { mm = 6 ; return TRUE; } else
	if(LEX.scan_word("Jul")) { mm = 7 ; return TRUE; } else
	if(LEX.scan_word("Aug")) { mm = 8 ; return TRUE; } else
	if(LEX.scan_word("Sep")) { mm = 9 ; return TRUE; } else
	if(LEX.scan_word("Oct")) { mm = 10 ; return TRUE; } else
	if(LEX.scan_word("Nov")) { mm = 11 ; return TRUE; } else
	if(LEX.scan_word("Dec")) { mm = 12 ; return TRUE; } else
	return FALSE;
}

bool file_scan_base::scan_line( str1 & str )
{
	p0p2 zone;
	LEX.p0p2_start( zone );
	LEX.scan_line(); // cset that excludes NUL NL and CR
	LEX.p0p2_stop( zone );
	str = zone;
	// LURK - this is MM3 only
	// str.no_comma();
	return LEX.scan_crlf();
}

bool file_scan_base::open_file( const u8 * filename )
{
	int t;
	t=MMAP.map_in_file( filename, FALSE );
	if(t) return FALSE;
	if( MMAP.nbytes == 0 ) {
		static u8 fake_buffer[] = {'\n', 0 };
		zone = p0p2( fake_buffer, (uns) 0 ); // see API
		// zone = p0p2( "\n", 0 ); // see API
		MMAP.close();
	} else {
		zone = p0p2( MMAP.page0, MMAP.nbytes );
	}
	LEX.set_file_zone( zone );
	// already thrown
	if(!LEX.check_nl_at_eof())
	{
		e_print("File %s does not end in NL\n",filename );
		return false;
	}
	return true;
}
