#ifndef Tree_PSG_LEX_PUNCT_NAME_h
#define Tree_PSG_LEX_PUNCT_NAME_h

#include "ints.h"
#include "buffer1.h"
#include "PSG_STUBS.h"
namespace PSG {

//	Tree_PSG_LEX_PUNCT_NAME.h

extern // buff += LEX.punct_name()
bool print_LEX_punct_name( buffer1 & name, u8 chr );
// ALIAS  print_LEX_punct_name( buffer1 & name, u32 chr );

}; // namespace
#endif
