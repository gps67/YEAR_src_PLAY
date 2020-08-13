
#include "Tree_PSG.h"
#include "Y_PARSE.h" // parameter

Tree_PSG::
Tree_PSG()
{
	POOL_PUNCT.PFX = "PUNCT"; // OK omit trailing _
	POOL_RW   .PFX = "RW";   // OK omit trailing _
	POOL_LEX  .PFX = "LEX"; // OK omit trailing _

	// CSET 
}

Tree_PSG::
~ Tree_PSG()
{
}

#include "dir_name_ext.h"

#define if_WARN_1 if(0)

bool Tree_PSG::
set_PSG_name( STR0 _name_ ) // "anystr" // "e1" // "gen_e1" // "e1_E1" // EXPR
{
	bool near_not_far = true;
	if(0) near_not_far = false; // use FULL PATH NAMES

	if(0) INFO("_name_ %s", _name_ ); // ../obj/gen_e1

	// e1 exprs E1 // PSG over MMAP 
	// gen_e1
	// ../obj/gen_e1
	//
	// PSG_Name = ../obj/gen_e1
	// LEX_Name = ../obj/gen_e1_lex	.lex .cc .o _yacc .y .cc .tab.hh .o
	// YACC_Name = ../obj/gen_e1_yacc .y .cc .tab.hh .o
	//
	// HERE_step LIBR_item PSG_udef .o .api
	// need to call SITE API LIBR/udef.api
	// EG PSG = "udef" {
	//  HERE // 
	//  HERE //  SCRIPT = HERE
	//  HERE // 
	// }

	// COMMENTED OUT // STRICT PAYLOAD //
	//
	// local_var PSG_Name = ... SELF ... // ITEM is in a ... Q ...
	//  DATA lhs_rhs 
	// local_var LEX_Name = LHS _ RHS // LHS="PSG_Name" RHS="_lex" // ,lex
	//  DATA lhs_rhs = PSG_lex // _Name // 
	// local_var YACC_Name = LHS _ RHS // LHS="YACC_Name" RHS="_yacc" // .y
	//  DATA lhs_rhs = PSG_yacc // _Name // 
	//  DATA lhs_rhs = PSG_psg // _Name // _DATA_SEGMENT STO //
	// // Parser // variations on Tuplo_PAIR_lhs_rhs_cmnt_code_data
	// // these are variations on Tuplo_PAIR_lhs_rhs_cmnt_code_data
	// // Relax! dont like it? rename "LHS" "RHS" to "ARGG_1" "ARG_2"
	// // Bridges from CXX_chaos _bridge_ DIAG_lhs_rhs // CMNT # LISP
	// // LANG: allow defered CMNT parameter, arriving on well known channel
	// // LANG: allow defered CMNT parameter, RETRO_placeholder NOW;
	// // LISP with Tuplo Blobs // and added paremeters in comments | ctxt
	// // CTXT thread_var PSG = Tuplo_PAIR_lhs_rhs_cmnt_code_data_ psg;
	// de-lint double mention "PSG" "psg" // MATCH as SAME // GEN as LHS
	// SPEC = A grammara called dir/name_something.psg
	//
	// OK avoiding SPEC = DATA_IN_FILE( filename.psg )
	// OK avoiding SPEC = BIND_var_SEGMENT_ITEM_RW
	// OK avoiding SPEC = CTXT.lookup( anystr ); // TOKENISED PREBUILT
	//
	// PSG = LEX + YACC // PSG = FLEX + BISON // 
	// PSG = SUBLEX + SEQUENCER
	// SEQ = PSG_ spec=SPEC lhs=LHS rhs=RHS //

/*
	// PSG = SEQ + LEXICON + SUBLEX_actually_used // WAS

	We now list our requests to the PSG machine

		u32_hilo idx = PSG++ // NEW N++;
	
	Maybe a shared scope with rules, but a coded environment

		PSG = idx // then PSG = NEAR.lookup( "PSG=idx;" );
	
	It has it's own shelf near BENCH

		BENCH.PSG = { SPEC }
		BENCH.PSG = { DATA }
		-----.-------------- // an Essex comma // decimal_point // unit_
		BENCH.PSG = { INST }
	
	INST = SPEC + DATA
		_ == INST used as JOIN with "_"
		TYPED_STRUCTS _ Tree_of_DATA
	
	MODULE = SOUND_Foundation

		SPEC_Module = well_known.SPEC_Module
		DATA_Module = STO_fetch_SPEC_Module_DATA_at_EXPR

		SPEC=Module.SPEC_Module // getter function api options
		EXPR = EA_in_ROM 
		 ROM_Module = PREBUILT_Module
		  // PARSE // name_both_type_var = "TEXT_TO_EOLN"
		  // TEXT_TO_EOLN = declare FN using C_prototype_fn_decl
		  // fn_decl markers ret_val ret_type [ret_var] fn_name fn_args
		  // DIALECT fn_ret_val 
		  // EXPLAIN ret_val from fn_CPU_CALL_retval_
		  // BENCH create var_type var = INIT_VAL; // TREE_LOCN
		  // so BENCH is building a MODULE, adding item to some_shelf
		  // shelves might be stack_temp or sto_ROM_editable_to_SEGMENT
		  // shelves might be SEGMENT sub-zone ITEM { ... }
		  // HERE we are saying that our SEGMENT has EA_EXPR = u32_hilo
		  // STO uses u32_hilo // every FILE should //
		  // MEM converts to u32_cpu when accepting value from MEM_hilo
		  // CPU module has u32_hilo_swapb type // TODO adjust config
		  // CPU syscall // ASM_OS_SYSCALL_machine machine;
		  // EA = ...
		  // EA_EXPR = ...
		  // EA_in_ROM = ... // u3_u5_u8_u16 // 
		  // u3 = LAYER [s] // [u3] = LAYER_%d_%s // %s // u3 NAME[u3] CMNT[u3]
		  // CMNT // HERE // reusing above line as example to rephrase
		  // u3 = LAYER [s] // HERE // WHERE HERE is { ... }
		  // HERE # [u3] = LAYER_%d_%s // %s // u3 NAME[u3] CMNT[u3]
		  //  u3 = u3 // the IDX input value of u3 // also named u3
		  // Name = NAME[u3]
		  // CMNT = CMNT[u3] // STO NAME.CMNT PEER_FIELD via [u3]
		  // Token "Name" adds the selection mechanism
		  // SPEC "LAYER.idx = u3_from_EA" // hints at the existence of
		  // SPEC "LAYER.idx = u3" // LAYER_t & LAYER = LAYER_[u3];
		  // SPEC "u3_from_EA" // u3 = bitfield_u3 = ... local names
		  // EXPR "LAYER.idx = u3" // obtain idx from somewhere
		  // EXPR "LAYER.idx = u3++" // allocate idx HERE
		  // EXPR "LAYER.Name = Name" // probably converts LHS ro local RHS alias
		  // EXPR "LAYER.CMNT = CMNT"
		  //  out.printf("LAYER_%d_%s // %s \n", u3, Name, CMNT );
		  //
		  // u5 = ITEM
		  has u32_hilo_swapb type // TODO adjust config
		  u32_hilo_DECODER = u32_hilo EA_in_ROM( ROM EA )
		  // EA is WRT EA_ZERO shared by all of SEGMENT
		  // EA_ZERO might be SEGMENT_ZERO

		  MAYBE u3_u5_u8_u16_hilo // in file
		  MAYBE u3_u5_u8_u16_cpu // in register
		  MAYBE u3_u5_u8_u16_swapb // wierd math, simply make it _cpu
		  MAYBE u16_u8_u3_u5_swapb // u3 in high bits of byte
		  MAYBE u16_u8_u5_u3_hilo // u3 in low bits of byte
		  MAYBE u16_u8_u5_u3_cpu // cpu = swapb_asm(u32_hilo) // inline

		  // This works for X86 because of the way registers subdivide
		  // reax eax ax ah al
		  // OFFS = u32_hilo TABLE[ al ] // al = u3_u5 
		  // OFFS = u32_hilo TABLE[ a2 ] // ah = u8 
		  // IDX u16 ax = eax >> 16; // topfil zeros or ROTATE retain
		  // u32_hilo & OFFS = TABLE[ u16_AX ] // ITEM is u32_hilo
		  // u16 IDX = u16_AZ_item_IDX;
		  // u32_cpu OFFS = ITEM_TABLE[ IDX ] 
		  // ITEM_TABLE = HEAP_of_u16_ITEMS // upto, check N, or dont
		  //
		  // This works on ARM becuase ## IDK maybe similar byte_of_R
		  // no swap_b neede, always in hilo 
		  // 32 bit ARM's are still supported ?
		  // 64 bit ARM required ... for some modules 
		  // 64 bit ARM required ... for getting benefit of some modules 
		  // hopefully low cost, Architecture_32
		  // dont forget low cost, Architecture_16
		  // unaligned Tuplo might as use ... u8 ...

		VIEW
		 STRUCT = BENCH_LOCN
		  with BENCH_VARS
		   ROM = ROM_of_Module(Module);
		   EA = recompute_EA_in_ROM
		 VIEW
		  INST = DECODER( u32_hilo )
		   INST.u32 = u32_hilo; // recompute adjacent changes too
		   INST.u32 = set_analyse_u32
		    var DECODER = Module_script.action_point
		    var DECODER = Module_script.action_point

*/


	// 
	// GUESS SUBLEX LHS JOIN="_" "lex" || "yacc" || "PSG"
	// WHERE IS OUTPUT DIRECTORY
	//

	// START set_PSG_name( _name_ ) // "e1" -aka- EXPR
	// THEME C operator precedence PREBUILT Tuplo
	// template VAL_TYPE_NUM NUM // typedef float NUM;
	// PIXELS fold into float's u23 or so
	// PIXELS fold into float's u20 and u3_opcode

	// template VAL_TYPE_NUM NUM // typedef float NUM;
	// template VAL_TYPE_NUM OBJ // typedef void * OBJ;
	// template VAL_TYPE_NUM NUM_i20 // i20 XPOS = half_than_million //
	// template VAL_TYPE_NUM NUM_IDX_u20 // 1K x 1K // HEAP_ITEM
	// template VAL_TYPE_NUM NUM_u20 // 1K x 1K // ZONE_ITEM


	// INFO("_name_ %s", _name_ ); // "../obj/gen_e1" // "EXPR_C"

	dir_name_ext path_name;
	path_name.decode_filename( _name_ );  // some/dir/NAME

	// some/dir/NAME 
	// BECOMES => some/dir/NAME_yacc.y .cc .o .tab.hh
	// BECOMES => some/dir/NAME_lex.lex .cc .o

	// some/dir/NAME - expected, remove any .ext // or BETTER throw 
	path_name.ext.clear(); // sanity test it was, recycle genre from point

	// after any change rebuild full_path_name
	path_name.mk_BOTH_path_name();

	// the filename of the PSG - before adding extra_suffixes
	// psg_name & psg_name = "gen_e1" // C EXPR PSG
	psg_name = path_name.near_path_name; // str1 = ... probably STR0
if(0)	e_print("psg_name == '%s'\n", (STR0) psg_name );
if(0)	path_name.test_print(); // path/../obj/gen_e1


  // return FAIL("STOP HERE");

  	// PUSH path_name.name // single _path_name_decoder
	buffer2 tmp_name_keeper = path_name.name; // gen_e1

 	path_name.ext.clear();
	path_name.name.put("_lex");
	path_name.mk_BOTH_path_name();
	if(near_not_far) {
			lex_name = path_name.near_path_name;
	} else {	lex_name = path_name.full_path_name;
	}
if(0)	e_print("lex_name == '%s'\n", (STR0) lex_name );
	// gen_e1_lex.lex
if(0)	path_name.test_print();

  	// POP path_name.name // single _path_name_decoder
	path_name.name.set( (STR0) tmp_name_keeper );

 	path_name.ext.clear(); // really no noeed tho
	path_name.name.put("_yacc");
	path_name.mk_BOTH_path_name();
	if(near_not_far) {
			yacc_name = path_name.near_path_name;
	} else {	yacc_name = path_name.full_path_name;
	}
	// gen_e1_yacc.y
if(0)	path_name.test_print();

	return true; // OK 
}

