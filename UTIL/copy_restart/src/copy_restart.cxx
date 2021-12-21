#include "dgb.h"
// #include "ints.h"
// #include <stdio.h>
#include "copy_restart.h"
// typedef unsigned int uns;

#include "copy_restart.h"
// #include "copy_file_dir.h" // Module += copy_restart.h module //
// #include "copy_file_dir.h" // Module += copy_list_file_dir module // foreach
// SCRIPT LIBR VFS_OPS vfs_client _on_ROM_RAM_API
// LIBR STO // Module { LIBR_STO } // AUTO GEN "${1:-LIBR_STO}" Module
// TOKEN "Module" triggers FILTER_MATCH_expr TOKEN & = item_name() // STO &
// STO & // ITEM == EA_ITEM // VAR ITEM_t ITEM = INIT_ITEM == INIT_NULL NBYTES SPEC
// SPEC api // LEX_PUNCT "&" // FLAG +LEX_EDGE_DETECT LEX_PARSER
// NAME %s_%s lhs rhs // DECODED_HERE VAR_NAME rhs
// DECODED HERE // CODE_POINT_code_point_t HERE = { HERE_t HERE } 
//# { HERE_t HERE } # lhs # rhs # 
//# run_SESS( HERE_t here ) { SESS } // SESS created itself as LANDMARK CODE_POINT
// { CODE POINT } // CODE.fieldname -BIND-SAME- POINT.fieldname_expr EXPR
// { EXPR } // 
// [ ITEM } // HERE_t here; // EXPLAIN HERE
//
// { PAIR } // AUTO GEN VOCAB POOL // word association // import top_csr_api
// { lhs rhs } //
// { %s %s } // AXIOM ITEM HERE lhs rhs // $0 == AXIOM $1 = lhs %2 = rhs
// AXIOM template_with u16_parameters // one // two // few // ten // N // & //
// SCRIPT & is the CXX operator("PUNCT('&')") // LEX_RESERVED_WORD_PUNCT
// LEX // ITEM_t == LEX_PUNCT_AMP_t LEX_item; // Item_t & item = &expr(ARGS)
// ITEM // NOUN in SESS // NBYTES_EA_KEPT = EXPR(EA_KEPT,ARGS)
// CODE // EA_KEPT // CODE POINT // PAIR of ITEM in common SORROUND
// CODE_POINT // EA_HERE == "LOCN NAME" // LANG += multi_part_name // SPACE //
// SPACE_DETECT // Parser Lexer SUBLEX // PSG SPACE // TEXT_with_added_INDENT
// LEX_DETECT // JOIN %s%s // JOIN "%s %s" // internal SPACE //
// LEX_PAIR // PAIR_DETECT // FILTER_MATCH // VOCAB VAR_POOL //
// TEMPLATE { %s %s }
// PSG_MATCH "MATCH { %s %s }; // CMNT" // lhs rhs CMNT CODE // ITEM = TOKEN
// TOKEN // id_of_var_in_SCOPE // SESS_VAR thread_local EA_ITEM_in_EXPR
// ALIAS EXPR
// NBYTES data == EA_BASE0 SPEC_t & SPEC = LOOKUP("SPEC") // first example is ow_id_str
// own_id str // Name == "SPEC" // FILTER get_EA_of("SPEC") // stringify NAME
// dialect // SPEAK Name_of_VAR == "SPEC" // MATCH %s // WINDOW_LINES_PARSER_LEX
// LEX Name == "SPEC" // type_name // var_name // sample_data // _name // _EA
// EA = lookup("ANYSTR") // SPEC
// EA = lookup("SPEC") // ANYSTR
// MIX WORLD // lhs_stream rhs_stream here_as_code_point /* with */ ARGS 
// CODE ARGS // CODE == HERE
// MATCH EVAL // RUN ARGS SESS // SESS

/*
	Note to Future SELF - writing PSG EDGE_DETECT - and_still_bison
	Note to Future SELF - name of ITEM is %s ARGS { SCRIPT }
	SCRIPT {
		// SCRIPT += HERE
		ARGS += NAME_as_ANYSTR // PSG_DETECTS_NAME_POOL_GEN_SCRIPT
		ITEM += NAME_AS_ANYSTR // PSG_RANGE_OF_POSSIBLE_MATCING_PHRASES
		NAME += "%s" // SCRIPT auto_adds_Q2
		AUTO_Q2 += {

		// simply inventing terms, declares them // nearby extra info avail
		// AUTO GEN // hide extra added syntax auto_gen_Q2 // MODE
		// PARSE MODE // "% "%s" // no Q2 first option second
		}
		MATCH "%s" ALIAS_POOL += ALIAS '"%s"' "%s" // %s // MATCH HERE
		MATCH_HERE { IDX = LOOKUP("IDX") KNOWING += ALIAS "HERE"
		GEN RULE name first instance after INIT_VAL
	
	 SCRIPT OBJECT SPEC INIT_VAL CURR_VAL AMP // APM == IDX // ALIAS from here

	}

*/

bool bool_main( int argc, char ** argv ) {

	set_prog_name( argv[0] ); // check this
//	gdb_sigaction( argv[0] ); // check this

//	gdb_invoke(false);

	const char * src_name = "/home/gps/G/RIPS/eg_rip_DVD_here/iso/"
	"THE_DVD_DISC_1.iso";
	const char * dst_dirname = "/nfs/NAS_ln_s/NAS3_b2_DVDS/iso";

	if( argc != 3 ) {
		errno = 22;
		FAIL("USAGE %s src dst_dirname # argc %d", get_prog_alias(), argc );
		WARN("test defaulting src and dst_dirname");
//		return false;
	} else {
		src_name = argv[1];
		dst_dirname = argv[2];
	}

	if(!copy_file_dir( src_name, dst_dirname )) {
		return FAIL_FAILED();
	}
	return true;
}

int main( int argc, char ** argv ) {
	if( bool_main( argc, argv )) {
		PASS("DONE");
		return 0;
	}
	if(errno) 
		return errno;
	WARN("NO ERRNO");
	return 1;
}
