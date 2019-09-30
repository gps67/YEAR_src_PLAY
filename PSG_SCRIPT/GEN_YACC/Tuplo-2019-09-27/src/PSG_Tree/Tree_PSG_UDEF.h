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

	bool print_list(
	 LEX_TOKEN_GROUP &  POOL,
	 buffer2 & out
	);

	bool build_tree();
	bool print_tree_as_files( );

};

 // SCRIPT is a MODULE but mostly SCRIPT

#endif  // PSG_UDEF
