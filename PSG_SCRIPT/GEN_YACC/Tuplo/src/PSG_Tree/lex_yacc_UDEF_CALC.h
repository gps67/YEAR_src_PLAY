#ifndef Tree_PSG_CALC_H
#define Tree_PSG_CALC_H

#include "PSG_STUBS.h"
#include "Tree_PSG.h"
#include "Tree_PSG_UDEF.h"
namespace PSG {

class Tree_PSG_UDEF_CALC : public Tree_PSG_UDEF { public:

// VIRTUAL // no C++ need to repeat mention it tho
//	bool build_tree(); // use default _lex then _yacc
	bool build_tree_lex();
	bool build_tree_yacc();

};


}; // namespace
#endif  // PSG_UDEF
