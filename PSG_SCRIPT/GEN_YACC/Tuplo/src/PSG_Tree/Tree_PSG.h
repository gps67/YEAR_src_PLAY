#ifndef Tree_PSG_H
#define Tree_PSG_H

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

#include "Tree_PSG_LEX_TOKEN.h"

class Tree_PSG { public: // PSG in MEM STO !MMAP // this is what we are building

 // ARGV // track origin of data used in SYSCALLS	
 // ARGV // names of lex and yacc file(s) in working dir

 	const char * lex_name_lex;
 	const char * yacc_name_yacc;

 // MOVE // decls used in api // near but not here
 // MOVE // STUBS might carry subset of typedef names;
 //
	  typedef u8 u8_idx;	// no loud fail at 256 tho

 // PFX_mylex eg ... RW_while PUNCT_SP LEX_value_str LEX_value_union
 // UDEF_ident_C99 // or simpler subset
 // UDEF needs these - others probably do too

	LEX_TOKEN_GROUP POOL_PUNCT;
	LEX_TOKEN_GROUP POOL_RW;
	LEX_TOKEN_GROUP POOL_LEX;

 // are we avoiding VTBL or diving right thru

	~Tree_PSG();
	Tree_PSG();

	LEX_TOKEN_GROUP POOL_[ u8_idx ] // PARTIAL u8 N = 3; // UDEF_ = N++
	 LEX_TOKEN_GROUP POOL_PUNCT	= POOL_[0];
	 LEX_TOKEN_GROUP POOL_RW	= POOL_[1];
	 LEX_TOKEN_GROUP POOL_LEX	= POOL_[2];
	 LEX_TOKEN_GROUP POOL_UDEF	= POOL_[N++];
	// fixup 0 means full is_empty knows not empty // -1 clip +1

	// u8_idx GRP = N++; /* know is_not_empty */
	// u8_idx GRP = get_IDX_of_LEX_TOKEN_GROUP("RW_%s") // RW

	bool add_GRP_RW_Name_Str(
		u8_idx GRP, 	// prefix "RW_%s"
		STR0 ReservedWord
	);



};

#endif

