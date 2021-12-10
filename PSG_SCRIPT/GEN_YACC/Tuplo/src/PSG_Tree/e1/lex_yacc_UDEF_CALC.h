#ifndef lex_yacc_CALC_H
#define lex_yacc_CALC_H

#include "PSG_STUBS.h"
#include "lex_yacc.h"
#include "lex_yacc_UDEF.h"
namespace PSG {

class lex_yacc_UDEF_CALC : public lex_yacc_UDEF { public:

// VIRTUAL // no C++ need to repeat mention it tho
//	bool build_tree(); // use default _lex then _yacc
	bool build_tree_lex();
	bool build_tree_yacc();

};


}; // namespace
#endif  // PSG_UDEF