STR0 Tree_PSG::
yacc_name_y(
	buffer2 & str // this is now rentrant 
){
	// REPLACE THIS WITH // bool gen_STR2_into( obj_hold<str2> & retval )
	// REPLACE THIS WITH // bool gen_STR2_into( buffer2 & out )
	// out is RETVAL
	// out is print_out via buffering filters // direct 
	// TEMPLATE {
	//  ... var decls move to STMT_GAP_SCOPE_START
	//  // TAMPLATE PARAMETER // out // tree_chunked_stream // _ROM
	//  buffer2 & out; // connected to tree chunked stream to generated file
	//  ... CODE upto HERE ...
	//  if(! tree.put_yacc_name_y( out )) return FAIL_FAILED();
	//  .... CODE_from_HERE ...
	//  // HINT That function can choose default, call script, FAIL_null()
	// }
	// tree_chunked_stream // shared buffer2 out // on_tide_level_high()
	//  at least LINES // but _lines_in_ can do that anyway
	// GEN places markers where tree stack completes starts upto_line pause
	//
	str = yacc_name;
	str.put(".y");
	return str;
}

STR0 Tree_PSG::
lex_name_lex(
	buffer2 & str // this is now rentrant 
){
	str = lex_name; // assignment copy init 0
	str.put(".lex");
	return str;
}

