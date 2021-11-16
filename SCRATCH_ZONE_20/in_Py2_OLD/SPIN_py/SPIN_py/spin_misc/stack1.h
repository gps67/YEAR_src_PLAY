#ifndef STACK1_H
#define STACK1_H

/*
	a quick stack of open elements
	not necessarily a PyObject
	but would use them
*/

class SP_App; // because of order of processing in SP_App.h
#include "SP_App.h" // we control it -- to some extent

struct stack1_item
{
	SPIN_elem_info * elem_info; // this needs direct access - not PyObject ?
	PyObject * atts; // current atts - or None Or NULL
	PyObject * zone; // list of contents - or NULL
	int wrap;
	PyObject * on_pop1;
	PyObject * on_pop2;

	void init( void );
	PyObject * name( void );
	int is_pre( void );
};

#define sp_max 40

struct stack1
{
	// static const unsigned sp_max = 40;
	int sp;
	int sp_count;
	stack1_item item[sp_max+1];

	SP_App * app;

	void init( void );
	void push( SPIN_elem_info * elem, AppAttrList atts );
	void add_item( AppAttrList atts );
	AppElem pop( void );
	stack1( SP_App * app_);
	~stack1( void );
	// fast lookup
	SPIN_elem_info * match_top_name( const SGMLApplication::CharString & S);
	// crude error
	void error( const char * s);
};



#endif
