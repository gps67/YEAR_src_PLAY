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
		plain XML ELEM gets loaded as XML tree

		 obj_hold<T_elem_inst>	elem_inst; // THIS // AUTO MACRO 

		FILTER may need to collect all EDITS into Q_BUFFER for LATER

		 obj_hold<XML_Stack_Layer_VTL> BUILDER_of_ELEM // THIS // rhyme

		CODE_PLUS { DECL class "UDEF" : public "UDEF_base" { ... } ; }

		 SCRIPT allows YOU to provide it's BASE class
		 OBJECT_base == "UDEF_base" // incl UDEF API CALL early

		BENCH LIBR == "UDEF"
		BENCH LIBR == "UDEF_base"
		BENCH LIST += NEW ITEM // makes some callbacks available in src

	BENCH CACHE LIST += NEW ITEM // makes some callbacks available in src
	BENCH CACHE LIST += NEW ITEM // LIST_NAME // "ANYSTR" // or cident
	BENCH ATTR_LIST += ATTR
	BENCH TEXT_ITEM |= TEXT
	BENCH TEXT_ITEM |= SDATA
	BENCH TEXT_ITEM |= ELEM_and_its_TREE
	BENCH TEXT_ITEM |= ITEM_OBJ // PARSED ELEM_TREE 

		BENCH += START ELEM ...

		BENCH += TEXT_ITEM ELEM // its ITEM and its TREE
		STREAM += TEXT_ITEM TEXT // MUX channels // AUTO GEN //
		STREAM += TEXT_ITEM SDATA // MUX channels [STREAM] // AUTO GEN //
		// compiler detechs MATCH use of name "STREAM" for VAR of NAME
		// BENCH expands that to NAME_VAR_as_VAR_NAME "STREAM_%d"
		// BENCH prefers "STREAM_%s" // which AUTO_USES STR == "%02X"
		// DIALECT %s // obviously that needs AUTO_PRINT // EA_DIRECT

	
	BENCH TEXT_ITEM |= ITEM_OBJ_scripted_subtype ... ARGS ...

		// PAIR "ITEM_OBJ_t" "ITEM_OBJ_scripted_subtype_t" //
		// from CODE_POINT "ITEM_OBJ" "ITEM_OBJ_scripted_subtype" "_t"
		// turns out "_t" is a clever inline parser_gen
		// recognise all forms of SUBLEX_over_IDENT
		// all forms of IDENT
		// all forms of SUBLEX
		// all forms of UDEF_PICK from LIST
	
	BENCH CACHE_LIST += PARSED LIST of TEXT_ITEM

		AUTO_VAR "CAPS"
		AUTO_DIALECT "ITEM_OBJ" and "_base" are known to PICK = VALUE
		AUTO_CACHE PICK = VALUE = EVAL EXPR ARGS

	OBJECT ITEM_OBJ // BIND LINK // requires ITEM_OBJ to be provided
	// ie UDEF typedef UDEF_ITEM ITEM_OBJ_t //
	// ie UDEF typedef UDEF_ITEM UDEF_FIVE_ITEM_EA_DETAIL UDEF//
	// ie UDEF typedef UDEF_ITEM_typename_varname//
	BENCH TEXT_ITEM |= PI // PARSED PI LEX LIST SP // SPELLED // PARSED //
	PARSE LEX_START_SUBLEX

		API += BENCH.CACHE.API

		elem_inst is almost usable as a Stack_Layer direct,
		but a few extra flags means not. Se we provide it
		along with this BENCH ADJACENT MACRO "MATCH"

		 WORK BENCH on "MATCH"

		 	MATCH == "THIS" || "SELF" || "ALIAS CODE_POINT"

			CACHE += CODE_POINT // so above syntax doesnt have to

			MATCH += "ALIAS" "%s" // PARAMETER_DIRECT_THROUGH_ASIS
			MATCH += "ALIAS" "'%s'" // PARAMETER_DIRECT // ASIS //

		The actual elem_inst might be un-optimised,
		particularly the lists of stretch length.
		Maybe the elem_inst should be split

		 WORK BENCH on "ELEM_STREAMED" "ELEM" IDX 
		 # archaeology says ELEM_%s is prototype of ITEM("ELEM")
		 # root step "ELEM_" is_actually_based_on_ "ELEM_STREAMED" "%"

	LIBR_name1_ELEM_name2( ELEM LIBR )
	LIBR_name1_ELEM_name2_UDEF_name3( ELEM LIBR UDEF )

		 BENCH ZONE( "ELEM_STREAMED" ) // KMOW_BASECLASS ASIS ELEM
		 BENCH ZONE( "ELEM" ) // : public "ELEM_STREAMED" // SUBCLASS
		 BENCH ZONE( "ELEM_LIBR_UDEF" ) // THESE FUNCTIONS BELONG HERE
		 # step up == "ELEM"
		 # stay_here == "LIBR"
		 # setp_next == "UDEF"
		 BENCH ZONE( "ELEM" ) // focus BENCH on "ELEM" // field_1
		 BENCH ZONE( "LIBR"  // focus BENCH on "%s") // field_2
		 BENCH ZONE( "UDEF"  // focus BENCH on "%s") // field_3

	 obj_hold<BENCH_ITEM>		BENCH_ITEM; // tools to edit elem_inst
	 obj_hold<T_elem_inst>		elem_inst; // this var this value

	 STACK
	  NAMESPACE
	   ELEM 
	    ATTR
	    ATTR
	   STREAMED
	    TEXT
	    ELEM
	    SDATA
	    MACRO
	    PI

	 STACK
	  NAMESPACE
	   ELEM 

	    ELEM += ATTR

	    	name "ELEM"
		value "ELEM_NAME"

		name "ATTR"
		value "attribute"

		name "VALUE"
		value "PARSED_VALUE %s" // DIALECT ARGV SIMPLE PRE ANYSTR
		// AUTO add CALL print_into_requoted CSET_FILTER_within_WORD
		// SERIALISE and PARSE shift gear from "SAMPLE VALUE" to "PARSED"
		value_mime_type // misnomer OK .. but is LEX_FORM_PICK_GEN
		OPTION GEN $s_%2d
		OPTION GEN ALIAS %s %s_%2s // LHS IDX // IDX +1 // +1 DIALECT
		DIALECT "%2d" // AUTO_DETECT VIEW AS "%3d"
		MACRO "%2d" expands evals to "%02d" // DIALECT REMAP STMT MACRO
		MACRO_NAME_looks_like_VALUE // _incl_PUNCT_CMNT_CODE_POINT_PLUS

		KNOW about "VALUE" NOT_cident MATCHES_PUNCT_in_STR_VAL

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

