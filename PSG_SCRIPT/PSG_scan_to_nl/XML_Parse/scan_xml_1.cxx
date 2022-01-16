#include "scan_xml_1.h"
#include "str1.h"
#include "stbl_spelling.h"
#include "lex_lookup.h"

#include <stdio.h>

#include "XML_DTD.h"


/*!
	a quick mock-up, to get the wheels moving on STBL_ITEM
	and delivery of varients, eg different & sdata ; forms
	by subroutines, when only & seen at top.

	http://www.w3.org/TR/2004/REC-xml-20040204/
*/

bool scan_xml_1::elem_name_expect()
{
	if(!LEX.scan_elem_name_expect()) {
		return false;
	}
	// convert lex.tokenised_namespace_elem to elem_spec via Stack
	if( LEX.used_elem_name_space ) {
		Stack->dtd->lookup_elem( _elem_name_space, _elem_name );
	} else {
		Stack->dtd->lookup_elem( _elem_name );
	}
	return true;
}

bool scan_xml_1::scan_file( const u8 * filename )
{

	str1 attr_name;
	str1 attr_val;
	str1 sdata_name;
	str1 text;

	p0p2 _item_ns;
	p0p2 _item_name;

	p0p2 _attr_name;
	p0p2 _attr_val;
	p0p2 _sdata_name;
	p0p2 _pi_name;
	p0p2 _text;

	Stack = new XML_Parse::XML_Stack_Base();
	Stack->dtd = new XML_DTD::T_DTD();


	int i;
	/*
		maybe map_file f; should be a part of LEX
		maybe the caller should open the file and provide data
	*/
	if(!open_file( filename, &LEX ))
		return false;

	LEX.text_gap();

	bool ok = true;
	while(1)
	{
		if( LEX.scan_char('<')) { // < 
		 if( LEX.scan_char('/')) { // < / 
			// GAP NOT ALLOWED BEFORE NAME // LEX.ml_gap();
			if(!elem_name_expect()) {
				break; // FAIL ACTION 
			}
			LEX.ml_gap();
			if( !LEX.scan_char_expect('>')) {
				break; // FAIL ACTION 
			}
			/*
				check that end-tag matches start tag
				call XML_TREE_STACK.ElementEndTag()
				it calls Elem->Info->ElementEnd( STACK )
				(no parameters)
			*/
		// 	printf("</%s>\n", (STR0)*LEX.tokenised_namespace_elem );
			printf("<</%s>>\n", (STR0)*LEX.tokenised_namespace_elem );
		 } else if( LEX.scan_char('!')) { // < !
			if( LEX.scan_chars('-','-')) { // < ! --
				scan_from_comment();
// #warning "not _word but ..."
			} else if( LEX.scan_word("[CDATA[") ) {
				scan_from_CDATA();
			} else if( LEX.scan_word("[cdata[")) {
				scan_from_CDATA();
			} else if( LEX.scan_word("DOCTYPE")) {
				scan_from_DOCTYPE();
			} else {
				LEX.report_WARN("TODO xxx have <!");
			}
		 } else if( LEX.scan_char('?')) {	// < ?
			if( LEX.scan_word("xml") ) {
				scan_from_PI_xml();
			} else if(LEX.scan_name_1(_pi_name)) {
				scan_from_PI_NAME( _pi_name );
			} else {
				scan_from_PI_ANON();
			}
			// rest of PI appears as TEXT - FIXUP

		 } else { // < - NOT </
			LEX.ml_gap();
			if(!elem_name_expect()) {
				break; // roll_back?
			}
			while(1)
			{
				if(LEX.scan_char('>')) {
					break;
				}
				if(LEX.scan_chars('/','>')) {
					printf(":%s ... /> - empty (closes in opening)\n", (STR0)* LEX.tokenised_namespace_elem );
					break;
				}

				LEX.ml_gap(); // NEWLINE

				if(!LEX.scan_attr_name(_attr_name)) {
					LEX.report_FAIL("missing attr_name or '>'");
					// roll_back?
				}
				// each attr_name can only happen ONCE!
				attr_name = _attr_name;
				// LEX.report_OK( attr_name.str );

				LEX.ml_gap(); // ALLOW NAUGHTY FILES
				if(!LEX.scan_char_expect('=')) {
					break; // FAIL ACTION 
				}
				LEX.ml_gap(); // ALLOW NAUGHTY FILES
				/*
					attr_val might have all sorts of
					cset multi-part complexity
					This call needs replacing ...
				*/
				if(!LEX.scan_attr_val_q2_expect(attr_val)) {
					LEX.report_FAIL("duplicate_message");
					break; // FAIL ACTION 
				}

				LEX.ml_gap();

				// attr_val = _attr_val;
				printf(":(!)%s(!ELEM)... %s=\"%s\"\n",
					(STR0) * LEX.tokenised_namespace_elem,
					(STR0) attr_name.str,
					(STR0) attr_val.str
				);
			}
		 }
		} else if( LEX.scan_char('&')) {
			if( LEX.scan_char('#')) {
				int chr = 0;
				if(LEX.scan_char( 'x')) {
					if(!LEX.scan_digits_hex_expect( chr)) {
						// reported
						break; // FAIL ACTION 
					}
					printf("SDATA: &#x%x;\n", chr );
				} else
				if(LEX.scan_int( chr)) {
					printf("SDATA: &#%d;\n", chr );
				} else {
					LEX.report_FAIL("expected int");
					break; // FAIL ACTION 
				}
			} else {
				if(!LEX.scan_sdata_name_expect(_sdata_name)) {
					break; // FAIL ACTION 
				}
				sdata_name = _sdata_name;
				printf("SDATA: &%s;\n", sdata_name.str );
			}
			if(!LEX.scan_char_expect(';')) {
				break; // FAIL ACTION 
			}
		} else if( LEX.scan_text_1( _text )) {
			text = _text;
			printf("TEXT: %s\n", text.str );
			
		} else if( LEX.text_gap()) {
		} else if( LEX.scan_nl()) {
		} else if( LEX.scan_eof()) {
			break;
		} else {
			LEX.report_FAIL("##UNKNOWN-ERROR##");
			int ch1 = LEX.get_byte();
			printf("SKIP: chr(%d)\n", ch1 );
		}
	}
	return ok;
}