/*
	mildly inefficient, but very convenient
	maybe recognise usage, code in a service
*/
STR0 Tree_PSG::
yacc_name_tab_hh(
	buffer2 & str
	// moving this from static to provided
	// this is now multi-user
){
	// LANG // flag on object str // first_set_var_lock_second_waits_for_slip_lock
	// LANG // slip_lock = spin_lock for N1xN2 then CALL // repeat or break
	// LANG // scheduler can throw with C++ semantics catch ask/be scheduler
	str.clear();
	put_yacc_name_tab_hh( str );
	return str;
}

bool Tree_PSG::
put_yacc_name_tab_hh( buffer2 & out ){
	// needs auto add if(!put(...)) return FAIL_FAILED()
	out.put( yacc_name ); // gen_e1_yacc 	// .y // .tab.hh
	out.put(".tab.hh");
	return true;
}

bool Tree_PSG::
put_include_Q2( buffer2 & out, buffer2 & incl_filename )
{
	out.put("#include \"");
	out.put( incl_filename );
	out.put("\"\n" );
	return true; // return out.am_ok // clearable error flag
}

bool Tree_PSG::
put_include_yacc_tab_hh( buffer2 & out )
{
	buffer2 incl_filename; // clear() done obvs
	put_yacc_name_tab_hh( incl_filename );

	return
	put_include_Q2( out, incl_filename );
  	// if_WARN_1 L("#warning yacc_name_tab_hh included ");
}

