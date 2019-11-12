#ifndef Tree_PSG_H
#define Tree_PSG_H

#include "dgb.h"
#include "obj_ref.h"
#include "obj_list.h"
#include "buffer2.h"

#include "Tree_PSG_LEX_TOKEN.h"

class Tree_PSG_RULE { public:
	str1 name;
	enum RULE_TYPE {
	 rule_ONE_OF,
	 rule_SEQ,
	 rule
	};
};

class Tree_PSG { public: // PSG in MEM STO !MMAP // this is what we are building

 // ARGV // track origin of data used in SYSCALLS	
 // ARGV // names of lex and yacc file(s) in working dir

	// str1 holds the malloced memory

 	str1 psg_name_base; // gen_e1 		... FILENAME SUBLEX
 	str1 lex_name_base; // gen_e1_lex 	.lex
 	str1 yacc_name_base; // gen_e1_yacc 	.y

	bool set_PSG_name( STR0 _name_ );

	STR0 yacc_name_y(buffer2 & str);
	STR0 lex_name_lex(buffer2 & str);
	STR0 yacc_name_tab_hh(buffer2 & str); // mildly inefficient

	bool put_include_Q2( buffer2 & out, buffer2 & incl_filename );

	bool put_yacc_name_tab_hh( buffer2 & out );
	bool put_include_yacc_tab_hh( buffer2 & out );

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

#if 0
	LEX_TOKEN_GROUP POOL_[ u8_idx ]; // PARTIAL u8 N = 3; // UDEF_ = N++
	 LEX_TOKEN_GROUP POOL_PUNCT	= POOL_[0];
	 LEX_TOKEN_GROUP POOL_RW	= POOL_[1];
	 LEX_TOKEN_GROUP POOL_LEX	= POOL_[2];
	 LEX_TOKEN_GROUP POOL_UDEF	= POOL_[N++];
#endif
	// fixup 0 means full is_empty knows not empty // -1 clip +1

	// u8_idx GRP = N++; /* know is_not_empty */
	// u8_idx GRP = get_IDX_of_LEX_TOKEN_GROUP("RW_%s") // RW

	bool add_GRP_RW_Name_Str(
		u8_idx GRP, 	// prefix "RW_%s"
		STR0 ReservedWord
	);

	// maybe make TOKEN have EA_double_step_expr_u32
	// maybe make TOKEN hold all links around
	// tok.print_TOKEN_name2( out, TOKEN )
	// maybe stuck here
	bool print_TOKEN_name_2(
		buffer2 & out,
		LEX_TOKEN_GROUP &  POOL,
		LEX_TOKEN_DECL * tok //  tok = POOL.LIST_Token[ i ];
	);

	// PFX_name -becomes- _name_
	bool print_TOKEN_name_3(
		buffer2 & out,
		LEX_TOKEN_GROUP &  POOL,
		LEX_TOKEN_DECL * tok //  tok = POOL.LIST_Token[ i ];
	);

	bool print_include_yacc_tab_hh( buffer2 & out );

	/*!
	*/
	// print list of each item in LEX_TOKEN_GROUP
	// THIS is the style guide
	bool print_list(
	 buffer2 & out,
	 LEX_TOKEN_GROUP &  POOL
	);

	/*!
		GEN lex and yacc files

		These are NOT virtual, because that is for build_tree()
		Once the tree is built, how it is printed is fixed.

		You could probably switch to another style of output
		By deriving from this, and making things virtual

		Also needs a namespace container


	*/
	bool print_tree_as_files( );

	// GEN // sections of LEX file

	bool gen_LEX( buffer2 & out ); // all of it
	bool gen_LEX_lex_return( buffer2 & out ); // integrate LEX api
	bool gen_LEX_RULES_eoln( buffer2 & out );
	bool gen_LEX_RULES_ident_values( buffer2 & out );

	// GEN // sections of YACC file

	bool gen_YACC( buffer2 & out ); // all of it

	bool gen_YACC_top_code( buffer2 & out );
	bool gen_YACC_str_of_token( buffer2 & out );
	bool gen_YACC_str_of_token_cases( buffer2 & out, LEX_TOKEN_GROUP & list );
	bool gen_YACC_union( buffer2 & out );
	bool gen_YACC_token_list_POOL( buffer2 & out, LEX_TOKEN_GROUP & list );
	bool gen_YACC_token_list( buffer2 & out );
	bool gen_YACC_type_list( buffer2 & out );
	bool gen_YACC_precedence_list( buffer2 & out );
	bool gen_YACC_start_top( buffer2 & out, const char * rule_name = NULL );
	bool gen_YACC_rules( buffer2 & out );

};

#endif

