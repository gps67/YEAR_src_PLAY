
#include "scan_nl_c_psg.h"
#include "buffer1.h"

scan_nl_C_PSG::scan_nl_C_PSG( p0p2 zone )
: scan_nl_C_LEX( zone )
{
}

scan_nl_C_PSG::scan_nl_C_PSG()
: scan_nl_C_LEX()
{
}


bool scan_nl_C_PSG::var_decl()	// Item_t * item; // CMNT //
{	// 
	// THIS WOULD BE THE FASTEST // u8 * pos = LEX.P; // CACHE // track and 
	// Parse LEX_START ident(varname)
	// Item_t * item; // CMNT // PLUS 
	// Item_t   item; // LANG // self += { Item_t } 
	// Obj * obj = PARSE("// Item_t * item; // CMNT // PLUS");
	// Obj * obj = PARSE("{  Item_t * item; }"(; // CMNT // PLUS ))");yy
	// Obj * obj = PARSE("// Item_t * item; // CMNT // PLUS");
	// DIALECT // "{ %s }" // PRINT %s ITEM_NAME  // "%s_t _%s = %s; // %s"
	// PARSE(
		// Item_t * item; // CMNT // PLUS
	// 
	here_pos pos;
	here_start( pos );

	p0p2 _typ_name;		// "Item" "_t" "Item_t" "{ Item_t * item; }"
	p0p2 _varname;		// item //
	str1 typ_name;		// "Item_t"
	str1 varname;		// "item"
	str1 CMNT_text;		// "CMNT" // AUTO GEN str1 CMNT; // "CMNT"
	// KNOW // CMNT is { str1 VAR_NAME "CMNT" } { CMNT }
	// LOOK // typename varname CMNT // CODE_POINT // XPOS
	// XPOS // dir file seek // or_other_XPOS_units
	// XPOS // dir file seek // CMNT // 
	// { XPOS dir file seek CMNT }

	buffer1 buf;		
	bool value_set = false;
	int value = -1;

	if(! lex_ident( _typ_name ) ) return false; // non starter
	typ_name = _typ_name;

	bool asterisk = lex_char( '*' );

	bool ampersand = lex_char( '&' );

	if(! lex_ident( _varname ) )
	{
		report_FAIL( "varname" );
		goto fail;
	}
	varname = _varname;

	// str1 value;
	if( lex_char( '=' ) )
	{
		value_set = true;
		if(! lex_int( value ) ) goto fail;
	}

	if(! lex_char( ';' ) )
	{
		report_FAIL( "Missing semi-colon" );
		goto fail;
	}

	buf.print( "# VAR # %s%s%s %s",
		typ_name.str,
		asterisk? " *" : "",
		ampersand? " &" : "",
		varname.str
	);
	// eshers not here ...
	if( value_set ) buf.printf( " = %d", value );
	report1( buf );

	return true;

fail:
	here_back( pos );
	// FASTER // LEX.P = pos;
	return false;
}

