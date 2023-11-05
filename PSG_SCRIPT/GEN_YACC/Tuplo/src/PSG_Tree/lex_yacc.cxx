
#include "lex_yacc.h"
#include "Y_PARSE.h" // parameter
using namespace PSG;

lex_yacc::
lex_yacc()
{
	POOL_PUNCT.PFX = "PUNCT"; // OK omit trailing _
	POOL_RW   .PFX = "RW";   // OK omit trailing _
	POOL_LEX  .PFX = "LEX"; // OK omit trailing _

	/*
		PUNCT "+="
		RW "if"
		LEX EOLN IDENT q
	*/

	// CSET 
}

lex_yacc::
~ lex_yacc()
{
}

/*
	TODO

	union is list of { type fieldname }

	token is list of { // relates to union }

	top is start

	%x start token used in LEX expr ... <token>...

	{ $include_file filename } -- added to PSG at place

	SEQ name += STEP get number of step $1 
	or at least log all tokens named

	STEP "+=" // add to token list LEX PUNCT

	pre-learn a list of common operator PUNCTS
	but only declare those mentioned in PSG
*/

#include "dir_name_ext.h"

#include "util_buf.h"

#define if_WARN_1 if(0)

bool lex_yacc::
set_PSG_name( STR0 e1, STR0 file_left ) // ("e1","../obj/gen_XXX") //
{
	psg_abbr = e1;
	INFO("psg_abbr = '%s'", (STR0) psg_abbr );
//	psg_name;
	bool near_not_far = true;
	if(0) near_not_far = false; // use FULL PATH NAMES

	if(1) INFO("file_left %s", file_left ); // ../obj/gen_XXX

#if 0
// DELETE THESE OLD COMMENTS or REWRITE THEM
	// e1 exprs E1 // PSG over MMAP 
	// gen_XXX
	// ../obj/gen_XXX
	//
	// PSG_Name = ../obj/gen_XXX
	// LEX_Name = ../obj/gen_XXX_lex	.lex .cc .o _yacc .y .cc .tab.hh .o
	// YACC_Name = ../obj/gen_XXX_yacc .y .cc .tab.hh .o
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
#endif


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


	// INFO("_name_ %s", _name_ ); // "../obj/gen_XXX" // "EXPR_C"

	dir_name_ext path_name;
	path_name.decode_filename( file_left );  // some/dir/NAME

	// some/dir/NAME 
	// BECOMES => some/dir/NAME_yacc.y .cc .o .tab.hh
	// BECOMES => some/dir/NAME_lex.lex .cc .o

	// some/dir/NAME - expected, remove any .ext // or BETTER throw 
	path_name.ext.clear(); // sanity test it was, recycle genre from point

	// after any change rebuild full_path_name
	path_name.mk_BOTH_path_name();

	// the filename of the PSG - before adding extra_suffixes
	// psg_name & psg_name = "gen_XXX" // C EXPR PSG
	psg_name = path_name.near_path_name; // str1 = ... probably STR0
if(0)	e_print("psg_name == '%s'\n", (STR0) psg_name );
if(0)	path_name.test_print(); // path/../obj/gen_XXX


  // return FAIL("STOP HERE");

  	// PUSH path_name.name // single _path_name_decoder
	buffer2 tmp_name_keeper = path_name.name; // gen_XXX

 	path_name.ext.clear();
	path_name.name.put("_lex");
	path_name.mk_BOTH_path_name();
	if(near_not_far) {
			lex_name = path_name.near_path_name;
	} else {	lex_name = path_name.full_path_name;
	}
if(0)	e_print("lex_name == '%s'\n", (STR0) lex_name );
	// gen_XXX_lex.lex
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
if(0)	path_name.test_print();

	return true; // OK 
}

STR0 lex_yacc::
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
	str = yacc_name; // init truncate add STR0 yacc_name filename_
	str.put(".y"); // filename.y
	return str; // cast to STR0 calls terminating_nul 
}

