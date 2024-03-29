#ifndef XML_DOC_H
#define XML_DOC_H

// # was 2007
// # now 2023 # TODO reconstruct # DIALECT #
// # API # XML_DIALECT # STO NBYTES # PLUS ADVICE EXYTA PLUS # EA ID IDX # 
// # IDX # itdx_t # typedef # named macro # usage_api { obj_t obj }
// # OBJ # "{ obj_t obj } # CMNT CODE_POINT # "{ CMNT CODE_POINT { SCRIPT }}"
// BENCH // ALLOC IDX_t idx = N_ALLOCATE ++ # CODE_POINT CMNT SCRIPT # teatime #
// BEENCH // ALLOC IDX // ALGORITHM 
// DOC // DIARY += TEXT in CTXT with API // MK_VAR // AUTO // HINT // EG_val
// XML // TREE_of_XML LEX == "{ ... }" // API += mk_VAR //
// DOC doc // # 
// DTD basic // TREE_BUILD_api xml_loader api // api += ASPECT // MERGE API //
// API SESS LIBR STO  ALIAS NBYTES STO_t EA EXPR SCRIPT // nested text PSG API 
// API NBYTES ALIAS CODE_POINT //
// SEGMENT_API EA SCRIPT NEARBY SPEC OBJ EXPR ITEM EA ALIAS //
// ALIAS SCRIPT += CODE_POINT CACHE { PRE END } 
/* mk mention */ 
// ALIAS SCRIPT += CODE_POINT CACHE { OBJ SPEX EXPR VAR_POOL LIBR UDEF SCRIPT } 

/*
	DIALECT must know CSET BYTE UTF8 NBYTES IDX EXPRS SEL == EXPR
	SCRIPT CODE_POINT += STMT ARGS 
	SCRIPT CODE_POINT += ITEM SPEC # LIBR += EXPLAIN #
	LIBR += SCRIPT += PHRASE += " { SPEC ITEM } " // Compile name { str0_t str0 ) 
	ITEM += EA // reminder that implements DECL SPEC SCRIPT_POOL SESS NOW IDX
	ITEM += IDX // MERGE IDX API // API += ITEM // DIALECT // NOUN // ITEM //
	EA += { typedef IDX_t IDX } // VAR CODE_POINT EA_ITEM IDX_ALLOC // 
	// import ALLOC // AUTO // CODE_POINT 
	PSG += ARGV_PSG STR0 CSET PSG DIALECT
	DIALECT += CODE_POINT += SCRIPT TEXT EXPR ITEM NAME ALIAS EXPR STO API
	API += STO // MERGE // same dialect // same name_space // scope //

	SCRIPT += SCRIPT_LINE // place your UDEF here " { ARGV } " // CMNT //
	CMNT += PARSE_SCRIPT_CMNT // FLAGS += "{ CSET == UTF8 }"

	SCRIPT_PSG += STMT_is_ARGV // SCRIPT_STEP_ARGV // STEP // AUTO LEX 
	SCRIPT_PSG += STMT_is_CODE_BLOCK_of_CODE // SCRIPT_STEP_STR0_NBYTES // STEP // AUTO LEX 
	LEX += LEX_MUX += ANYSTR += STR0 += NBYTES_NAME 
	// += ID_ATTR
	CSET += PICK_UTF8 += API STR0 NBYTES ALIAS SPEC STO 
	KNOW_LEX CIDENT ANYSTR COMPILED_SCRIPT ARGS 
	COMPILED += SCRIPT DATA ARGS // ALIAS // LIBR // += FEATURE MERGE
	
	LEX WORD = LANG STR0 p0p2 p0p2_api += get_P0_get_P2 ZZ
*/



#include "XML_proto.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "str2.h"

namespace XML_DOC {
 using namespace XML_DTD;


 /*!
	group subclsses in doxygen report
 */
 struct GRP_XML_DOC { };

 /*!
	group subclsses in doxygen report
 */
 struct GRP_XML_DOC_obj : public obj_ref { };

 /*!
	T_text_frag is a virtual base class, 
	T_text_frag is obj_ref0_t refcounted;

	SCRIPT FRAGMENTS scattered in the_WILD 
	PARSED SCRIPT CACHE // build EXPR TREE of SCRIPT API
	
	one_of {
		T_elem_inst
		T_sdata_char
		T_text_frag 
	 //	 UTF8 == "STR0" // compile "EXPR" to get ALIAS WITH_PUNCT
	 //	 STR0 is T_text_frag 
	 //	 ALIAS = CACHED TDX from STR0 // UTF8
		T_word_ascii
		T_word_8859
		T_word_utf8
		T_word_list
	}
 */
 struct T_text_frag : public GRP_XML_DOC_obj 
 {
	virtual ~T_text_frag() {};
 };

 /*!
	An element, and all thats in it (an instance of an elem).

	info might be double stepped, or be an alias

	elem->info->name
	atts[i]->info->name
	text[i] -> (T_text_frag *) VTBL

	Note that T_text_frag is a virtual base class.
	It might be an elem, text, sdata_char, or even
	a custom type.

	SPIN_py used PyObject * as the virtual base,
	which allows an element to be converted into ANY object,
	but how should that object behave (API).

 */
 struct T_elem_inst : public T_text_frag
 {
	T_elem_spec *		info;
	obj_list<T_attr_val>	atts;
	obj_list<T_text_frag>	text;	// string or elem
 };

