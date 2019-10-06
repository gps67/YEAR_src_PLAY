#ifndef Tree_PSG_UDEF_H
#define Tree_PSG_UDEF_H

#include "Tree_PSG.h"

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

	// maybe make TOKEN have EA_double_step_expr_u32
	// maybe make TOKEN hold all links around
	// tok.print_TOKEN_name2( out, TOKEN )
	// maybe stuck here
	bool print_TOKEN_name_2(
		buffer2 & out,
		LEX_TOKEN_GROUP &  POOL,
		LEX_TOKEN_DECL * tok //  tok = POOL.LIST_Token[ i ];
	);

	bool print_list(
	 buffer2 & out,
	 LEX_TOKEN_GROUP &  POOL
	);

	bool build_tree();
	bool print_tree_as_files( );

};

 // SCRIPT is a MODULE but mostly SCRIPT

#endif  // PSG_UDEF