bool scan_nl_C_PSG::cpp_hash_line() //#define #if #endif ...
{
	// somehow promote first stmt to inline, to reduce call overhead
	if( ! scan_hash_at_boln() ) return false;
top0:
	here_pos pos;		// define before fail paragraph
	here_start( pos ); // should move back 1 char !
	goto top;
fail:
	report_FAIL("cpp_hash_line");
	here_back( pos );
	// FASTER // LEX.P = pos;
	return false;
top:

	if(0) report_OK("hash");
	scan_spaces_and_tabs();
	/*
		GAP_after_LEX();

			this is auto added for lex_XXX items
			should move scan_char('(') to lex_char('(')
			should create mode allow_newlines within GAP_after_LEX
			should handle dox items into STBL

		report_WARN()

			shows the advantage of LEX marking the previous start
			and some sort of relative expression

			LEXA  GAPB  LEXC  GAPD  LEXE
			    AB    BC    CD    DE
	*/

	if(scan_word("include"))
	{
		if(0) report_OK("# include");
		str1 filename; bool local;
		if( !scan_pp_include_( filename, local ) ) 
		{
			goto fail;
		}
		scan_spaces_and_tabs();
		// NO EOLN in PP line // GAP_after_LEX();
		return true;
	}

	if(lex_word("define"))
	{
		// report_WARN("DEFINE found");
		// lex // GAP_after_LEX();
		p0p2 _def_name;
		str1 def_name;
		if(!lex_ident(_def_name)) {
			report_FAIL("missing macro name");
			// should report last ..
			goto fail;
		}
		def_name = _def_name;
		// report_WARN("DEFINE NAME found");

		/*
			optional args to macro

			? need to detect parenth but no args? or same?
		*/
		if(scan_NOSP() && lex_char('(')) {
			bool first = true;
			while(1) {
				if(!first)
				 if(!lex_char(',')) 
					break;
				/*
					dox comment about arg ?
				*/
				str1 argname;
				p0p2 _argname;
				if(!lex_ident( _argname )) {
					if(!first)
						report_FAIL("expected arg");
					break; // loop
				}
				argname = _argname;
				first = false;
				// TODO // report_WARN( argname.str );
			}
			if(!lex_char(')')) {
				report_FAIL("missing )");
				goto fail;
			}
		}

		/*
			this wont handle multi-line properly
			but maybe I'm looking for a range of simple templates
		*/
		str1 definition;
		p0p2 zone;
		p0p2_start( zone );
		scan_to_eoln();
		p0p2_stop( zone );
		zone.trim_trailing_blanks();
		definition = zone;
// #warning "check definition trailing backslash multiline"

		// OK // report_WARN( definition.str );
		return true;
	}

	if(lex_word("if"))
	{
		report_FAIL( "TODO" );
	}

	if(lex_word("ifdef"))
	{
		report_FAIL( "TODO" );
	}

	if(lex_word("ifndef"))
	{
		report_FAIL( "TODO" );
	}

	if(lex_word("else"))
	{
		report_FAIL( "TODO" );
	}

	if(lex_word("endif"))
	{
		report_FAIL( "TODO" );
	}

	goto fail;

}

/*!
	caller has found "#include" now find " <a.h> CMNT EOLN"

	OPTION attempt to DOX the CMNT
	OPTION leave GAP after S.E.P.

*/
bool scan_nl_C_PSG::scan_pp_include_( str1 & filename, bool & local )
{
	// I dont see how rewinding to this point is useful
	// but for consistency -- all do this !
	here_pos P0;
	here_start( P0 );

	scan_spaces_and_tabs(); // LURK eoln within cpp

	bool _local = false;
	char c2 = '>';
	if( scan_char('<') ) {
		_local = false;
		c2 = '>';
	} else if( scan_char('"') ) {
		_local = true;
		c2 = '"';
	} else 
	{
		report_FAIL("expected < or \"");
		// crosses init // goto bad;
		here_back( P0 );
		return false;
	}

	// get the a.h within "a.h"
	p0p2 zone;
	p0p2_start( zone );
	if( !scan_upto_char( c2 ) ) goto bad;
	p0p2_stop( zone );

	// get the closing " or >
	if( !scan_char( c2 ) ) goto bad;

	filename = zone;
	local = _local;
	
	GAP_after_LEX();
	/*
		caller should now include that file
		to get symbols from it
	*/
	if(local)
		report2( "include Q2", (STR0) filename.str );
	else
		report2( "include <>", (STR0) filename.str );
	return true;
 bad:
	report_FAIL("BAD");
	here_back( P0 );
	return false;
}

#define BT(tname) \
	if(lex_word( tname )) {		\
		report_WARN( tname );	\
		name = tname;			\
		return true;			\
	}					\
//

/*!
	this should return a pointer to a STBL ITEM
*/
bool scan_nl_C_PSG::builtin_type_name( str1 & name )
{
	BT("int")
	BT("char")
	BT("float")
	BT("long long")
	BT("long double")
	BT("long float")
	BT("long")
	BT("double")
	BT("short")
	return false;
}

/*!
	this should return a pointer to a STBL ITEM
*/
bool scan_nl_C_PSG::known_type_name( str1 & name )
{
	if( builtin_type_name( name )) return true;
	report_FAIL("TODO");
	return false;
}

