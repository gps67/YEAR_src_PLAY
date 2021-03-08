#ifndef yacc_lex_CALC_H
#define yacc_lex_CALC_H

#include "PSG_STUBS.h"
#include "yacc_lex.h"
#include "yacc_lex_UDEF.h"
namespace PSG {

class yacc_lex_UDEF_CALC : public yacc_lex_UDEF { public:

// VIRTUAL // no C++ need to repeat mention it tho
//	bool build_tree(); // use default _lex then _yacc
	bool build_tree_lex();
	bool build_tree_yacc();

};


}; // namespace
#endif  // PSG_UDEF
