#ifndef Tree_PSG_H
#define Tree_PSG_H

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

#include "Tree_PSG_LEX_TOKEN.h"

class Tree_PSG { public:

 // ARGV // track origin of data used in SYSCALLS	

 	const char * lex_name_lex;
 	const char * yacc_name_yacc;

	// nested class is LOCAL not exported // STUBBED

	typedef u8 u8_idx;	// no loud fail at 256 tho

	// UDEF needs these - others probably do too

	LEX_TOKEN_GROUP POOL_PUNCT;
	LEX_TOKEN_GROUP POOL_RW;
	LEX_TOKEN_GROUP POOL_LEX;

	~Tree_PSG();
	Tree_PSG();

	bool add_GRP_RW_Name_Str(
		u8_idx GRP, 	// prefix "RW_%s"
		STR0 ReservedWord
	);



};

#endif

