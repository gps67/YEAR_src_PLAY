#ifndef scan_xml_H
#define scan_xml_H
// ONLY for various
// #include "file_scan_base.h"

	// xml uses scan_file
	//  c1 uses file_scan !!

#include "scan_to_nl.h"
#include "scan_nl_xml_lex.h"

#include "scan_file_base.h"

#include "XML_proto.h"
#include "XML_Parse.h"

using namespace XML_DTD;
using namespace XML_DOC;

/*!
	scan_xml_1 holds the input file (NOT STREAM??)
	and the output Stack (partially built tree)

	It provides functions that parse phrases,
	various variables used on the way, etc
*/
class scan_xml_1 : public scan_file_base
{
 public:

	obj_hold<XML_Parse::XML_Stack_Base>	Stack;
	mmap_file f;
	scan_nl_XML_LEX LEX;

	scan_xml_1()
	: scan_file_base()
	{
	}

	~scan_xml_1()
	{
	}


	bool scan_file( const u8 * filename );

	// still LEX'ing around - third level! move to STBL
	bool elem_name_expect();

	bool scan_from_PI_xml();
	bool scan_from_PI_NAME( p0p2 _pi_name );
	bool scan_from_PI_ANON();

	bool scan_from_comment();

	// DTD stuff
	bool scan_from_CDATA();
	bool scan_from_DOCTYPE();
	bool scan_from_ELEMENT();
	bool scan_from_ATTLIST();
	bool scan_from_ENTITY();
	// percent_entity
	// percent_entity_semicolon

//	int x_start;
//	int y_start;

//	void get_x_y_start();

	// elem_spec * _elem_spec;
	p0p2 _elem_name_space;
	p0p2 _elem_name;

};
#endif
