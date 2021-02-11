#ifndef scan_data_h
#define scan_data_h

#include "own_str.h"

class Parse_Expr
{
 public:
};

class Parse_Expr_Commented
{
 public:
	own_str dox_comment_block;
	own_str dox_comment_line;
	Parse_Expr * expr;
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
