#include "scan_nl_c_lex.h"	// included the following

int scan_nl_C_LEX::cset_inited_b = 0; // init defined as = 0;

void scan_nl_C_LEX::init_csets_C(void)
{
	// maybe add force_init_csets() which sets cset_inited_a = 0;
	if( cset_inited_b ) return;
	cset_inited_b = 1;

	scan_to_nl::init_csets();
	printf("##### C_LEX init_csets_C() ###\n");

}

scan_nl_C_LEX::scan_nl_C_LEX()
: scan_to_nl()
{
	init_csets_C();
}

scan_nl_C_LEX::scan_nl_C_LEX( p0p2 zone )
: scan_to_nl( zone )
{
	init_csets_C();
}

bool scan_nl_C_LEX::scan_dox_comment_to_eoln( str1 & cmnt )
{
	// LURK dox has //<! //! ...
	if( !scan_chars( '/', '/', '!' ) ) return false;
	scan_spaces_and_tabs(); // skip initial leading
	p0p2 zone;
	p0p2_start( zone );
	scan_to_eoln();
	p0p2_stop( zone );
	zone.trim_trailing_blanks();
	cmnt = zone;
	scan_nl();
	return true;
}

bool scan_nl_C_LEX::scan_comment_to_eoln( str1 & cmnt )
{
	if( !scan_chars( '/', '/' ) ) return false;
	scan_spaces_and_tabs(); // skip initial leading
	p0p2 zone;
	p0p2_start( zone );
	scan_to_eoln();
	p0p2_stop( zone );
	zone.trim_trailing_blanks();
	cmnt = zone;
	scan_nl();
	return true;
}

bool scan_nl_C_LEX::scan_dox_comment_block( str1 & str )
{
	if( !scan_chars( '/', '*', '!' ) ) return false;
	if(! scan_comment_block_tail( str ))
	{
	}
	return true;
}

bool scan_nl_C_LEX::scan_comment_block( str1 & str )
{
	if( !scan_chars( '/', '*' ) ) return false;
	if(! scan_comment_block_tail( str ))
	{
	}
	return true;
}

bool scan_nl_C_LEX::scan_comment_block_tail( str1 & str )
{
	// committed to this path
	p0p2 zone;
	p0p2_start( zone );
	while(1)
	{
		scan_upto_char_or_eoln('*');
		if( scan_char('*') )
		{
			while( scan_char('*') ) /* multiple asterisks */ ;
			if( scan_char('/') )
			{
				p0p2_stop( zone );
				zone.p2 -=2;
				str = zone;
				return true;
			} else
				continue;
		} else if( scan_nl() )
		{
			continue;
		} else if( scan_eof() )
		{
			report_FAIL("expected * /\n");
			return false;
		}
	}
}

bool scan_nl_C_LEX::peek_pp_hash_at_boln()
{
	// presume not EOF ?
	if( *P != '#' ) return false;
	if( P != P0 ) {
		report_WARN("cpp hash # not at BOLN\n");
		return false;
	}
	return true;
}

bool scan_nl_C_LEX::scan_pp_hash_at_boln()
{
	if( *P != '#' ) return false;
	if( P != P0 ) {
		report_WARN("cpp hash # not at BOLN\n");
		return false;
	}
	scan_spaces_and_tabs();
	return true;
}


bool scan_nl_C_LEX::GAP_after_LEX()
{
	str1 drop;

	bool found = false;

	P_start_lex = P_start_next;	// old end WAS new beginning
	P_start_gap = P;		// this is where we are
	P_start_next = P;		// just in case

	bool found1 = true;
	while( found1 )
	{
		found1 = false;
		if( scan_nl() ) found1 = true;
		if( scan_crlf() ) found1 = true;
		if( scan_spaces_and_tabs() ) found1 = true;
		if( scan_dox_comment_block( zone_dox_block ) ) found1 = true;
		if( scan_dox_comment_to_eoln( zone_dox_line ) ) found1 = true;
		if( scan_comment_to_eoln( drop ) ) found1 = true;
		if( scan_comment_block( drop ) ) found1 = true;
		if( scan_spaces_and_tabs() ) found1 = true;

		if( found1 ) found = true;
	}
	P_start_next = P;		// pointing to next lex
	GAP_after_LEX_found = found;
	return GAP_after_LEX_found;
}

