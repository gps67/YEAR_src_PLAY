#ifndef file_scan_base_H
#define file_scan_base_H
/*
	scan file.h	scan mm3 file outputting the data
	Graham Swallow 

	RESTARTED as scan_file - but simpler see there
	RESTARTING again as lex_NAME lex_STEP STEP = parse EXPR
	AND NOW 2022

        //  c1 uses file_scan 
	// xml uses scan_file

	// scan_nl uses p0p2 from mmap
	// lex_NAME
	// psg_NAME
	// chr_BYTES
	// utf8_PLUS // a1_a2_STAR // limited pass though functionality
	// utf8_PLUS // NOT HEURISTIC LOOKS_LIKE_EURO_CYRILLIC_BYTE_ CONFLICTS UNION USAGE OF BYTE CSET_BYTE; 
	// a1_a2_STAR // 
	// BYTE IDX2 = LOOKUP IDX1 # EG UFT8 = BYTE_EURO # DIALECT #
	// FILTER API TRU TABLE[ u8_idx ]
	// UTF8_IN_c2 // limited pass though functionality

	// parse_to_eoln // scan_nl // RO //
	// needs mmap_p0p2 // no change no relocate // 

*/

#include <stdio.h> // fprintf

#include "map_file.h"	// mmaps in MMAP (readonly)
#include "str1.h"	// str1

extern int trans_id;

#include "scan_nl_c_lex.h"	// C Lex scanner upto NL

class file_scan_base
{
	// utf8_PLUS // a1_a2_STAR // limited pass though functionality
	// CSET_BYTE byte; // CODE_POINT.CSET == "CSET_BYTE" // EU mostly
	// token STRING "byte" { VAR { CSET_BYTE byte; // CODE_POINT } } // cmnt	// CMNT == "" // static VAR available for GET_API // 
	// token STRING "CODE_POINT" { VAR { ITEM_t ITEM; CMNT } DATA_BUFFER
	// item SPEC nbytes
	// SPEC // N_BYTES[ nbytes ] // shrink_wrap data_buffer // 
	// spec STRUCT { SPEC_STRUCT } // MINI_MACHINE LIBR[item] MMAP
 public:
	mmap_file MMAP; // MMAP // fd //		// P = BASE + OFFS // p0p2( page0, nbytes ) // fd
	scan_nl_C_LEX LEX;	// UDEF_PARSER_HERE // LEX.zone == FILE.zone
	p0p2 zone;		// entire_file = (p0p2) TEXT_OF_FILE // in_MMAP

	// must have XY_CSR_xpos // TODO XYZZ

	bool open_file( const u8 * filename );
	bool open_file( const char * filename ) { return open_file( (const u8 *) filename ); }

	// scan_line FOUND_DESC // import API scan_%s( ARGS ) // scan_Jan
	bool scan_Jan( int & mm ); // any Mon - English with exact Cap's
	// FILTER Scan_Jan == "scan_Jan" // "scan_%s(...) //
	// ... BUILD_PARSE_POINT // example for your dialect not here

	// file_scan_UDEF : public file_scan_base // _t // HERE // CSR // EA

	/*
		FROM HERE ON IN 

			MMAP LEX zone { THIS == "filename.txt" // CONTENTS_of }

			THIS == 
			"SCROLL with PARSER += LEX MMAP += EA_BASE_OFFS"

			UDEF class from here on is LIBR_VOCAB_for_DIALECT

			DIALECT += LIBR_VOCAB // connects OBJ now // BUILD FOLLOWS
			LIBR_VOCAB += ...

		mmap_file MMAP; // MMAP // fd // P = BASE + OFFS // P0P2 //
		// MMAP.p0p2 // entire_file data_buffer ALIAS ITEM CSET_BYTE
		// CSET_BYTE_PLUS_UTF8_via_ { utf8_encoder 8859_encoder BYTE_encoder }
		scan_nl_C_LEX LEX;	// UDEF_PARSER_HERE // LEX.zone == FILE.zone
		p0p2 zone;		// entire_file = (p0p2) TEXT_OF_FILE // in_MMAP
	*/

	bool scan_line( str1 & title );
/*
	bool scan_line_no_comma( str1 & title )
	{
		if( scan_line( title ) )
		{
			title.no_comma();
			return true;
		} else
			return false;
	}
*/

	bool scan_eof()
	{
		return LEX.scan_eof();
	}

	file_scan_base();

};

#endif