bool Tree_PSG::
print_list(
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL
) {
	// maybe need longest first
	// maybe need Q2 punct fixing

	int n = POOL.LIST_Token.N();
	for( int i =0; i<n; i++ ) {
		LEX_TOKEN_DECL * tok = POOL.LIST_Token[ i ];

		if( tok-> Value ) {

			// what if Value uses Q2

			out.put( '"' );
			out.put( tok-> Value );
			out.put( '"' );

			out.put( " return TOKEN(" );
			print_TOKEN_name_2( out, POOL, tok );
			out.put( ");\n" );
		} else {
			// silent // no rules for edges
		}

	};
	// tok out of scape
	// INFO("is this called. value = '%s'", tok->Value );
	// INFO("is this called. out = '%s'", (STR0) out );
	return true;
}


/*
	The TOKEN = POOL_tok
	
	Because PFX_ comes from pool
	Because we dont have an uplink
	Because we didn't take "PFX_"
	
	On the other hand
	u4_tok taken from POOL somewhere
	
	In a sense, POOL is a list, idx in list ...
	but we have forgotten that ?
*/
bool Tree_PSG::
print_TOKEN_name_3(
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL,
	LEX_TOKEN_DECL * tok //  tok = POOL.LIST_Token[ i ];
) {
	//	out.put( "/* LEX_ */ " );
	//	out.put( POOL.PFX );	// MADNESS HERE
		out.put( '_' );
		out.put( tok-> Name );
		out.put( '_' );
		return true;
}

