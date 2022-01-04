#ifndef file_scan_base_H
#define file_scan_base_H
/*
	scan file.h	scan mm3 file outputting the data
	Graham Swallow 

	RESTARTED as scan_file - but simpler see there
	RESTARTING again as lex_NAME lex_STEP STEP = parse EXPR

        //  c1 uses file_scan 
	// xml uses scan_file

	// scan_nl uses p0p2 from mmap
	// lex_NAME
	// psg_NAME
	// chr_BYTES
	// utf8_PLUS // a1_a2_STAR // limited pass though functionality

	// parse_to_eoln // scan_nl // RO //
	// needs mmap_p0p2 // no change no relocate // 

*/

#include <stdio.h> // fprintf

#include "map_file.h"	// mmaps in file (readonly)
#include "str1.h"	// str1

extern int trans_id;

#include "scan_nl_c_lex.h"	// C Lex scanner upto NL

class file_scan_base
{
 public:
	mmap_file f;
	scan_nl_C_LEX LEX;
	p0p2 zone;

	bool open_file( const u8 * filename );
	bool open_file( const char * filename ) { return open_file( (const u8 *) filename ); }

	bool scan_Jan( int & mm ); // any Mon - English with exact Cap's
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
