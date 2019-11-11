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

	bool build_tree();
	bool build_tree_lex();
	bool build_tree_yacc();

};

 // SCRIPT is a MODULE but mostly SCRIPT

#endif  // PSG_UDEF