/*
	The TOKEN = POOL_tok
	
	Because PFX_ comes from pool
	Because we dont have an uplink
	Because we didn't take "PFX_"
	
	On the other hand
	u4_tok taken from POOL somewhere
	
	In a sense, POOL is a list, idx in list ...
	but we have forgotten that ?
*/
bool Tree_PSG::
print_TOKEN_name_2(
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL,
	LEX_TOKEN_DECL * tok //  tok = POOL.LIST_Token[ i ];
) {
	//	out.put( "/* LEX_ */ " );
		out.put( POOL.PFX );	// MADNESS HERE
		out.put( '_' );
		out.put( tok-> Name );
		return true;
}


/*	MACRO L("C_CODE_LINE") // MUST BE "const Q2 str to join \n"

	adds "\n" so you dont have to
	maybe rename out_put_L
*/

#define L(str) out.put( str "\n")
// its a function like macro so invoker provides semicolon

bool Tree_PSG::
gen_LEX_lex_return( buffer2 & out )
{
 L("// GEN");
 L("#include <string>");
 L("#include \"buffer1.h\"");
 L("#include \"Y_PARSE.h\""); //
 L("using namespace YY;"); //
 if(1) {
	put_include_yacc_tab_hh( out );
 }
 L("/*");
 L("        lookahead may mean any number of tokens, not 1");
 L("");
 L("        quick test shows actually OK 1 as long as yacc strdup's immediately");
 L("        but maybe");
 L("*/");
 L("");
 L("// nlex16 = N circular list of vars for single return value");
 L("// We dont know when each will be dropped just allow 16 as a LEX horizon");
 L("// static const int nlex16 = 64;");
 L("   static const int nlex16 = 16;");
 L("// static const int nlex16 = 1;");
 // L("static buffer1 lex_buffer;");
 L("static buffer1 lex_pool[nlex16]; // RETVAL STRVAL // backing store for STR0 ");
 L("static int nlex_pos = 0;");
 L("");
 L("/*");
 L("        lex found, return tok");
 L("");
 L("        lex found the string at yytext, yyleng");
 L("        use the next storage slot (reuse nlex16 items later)");
 L("        copy string to buffer, as str0");
 L("        set yylval /union/ .lex_buff as returning string value");
 L("        return tok as lex type");
 L("");
 L(" */");
 L("int lex_return( int tok )");
 L("{");
 L("        // skip first cell once, get it next time, or init -1");
 L("        nlex_pos = (nlex_pos+1) % nlex16;");
 L("        int pos = nlex_pos;");
 L("        lex_pool[ pos ].set( yytext, yyleng );");
 L("        // TESTED with includes removed FOUND");
 L("        // YYtext and yyleng are available");
 L("        // yylval isnt - it is the union later in the same gen2 file");
 L("        // LATER means LINE 2200");
 L("        // EXCEPT this is LINE 3630");
 L("        // ");
 L("        // ");
// require tab.hh ??  L("        extern YYSTYPE yylval; // forward");
 L("        yylval.lex_buff = (str0) lex_pool[ pos ];");
 L("        return tok;");
 L("}");
 L("");
 L("// define SAVE_TOKEN  yylval.string = new std::string(yytext, yyleng)");
 L("#define TOKEN(t)    (yylval.token = t)");
 L("");

	return true;
}

#include "util_buf.h"


/*!
	The PSG Tree already exists (with any adaptors for FLEX BISON)

	Print out the .lex and .y files
	Gererate them from the Tree_PSG
*/
bool Tree_PSG:: print_tree_as_files( ) {
	buffer2 tmp_str;

	buffer2 out_lex; // entire LEX file text
	buffer2 out_y; // entire YACC file text

	// generate 2 files

	if(!gen_LEX( out_lex ))return FAIL_FAILED();
	if(!gen_YACC( out_y ))return FAIL_FAILED();

	// write out 2 file buffers

	if(!blk_write_to_file(out_lex, lex_name_lex(tmp_str)))
		{ return FAIL_FAILED(); }
	if(!blk_write_to_file( out_y, yacc_name_y(tmp_str) ))
		return FAIL_FAILED();

	// flood the TTY if debugginh

	if(0) { 
	 e_print( "%s", (STR0) out_lex );
	 e_print( "%s", (STR0) out_y   );
	}
	return true;
}

