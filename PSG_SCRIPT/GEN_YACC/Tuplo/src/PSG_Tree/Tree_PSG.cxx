
#include "Tree_PSG.h"

Tree_PSG::
Tree_PSG()
{
	POOL_PUNCT.PFX = "PUNCT"; // OK omit trailing _
	POOL_RW   .PFX = "RW";   // OK omit trailing _
	POOL_LEX  .PFX = "LEX"; // OK omit trailing _
}

Tree_PSG::
~ Tree_PSG()
{
}

#include "dir_name_ext.h"

#define if_WARN_1 if(0)

bool Tree_PSG::
set_PSG_name( STR0 _name_ )
{
	INFO("%s", _name_ );
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

	// name == "gen_e1" // the generated files of the EXPR_ONE parser
	dir_name_ext path_name;
	path_name.decode_filename( _name_ );  // some/dir/NAME
	// some/dir/NAME 
	// BECOMES => some/dir/NAME_yacc.y .cc .o .tab.hh
	// BECOMES => some/dir/NAME_lex.lex .cc .o

	path_name.ext.clear(); // sanity test it was, recycle genre from point
	path_name.mk_full_path_name();
	psg_name_base = path_name.full_path_name; // NORMALISED left-prefix
	// path/gen_e1
if(0)	path_name.test_print();
	buffer2 tmp_name_keeper = path_name.name; // gen_e1

	path_name.name.put("_lex");
 	path_name.ext.clear();
	path_name.mk_full_path_name();
	lex_name_base = path_name.full_path_name;
	// gen_e1_lex.lex
if(0)	path_name.test_print();

	path_name.name.set( (STR0) tmp_name_keeper );

	path_name.name.put("_yacc");
 	path_name.ext.clear();
 if(1) {
	path_name.mk_near_path_name();
	yacc_name_base = path_name.near_path_name;
 } else {
	path_name.mk_full_path_name();
	yacc_name_base = path_name.full_path_name;
 }
	// gen_e1_yacc.y
if(0)	path_name.test_print();

	return true; // OK 
}

STR0 Tree_PSG::
yacc_name_y(){
	static buffer2 str; // this is not rentrant 
	str = yacc_name_base;
	str.put(".y");
	return str;
}

STR0 Tree_PSG::
lex_name_lex(){
	static buffer2 str; // this is not rentrant 
	str = lex_name_base;
	str.put(".lex");
	return str;
}

/*
	mildly inefficient, but very convenient
	maybe recognise usage, code in a service
*/
STR0 Tree_PSG::
yacc_name_tab_hh(){
	static buffer2 str; // this is not multi-user
	str.clear();
	put_yacc_name_tab_hh( str );
	return str;
}

bool Tree_PSG::
put_yacc_name_tab_hh( buffer2 & out ){
	// needs auto add if(!put(...)) return FAIL_FAILED()
	out.put( yacc_name_base );
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


#define L(str) out.put( str "\n")
// its a function like macro so invoker provides semicolon

bool Tree_PSG::
gen_LEX_lex_return( buffer2 & out )
{
 L("// GEN");
 L("#include <string>");
 L("#include \"buffer1.h\"");
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
 L("// static const int nlex16 = 64;");
 L("   static const int nlex16 = 16;");
 L("// static const int nlex16 = 1;");
 // L("static buffer1 lex_buffer;");
 L("static buffer1 lex_pool[nlex16];");
 L("static int nlex_pos = 0;");
 L("");
 L("/*");
 L("        lex found, return tok");
 L("");
 L("        lex has found the string at yytext, yyleng");
 L("        use next storage slot (reuse nlex16 items later)");
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


bool Tree_PSG:: print_tree_as_files( ) {
	buffer2 out_lex;
	buffer2 out_y;

	// return true; PARAMETER out += added_text

	if(!gen_LEX( out_lex ))return FAIL_FAILED();
	if(!gen_YACC( out_y ))return FAIL_FAILED();

	if(!blk_write_to_file( out_lex, lex_name_lex() ))return FAIL_FAILED();
	if(!blk_write_to_file( out_y, yacc_name_y() ))return FAIL_FAILED();

	if(0) { 
	 e_print( "%s", (STR0) out_lex );
	 e_print( "%s", (STR0) out_y   );
	}
	return true;
}

bool Tree_PSG::
gen_LEX( buffer2 & out )
{
	L("");
	L("%{"); // CODE section
	L("");

	gen_LEX_lex_return( out );

 if(1) {
	put_include_yacc_tab_hh( out );
 }

	L("");
	L("%}");

	L("");
	L("%option noyywrap");
	L("");

	L("%%");
	L("");

	// these should be in build_tree -> POOL_LEX 

	// GEN from LEXP list 
	// need to declare "LEX_EOLN" as TOKEN with NULL str rule
	L("\\r\\n	yylineno++; return TOKEN(LEX_EOLN);");
	L("\\n	yylineno++; return TOKEN(LEX_EOLN);");
	L("[ \\t\\r\\n]	return TOKEN(LEX_WS);");

	L("[a-zA-Z_][a-zA-Z0-9_]*	return lex_return( LEX_IDENTIFIER );");
	L("-?[0-9]+\\.[0-9]*	return lex_return( LEX_DOUBLE );");
	L("-?[0-9]+	return lex_return( LEX_INTEGER );");

	L("");
	// out.put("# LIST RW\n");
	print_list( out, POOL_RW );
	L("");
	// out.put("# LIST PUNCT\n");
	print_list( out, POOL_PUNCT );
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
	gen_YACC_union( out );
	gen_YACC_token_list( out );
	gen_YACC_type_list( out );
 L("%right PUNCT_CARET // dumb");
 L("%start top");
 L("%%");
	gen_YACC_rules( out );
	return true;
}

bool Tree_PSG::
gen_YACC_top_code( buffer2 & out )
{


 L("          #include <cstddef>");
 L("          #include <stdio.h>");
 L("  //      #include <string>");
 L("          extern const char * str_of_token( int token );");

 // before tab,h so that namespace exists
 L("          #include \"../src/EXPRS.h\"");
 // L("          typedef ::EXPRS::EXPR EXPR; // this doesnt work");
 // L("          typedef struct EXPR; // this doesnt work");
 // also some editing of exprs.h
 L("          using namespace EXPRS; // ... E1 ..."); // 

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
 L("          // extern \"C\" int yyparse ();");
 L("          extern int yyparse (void);");
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
 L("%union {");
// NO L(" EXPR * expr;"); // almost
// NO  L(" EXPRS:: struct EXPR * expr;");
// NO L(" struct EXPRS:: EXPR * expr;");
// L(" struct ::EXPRS:: EXPR * expr;");
// L(" ::EXPRS:: EXPR * expr;");
// L(" EXPR * expr;");
// L(" struct EXPR_t * expr;"); // almost
 L(" struct EXPR * expr;"); // almost // incomplete type?
 L(" int token;");
 L(" const char * lex_buff;");
 L("}");
	return true;
}

bool Tree_PSG::
gen_YACC_token_list( buffer2 & out )
{
	gen_YACC_token_list_POOL( out, POOL_PUNCT );
	gen_YACC_token_list_POOL( out, POOL_RW );
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
 // <expr> is the fieldname
 // expr_ident is the rulename
 L("%type <expr> expr_ident");
 L("%type <expr> expr");
 L("%type <token> BOP");
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
