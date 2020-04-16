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
	 rule_DATA, // PSG_STRUCT _SPEC DECODER( TOKEN ) { DIAG } api
//	 rule_ _LIST _VECT _ITEM_t api EXPRS("anystr")
	 rule_TOKEN, // PLUS // alias explains // UDEF
	 rule_lex_ITEM, // n_bytes probably maybe P0P2
	 rule_NAMED, // ITEM_NAMED //
	 rule_all_of_these_are_unwritten // at the mo //
	};
};

/*
	A Tree is an entire GRAMMAR
	A second Tree could share the same SEGMENT or INDEX_LAYOUT (api)
	One day Tree's will call on LIBR for common definitions

	A Tree_PSG is a BUILDER of an entire GRAMMAR Tree
	It uses a lot of GEN code to print namelex.lex and nameyacc.y
	Along with requiring EXPRS.cxx // c_api_C_OPERATOR_EXPRS
	Along with requiring EXPRS.cxx // cpp_api_CPP_OPERATOR_EXPRS // CTOR
	require Tuplo.h and TreeBuilder_ and _TreePrinter_

	GEN writes PSG and SUBLEX to _tree_ to _printer_
	PSG has a generic Tree, which can be parametised to printer

	With some RTFM, will be able to export lots of FSM machines
	and pick TOP = top TEXT = str1 TEXT = buffer2 TEXT = stdin
	STREAM PRELOAD_text through RM_loader or_follow PREBUILT_table
	every CODE point is SOURCE_CODE // possibly_alongside_SEGMENT

	TODO REFACTOR group "PSG_name" psg_ lex_ yacc_ psg_ AS Name_t & name;
	FIND LOCN = ITEM = TOKEN_ID _EXPR SUBLEX_variations_EA_ITEM_EXPR
	TOKEN NAME = "PSG_name"
	MATCH set_%s NAME ARGS // as_part_of_C_api_function_declaration
	GEN CVAR psg_name NAME SUBLEX(NAME) sublex_

	eg LEX_TOKEN_GROUP POOL_PUNCT; // += "[a-zA-Z0-9_$] // += "[%:;]"
	 CSET pool_var_subaddr += "CSET_for_("anystr") += "+="

	 Tree is sum of incoming stream of TOKENISED ARGV
	 DATA = SPEC over INSTREAM // TOKENISED

	 Tree is a GEN of yacc files and some api
	 hence it is "../obj/gen_e1_lex.lex // EXPR_lex.o _yacc
*/

class Tree_PSG { public: // PSG in MEM STO !MMAP // this is what we are building

 // ARGV // track origin of data used in SYSCALLS	
 // ARGV // names of lex and yacc file(s) in working dir

	// str1 holds the malloced memory

	// e1 is ( EXPR ) // ( ARGV ) // api_of_e1
	// gen_e1 // local_ROOT_e1 // PSG_of_E1
	// probably GET_EA HEAP ITEM

 	str1 psg_name; // gen_e1 		... FILENAME SUBLEX
 	str1 lex_name; // gen_e1_lex 	.lex
 	str1 yacc_name; // gen_e1_yacc 	.y

 	str1 psg_item; // gen_e1_psg // ##_psg##_reappears##_as //

	//	CVAR NAME str1 psg_name; // "gen_e1" 
	//	MATCH %s_psg NAME == "gen_e1"
	//	EXPORT . /*as*/ NAME 
	//	EXPORT . /*.subfield.*/ NAME /**?
	//	MATCH . EXPR { SCRIPT }

	// THIS EA_TREE _of_PSG GEN_CODE_ROM_LOCN  
	// The Name of the grammar "gen_e1" // generated psg for e1 exprs E1
	// BENCH LOCN 0xFFFF . // EA_HERE is "." ID_t & PSG_name_tok;


	bool set_PSG_name( STR0 _name_ ); // anystr == _name_ == "../obj/gen_e1"
	// PSG "name" // e1 exprs E1 
	// PSG " // e1 exprs E1 // " { SCRIPT } // where SCRIPT is PARSED_TEXT

	STR0 yacc_name_y(buffer2 & str);
	STR0 lex_name_lex(buffer2 & str);	// flex_machine_for_PSG
	STR0 yacc_name_tab_hh(buffer2 & str);	// TOKEN_POOL lexicon;

// NAME = "../obj/gen_e1" // the filename_exts used are FIXED or GLOBAL config
// TEXT generators:
//  GEN // uses a lot of buffer2's // refactor buffer2 to accept buf60

	/* PIVOT POINT
		instead of (as well as) having BASE_TYPE buffer2
		type_match c_prototype_api SELF in ARGS

		derived class makes things very close-to-hand
		c_func_ARGS ARGS {
		 c_func = c_prototype c_name 
		  prototype
		   INTRET cident ARGS
		  ARGS
		  ARG_i [s]
		   N_LIST N==0 .is_empty()
		   ARG_i 
		    type_lhs
		    bind_opts // "&" "*" "**" // CT_SPEC_RefCount_responsibility
		    bind_opts // == = -> ".subfield.%s" 
		    bind_opts // FILTER_NAME to VAR use TOK // poss const var0
		}

		BENCH can promote ELF info API_SPEC // ALSO available in ROM

		ARGS now binds to a point in ROM

	*/

	bool put_include_Q2( buffer2 & out, buffer2 & incl_filename );
	//	#include "incl_filename"; 

	bool put_yacc_name_tab_hh( buffer2 & out );
	//	../obj/gen_e1_yacc.tab.hh

	bool put_include_yacc_tab_hh( buffer2 & out );
	//	#include "../obj/gen_e1_yacc.tab.hh"; 

 // MOVE // decls used in api // near but not here
 // MOVE // STUBS might carry subset of typedef names;
 //
	  typedef u8 u8_idx_t;	// no loud fail at 256 tho
	  // NO WANT u8_idx AVAIL // for varname // not typename

 // PFX_mylex eg ... RW_while PUNCT_SP LEX_value_str LEX_value_union
 // UDEF_ident_C99 // or simpler subset
 // UDEF needs these - others probably do too

	LEX_TOKEN_GROUP POOL_PUNCT;
	LEX_TOKEN_GROUP POOL_RW;
	LEX_TOKEN_GROUP POOL_LEX;

 // XXXX

 	// PHRASE_BOOK
	// lookup("STR") 
	// TABLE[u8] of ITEM = (TABLE=table_item)[ idx_item ] // each u64_word

// DOING
//	PGG_PHRASE_LIST
//	ZZ

 // are we avoiding VTBL or diving right thru

	~Tree_PSG();
	Tree_PSG();

#if 0
	LEX_TOKEN_GROUP POOL_[ u8_idx_t ]; // PARTIAL u8 N = 3; // UDEF_ = N++
	 LEX_TOKEN_GROUP POOL_PUNCT	= POOL_[0];
	 LEX_TOKEN_GROUP POOL_RW	= POOL_[1];
	 LEX_TOKEN_GROUP POOL_LEX	= POOL_[2];
	 LEX_TOKEN_GROUP POOL_UDEF	= POOL_[N++];
#endif
	// fixup 0 means full is_empty knows not empty // -1 clip +1

	// u8_idx_t GRP = N++; /* know is_not_empty */
	// u8_idx_t GRP = get_IDX_of_LEX_TOKEN_GROUP("RW_%s") // RW

	// tok.print_TOKEN_name_1( out, TOKEN ) "%s"
	bool add_GRP_RW_Name_Str(
		u8_idx_t GRP, 	// prefix "RW_%s"
		STR0 ReservedWord // MATCH // ? //
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

