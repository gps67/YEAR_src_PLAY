#ifndef Tree_PSG_CALC_H
#define Tree_PSG_CALC_H

#include "Tree_PSG.h"
#include "Tree_PSG_UDEF.h"

class Tree_PSG_UDEF_CALC : public Tree_PSG_UDEF { public:

// VIRTUAL // no C++ need to repeat mention it tho
//	bool build_tree(); // use default _lex then _yacc
	bool build_tree_lex();
	bool build_tree_yacc();

};


#endif  // PSG_UDEF