bool Tree_PSG::
gen_LEX( buffer2 & out ) // gen the entire files text
{
	L("");
	L("%{"); // CODE section
	L("");

	// utility functions that carry return " RETVAL STRVAL" from LEX
	gen_LEX_lex_return( out );

	// our code will need the table of tokens that YACC maintains for us
	if(1) {
		put_include_yacc_tab_hh( out );
	}

	L("");
	L("%}"); // CODE section END

	L("");
	L("%option noyywrap"); // forgotten, but it goes here
	L("");

	L("%%");
	L("");

	gen_LEX_RULES_eoln( out ); // must match FLEX / YY line no ++

	gen_LEX_RULES_ident_values( out ); // VALUE is union .

	L("");
	// out.put("# LIST RW\n");
	print_list( out, POOL_RW ); 
	L("");
	// out.put("# LIST PUNCT\n"); // unless FLEX matches longest first ?
	print_list( out, POOL_PUNCT ); // in LEX longest first order
	L("");
	// out.put("# LIST LEX\n");
	print_list( out, POOL_LEX );

	L("");
	L(". printf(\"Unknown token!\\n\"); yyterminate();");
	L("");

	L("%%");
	L("");
	return true;
}

// sort of a macro
// PTN looks like PIN actually PATTERN
static bool gen_PTN_CODE_TOK(
	buffer2 & out, 
	const char * PTN,
	const char * CODE,
	const char * TOK
) {
	out.print( "%s %s return TOKEN(%s);\n", PTN, CODE, TOK );
	return true;
}

bool Tree_PSG:: gen_LEX_RULES_eoln( buffer2 & out )
{
// MAYBE move to psg.yylineno // some new %tree.parameter
 gen_PTN_CODE_TOK( out, "\\r\\n", "yylineno++;",  "LEX_EOLN");
 gen_PTN_CODE_TOK( out, "\\n",    "yylineno++;",  "LEX_EOLN");
 gen_PTN_CODE_TOK( out, "[ \\t\\r\\n]", "", 	  "LEX_WS");
	return true;
}

// sort of a macro // LOCAL to this file
static bool gen_PTN_lex_return_TOK(
	buffer2 & out, 
	const char * PTN,
	const char * CODE,
	const char * TOK
) {
	out.print( "%s %s return lex_return(%s);\n", PTN, CODE, TOK );
	return true;
}
bool Tree_PSG:: gen_LEX_RULES_ident_values( buffer2 & out )
{
 gen_PTN_lex_return_TOK( out, "[a-zA-Z_][a-zA-Z0-9_]*", "", "LEX_IDENTIFIER");
 gen_PTN_lex_return_TOK( out, "-?[0-9]+\\.[0-9]*","      ", "LEX_DOUBLE");
 gen_PTN_lex_return_TOK( out, "-?[0-9]+", "              ", "LEX_INTEGER");
	return true;
}

////////////////////////////

bool Tree_PSG::
gen_YACC( buffer2 & out )
{
 // L("%define api.namespace {::EXPRS}");
 // not in C
 // %define variable 'api.namespace' is not used
 // %define api.namespace {::EXPRS}
 /*
 	on_error
	 if(!gen_()) return FAIL_FAILED;
 	on_error
	 if(!gen_()) goto ret_FAIL_FAILED;
 	on_error
	 if(!gen_()) {

	 	FAIL("on_error ARGS"); // todo FAIL edits ARGS sublex_link 
	 	WARN("on_error ARGS"); // todo WARN edits ARGS sublex_link 
		// ARGS == ARG[v] // Layer = SCRIPT EVAL expr_ARGS
		// SEQ "on_error_CALL" ... ARGV ...
		// SEQ on_error CALL ...
		// SEQ "FAIL" CALL ...
		// SEQ "INFO" CALL ... // tap on/off Bench_dgb
	 } else {
		 // OK
	 }
	 goto_step_next;	// EDGE //
 */

	L("%{");
	L("");
	gen_YACC_top_code( out );
	gen_YACC_str_of_token( out );
	L("%}");
	// declare the parameter to get i// int yyparse( HERE );
	gen_yyparse_parameter( out ); // see Y_PARSE.h
	gen_YACC_union( out );
	gen_YACC_token_list( out );
	gen_YACC_type_list( out );
	gen_YACC_precedence_list( out );
	gen_YACC_start_top( out, "top" );

	L("");
	L("%%");
	L("");

	gen_YACC_rules( out );
	return true;
}

