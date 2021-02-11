#ifndef scan_nl_xml_lex_H
#define scan_nl_xml_lex_H

#include <stdio.h> // fprintf

#include "str1.h"	// str1
#include "scan_to_nl.h"	// getc buffer with lex matching

extern int trans_id;

class scan_nl_XML_LEX : public scan_to_nl
{
 public:

	str1	zone_dox_line;
	str1	zone_dox_block;

	str1	zone_dox_pre_line;
	str1	zone_dox_pre_block;
	str1	zone_dox_end_line;
	str1	zone_dox_end_block;
	str1	zone_cmnt_line;
	str1	zone_cmnt_block;
	bool GAP_after_LEX();

	bool ml_gap();
	bool text_gap();
	bool text_gap( p0p2 & gap );

	/*
		statics

		also mentioned in the .cxx file 
	*/
	static int cset_inited_xml; // init defined as = 0;

	/*
		actually this could vary per-lexer, so not static ?

		Do you want to allow above ASCII into xml names?
		If so, its probably, the valid ASCII a1/a2
		with "any" char above 128 (128+32), add any above 255
	*/
	static bool cset_xml_ident_above_255;
	static cset_bit_map cset_xml_ident_a2;
	static cset_bit_map cset_xml_ident_a1;
	static cset_bit_map cset_xml_attr_text; // not q2
	static cset_bit_map cset_xml_text;

	scan_nl_XML_LEX();
	scan_nl_XML_LEX( p0p2 zone );

	void init_csets(void);

	/*
		really this should resolve namespaces ?
	*/
	bool scan_name_1( p0p2 & name );
	bool scan_elem_name( p0p2 & name );
	bool scan_elem_name_expect( p0p2 & name );
	bool scan_attr_name( p0p2 & name );

	bool used_elem_name_space;
	p0p2 _elem_name_space;
	p0p2 _elem_name;
	// obj_hold ???
	str2 * tokenised_namespace_elem;
	// resolved ... p0p2 _elem_spec;
	bool scan_elem_name_expect();

	bool scan_sdata_name( p0p2 & name );
	bool scan_sdata_name_expect( p0p2 & name );

	bool scan_name_1_expect( p0p2 & name, const char * tname );

	bool scan_attr_val( str1 & val );
	bool scan_attr_val_q2_expect( str1 & val );

	bool scan_text_1( p0p2 & val );

// RESIDUE ...


};

#endif
