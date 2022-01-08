#include "scan_c1.h"
#include "str1.h"
#include "buffer1.h"

#include <stdio.h>

void scan_c1::get_x_y_start()
{
	LEX.get_x_y( x_start, y_start );
}

bool scan_c1::var_decl()	// Item_t * item; // CMNT //
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
	LEX.here_start( pos );

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

	if(! LEX.lex_ident( _typ_name ) ) return false; // non starter
	typ_name = _typ_name;

	bool asterisk = LEX.lex_char( '*' );

	bool ampersand = LEX.lex_char( '&' );

	if(! LEX.lex_ident( _varname ) )
	{
		LEX.report_FAIL( "varname" );
		goto fail;
	}
	varname = _varname;

	// str1 value;
	if( LEX.lex_char( '=' ) )
	{
		value_set = true;
		if(! LEX.lex_int( value ) ) goto fail;
	}

	if(! LEX.lex_char( ';' ) )
	{
		LEX.report_FAIL( "Missing semi-colon" );
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
	LEX.report1( buf );

	return true;

fail:
	LEX.here_back( pos );
	// FASTER // LEX.P = pos;
	return false;
}

bool scan_c1::cpp_hash_line() //#define #if #endif ...
{
	// somehow promote first stmt to inline, to reduce call overhead
	if( ! LEX.scan_hash_at_boln() ) return false;
top0:
	here_pos pos;		// define before fail paragraph
	LEX.here_start( pos ); // should move back 1 char !
	goto top;
fail:
	LEX.report_FAIL("cpp_hash_line");
	LEX.here_back( pos );
	// FASTER // LEX.P = pos;
	return false;
top:

	if(0) LEX.report_OK("hash");
	LEX.scan_spaces_and_tabs();
	/*
		LEX.GAP_after_LEX();

			this is auto added for lex_XXX items
			should move scan_char('(') to lex_char('(')
			should create mode allow_newlines within GAP_after_LEX
			should handle dox items into STBL

		LEX.report_WARN()

			shows the advantage of LEX marking the previous start
			and some sort of relative expression

			LEXA  GAPB  LEXC  GAPD  LEXE
			    AB    BC    CD    DE
	*/

	if(LEX.scan_word("include"))
	{
		if(0) LEX.report_OK("# include");
		str1 filename; bool local;
		if( !scan_pp_include_( filename, local ) ) 
		{
			goto fail;
		}
		LEX.scan_spaces_and_tabs();
		// NO EOLN in PP line // LEX.GAP_after_LEX();
		return true;
	}

	if(LEX.lex_word("define"))
	{
		// LEX.report_WARN("DEFINE found");
		// lex // LEX.GAP_after_LEX();
		p0p2 _def_name;
		str1 def_name;
		if(!LEX.lex_ident(_def_name)) {
			LEX.report_FAIL("missing macro name");
			// should report last ..
			goto fail;
		}
		def_name = _def_name;
		// LEX.report_WARN("DEFINE NAME found");

		/*
			optional args to macro

			? need to detect parenth but no args? or same?
		*/
		if(LEX.scan_NOSP() && LEX.lex_char('(')) {
			bool first = true;
			while(1) {
				if(!first)
				 if(!LEX.lex_char(',')) 
					break;
				/*
					dox comment about arg ?
				*/
				str1 argname;
				p0p2 _argname;
				if(!LEX.lex_ident( _argname )) {
					if(!first)
						LEX.report_FAIL("expected arg");
					break; // loop
				}
				argname = _argname;
				first = false;
				// TODO // LEX.report_WARN( argname.str );
			}
			if(!LEX.lex_char(')')) {
				LEX.report_FAIL("missing )");
				goto fail;
			}
		}

		/*
			this wont handle multi-line properly
			but maybe I'm looking for a range of simple templates
		*/
		str1 definition;
		p0p2 zone;
		LEX.p0p2_start( zone );
		LEX.scan_to_eoln();
		LEX.p0p2_stop( zone );
		zone.trim_trailing_blanks();
		definition = zone;
// #warning "check definition trailing backslash multiline"

		// OK // LEX.report_WARN( definition.str );
		return true;
	}

	if(LEX.lex_word("if"))
	{
		LEX.report_FAIL( "TODO" );
	}

	if(LEX.lex_word("ifdef"))
	{
		LEX.report_FAIL( "TODO" );
	}

	if(LEX.lex_word("ifndef"))
	{
		LEX.report_FAIL( "TODO" );
	}

	if(LEX.lex_word("else"))
	{
		LEX.report_FAIL( "TODO" );
	}

	if(LEX.lex_word("endif"))
	{
		LEX.report_FAIL( "TODO" );
	}

	goto fail;

}