STR0 lex_yacc::
lex_name_lex(
	buffer2 & str // this is now rentrant 
){
	str.clear();
	str.put(lex_name); // assignment copy init 0
	str.put(".lex");
	return str;
}

/*
	eg "gen_XXX_yacc.tab.hh"
	obtain yacc_name_tab.hh 
	via buffer2
*/
STR0 lex_yacc::
yacc_name_tab_hh(
	buffer2 & str
){
	str.clear();
	put_yacc_name_tab_hh( str );
	return str;
}

/*
	eg "gen_XXX_yacc.tab.hh"
	append filename
	into buffer2 -- but dont clear it
*/
bool lex_yacc::
put_yacc_name_tab_hh( buffer2 & out ){
	out.put( yacc_name ); // gen_XXX_yacc 	// .y // .tab.hh
	out.put(".tab.hh");
	return true;
}

/*
	#include "filename" // no comment tho
*/
bool lex_yacc::
put_include_Q2( buffer2 & out, buffer2 & incl_filename )
{
	out.put("#include \"");
	out.put( incl_filename );
	out.put("\"\n" );
	return true; // return out.am_ok // clearable error flag
}

/*
	#include "gen_XXX_yacc.tab.hh" 
*/
bool lex_yacc::
put_include_yacc_tab_hh( buffer2 & out )
{
	out.put("/* yacc generated TOKENs header UNION YYSTYPE */\n");
	buffer2 incl_filename; // clear() done obvs
	put_yacc_name_tab_hh( incl_filename ); // %s_.tab.hh

	return
	put_include_Q2( out, incl_filename );
	// return bool OK // mem didn't fail
}