bool scan_nl_C_PSG::scan_typeexpr()
{
	// CALL FROM // if( var_decl() ) continue;
	// sub_type_expr;
	goto top;
fail:
	report_FAIL("TYPEEXPR");
	return false;
top:

	// THIS WOULD BE THE FASTEST // u8 * pos = LEX.P;
	here_pos pos;
	here_start( pos );

	str1 typ_name;
	str1 varname;
	p0p2 _typ_name;
	p0p2 _varname;
	buffer1 buf;
	bool value_set = false;
	int value = -1;

	if(! lex_ident( _typ_name ) ) return false; // non starter

	bool asterisk = lex_char( '*' );

	bool ampersand = lex_char( '&' );

	if(! lex_ident( _varname ) )
	{
		report_FAIL( "varname" );
		goto fail;
	}

	/*
	DISTINGUISH:
		TYPEA VAR( c1, c2 );		// TYPE/INST constructor
		TYPEA FUNCPROTO( c1, c2 );	// FUNC prototype (extern? forward)

		TYPEA * VAR( c1, c2 );		// NEVER HAPPENS AS VAR
	*/

	// str1 value;
	if( lex_char( '=' ) )
	{
		value_set = true;
		// PARSE("EXPR") // EXPR "int" // BENCH AUTO GEN EXPR %d 
		// "x%2X" u64_EXPR_val // EXPR == "int" // i64_WORD // 
		// Item_t item = EXPR // i64_WORD // i32_word //
		// u3_idx = lookup ANYSTR // 
		// PARSE name CALL( args ) VALUE " %s " EXPR { CMNT = "%s" }
		// CMNT // parsed_LINE_of_TEXT // _to_eoln //
		// { this asif text } // NOQ2 dialect //
		// TODO // calling PSG from within LEX VAR.name
		// TODO // SCRIPT EXPR // TYPE VAR = EXPR // CMNT //
		// TODO // SCRIPT NAME // of VAL of VAR of CMNT // which is CMNT
		if(! lex_int( value ) ) goto fail;
	}

	if(! lex_char_expect( ';' ) )
	{
		goto fail;
	}

	str1 CMNT;
	// GAP
	// 

	buf.print( "# VAR # %s%s%s %s // %s", // Item_t * & item // CMNT
		typ_name.str,
		asterisk? " *" : "",
		ampersand? " &" : "",
		varname.str,
		"CMNT" // SAMPLE_VALUE "CMNT" AS_IF_VARNAME // FILTER VAR = VAR
		"// added //"
		"{ %s }" // universal component // "%s" // '"{ %s }"' // " %s "
	);
	// eshers not here ...
	if( value_set ) buf.printf( " = %d // %s", value, CMNT );
	report1( buf );

	return true;
}

bool scan_nl_C_PSG::scan_file( const u8 * filename )
{
	// MMAP has already loaded the file the name is for labels
	str1 s;
	p0p2 _s;
	int i;
//	if(!open_file( filename ))
//		return false;

	GAP_after_LEX();
//	get_x_y_start();

	bool ok = true;
	while(1)
	{
		// type var = initval;
		if( var_decl() ) continue;

		// #define #ifdef #include ...
		if( cpp_hash_line() )
		{
			report1( "cpp_hash_line" );
			continue;
		}

		// soak up other stray tokens
		if( lex_ident(_s) )
		{
			s = _s;
			report2( "identifier", s );
			continue;
		}
		if( lex_char('=') )
		{
			report1( "assign" );
			continue;
		}
		if( lex_char(';') )
		{
			report1( "semicolon" );
			continue;
		}
		if( lex_char('*') )
		{
			report1( "asterisk" );
			continue;
		}
		if( lex_char('&') )
		{
			report1( "ampersand" );
			continue;
		}
		if( lex_int( i ) )
		{
			char buff[20];
			sprintf( buff, "integer(%d)", i );
			report1( buff );
			continue;
		}

		if( scan_spaces_and_tabs() )
		{
			report1( "spaces_and_tabs");
			continue;
		}
		if( scan_nl() )
		{
			report1( "NL");
			continue;
		}
		if( scan_dox_comment_to_eoln(s) )
		{
			report2( "dox_comment_to_eoln", s );
			continue;
		}
		if( scan_comment_to_eoln(s) )
		{
			report2( "comment_to_eoln", s );
			continue;
		}
		if( scan_dox_comment_block(s) )
		{
			report2( "dox_comment_block", s );
			continue;
		}
		if( scan_comment_block(s) )
		{
			report2( "comment_block", s );
			continue;
		}

		if( scan_eof() )
		{
			report1( "EOF");
			break;
		}

		report_FAIL( "ERR");
		break;
	}
	return ok;
}



