#ifndef XML_Parse_H
#define XML_Parse_H

/*
	namespace XML_Parse
	namespace XML_DTD
	namespace XML_DOC
*/
#include "XML_proto.h"
#include "obj_ref.h"
#include "HT_table.h"


namespace XML_Parse {
 using namespace XML_DOC;
 using namespace XML_DTD;

 /*!
	group subclsses in doxygen report
 */
 struct GRP_XML_Parse { };

 /*!
	group subclsses in doxygen report
 */
 struct GRP_XML_Parse_obj : public obj_ref { };

 /*!
	whilst the DOC is being read in, the XML_Parser keeps a stack

	Each item on the stack, is an elem_inst, but incomplete.

	Each ALSO has a MODE which controls how the item is parsed,
	this is contentious, as it breaks the simple design of
	'sorting it out at the end-element'. Instead it opens the
	door to analysing the element surface and deep structure.
	EG SVG/XUL might parse <window width="1234" ... expecting
	the width attribute to be an integer, and going directly
	to a struct xul_window->width, using that struct as the parser!

	MODE should come from the elem_spec (VTBL) - or shoudl it ?

 */
 class XML_Stack_Layer_VTL : public GRP_XML_Parse_obj
 {
  public:
	/*!
		plain XML gets loaded as XML tree

		elem_inst is almost usable as a Stack_Layer direct,
		but a few extra flags means not.

		The actual elem_inst might be un-optimised,
		particularly the lists of stretch length.
		Maybe the elem_inst should be split
	*/
	 obj_hold<T_elem_inst>		elem_inst;

	XML_Stack_Layer_VTL()
;
//	: elem_inst()
//	{
//	}

	~XML_Stack_Layer_VTL();

	/*!
		delete but dont delete

		if elem_inst isnt used elsewhere - keep it
	*/
	void recycle();

	/*!
		setup, maybe recover after recycle
	*/
	void re_init( T_elem_spec * elem_spec);

 };

 class XML_Stack_Base : public GRP_XML_Parse_obj
 {
	/*!
		this is a 'fake' element
		try not to add it to any lists
	*/
	obj_list<XML_Stack_Layer_VTL>	stack_recycle;
	obj_list<XML_Stack_Layer_VTL>	stack;

  public:
	obj_hold<T_DTD>			dtd;
	XML_Stack_Layer_VTL *		sp;

	void push(T_elem_spec * elem_spec);
	void pop();
 };

};


#endif

