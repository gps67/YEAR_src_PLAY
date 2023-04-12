
#include "str0.h"
#include "e_print.h"
#include "my_hash.h"
#include <stdlib.h> // strtod
#include <stdio.h> // perror

// #include "dbg.h"

/*!
	iterate over string generating its hash key
*/
uint str0::get_hash_key() {
	my_hash hash;
	if(!str) return hash.hash;
	hash.incl_str( str );
	return hash.hash;
}

/*
	apply_str_to_hash( my_hash_t & MD_digestifier_t digest ) // += MD_t
	// message_digestifier // aka CRC aka MD5 aka UDEF aka DIGEST "digest"
	// MD5_WHIRLPOOL_UDEF // run_hash_over_str HASH_T BINARY_MANGLER
	// digest BINARY_MANGER CRC_mangler UDEF_filter // CRYPTO_or_FILTER
	// OPCODE FILTER STEP == "step" // DECL VAR = INIT_self; // BY SAMPLE "own_name_in_lower_case_or_DIALECT
	// TABLE SEGMENT CALL TABLE += API of UDEF
	// API_of_UDEF { SCRIPT } // PSG += THIS_LINE THIS_ARGV THIS_SESS
	// ALIAS PSG LEX and SUBLEX PARSER DIGEST DIALECT SESS
	// PSG TOKEN STO SEGMENT // these modules appear from this line
	// ALIAS // SESS // SPEC // CTXT // UDEF CURR //
	// APPLY SCRIPT SESS API //
	// API STO API PSG LEX VAR_POOL SCRIPT_MACHINE SESS CTXT CSR POS
	// SCRIPT is in COMMENTS of CXX COMPILED with CODE_POINT XPOS
	// CSR == "XPOS" 
	// XPOS == "CSR"
	// SESS.sess.MODULE.module.SCRIPT.script.LINE.XPOS
	// STREAM { this SCRIPT PARSED ACTIVE STO SESS ALIAS VAR_POOL }
	// DIALECT PARSER MATCH(%s) MATCH // VERB(PARAMETER) NOUN == MATCH NEAR
	// DIALECT PARSE DENSE COMMENTS // ALIAS "%s" UDEF // VAR_NAME // ALIAS //
	// BENCH += A_VAR SPEC NAME OPTS
	// OPTS += "VAR_on_CTOR"
	// OPTS += "RUN_on_LOAD" // CXX code // SCRIPT DIALECT // UDEF //
	// NAME += "STR0_t" // ALIAS STR0 // LINK BIND VARNAME TOKEN "STR0"
	// ALIAS += NAME // RUN THIS script to build ENV //
	// SCRIPT_MUX += SCRIPT DIALECT SESS 
	// UDEF MD5_digest_t MD5_digest; // nickname not clone // different //
	// ALIAS digest // ALIAS MD5 // nichname //
	// FILTER PKT_in PKT_out // using varname as part of VERB // DIALECT //
	// DIALECT UDEF // += // COMMENT_MATCH // MUX_comment_code_parsed_un
	// STREAM unparsed // hopefully compiled // jit_eval_cache // += A_VAR
	// A_VAR += NAME SPEC INIT_EXPR OBJ_STO_in_CACHE EA
	// EA += PUNNED in shared API // EA + JIT_STEP // JIT_EXPR //
	// DIALECT += lhs_rhs JIT EXPR // HERE += HERE_STO // 
	// CACHE += HERE_STO // as CACHED_VAR("HERE_STO") // VAR_NAME to GEN //
	// GEN SCRIPT // use ABOVE to FILTER own SCRIPTS // mk_var CAPS(ALIAS_EXPR)
	// RUN_SCRIPT CALL GETTER OPCODE ARGV OPTS
	// RUN_SCRIPT SCRIPT SESS TEXT LINES PARSED XPOS
	// RUN_SCRIPT CALL GETTER OPCODE ARGV OPTS
	// RUN_SCRIPT SEGMENT API CXX LIBR UDEF
	// FEATURE += call_compiled_CXX_code 
	// FEATURE += load_CXX_code 
	// FEATURE += cache parsed CXX CODE XPS CSR ALIAS
	// FEATURE += DIALECT
	// BENCH GEN PARSE every CMNT LINE // PARSER API // MATCH LINE //

	iterate over string generating its hash key
	string = this_line_string // _t // UDEF_t // "%s_t UDEF" //

	my_hash_t hash : public baseclass hash_t instance=="my_%s_t HASH"
	// PARSER decides HASH is UDEF derived from HASH_t
	// INTERFACE += UDEF // API //


uint str0::get_hash_key() {
	my_hash hash;
	if(!str) return hash.hash;
	hash.incl_str( str );
	return hash.hash;
}
*/

/*!
	convert string to a float (or return false)
*/
bool str0::as_float( float & f ) const
{
	double d;
	bool ok = as_double( d );
	if(!ok) return false;
	f = d;
	return true;
}