bool lex_yacc::
print_list(
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL
) {
	// maybe need longest first // dont think so 
	// maybe need Q2 punct fixing

	int n = POOL.LIST_Token.N();
	for( int i =0; i<n; i++ ) {
		LEX_TOKEN_DECL * tok = POOL.LIST_Token[ i ];

		if( tok-> Value ) {

			static const int left_width = 8; /*
				"repeat" is 8 wide
			*/

			// what if Value uses Q2

			int l1 = out.nbytes_used;

			out.put( '"' );
			out.put( tok-> Value ); // "if" ">>>=" 
			out.put( '"' );

			int l2 = out.nbytes_used;
			int n_sp = left_width - (l2 - l1); // left_width n_sp
			while( n_sp-- > 0 )
			 out.put( ' ' );

			// one extra space, just in case // at least 1
			out.put( " return TOKEN(" );
			print_TOKEN_name_2( out, POOL, tok ); // PFX _ Name
			out.put( ");\n" );
		} else {
			// silent // no rules for edges
		}

	}
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
bool lex_yacc::
print_TOKEN_name_3( // "_LEX_name_" / fake printable string
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL,
	LEX_TOKEN_DECL * tok //  tok = POOL.LIST_Token[ i ];
) {
		out.put( '_' );
		print_TOKEN_name_2( out, POOL, tok ); // "LEX_NAME"
		out.put( '_' ); // "_LEX_NAME_"
		return true;

	//	out.put( "/* LEX_ */ " );
		out.put( '_' );
		out.put( POOL.PFX );	// "PUNCT" "RW" or "LEX"
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
bool lex_yacc::
print_TOKEN_name_2( // PFX _ Name // "PUNCT_COMMA"
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL, // PFX = POOL.PFX
	LEX_TOKEN_DECL * tok //  tok = POOL.LIST_Token[ i ]; Name = tok.Name
) {
	//	out.put( "/* LEX_ */ " );
		out.put( POOL.PFX );
		out.put( '_' );
		out.put( tok-> Name );
		return true;
}

bool lex_yacc::
gen_YACC_cat_file( buffer2 & out, STR0 filename ) {
	return gen_OUT_cat_file( out, filename );
}

bool lex_yacc::
gen_LEX_cat_file( buffer2 & out, STR0 filename ) {
	return gen_OUT_cat_file( out, filename );
}

bool lex_yacc::
gen_OUT_cat_file( buffer2 & out, STR0 filename )
{
	// NOTE // expect can place // comments before FILE

	bool ok = true;

	out.print_ln("// INCL // ");
	out.print_ln("// INCL // including file '%s' //", (STR0) filename );
	out.print_ln("// INCL // --------------- START ----------------- //");

	int K_max = 32;
	buffer2 filed;
	if(!blk_read_entire_file( filed, filename, K_max )) {
		out.print("// INCL // ABSENT FILE '%s' //\n", (STR0) filename );
	}
	out.put( filed );

	out.print_ln("// INCL // --------------- END ----------------- //");
	out.print_ln("// INCL // included file '%s' //", (STR0) filename );
	out.print_ln("// INCL //");
	if(ok)
		return PASS("%s", filename );
	else
		return FAIL("%s", filename );
}

/*
	MACRO L1("C_CODE_LINE") // MUST BE L1("const Q2 str") to join "\n"
	// does no interpretation of string
	// only C interpretation of \r etc in string source

	adds "\n" so you dont have to
*/
#define L1(str) out.put( str "\n")

bool lex_yacc::
gen_LEX_code_start( buffer2 & out )
{
 L1("// gen_LEX_code_start() // headers ");
 L1("//somehow before here is stddef stdio - added by flex");
 L1("#include \"buffer1.h\"");
 L1("#include \"Y_PARSE.h\""); //
 L1("using namespace YY;"); //
 if(1) {
	put_include_yacc_tab_hh( out );
 }
 L1("");
 return true;
}

bool lex_yacc::
gen_LEX_lex_return( buffer2 & out )
{
 L1("/*");
 L1("// GEN	// gen_LEX_lex_return() // gen LEX lex_return() ////////////////");
 L1("	usage  lex_return(tok) || FAIL");
 L1("		int lex_return( int tok ); // STR in yylval.lex_buff ");
 L1("");
 L1("");
 L1("	//	RETURN token as lex retval");
 L1("	//	RETURN string in UNION.lex_buff field");
 L1("");
 L1("	//	We must keep the right string, of several, so we keep 16");
 L1("	//	with this simple round robin loop of slots");
 L1("");
 L1("	//	PROBLEM: lookahead may mean any number of tokens, not 1");
 L1("	//	QUICK TEST SHOWS actually OK 1");
 L1("	//	as long as yacc strdup's immediately");
 L1("	//	Not sure but maybe");
 L1("");
 L1("	//	So call return lex_return( TOKEN ); and it obtains lex_buff");
 L1("	//	for LEX.value for LEX.cident99 for LEX.anystr");
 L1("	}");
 L1("");
 L1("");
 L1("*/");
 L1("");
 L1("// nlex16 = N");
 L1("	// circular list of vars for single return value");
 L1("	// We dont know when each will be dropped just have plenty");
 L1("");
 L1("	// static const int nlex16 = 64;");
 L1("	   static const int nlex16 = 16;");
 L1("	// static const int nlex16 = 1;");
 L1("");
 L1("	// array [ N ] of buffer1 // copy of LEX str in lex_pool[idx]");
 L1("	static buffer1 lex_pool[nlex16]; // backing store for LEX STR0 ");
 L1("");
 L1("	// _pos in loop");
 L1("	static int nlex_pos = 0;");
 L1("");
 L1("/*!");
 L1("        int lex found( LEX_ITEM_TOKEN ); // STR0 avail, kept here ");
 L1("	//	RETURN token as lex retval"); // eg LEX_CIDENT LEX_VALUE_FLOAT
 L1("	//	RETURN string in UNION.lex_buff field");
 L1("");
 L1("        lex found the string at yytext, yyleng");
 L1("        keep a copy of the parsed str");
 L1("        keep in the next storage slot (reuse nlex16 items later)");
 L1("        copy string to buffer, as str0");
 L1("        set yylval /union/ .lex_buff as returning string value");
 L1("        return tok as lex type");
 L1("");
 L1(" */");
 L1("int lex_return( int tok )");
 L1("{");
 L1("        // pos == nlex_pos // this kept lex in circular loop of buffer1");
 L1("        int pos = nlex_pos;");
 L1("        // ");
 L1("        // step to next slot in round_robin loop");
 L1("        nlex_pos = (nlex_pos+1) % nlex16;");
 L1("        // ");
 L1("        // save a copy of the the text of the lex"); // drop old value
 L1("        lex_pool[ pos ].set( yytext, yyleng ); // buffer2");
 L1("        // ");
 L1("        // return a STR0 of LEXs text; ");
 L1("        yylval.lex_buff = (str0) lex_pool[ pos ]; // STR0 from buffer2");
 L1("        // ");
 L1("        return tok;");
 L1("}");
 L1("");
 L1("// yylval.fieldname = UNION.fieldname ; // returned extra info for TOKEN");
 L1("// define SAVE_TOKEN  yylval.string = new std::string(yytext, yyleng)");
 L1("// define XXXX_TOKEN  yylval.lex_buff = NEXT_SLOT(yytext, yyleng)");
 L1("#define TOKEN(t)    (yylval.tokn = t)"); // and return RETVAL == t
 L1("");

	return true;
}

// this is GEN of what will be run after compile
// it uses FIELD "lex_var_start_symbol";
// that must be provided by ...
bool lex_yacc::
gen_LEX_start_symbol( buffer2 & out )
{
	POOL_START.print_lines(out);
	return true;

	L1("// ");
	L1("// lex_start_symbol - emit as TOKEN once");
	L1("// ");
	L1("// TODO rewrite - splice code to top of lexer function XXXX");
	L1("// ");
	return true;
	L1("extern int lex_start_symbol; // you set this // first added token only");
	L1("if(lex_start_symbol) {");
	L1("  int tok = lex_start_symbol;");
	L1("  lex_start_symbol = 0;");
	L1("  return lex_return(tok);");
	L1("}");
	L1("");
	return true;
}


/*!
	The PSG Tree already exists (with any adaptors for FLEX BISON)

	Print out the .lex and .y files
	Generate them from the lex_yacc
*/
bool lex_yacc:: print_tree_as_files( ) {
	buffer2 tmp_str;

	buffer2 out_lex; // entire LEX file text
	buffer2 out_y; // entire YACC file text

	out_lex.get_space( 1024 * 8 );
	out_y  .get_space( 1024 * 8 );

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

bool lex_yacc::
gen_LEX( buffer2 & out ) // gen the entire files text
{
	out.get_space( 1024 * 4 );

	L1("/*");
	L1("  LEX file.");
	L1("  +-------------");
	L1("  |  definitions");
	L1("  | %%");
	L1("  |  rules");
	L1("  | %%");
	L1("  |  user code");
	L1("  +-------------");
	L1("");
 # if 0
	L1("   non // comments can be at BOLN except in rules section");
	L1("   / * ... * / at BOLN in rules look like RE rules");
	L1("   // comments are not comments they are copied through");
	L1("   // comments can only be in copied C++ or gcc code");
	L1("   user code goes at end but the definitions at top");
	L1("   rule code needs includes for them to be legal");
	L1("   RTFM");
	L1("   %{ COPIED CODE %} // %top{ COPIED CODE TOP }");
	L1("   // OWN LINE RULES for '%top{' '}' and '%{' '%}' ");
	L1("   // C++ comments copied through, upto gcc");
	L1("");
 # endif
	L1("");
	L1("   this is in definitions SECTION // see 5.0");
	L1("");
	L1("*/");
	L1("");

	// mechanism to insert first token of lex_start_symbol
	gen_LEX_start_symbol( out );

	L1("%{"); // on line of its own unindented
	// this comment is in CODE and gcc is OK with C++ // comments
	L1("// within definitions %{ is A CODE SECTION BEGIN // like %TOP see 5.1 ");
	L1("");

	// include buffer1 Y_PARSE using namespace YY
	gen_LEX_code_start( out );

	// utility functions that carry return " RETVAL STRVAL" from LEX
	gen_LEX_lex_return( out );

	// our code will need the table of tokens that YACC maintains for us
	if(1) {
		put_include_yacc_tab_hh( out );
	}

	L1(" // CODE section END");
	L1("%}"); // on line of its own unindented
	// outside code // comments dont work
	// /* */ does not have to indented, in definitions
	L1("");
	L1("/* still in definitions section */");
	L1("");
	L1("/* yywrap is a macro called at every end of file */");
	L1("%option noyywrap"); // called not provided
	L1("");


	L1(" /* definions section END */"); // 
	L1("%%");
	L1(" /* rules section BEGIN */"); // cannot be at BOLN
	L1("");

// C and C++ comments are always added here
// not sure what happens in AFM JSON etc
// probably untriggered

	L1(" /* added START condition rules for comments and string */");
	L1("");
	L1("\"/*\"	{");
	L1("	BEGIN(CMNT_C);");
	L1("	}");


	// OR START_x_RULE_ACTION( "CMNT_C", "[^*\\n]*", "...");
	// OPTION build REGEXP to buffer1 to STR0
	L1("<CMNT_C>[^*\\n]*        /* eat anything that's not a '*' */");
	L1("<CMNT_C>\"*\"+[^*/\\n]*   /* eat up '*'s not followed by '/'s */");
	L1("<CMNT_C>\\n             yylineno++;");

	buffer1 L;
#define L1b(L) out.put( (STR0) L ); out.put("\n")
static const char Q2 = '"';
	L.clear();
	L.print("<%s>", 	"CMNT_C"); // start tag
	L.print("%c%c%c%c",	Q2, '*', Q2, '+' );
	L.print("%c%c%c",	Q2, '/', Q2 );
	L.print("\t%s",	        "BEGIN(INITIAL);");
	L1b(L);


	L1("");
//	L1(" // OK that drops the CMNT text ");

	L.clear();
//	L.print("%c%s%c", Q2, "\\\"", Q2 );
	L.print("[%c]", Q2 );
	L.print("\t%s",		"BEGIN(STRING_C);");
//	L1b(L);
	L.clear();
	L.print("<%s>", 	"STRING_C"); // start tag
	L.print("[^%s]*",	"\\\"" );
	L.print("\t%s",	        "return TOKEN(LEX_STRING);" );
//	L1b(L);

	L1("");
	L1("");
	L1("");

	// comments may not be where pattern_RE goes - at start of line
	L1(" /* LIST RW reserved word */");
	L1("");
	print_list( out, POOL_RW ); 
	L1("");

	// reserved words must appear before identifier [A-Za-z_]...
	// floats which can start with . must be before PUNCT_DOT
	// 

	L1(" /* gen_LEX_RULES _eoln() AFTER RW_s */");
	L1("");
	gen_LEX_RULES_eoln( out ); // must match FLEX / YY line no ++
	L1("");

	L1(" /* gen_LEX_RULES _ident_values() */");
	L1("");
	gen_LEX_RULES_ident_values( out ); // VALUE is union .
	L1("");

	L1(" /* LIST PUNCT */"); // unless FLEX matches longest first ?
	L1(" /* beware .123 float seeing PUNCT_DOT */"); 
	L1("");
	print_list( out, POOL_PUNCT ); // in LEX longest first order
	L1("");

	L1(" /* LIST LEX */");
	L1("");
	print_list( out, POOL_LEX );
	L1("");

	L1(" /* INCL LEX */");
	if(! gen_LEX_include_RULES( out ) ) {
		// continue without INCL file
		WARN("gen_LEX_include_RULES FAILED");
	}

	L1(". printf(\"Unknown token!\\n\"); yyterminate();");
	L1("");

	L1(" /* rules section END*/"); // cannot be at BOLN
	L1("%%");
	L1("// code section BEGIN // to eof // is copied through");
	L1("// ");
	L1("");
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

bool lex_yacc:: gen_LEX_RULES_eoln( buffer2 & out )
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
bool lex_yacc:: gen_LEX_RULES_ident_values( buffer2 & out )
{
 //
 // TODO it is supposed to register LEX_NAME
 // what is the return type ? <tokn> ?
 //
 // HMM option of indent on following line \n if( n_sp<0 )
 // 
 gen_PTN_lex_return_TOK( out, "[a-zA-Z_][a-zA-Z0-9_]*", "", "LEX_IDENTIFIER");
 gen_PTN_lex_return_TOK( out, "-?[0-9]+\\.[0-9]*","      ", "LEX_DOUBLE");
 gen_PTN_lex_return_TOK( out, "-?[0-9]+", "              ", "LEX_INTEGER");
 gen_PTN_lex_return_TOK( out, "<STRING_C>[^\"]*", "      ", "LEX_STRING");
	return true;
}

////////////////////////////

bool lex_yacc::
gen_YACC( buffer2 & out ) // all of it
{
	out.get_space( 1024 * 4 );

 // L1("%define api.namespace {::EXPRS}");
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

	gen_YACC_declarations( out );
	L1("%{");
	L1("");
	gen_YACC_top_code( out );
	gen_YACC_str_of_token( out );
	L1("%}");
	// declare the parameter to get i// int yyparse( HERE );

	gen_yyparse_parameter( out ); // see Y_PARSE.h
	gen_YACC_union( out );
	gen_YACC_token_list( out );
	gen_YACC_type_list( out );
	gen_YACC_precedence_list( out );
	gen_YACC_start_rule_top( out, "top" );

	L1("");
	L1("%%");
	L1("");

	L1(" /* INCL YACC */");
	// (1) (DONE) load PSG with rules
	// (2) (HERE) GEN text for rules from PSG tree
	gen_YACC_include_RULES( out );

	L1("");
	L1(" /* EOF */");
	L1("");
	return true;
}

bool lex_yacc::
gen_YACC_declarations( buffer2 & out )
{
 if(0) {
  L1("%define parser.error detailed");
  L1("%define api.pure full");
 }
 return PASS("TODO");
}

bool lex_yacc::
gen_YACC_top_code( buffer2 & out )
{


 L1("          #include <cstddef>");
 L1("          #include <stdio.h>");
 L1("  //      #include <string>");
 // NOT WORKING - to create yyparse(void*gps_client_data) //
 // then it would be local and still available to entire FSM
 //L1("  #define YYPARSE_PARAM gps_client_data ");
 L1("	#include \"Y_PARSE.h\" ");
 L1("");
 L1("          extern const char * str_of_token( int token );");

 // before tab,h so that namespace exists
 // STUBS for all mk_new_EXPR( args ) used in YACC RULE MATCH CODE
 L1("          #include \"../src/EXPRS_STUBS.h\"");
 // L1("          typedef ::EXPRS::EXPR EXPR; // this doesnt work");
 // L1("          typedef struct EXPR; // this doesnt work");
 // also some editing of exprs.h
 L1("          using namespace EXPRS; // ... E0 ..."); // 

 if(1) {
	put_include_yacc_tab_hh( out );
 }

 L1("          #include \"str1.h\"");
 L1("  #if 0");
 L1("          struct YYSTYPE;");
 L1("          struct YYLTYPE;");
 L1("          int  yylex (YYSTYPE*, YYLTYPE*);");
 L1("  #else");
 L1("          int yylex(void);");
 L1("  #endif");
 L1("          extern int yylineno;");
 L1("          extern char * yytext;");
 L1("          // extern int yyparse (void);");
 if(0) {
 L1("          void yyerror(const char * msg) {");
 L1("                  printf(\"# ERR # line %d # %s\\n\", yylineno, msg );");
 L1("          // no   printf(\"# ERR # col %d # %s\\n\", @$.first_column, msg );");
 L1("          }");
 }

 if(0) {
 // this is also bad syntax // MUDFIX
  L1("// EG CXX ");
  L1("namespace NS {");
  L1("  struct CLS { int z; }; ");
  L1("};");
  L1(" union MY_UNION {");
  L1("  NS:CLS * PTR; ");
  L1("};");
  L1("using namespace NS;");
  L1(" union MY_UNION_two {");
  L1("  CLS * PTR; ");
  L1("};");
 }

	return true;
}


bool lex_yacc::
gen_YACC_str_of_token( buffer2 & out )
{
 L1("");
 L1(" const char * str_of_token( int tok ) {");
 L1("  switch( tok ) {");

 L1("  // PUNCT_ any literal strings almost");
	gen_YACC_str_of_token_cases( out, POOL_PUNCT );
 L1("");
 L1("  // RW_ reserved words");
	gen_YACC_str_of_token_cases( out, POOL_RW );
 L1("");
 L1("  // LEX_ items have no value so return SYMBOL");
	gen_YACC_str_of_token_cases( out, POOL_LEX );

 L1("");
 L1("   default:");
 L1("     static char as_hex[20];");
 L1("     snprintf( as_hex, sizeof(as_hex), \"x%2X\", tok );");
 L1("     return as_hex;");
 L1("");

 L1("  } // switch");
 L1(" } // fn gen_YACC_str_of_token");
 L1("");
	return true;
}

bool lex_yacc::
gen_YACC_str_of_token_cases( buffer2 & out, LEX_TOKEN_GROUP & POOL )
{
	/*
		gen_YACC_str_of_token_cases( out, POOL_PUNCT );
		gen_YACC_str_of_token_cases( out, POOL_RW );
		gen_YACC_str_of_token_cases( out, POOL_LEX );
	*/
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
		print_TOKEN_name_2( out, POOL, tok ); // PFX _ Name
		if(value_present) {
			out.put(":\t return \"");
			out.put( tok-> Value );
			out.put("\";\n");
		} else {
		/*
			case LEX_EOLN:        return "_LEX_EOLN_"; // 
			These are LEX_IDENTIFIER tokens
			which have no value themselves, 
			value passed in union "LEX_DOUBLE"
			value does not exists "LEX_EOLN"
			create a fake "_LEX_DOUBLE_" printable string
		*/
			out.put(":\t return \"");
			print_TOKEN_name_3( out, POOL, tok ); // _ Name _
		// long tracing print
		//	out.put("\";\n");
			out.put("\"; // print_TOKEN_name_3 //\n");
			// str_of_token
			// UNUSED PUNCT_name
			//   USED LEX_name
			// we dont have values for LEX_ NAME
			// eg case LEX_DOUBLE: return "_DOUBLE_"
		}
	}
	return true;
}

bool lex_yacc::
gen_YACC_union( buffer2 & out )
{
//	L1("%parse-param {SOMETYPE * parser} ");
	L1("");
 L1(" %union {");
// NO L1(" EXPR * expr;"); // almost
// NO  L1(" EXPRS:: struct EXPR * expr;");
// NO L1(" struct EXPRS:: EXPR * expr;");
// L1(" struct ::EXPRS:: EXPR * expr;");
// L1(" ::EXPRS:: EXPR * expr;");
// L1(" EXPR * expr;");
// L1(" struct EXPR_t * expr;"); // almost

// this list comes from %left <tokn> PUNCT_STAR // "token_type token"

 L1("  struct EXPR * expr;"); // almost // incomplete type?
 L1("  u32 e32;"); // retval is int
 L1("  int tokn;");
 L1("  const char * lex_buff;"); // via several buffer2 ring of holders
 L1(" }");
	return true;
}

bool lex_yacc::
gen_YACC_token_list( buffer2 & out )
{
	/*
		RULES section longest first for match priority
		%left section tightest precedence last
		rework %token <tokn> PUNCT_LT_LT_EQUAL
		as %left <tokn> PUNCT_LT_LT_EQUAL

	*/
	L1("");
	gen_YACC_token_list_POOL( out, POOL_PUNCT );
	L1("");
	gen_YACC_token_list_POOL( out, POOL_RW );
	L1("");
	gen_YACC_token_list_POOL( out, POOL_LEX );
	return true;
}

bool lex_yacc::
gen_YACC_token_list_POOL( buffer2 & out, LEX_TOKEN_GROUP & POOL )
{
   int n = POOL.LIST_Token.N();
   for( int i =0; i<n; i++ ) {
      LEX_TOKEN_DECL * tok = POOL.LIST_Token[ i ];
      out.put("%token <tokn> ");
      print_TOKEN_name_2( out, POOL, tok ); // PFX _ Name
      out.put("\n");
   }
	return true;
}

bool lex_yacc::
gen_YACC_type_list( buffer2 & out )
{
 // L1("%type <unionfieldname> rulename");
	 L1("");
	L1("%type <expr> expr_ident");
	L1("%type <expr> expr");
	L1("%type <expr> EXPR_line");
	L1("%type <expr> lines"); // slight lie to bootstrap
//	L1("%type <e32> e32_expr"); // need to define u32 E32 instead of EXPR *
//	L1("%type <tokn> BOP");
	return true;
}

bool lex_yacc::
gen_YACC_precedence_list( buffer2 & out )
{
	// need to match operator to PUNCT sort by operator precedence
	// %left <tokn> PUNCT_PLUS PUNCT_MINUS
	// loose first, low precedence
	// tight last, high precedence

	// DUMP_TABLE requires TABLE_BUILT 
	// probably OPERATOR_PRECEDENCE (sort)

	// BUILT_TREE_operator_precedence_tree
	// add_c_api_operators( tree )
	// gen_YACC_operator_precedence( out, table )

// https://www.gnu.org/software/bison/manual/html_node/Precedence-Decl.html

// TODO convert '<<=' to PUNCT_LT_LT_EQ 

	 L1("");
//	 L1("%precedence PUNCT_PLUS");
	 L1("%left PUNCT_PLUS PUNCT_MINUS");
	 L1("%left PUNCT_STAR PUNCT_SLASH");
	 L1("%right PUNCT_CARET");
	return true;
}

bool lex_yacc::
gen_YACC_start_rule_top( buffer2 & out, const char * top_rule_name ){
	if(!top_rule_name) top_rule_name = "top";
	out.print("%%start %s\n", top_rule_name );
	 // L1("%start top");
	return true;
}

bool lex_yacc::
gen_LEX_include_RULES( buffer2 & out )
{
 	// CHEAT: include a manually edited file
	// ../src/e1.y_RULES

	STR0 lhs = "../src/";
	STR0 psg = psg_abbr;
	STR0 rhs = ".lex_RULES";

	buffer1 filename;
	filename.print("%s%s%s", lhs, psg, rhs );

	if(!gen_LEX_cat_file( out, filename )) {
		return FAIL("%s", (STR0) filename );
	}

	return true;
}

bool lex_yacc::
gen_YACC_include_RULES( buffer2 & out )
{
 	// CHEAT: include a manually edited file
	// ../src/e1.y_RULES // filename
	STR0 lhs = "../src/";
	STR0 rhs = ".y_RULES";
	buffer1 filename;
	filename.print("%s%s%s", lhs, (STR0) psg_abbr, rhs );

	if(!gen_YACC_cat_file( out, filename )) {
		return FAIL("%s", (STR0) filename );
	}

	return true;
}

