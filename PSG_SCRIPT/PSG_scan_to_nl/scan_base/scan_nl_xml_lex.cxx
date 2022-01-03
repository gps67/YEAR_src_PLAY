#include "scan_nl_xml_lex.h"	// included the following
#include "stbl_spelling.h"


/*
	statics are just like "extern" things
	define them in the .CXX corresponding to the .H
*/
int scan_nl_XML_LEX::cset_inited_xml = 0; // init defined as = 0;

cset_bit_map scan_nl_XML_LEX::cset_xml_ident_a1;
cset_bit_map scan_nl_XML_LEX::cset_xml_ident_a2;
cset_bit_map scan_nl_XML_LEX::cset_xml_text;

scan_nl_XML_LEX::scan_nl_XML_LEX()
: scan_to_nl()
{
	init_csets();
}

scan_nl_XML_LEX::scan_nl_XML_LEX( p0p2 zone )
: scan_to_nl( zone )
{
	init_csets();
}

void scan_nl_XML_LEX::init_csets(void)
{

	// maybe add force_init_csets() which sets cset_inited_xml = 0;
	if( cset_inited_xml ) return;
	cset_inited_xml = 1;

	printf("##### XML init_csets() ###\n");
	scan_to_nl::init_csets();

	cset_xml_ident_a1.set_null();
	cset_xml_ident_a2.set_null();
	cset_xml_ident_a1 |= cset_AZaz;
	cset_xml_ident_a2 |= cset_AZaz_;
#if 1
	// ignore ns:name // see as CIDENT 
	cset_xml_ident_a2.set_bit( ':' );
	cset_ident_a2.set_bit(':');
#endif

	cset_xml_text.set_null();
	cset_xml_text.set_range( ' ', 126 );
	cset_xml_text.reset_bit( '&' );
	cset_xml_text.reset_bit( '<' );
//	cset_xml_text.reset_bit( ']' ); // TODO

/*
	cset_xml_attr_text.set_null();
	cset_xml_attr_text |= cset_xml_text;
	cset_xml_attr_text.reset_bit( '"' );
*/
}

/*
	LEX should be a friend of STBL_ITEM
	There should only be a few variations of a1_a2_star
	with added detections of ASCII/UTF8/8859/C16/C32/...
	then with added namespace separation, and OPTIONAL ALLOW GAP
	so that 'space:name' can be tokenised in one go

	STBL_ITEM might then be resolved by its parse context + owner,
	eg UL/LI finds LI as the first name in its quick-list every time.
	That is then a link to the resolved token, so there is no need
	to malloc the string, tokenise it, hash-lookup, ...

	The benefit of STBL !!
*/

bool scan_nl_XML_LEX::scan_name_1( p0p2 & ident )
{
	return scan_ident( ident ); // use a1 a2 from init_cset

#if 0
	if(!peek_a1( cset_xml_ident_a1 ) ) return false;
	p0p2 zone;
	// I dont see how this could fail ...
	if(!scan_a1_a2_star(
		zone,
		cset_xml_ident_a1,
		cset_xml_ident_a2
	)) return false;
	ident = zone;
	return true;
#endif
}

bool scan_nl_XML_LEX::scan_name_1_expect( p0p2 & ident, const char * tname )
{
	bool ok = scan_elem_name( ident );
	if(ok) return ok;
	report_FAIL( "expected %s", tname );
	return ok;
}

bool scan_nl_XML_LEX::scan_elem_name( p0p2 & ident )
{
	return scan_name_1( ident );
}

bool scan_nl_XML_LEX::scan_attr_name( p0p2 & ident )
{
	return scan_name_1( ident );
}

bool scan_nl_XML_LEX::scan_sdata_name( p0p2 & ident )
{
	return scan_name_1( ident );
}

bool scan_nl_XML_LEX::scan_elem_name_expect( p0p2 & ident )
{
	return scan_name_1_expect( ident, "elem_name" );
}

bool scan_nl_XML_LEX::scan_sdata_name_expect( p0p2 & ident )
{
	return scan_name_1_expect( ident, "sdata_name" );
}