 /*
 struct T_elem_as_object : public T_text_frag
 {
	obj_ref * object;

	virtual void print_tree( SGML_PRINTER & printer ); 
		converts the object back to an XML subtree
		with care that might even be a sublist subsegment
		This is inherited from T_text_frag of course!

	virtual bool filter( SGML_FILTER & filter );
		filter.dtd2 is not filter.dtd1 (doc's dtd)
		filter.stack2 
		filter.grove2
		filter.table_of_contents2
		may be a simple copy object
		maybe copy pointer over
		maybe deep copy
		maybe covertion process - specified in filter
		(cant have code in filter that does this
		without calling generic hooks for this class ...)
 };
 */

 /*!
	An sdata_char is a char, usually named, but maybe numeric.

	It might not be as unique as elem_spec or attr_spec,
	but it might be.

	virtual for complex cases?

	I WOULD PREFER THIS IN THE DTD zone,
	but it is referenced many times as a T_text_frag from the DOC

	I would also like many sets of symbols to be shared,
	carried in their own imported DTD (font of chars ...)
 */
 struct T_sdata_char : public T_text_frag
 {
	// yes its specific to the dtd!
	T_DTD *		dtd;
	obj_hold<str2>	name;
	int		unicode_char;	// or NUL
	str1		utf8_string;	// only-if unicode_char is NUL
 };

 /*!
	a generic base class for a named object, with VTBL, ref_count

	used to hold "ASCII" or "en" or "My_MIME_Tag"

	There is some scope for Enum_Group (a generic class somewhere)
 */
 struct T_some_token : public GRP_XML_DOC_obj
 {
	obj_hold<str2>	name;
 };

 /*!
	"ZLIB_COMPRESSED"

 */
 struct T_encoding_token : public T_some_token 
 {
	T_encoding_token()
	{
		// lookup in spelling!
		// lookup in table of encodings
		// use a static !
		name = new_str2_static( "plain" );
	}
 };

 /*!
	"en"
 */
 struct T_language_token : public T_some_token 
 {
	T_language_token()
	{
		// lookup in spelling!
		// lookup in table of encodings
		// use a static !
		name = new_str2_static( "en" );
	}
 };

 /*!
	"ASCII"
 */
 struct T_cset_token : public T_some_token 
 {
	T_cset_token()
	{
		// lookup in spelling!
		// lookup in table of encodings
		// use a static !
		name = new_str2_static( "ASCII" );
	}
 };

 /*!
	8859-en-base64

	at some stage I wanted T_text_frag to describe itself
	one often reused pointer, selects 3 attributes
	but that adds yet another word per word!
	also lang neutral &amp; (blends in with container)
 */
 struct T_enc_cset_lang : public GRP_XML_DOC_obj
 {
	obj_hold<T_cset_token>		cset;
	obj_hold<T_language_token>	language;
	obj_hold<T_encoding_token>	encoding;
	// direction_left_to_right
 };

 /*!
	a single word (or fragment of a multi-part-word)

	add: enc_cset_lang -> one of {
		(plain,ascii,en)
		(plain,8859,en)
		(plain,byte,en)
		(plain,utf8,en)
	}
	// Thats an EXTRA parameter for soft decoding (lang)
 */
 struct T_word : public T_text_frag
 {
	// get these from flags ?
	// move these down to T_text_frag
	virtual bool is_word() { return true; }
	virtual bool is_word_list() { return false; } // one only
	virtual bool is_word_edge_left() { return true; }
	virtual bool is_word_edge_right() { return true; }
	virtual bool is_multi_part_word() { return true; }
	virtual bool is_whitespace() { return false; }
	virtual bool has_whitespace() { return false; } // internal
	virtual bool is_MPW_with_adornments() { return false; } // CAP1 quotes stop
	virtual bool is_left_to_right() { return true; } // cset/lang?
 };

 /*!
	text is composed of ASCII chars, probably not CONTROL.

	MAY CONTAIN & <

	SHOULD NOT contain <b>bold</b> &amp;
	unless .. unsure ..

	NEEDS flag: may contain nuts
 */
 struct T_word_ascii : public T_word
 {
	str1 text;
 };

 /*!
	normal word lists have INFERRED single space between words
 */
 struct T_word_whitespace : public T_word_ascii
 {
        virtual bool is_whitespace() { return true; }
 };

 /*!
	I only do 8859-9 (its the one with Euro symbol)

	8859-1 will be folded in blind!

	8859-2 will be common for some regions, and is to
	be treated to its own class (or soft via BYTE)
 */
 struct T_word_8859_9 : public T_word
 {
	str1 text;
 };

 /*!
	word probably (certainly) contains char above 127

	NEED FLAG: contains chars above 255
 */
 struct T_word_utf8 : public T_word
 {
	str1 utf8;
 };

 /*!
	I'm focussing on ascii/BYTE/8859/utf8_8859/utf8_catchall_L2R
	but you can specialise into urdu when ready
	
 //
 struct T_word_BIG5 : public T_word
 {
	T_strBIG5 strBIG5;
 };
 */

 /*!
	A list of words, PROBABLY with spaces removed.

	A word _MIGHT_ be a list of words (if you want it to be).

	A multi-part-word is a special holder which contains word-fragments.
	For simplicity, they are T_text_frag * any, even words.
	Spaces will be explicitly mentioned (a multi-part-word
	is not wrappable, but it MIGHT be hyphenatable, and splittable.
	It gets complex, but its all behind the door).
 */
 struct T_word_list : public T_text_frag
 {
	obj_list<T_word>	word_list; // tautology
 };

}; // XML_DOC

#endif
