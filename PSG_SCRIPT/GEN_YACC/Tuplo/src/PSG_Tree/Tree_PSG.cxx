
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


bool Tree_PSG::print_list(
	buffer2 & out,
	LEX_TOKEN_GROUP &  POOL
) {
	// maybe need longest first
	// maybe need Q2 punct fixing

	int n = POOL.LIST_Token.N();
	for( int i =0; i<n; i++ ) {
		LEX_TOKEN_DECL * tok = POOL.LIST_Token[ i ];

		out.put( '"' );
		out.put( tok-> Value );
		out.put( '"' );

		out.put( " return TOKEN(" );
		print_TOKEN_name_2( out, POOL, tok );
		out.put( ");\n" );

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
 L("struct EXPR;");
 out.put("#include \"");
 out.put( "gen_e1_yacc.tab.hh" );
 // out.put( yacc_name_yacc );
 out.put("\"");
 L("");
 L("/*");
 L("        lookahead may mean any number of tokens, not 1");
 L("");
 L("        quick test shows actually OK 1 as long as yacc strdup's immediately");
 L("        but maybe");
 L("*/");
 L("");
 L("// static const int nlex16 = 64;");
 L("static const int nlex16 = 16;");
 L("// static const int nlex16 = 1;");
 L("static buffer1 lex_buffer;");
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

	if(!blk_write_to_file( out_lex, lex_name_lex ))return FAIL_FAILED();
	if(!blk_write_to_file( out_y, yacc_name_yacc ))return FAIL_FAILED();

	if(0) { 
	 e_print( "%s", (STR0) out_lex );
	 e_print( "%s", (STR0) out_y   );
	}
	return true;
}

bool Tree_PSG::
gen_LEX( buffer2 & out )
{
	L("%{");
	L("");

	gen_LEX_lex_return( out );

	L("");
	L("%}");
	L("");
	L("%option noyywrap");
	L("");
	L("%%");
	L("");

	// these should be in build_tree -> POOL_LEX 

	// GEN from LEXP list 
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
 L("          #include \"../src/EXPRS.h\"");
 L("          using namespace EXPRS;");
 out.put("#warning fixed file name gen_ e1_\n");
 out.put("#include \"");
 // out.put( yacc_name_yacc );
 // out.put( lex_name_lex );
 out.put( "gen_e1_yacc.tab.hh\" // bison lists PUNCT_PLUS as int");
 out.put("\"");
 L("");
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

		out.put("   case ");
		print_TOKEN_name_2( out, POOL, tok );
		out.put(":\t return \"");
		out.put( tok-> Value );
		out.put("\";\n");
	}
	return true;
}

bool Tree_PSG::
gen_YACC_union( buffer2 & out )
{
 L("%union {");
 L(" EXPR * expr;");
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
 L("// RULES ");
 L("// RULES ");
 L("// RULES ");
 L("// RULES ");
 buffer2 filed;
 // bug // open_RO not reporting filename
 if(!blk_read_entire_file( filed, "../src/e1.y_RULES", 32 )) return FAIL_FAILED();
 out.put( filed );
	return true;
}
