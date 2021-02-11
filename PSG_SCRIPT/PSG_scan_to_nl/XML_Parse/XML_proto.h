#ifndef scan_data_xml_proto_H
#define scan_data_xml_proto_H

/*
	forward declarations of things used in XML

	incomplete declarations can be used as obj_hold<T>
*/

/*!
	DOC = Document data
*/
namespace XML_DOC {

 struct GRP_XML_DOC;
 struct GRP_XML_DOC_obj;

 struct T_elem_inst;	// element and its contents
 struct T_attr_val;	// attribute and value
 struct T_text_frag;	// VTL text gap char word_list app_obj ...
 struct T_sdata_char;	// T_text_frag: named sdata char, or numbered one
 struct T_entity;	// T_text_frag: named entity inclusion of some sort
};

/*!
	DTD = Document Type Declaration (from XML)
*/
namespace XML_DTD {

 struct GRP_XML_DTD;
 struct GRP_XML_DTD_obj;

 struct T_NS;
 struct T_NS_POOL;

 struct T_DTD;		// one DTD per document, one in, two out, one mid, ...
 struct T_elem_spec;	// elements name, rule (dtd_frag), atts-list
 struct T_attr_spec;	// attributes declaration (specific to one element)
 struct T_dtd_frag;	// (A | B C)* RULE
};

/*!
	XML Parse things, Stack, STBL, ..
*/
namespace XML_Parse {

 struct GRP_XML_Parse;
 struct GRP_XML_Parse_obj;

 class XML_Stack_Base;	// nested place within document, parse cursor too ?
 class XML_Parse_Top;	// input file, Stack, filter, output file(s), dtd, mode
};

#endif

