#ifndef Tree_PSG_UDEF_AFM_H
#define Tree_PSG_UDEF_AFM_H

#include "PSG_STUBS.h"
// #include "Tree_PSG.h"
#include "Tree_PSG_UDEF.h"
namespace PSG {

 /* OR DIFFERENTLY NAMED
 class PSG_UDEF : public PSG_Tree_Module {
 	// Module = Loaded with ARGV_of_EXPR ; scripted with SCRIPT DATA
	// Loaded with elf shared_global_data_zone
	// ZONE_NBYTES {
	//  TOKENISED_TEXT
	//   TEXT _asis _utf8 _lib_8859_api
	//    // API = lib_8859_to_utf8 //
	//    // DATA = MIXED_but_mostly_builtin_api 
	//    // PARSE = CSET_ASCII_PLUS
	//    // FILTER += BASE64_field_in_filename_URL //
	//    // ASM = CPU = MACHINE = DIAG += FILTER_CSET_ZLIB_BASE64_FILTER
	//    // PARSE = CSET_ASCII_PLUS 8859 UTF8 PLUS
	//    // SUBLEX = SUB_LEX_filename_CSET_ASCII_PLUS 8859 UTF8 PLUS
	//    // SUB_LEX_filename_CSET_ASCII_PLUS // _PLUS_ 8859 UTF8 PLUS
	//   TEXT CSET DECODER // direct to DATA 8-bit clean NON-TTY SEGMENTS
	//    NBYTES = $0.nbytes() // with implied (NBYTES_t) { OFFS N }
	//    // $0 means this TREE_of_SPEC_and_LOCN_and_DATA // all of it
	//    // $0 means this ITEM_of_SPEC_and_LOCN_and_DATA // near to it
	//	// this == root_node_of_PSG_TREE_and_PARSER_MACHINE
	//	// this == leaf_node_of_PSG_TREE_and_PARSER_MACHINE
	//	// this == root_node_is_leaf_node
	//	// this == root_node_any_leaf_node // PARSER MACHINE
	// PSG = { // += _t // auto filter simplify view sublex explains _t
	//  PSG_MODULE "PSG" = {
	//   // MODULE // SCRIPT // BOOT_STRAP // CORE_TYPE //
	//   // RULE SEQ L I S T // CODE_ATTACHED SCRIPT = NBYTES
	//   // RULE ONE L I S T // NO_CODE_ATTACHED builtin foreach item LIST { SCRIPT }
	//   // RULE LINES TEXT NBYTES // as_P0P2 // as_P0_N // as_UDEF
	//   // UDEF starts with PSG_Module psg.root_node
	//   // UDEF follows with PSG_leaf psg.detail //
	//   // TRIP TREE PSG_leaf_node ATTR ... VALUE ...
	//   // LEX POOLS LEX_RW LEX_TOKEN LEX_VALUE //
	//   // PRINTABLE LEX_VALUE // EMIT_or_OMIT // drop LEX_ prefix
	//  }
	// }


	// }
 };
 */

 /*
 	AUTO import PEER FEATURE in same MODULE
 	AUTO import base_libr STR0 list
 	import base_libr TOKEN_MEANINGS_and_other_LOOKUP
 */


class Tree_PSG_UDEF_AFM : public Tree_PSG_UDEF { public:

// 	virtual bool build_tree();
 	virtual bool build_tree_lex();
 	virtual bool build_tree_yacc();

	// UDEF gets a head start, hope it sort of works :-)
	// eg MATRIX works where NUM was
	// eg STRING likes += rhs
	// eg LISTS like += item // api_is_with_list_impl_new_item
	// LIBR could make this a UTIL over PSG_tree, not subclass, OPTS
// 	virtual bool add_lex_for_C_EXPR();
// 	virtual bool add_yacc_for_C_EXPR();
 	virtual bool add_lex_for_AFM();
 	virtual bool add_yacc_for_AFM();

};

}; // namespace
#endif  // PSG_UDEF