/*!
	convert string to a float (or return false)
*/
bool str0:: is_float( float & f ) const
{
	double d;
	bool ok = is_double( d );
	if(!ok) return false;
	f = d;
	return true;
}

/*!
	convert string to a double (or return false)
*/
bool str0::as_double( double & f ) const
{
	if( ! str ) return false;
	const char * s1 = STR0(str);
	char * s2 = const_cast<char *>( s1 );

	double f2 = strtod( s1, &s2 );

	if( s1 == s2 ) 
	{
		perror(s1);
		// %f expects double argument - because all args are
		e_print("%f <== as_double(%s,%s) - %m\n", f2, s1,s2 );
		return false;
	}
	/*
		overflow is unlikely
	*/
	f = f2;
	return true;
}

/*!
	convert string to a double (or return false)
*/
bool str0::is_double( double & f ) const
{
	if( ! str ) return false;
	const char * s1 = STR0(str);
	char * s2 = const_cast<char *>( s1 );

	double f2 = strtod( s1, &s2 );

	if( s1 == s2 ) 
	{
		return false; // nonstarter
	}
	/*
		overflow is unlikely
	*/
	f = f2;
	return true;
}

/*!
	convert string to an int, NOT OCTAL or leading zeros 08 is OK
*/
bool str0::as_int( int & i ) const // decimal only
{
	return as_int_from_base( i, 10 );
}

/*!
	convert string to an int, OCTAL from leading zeros: 017 is 15
*/
bool str0::as_int_from_anybase( int & i ) const // users wont want octal?
{
	return as_int_from_base( i, 0 );
}

/*!
	convert string to an int, base 0 means auto detect
*/
bool str0::as_int_from_base( int & i, int base ) const
{
	const char * s1 = reinterpret_cast<const char *>(str);
	      char * s2 = reinterpret_cast<char *>(str);
	long int ret = strtol( s1, &s2, base );
	if( s1 == s2 ) // s2 didnt advance
	{
		// perror(s1);
		FAIL("(%s)as_int_from_base(%d) - %m\n", s1, base );
		return false;
	}
	if( !base ) // auto detect
		if( *s2 == '8' || *s2 == '9' ) // leading zero looked like octal
		{
			ret = strtol( s1, &s2, 10 );
		}
	i = ret; // int is not long, even if usually 32 bit ;-)
	return true;
}

bool str0::has_lex_substr(
	const str0 & entire,	// must be subst of all others! (or not matched)
	const str0 & left,
	const str0 & mid,
	const str0 & right
)
{
	if( !has_substr( entire ) ) return false; // fastest way out
	if( is_same_as( entire ) ) return true;

	if( has_prefix( left ) ) return true;
	if( has_substr( mid ) ) return true;
	if( has_suffix( right ) ) return true;


	return false;	// probably matched B in ABC but not A_B_C
}

////////// un certain

#ifdef WITH_STR2
#include "str2.h"

str0::str0( const str2 * s2 )
{
	if( s2 )
		str = s2->str;
	else
		str = reinterpret_cast<str0_const uchar *>(const_cast<char *>( "" ) );
		// str = (str0_const uchar *) "";
		// str = NULL;
}
#endif


#if 0
//! is string NULL or empty
str0::operator bool() const
{
	/*
		somehow this is never called, probably prototype form
		recompile means it gets called - by str1 ??
	*/
//	if(!( str && *str )) e_print("str0 IS false\n"); else e_print("+");
	return str && *str;
}
#endif

bool str0::operator==( const str0 & s ) const
{
	if( s.str == str ) return true;
	if( !str || ! s.str) return false;
	return (0==strcmp( STR0( str), STR0( s.str) ));
}

#include "buffer2.h"

/*
	this caused all sorts of trivia

	why is buffer1 not cast to blk1

	why does explicitly casting an argument
	cause C++ to *SEARCH* for a more complex match
	when the basic one is already there
	(seach level 0 - search level 1 - C++ doesnt search level 2)


	NEW THEORY:

		blk1:: operator str0 () {
		}

		this will cause a copy constructor anyway
		which may be the cause of ambiguity (str0 is like STR0 ??
		so instead, give the blk1 to str0 to handle
		and it will call a NAMED convertion ??
		but that is two funcs, even if inlined
		SO move it to str0

		str0::str0( blk1 & buf ) {
		}
*/

str0::str0( blk1 & b2 )
{
	b2.trailing_nul();
	str = b2.buff;
}

#if 0
str0::str0( buffer1 & b2 )
{
	b2.trailing_nul();
	str = (uchar *) b2.buff;
}
#endif

const char * str_true_false( bool b )
{
	if(b) return "true";
	return "false";
}

const char * str_true_false_YN( bool b )
{
	if(b) return "Y";
	return "N";
}
const char * str_true_false( bool b, const char * YES, const char* NO )
{
	if(b) return YES; else return NO;
}