bool Tree_PSG::
gen_YACC_top_code( buffer2 & out )
{


 L("          #include <cstddef>");
 L("          #include <stdio.h>");
 L("  //      #include <string>");
 // NOT WORKING - to create yyparse(void*gps_client_data) //
 // then it would be local and still available to entire FSM
 //L("  #define YYPARSE_PARAM gps_client_data ");
 L("	#include \"Y_PARSE.h\" ");
 L("");
 L("          extern const char * str_of_token( int token );");

 // before tab,h so that namespace exists
 // STUBS for all mk_new_EXPR( args ) used in YACC RULE MATCH CODE
 L("          #include \"../src/EXPRS_STUBS.h\"");
 // L("          typedef ::EXPRS::EXPR EXPR; // this doesnt work");
 // L("          typedef struct EXPR; // this doesnt work");
 // also some editing of exprs.h
 L("          using namespace EXPRS; // ... E0 ..."); // 

 if(1) {
	put_include_yacc_tab_hh( out );
 }

 L("          #include \"str1.h\"");
 L("  #if 0");
 L("          struct YYSTYPE;");
 L("          struct YYLTYPE;");
 L("          int  yylex (YYSTYPE*, YYLTYPE*);");
 L("  #else");
 L("          int yylex(void);");
 L("  #endif");
 L("          extern int yylineno;");
 L("          extern char * yytext;");
 L("          // extern int yyparse (void);");
 L("          void yyerror(const char * msg) {");
 L("                  printf(\"# ERR # line %d # %s\\n\", yylineno, msg );");
 L("          // no   printf(\"# ERR # col %d # %s\\n\", @$.first_column, msg );");
 L("          }");

 if(0) {
 // this is also bad syntax // MUDFIX
  L("// EG CXX ");
  L("namespace NS {");
  L("  struct CLS { int z; }; ");
  L("};");
  L(" union MY_UNION {");
  L("  NS:CLS * PTR; ");
  L("};");
  L("using namespace NS;");
  L(" union MY_UNION_two {");
  L("  CLS * PTR; ");
  L("};");
 }

	return true;
}


bool Tree_PSG::
gen_YACC_str_of_token( buffer2 & out )
{
 L("");
 L(" const char * str_of_token( int tok ) {");
 L("  switch( tok ) {");

	gen_YACC_str_of_token_cases( out, POOL_PUNCT );
	gen_YACC_str_of_token_cases( out, POOL_RW );
	gen_YACC_str_of_token_cases( out, POOL_LEX );

 L("   default:");
 L("     static char as_hex[20];");
 L("     snprintf( as_hex, sizeof(as_hex), \"x%2X\", tok );");
 L("     return as_hex;");
 L("  }");
 L(" }");
 L("");
	return true;
}

bool Tree_PSG::
gen_YACC_str_of_token_cases( buffer2 & out, LEX_TOKEN_GROUP & POOL )
{
        int n = POOL.LIST_Token.N();
        for( int i =0; i<n; i++ ) {
                LEX_TOKEN_DECL * tok = POOL.LIST_Token[ i ];

		bool value_present = true;
		if( !tok->Value ) value_present = false;
		else 
		if( tok->Value == "" ) value_present = false;
		else 
	/* used in PUNCT
		if( tok->Value == " " ) value_present = false;
		else 
		if( tok->Value == "-" ) value_present = false;
		else 
		if( tok->Value == "\n" ) value_present = false;
		else 
		if( tok->Value == "{}" ) value_present = false;
		else 
		if( tok->Value == "''" ) value_present = false;
	*/	;

		out.put("   case ");
		print_TOKEN_name_2( out, POOL, tok );
		if(value_present) {
			out.put(":\t return \"");
			out.put( tok-> Value );
			out.put("\";\n");
		} else {
			out.put(":\t return \"");
			print_TOKEN_name_3( out, POOL, tok );
			out.put("\";\n");
		}
	}
	return true;
}