bool scan_nl_XML_LEX::scan_elem_name_expect()
{
	used_elem_name_space = false;
	/*
		its not quite proper, but I use the same LEX
		for name and its space.
	*/
	if(!scan_name_1_expect( _elem_name_space, "namespace_elem_name" )) {
		return false;
	};
	if(!scan_char( ':' )) {
		used_elem_name_space = false;
		_elem_name = _elem_name_space;
		_elem_name_space.p2 = _elem_name_space.p0;
		// problem with CSET not yet converted - must match
/*
 MOVE
		This should be done by the caller
		because it MIGHT be able to resolve the name in one go

		To facilitate that, the caller may need to pass a working
		area which includes a buffer where CSET's can be converted,
		or taken directly from p0p2 source (CSET same)

		STBL might like to provide such an item,
		including CSET recognicion, eg ASCII = no-conv-for this-word

		The parent might be able to lookup p0p2 left, right,
		left_n_right in its own fast lookup list (UL/LI)

		ALSO: STBL item lex:
		grab first 4 bytes of string into an aligned word
		padded with NUL's (or none) will catch most strings entirely!
		If processing list, check it before the hash.
		Then if the hash is zero, and last byte is NUL,
		have saves calling hash on 123 (soon 4567)
*/
		tokenised_namespace_elem = STR2_TOKENISE( _elem_name );
		// TODO RESOLVE
		return true;
	};

	if(!scan_name_1_expect( _elem_name, "namespace_elem_name" )) {
		return false;
	};
	used_elem_name_space = true;
	p0p2 space_name;
	space_name.p0 = _elem_name_space.p0;
	space_name.p2 = _elem_name.p2;
	tokenised_namespace_elem = STR2_TOKENISE( space_name );
	// TODO RESOLVE
	return true;
}

bool scan_nl_XML_LEX::scan_attr_val_q2_expect( str1 & val )
{
	// should really ROLL BACK when not found!
	// this is maybe a feature of expect ?
	// but overhead is low because EXPECTed!

	here_pos P0;
	here_start( P0 );

	if(!scan_char_expect('"')) { goto fail; }

	// actually VAL might come in parts ...
	// its also trick for it to fail?

	if(!scan_attr_val(val)) { goto fail; }

	if(!scan_char_expect('"')) { goto fail; }

	return true;
fail:
	here_back( P0 );
	return false;
}


/*
	always return true - could be an empty string?
*/
bool scan_nl_XML_LEX::scan_attr_val( str1 & val )
{
	p0p2 zone;
	p0p2_start( zone );
	if(!scan_upto_char( '"' ) )
	{
		if(0) return false; // debug
		// must set VAL to NUL // return true;
	}
	p0p2_stop( zone );
	val = zone;
	return true;
}

bool scan_nl_XML_LEX::GAP_after_LEX()
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

		if( found1 ) found = true;
	}
	P_start_next = P;		// pointing to next lex
	GAP_after_LEX_found = found;
	return GAP_after_LEX_found;
}

bool scan_nl_XML_LEX::ml_gap()
{
	bool found = false;
	bool found1 = true;
	while( found1 )
	{
		found1 = false;
		if( scan_nl() ) found1 = true;
		if( scan_crlf() ) found1 = true;
		if( scan_spaces_and_tabs() ) found1 = true;

		if( found1 ) found = true;
	}
	return found;
}

bool scan_nl_XML_LEX::text_gap()
{
	bool found = false;
	bool found1 = true;
	while( found1 )
	{
		found1 = false;
		if( scan_nl() ) found1 = true;
		if( scan_crlf() ) found1 = true;
		if( scan_spaces_and_tabs() ) found1 = true;

		if( found1 ) found = true;
	}
	return found;
}

bool scan_nl_XML_LEX::text_gap( p0p2 & gap )
{

	// a1 must be a subset of a2
	if(!peek_a1( cset_xml_ident_a1 ) ) return false;

	p0p2 zone;
	p0p2_start( zone );
	if(!text_gap())
	{
		return false;
	}
	p0p2_stop( zone );
	gap = zone;
	return true;
}

bool scan_nl_XML_LEX::scan_text_1( p0p2 & str )
{
	if(!peek_a1( cset_xml_text ) ) return false;

	p0p2 zone;
	p0p2_start( zone );
	scan_a1_plus( cset_xml_text );
	p0p2_stop( zone );
	str = zone;
	return true;
}





