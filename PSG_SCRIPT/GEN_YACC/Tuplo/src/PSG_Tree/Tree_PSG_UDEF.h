#ifndef Tree_PSG_UDEF_H
#define Tree_PSG_UDEF_H

#include "PSG_STUBS.h"
#include "Tree_PSG.h"
namespace PSG {

 /* OR DIFFERENTLY NAMED
 class PSG_UDEF : public PSG_Tree_Module {
 };
 */

 /*
 	AUTO import PEER FEATURE in same MODULE
 	AUTO import base_libr STR0 list
 	import base_libr TOKEN_MEANINGS_and_other_LOOKUP
 */


class Tree_PSG_UDEF : public Tree_PSG { public:

	virtual bool build_tree();
	virtual bool build_tree_lex();
	virtual bool build_tree_yacc();

	// UDEF gets a head start, hope it sort of works :-)
	// eg MATRIX works where NUM was
	// eg STRING likes += rhs
	// eg LISTS like += item // api_is_with_list_impl_new_item
	// LIBR could make this a UTIL over PSG_tree, not subclass, OPTS
	virtual bool add_lex_for_C_EXPR();
	virtual bool add_yacc_for_C_EXPR();

};

}; // namespace
#endif  // PSG_UDEF
