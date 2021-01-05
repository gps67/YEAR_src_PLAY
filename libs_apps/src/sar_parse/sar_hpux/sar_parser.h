#ifndef SAR_PARSER_H
#define SAR_PARSER_H

#include <stdio.h>
#include <misc.h>
#include "zfile_line_in.h"
#include "sar_parser_parts.h"
#include "sar_parser_temps.h"
#include "tbl_collect.h"

struct sar_parser : public sar_parser_temps {

	zfile_line_in in;
	bool eof;
	char * line;

	//! tbl_collect owns data referred to by THIS 
	tbl_collect * tbl;

	sar_parser(
		tbl_collect * _tbl // MUST have spec already set ...
	);
	~sar_parser();

	static bool zfile_to_dset(
		const char * site_name,
		const char * cmd_name,		// sar
		const char * zfilename,
		dset_api * dset
	);

// --

	bool get_line() {
		if( line ) return TRUE;
		line = in.getline();
		if( !line ) {
			eof = TRUE;	// EOF usually
			return FALSE; 
		}
		return TRUE;
	}

	void used_line() {
		line = NULL;
	}

	bool blank_line_peek() {
		if( !get_line() ) return FALSE;
		if( 0==strcmp( line, "\n" )) {
			// PEEK - dont remove line // used_line();
			return TRUE;
		}
		return FALSE;
	}

	bool blank_line() {
		if( blank_line_peek() ) {
			used_line();
			return TRUE;
		}
		return FALSE;
	}

	bool open( const char * filename );

	bool nibble_time();

	bool header_line();
	bool header();

	bool cpu_line_u();
	bool cpu_line_q();
	bool disk_line();


	bool sys_line_1();
	bool sys_line_2();
	bool sys_line_3();
	bool sys_line_4();
	bool sys_line_5();
	bool sys_line_6();
	bool sys_line_7();

	bool stanza();
	bool trailer();
	bool z_parse( const char * zfilename );

};
#endif