/*
	here is a typical standards mess up,
		<?xml version="1.0"?>
		<?xml version="1.0" encoding="UTF-8" standalone='yes'?>
	the inner text has to be interpreted, like attributes of an elem
	but OTHER PI's dont have any imposed structure
	wouldnt it be better to have <XML> like <HTML> does ??
	
	<!DOCTYPE name SYSTEM "file.dtd">
	<!DOCTYPE name [ ... ]>
		<!ELEMENT name (#PCDATA)>
		<!ATTLIST pre xml:space (preserve) #FIXED 'preserve')>
		<!ATTLIST p xml:space (default|preserve) 'preserve')>
		<!ATTLIST ukdesc xml:lang CDATA 'en'>
		<!ELEMENT name _SPEC_ >
		<!ELEMENT name EMPTY >
		<!ELEMENT name ANY >
		<!ELEMENT name _MIXED_ >
		<!ELEMENT name _CHILDREN_ >
		_CHILDREN_:	(a|b|c)+	? * +
		_CHILDREN_:	(a,b,c)?	? * +
		_CHILD_:	a?	? * +
		_CONTENT_PARTICLE_
		_MIXED_:	( #PCDATA )
		_MIXED_:	( #PCDATA | A | B )*	-- NOT +
		_CP_:		&somename;
	<!ATTLIST elemname	-- multimples OK
		attrname ID #REQUIRED
		attrname CDATA #REQUIRED
		attrname CDATA #IMPLIED		-- no default value
		attrname CDATA #FIXED "default" -- says cant change?
		attrname CDATA defaultval
		attrname (one|two|three) "two"	-- eg ..?
	>
	<!ENTITY % incls SYSTEM "http://..../incls.entities"> %incls;
	<!ENTITY % draft 'INCLUDE'>
	<![%draft;[ ...more_dtd_decls_or_text?... ]]>
*/


bool scan_xml_1::scan_from_comment()
{
	LEX.report_WARN("TODO parse_from_comment");
	return true; // or throw?
}

bool scan_xml_1::scan_from_CDATA()
{
	LEX.report_WARN("TODO parse_from_CDATA ]]>");
	return true; // or throw?
}

bool scan_xml_1::scan_from_DOCTYPE()
{
	LEX.report_WARN("TODO <!DOCTYPE ... ");
	return true;
}

bool scan_xml_1::scan_from_ELEMENT()
{
	return true;
}

bool scan_xml_1::scan_from_ATTLIST()
{
	return true;
}

bool scan_xml_1::scan_from_ENTITY()
{
	return true;
}

bool scan_xml_1::scan_from_PI_xml()
{
	p0p2 _attr_name;
	while(1) {
		LEX.ml_gap();
		if(LEX.scan_word("version=\"1.0\"")) {
			LEX.report_OK("PI:XML:verion=1.0");
			continue;
		}
		if(LEX.scan_word("encoding=\"UTF-8\"")) {
			LEX.report_OK("PI:XML:encoding=UTF-8");
			continue;
		}
		if(LEX.scan_word("standalone=\"yes\"")
		|| LEX.scan_word("standalone=\'yes\'")) {
			LEX.report_OK("PI:XML:encoding=UTF-8");
			continue;
		}
		if(LEX.scan_name_1(_attr_name)) {
			if(!LEX.scan_char_expect('=')) {
				return false;
			}
			str1 attr_val;
			if(!LEX.scan_attr_val_q2_expect(attr_val)) {
				LEX.report_FAIL("duplicate_message");
				return false;
			}
			LEX.report_OK("PI:XML:name='val'");
		}
		if(LEX.scan_chars('?', '>')) {
			break;
		}
		LEX.report_FAIL("PI");
		return false;
	}
	return true;
}

bool scan_xml_1::scan_from_PI_NAME( p0p2 _pi_name )
{
	str1 pi_name;
	pi_name = _pi_name;
	printf("PI <?%s\n", pi_name.str );
	return true;
}

bool scan_xml_1::scan_from_PI_ANON()
{
	printf("PI <?%s\n", "ANON" );
	return true;
}


