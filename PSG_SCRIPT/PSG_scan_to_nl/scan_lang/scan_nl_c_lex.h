#ifndef scan_nl_c_lex_H
#define scan_nl_c_lex_H

#include <stdio.h> // fprintf

#include "str1.h"	// str1
#include "scan_to_nl.h"	// getc buffer with lex matching

extern int trans_id;

/*!
*/
class scan_nl_C_LEX : public scan_to_nl
{
 public:
 	// scanned LEX items are placed here
	str1	zone_dox_line;
	str1	zone_dox_block;
	str1	zone_dox_pre_line;
	str1	zone_dox_pre_block;
	str1	zone_dox_end_line;
	str1	zone_dox_end_block;
	str1	zone_cmnt_line;
	str1	zone_cmnt_block;

	/*
		statics - see .cxx where also defined
	*/
	static int cset_inited_b;

	scan_nl_C_LEX();
	scan_nl_C_LEX( p0p2 zone );

	void init_csets_C(void);

 	bool GAP_after_LEX();

	bool peek_pp_hash_at_boln();
	bool scan_pp_hash_at_boln();
//	bool scan_pp_include( str1 & filename, bool & local );

	bool scan_dox_comment_to_eoln( str1 & cmnt );
	bool scan_comment_to_eoln( str1 & cmnt );
	bool scan_dox_comment_block( str1 & str );
	bool scan_comment_block( str1 & str );
	bool scan_comment_block_tail( str1 & str );
};

#endif