/*!
	caller has found "#include" now find " <a.h> CMNT EOLN"
*/
bool scan_c1::scan_pp_include_( str1 & filename, bool & local )
{
	// I dont see how rewinding to this point is useful
	// but for consistency -- all do this !
	here_pos P0;
	LEX.here_start( P0 );

	LEX.scan_spaces_and_tabs(); // LURK eoln within cpp

	bool _local = false;
	char c2 = '>';
	if( LEX.scan_char('<') ) {
		_local = false;
		c2 = '>';
	} else if( LEX.scan_char('"') ) {
		_local = true;
		c2 = '"';
	} else 
	{
		LEX.report_FAIL("expected < or \"");
		// crosses init // goto bad;
		LEX.here_back( P0 );
		return false;
	}

	// get the a.h within "a.h"
	p0p2 zone;
	LEX.p0p2_start( zone );
	if( !LEX.scan_upto_char( c2 ) ) goto bad;
	LEX.p0p2_stop( zone );

	// get the closing " or >
	if( !LEX.scan_char( c2 ) ) goto bad;

	filename = zone;
	local = _local;
	
	LEX.GAP_after_LEX();
	/*
		caller should now include that file
		to get symbols from it
	*/
	LEX.report2( "include", (STR0) filename.str );
	return true;
 bad:
	LEX.report_FAIL("BAD");
	LEX.here_back( P0 );
	return false;
}

#define BT(tname) \
	if(LEX.lex_word( tname )) {		\
		LEX.report_WARN( tname );	\
		name = tname;			\
		return true;			\
	}					\
//

/*!
	this should return a pointer to a STBL ITEM
*/
bool scan_c1::builtin_type_name( str1 & name )
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
bool scan_c1::known_type_name( str1 & name )
{
	if( builtin_type_name( name )) return true;
	LEX.report_FAIL("TODO");
	return false;
}

bool scan_c1::scan_typeexpr()
{
	// CALL FROM // if( var_decl() ) continue;
	// sub_type_expr;
	goto top;
fail:
	LEX.report_FAIL("TYPEEXPR");
	return false;
top:

	// THIS WOULD BE THE FASTEST // u8 * pos = LEX.P;
	here_pos pos;
	LEX.here_start( pos );

	str1 typ_name;
	str1 varname;
	p0p2 _typ_name;
	p0p2 _varname;
	buffer1 buf;
	bool value_set = false;
	int value = -1;

	if(! LEX.lex_ident( _typ_name ) ) return false; // non starter

	bool asterisk = LEX.lex_char( '*' );

	bool ampersand = LEX.lex_char( '&' );

	if(! LEX.lex_ident( _varname ) )
	{
		LEX.report_FAIL( "varname" );
		goto fail;
	}

	/*
	DISTINGUISH:
		TYPEA VAR( c1, c2 );		// TYPE/INST constructor
		TYPEA FUNCPROTO( c1, c2 );	// FUNC prototype (extern? forward)

		TYPEA * VAR( c1, c2 );		// NEVER HAPPENS AS VAR
	*/

	// str1 value;
	if( LEX.lex_char( '=' ) )
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
		if(! LEX.lex_int( value ) ) goto fail;
	}

	if(! LEX.lex_char_expect( ';' ) )
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
	LEX.report1( buf );

	return true;
}

bool scan_c1::scan_file( const u8 * filename )
{
	str1 s;
	p0p2 _s;
	int i;
	if(!open_file( filename ))
		return false;

	LEX.GAP_after_LEX();
	get_x_y_start();

	bool ok = true;
	while(1)
	{
		// type var = initval;
		if( var_decl() ) continue;

		// #define #ifdef #include ...
		if( cpp_hash_line() )
		{
			LEX.report1( "cpp_hash_line" );
			continue;
		}

		// soak up other stray tokens
		if( LEX.lex_ident(_s) )
		{
			s = _s;
			LEX.report2( "identifier", s );
			continue;
		}
		if( LEX.lex_char('=') )
		{
			LEX.report1( "assign" );
			continue;
		}
		if( LEX.lex_char(';') )
		{
			LEX.report1( "semicolon" );
			continue;
		}
		if( LEX.lex_char('*') )
		{
			LEX.report1( "asterisk" );
			continue;
		}
		if( LEX.lex_char('&') )
		{
			LEX.report1( "ampersand" );
			continue;
		}
		if( LEX.lex_int( i ) )
		{
			char buff[20];
			sprintf( buff, "integer(%d)", i );
			LEX.report1( buff );
			continue;
		}

		if( LEX.scan_spaces_and_tabs() )
		{
			LEX.report1( "spaces_and_tabs");
			continue;
		}
		if( LEX.scan_nl() )
		{
			LEX.report1( "NL");
			continue;
		}
		if( LEX.scan_dox_comment_to_eoln(s) )
		{
			LEX.report2( "dox_comment_to_eoln", s );
			continue;
		}
		if( LEX.scan_comment_to_eoln(s) )
		{
			LEX.report2( "comment_to_eoln", s );
			continue;
		}
		if( LEX.scan_dox_comment_block(s) )
		{
			LEX.report2( "dox_comment_block", s );
			continue;
		}
		if( LEX.scan_comment_block(s) )
		{
			LEX.report2( "comment_block", s );
			continue;
		}

		if( LEX.scan_eof() )
		{
			LEX.report1( "EOF");
			break;
		}

		LEX.report_FAIL( "ERR");
		break;
	}
	return ok;
}


