#ifndef scan_data_h
#define scan_data_h

#include "own_str.h"

class Parse_Expr : public Parse_Expr_Commented
{
 public:
	Parse_Expr * expr;
};

class Parse_Expr_Commented: public ITEM_commented
{
 public:
	own_str dox_comment_block;
	own_str dox_comment_line;
};

class Parse_Expr_ITEM : public OBJECT_t // MODULE_ROOT_OBJECT_SPEC //
{
 public:
 	bool get_Identifier( buffer1 & Identifier ); // if NOT return FAIL EXPLAIN
	// AUTO FAIL EXPLAIN LOG // DATA_SAVE_VARS // RUN_in_SANDPIT // DIRECT //
	// GEN DIRECT as TEMPLATE .cxx .h .doc .edit .sess .csr
	// PARSER is looking at THIS // HERE = LOOKUP Identifier // ANYSTR //
	// ANYSTR uses mechanical_ALLOC "idx_t itx = N_alloced ++" " N == %s_N_alloced "
	// N == "%s_147_N_allocated" // N_allocated == "147" // N //
};

class Parse_Expr_Identifier : public Parse_Expr
{
 public:
	own_str identifier;
};

class Parse_Expr_ : public Parse_Expr
{
 public:
	own_str identifier;
};

class Parse_Expr_10 : public Parse_Expr
{
 public:
	
};

struct E
{
	Parse_Expr * e;

	E()
	{
		e = NULL;
	}

	void operator=( Parse_Expr * e2 )
	{
		e = e2;
	}
};

#endif