bool Tree_PSG::
gen_YACC_union( buffer2 & out )
{
//	L("%parse-param {SOMETYPE * parser} ");
	L("");
 L(" %union {");
// NO L(" EXPR * expr;"); // almost
// NO  L(" EXPRS:: struct EXPR * expr;");
// NO L(" struct EXPRS:: EXPR * expr;");
// L(" struct ::EXPRS:: EXPR * expr;");
// L(" ::EXPRS:: EXPR * expr;");
// L(" EXPR * expr;");
// L(" struct EXPR_t * expr;"); // almost

// this list comes from %left <token> PUNCT_STAR // "token_type token"

 L("  struct EXPR * expr;"); // almost // incomplete type?
 L("  u32 e32;"); // retval is int
 L("  int token;");
 L("  const char * lex_buff;"); // via several buffer2 ring of holders
 L(" }");
	return true;
}

bool Tree_PSG::
gen_YACC_token_list( buffer2 & out )
{
	/*
		RULES section longest first for match priority
		%left section tightest precedence last
		rework %token <token> PUNCT_LT_LT_EQUAL
		as %left <token> PUNCT_LT_LT_EQUAL

	*/
	L("");
	gen_YACC_token_list_POOL( out, POOL_PUNCT );
	L("");
	gen_YACC_token_list_POOL( out, POOL_RW );
	L("");
	gen_YACC_token_list_POOL( out, POOL_LEX );
	return true;
}

bool Tree_PSG::
gen_YACC_token_list_POOL( buffer2 & out, LEX_TOKEN_GROUP & POOL )
{
   int n = POOL.LIST_Token.N();
   for( int i =0; i<n; i++ ) {
      LEX_TOKEN_DECL * tok = POOL.LIST_Token[ i ];
      out.put("%token <token> ");
      print_TOKEN_name_2( out, POOL, tok );
      out.put("\n");
   }
	return true;
}

bool Tree_PSG::
gen_YACC_type_list( buffer2 & out )
{
 // L("%type <unionfieldname> rulename");
	 L("");
	L("%type <expr> expr_ident");
	L("%type <expr> expr");
	L("%type <expr> EXPR_line");
	L("%type <expr> lines"); // slight lie to bootstrap
	L("%type <e32> e32_expr"); // need to define u32 E32 instead of EXPR *
//	L("%type <token> BOP");
	return true;
}

bool Tree_PSG::
gen_YACC_precedence_list( buffer2 & out )
{
	// need to match operator to PUNCT sort by operator precedence
	// %left <token> PUNCT_PLUS PUNCT_MINUS
	// loose first, low precedence
	// tight last, high precedence

	// DUMP_TABLE requires TABLE_BUILT 
	// probably OPERATOR_PRECEDENCE (sort)

	// BUILT_TREE_operator_precedence_tree
	// add_c_api_operators( tree )
	// gen_YACC_operator_precedence( out, table )

// https://www.gnu.org/software/bison/manual/html_node/Precedence-Decl.html

// TODO convert '<<=' to PUNCT_LT_LT_EQ 

	 L("");
//	 L("%precedence PUNCT_PLUS");
	 L("%left PUNCT_PLUS PUNCT_MINUS");
	 L("%left PUNCT_STAR PUNCT_SLASH");
	 L("%right PUNCT_CARET");
	return true;
}

bool Tree_PSG::
gen_YACC_start_top( buffer2 & out, const char * rule_name ){
	if(!rule_name) rule_name = "top";
	out.print("%%start %s\n", rule_name );
	 // L("%start top");
	return true;
}

bool Tree_PSG::
gen_YACC_rules( buffer2 & out )
{
 L("// RULES ");
 L("// RULES ");
 buffer2 filed;
 // bug // open_RO not reporting filename
 if(!blk_read_entire_file( filed, "../src/e1.y_RULES", 32 )) return FAIL_FAILED();
 out.put( filed );
	return true;
}
