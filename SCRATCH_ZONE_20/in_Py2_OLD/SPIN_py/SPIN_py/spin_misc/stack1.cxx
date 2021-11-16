#include "SP_App.h" // we control it -- to some extent
#include "stack1.h" //
#include "app_link.h" // string_match

#define DST if(0) // deep stack test debugging printfs

void stack1_item::init( void )
{
	elem_info = 0;
	atts = 0;
	wrap = 0;
	zone = 0;
	on_pop1 = 0;
	on_pop2 = 0;
}

AppString stack1_item::name( void ) {
	if(!elem_info) {
		// LURK set error
		return NULL;
	}
	return elem_info->name;
}

int stack1_item::is_pre( void ) {
	if(!elem_info) {
		return 0;
	}
	return elem_info->IS_pre();
}

void stack1::init( void ) {
	for( int i = 0; i <= sp_max; i++ ) { // lurk allowing sp_max + 1
		item[i].init();
	}
	sp = 0; // must be set to a usable value before set_wrap
	sp_count = 0; // zero element is wasted but needed
	// initialise base stack item
	app->set_wrap( 1 ); // LURK DEFAULT
	// done by set_wrap // item[0].wrap = 1;
	// warning item[0] is not fully valid
	// item[0].elem_info is NOTHING
}

void stack1::error( const char * s )
{
	app->app.raise_error( s );
}

void stack1::add_item( PyObject * zitem ) 
{
	if( sp_count >= sp_max ) {
		printf("add_item: FAIL: sp_count = %d\n", sp_count );
		error("stack1::add_item");
		return; // invalid top of stack - too bad!
	}
	if(!item[sp].zone)
	{
		printf("THIS CANNOT HAPPEN" );
		printf("add_item: NULL zone" );
		error("stack1::add_item");
		return; // invalid top of stack - too bad!
	}
	PyList_Append( item[sp].zone, zitem );
}

void stack1::push( SPIN_elem_info * elem, AppAttrList atts ) {
	int wrap = item[sp].wrap; // sp always valid 
	// nb setting is_pre on the elem effects wrap ...
	// printf("item[%d].wrap == %d)",sp,wrap);
	sp_count++; 
	// sp_count can go out of range
	if( sp_count >= sp_max ) {
		printf("PUSH: FAIL: sp_count = %d\n", sp_count );
		error("stack1::push");
		return; // invalid top of stack - too bad!
	}
	sp = sp_count;
	item[sp].elem_info = elem;
	item[sp].atts = atts;
	item[sp].zone = PyList_New(0); // does INCREF
	if(!item[sp].zone)
	{
		error("PyList_New()"); // malloc error
		return;
	}
	Py_INCREF( elem );
	Py_INCREF( atts );
	item[sp].wrap = wrap;
	if( elem->IS_pre() ) {
		if(wrap) {
			// printf("Switching wrap off\n");
			app->set_wrap( 0 );
			// done by app // item[sp].wrap = X;
		} 
		else
			;// printf("Switching wrap  - ITS ALREADY off\n");
	}
DST	printf("PUSH: %s sp_count(%d)\n", 
		PyString_AsString( elem->name ),sp_count);
}
AppElem stack1::pop( void ) {
	int sp_old = sp_count; // both may be out of bounds
	sp_count--;
	// cannot deal with previous frame being out of bound
	// though sp should still be stuck at top ...
	if( sp_old >= sp_max ) { // decr done but sp_old shall be used ...
		printf("POP: sp_old(%d) >= sp_max(%d)\n", sp_old, sp_max);
		// Error
		return NULL;
	}
	// more close tags than start tags -- restore some sanity
	if( sp_count < 0 ) {
		printf("POP: sp_count(%d) < 0\n", sp_count);
		sp_count++; // lurk
		error("stack1::pop");
		return NULL;
	}
	// print the current depth, and active name ?? delete
DST	printf("POP: %s sp_count(%d)\n", 
		PyString_AsString( item[sp].elem_info->name ),sp_count);
	// point to the new stack top
	sp = sp_count;
	if( item[sp_old].wrap != item[sp].wrap ) {
		app->set_wrap( item[sp].wrap );
		// which also sets here !
	}

	// return a SPIN_elem_obj (instead of a tuple)
	AppElem ret = 0;
	ret = (AppElem) new_SPIN_elem_obj(
		(PyObject *)item[sp_old].elem_info,
		item[sp_old].atts,
		item[sp_old].zone );
	Py_DECREF( (PyObject *)item[sp_old].elem_info ); 
	Py_DECREF( item[sp_old].atts );
	Py_DECREF( item[sp_old].zone );

	item[sp_old].elem_info = NULL;
	item[sp_old].atts = NULL;
	item[sp_old].zone = NULL;
	item[sp_old].init(); //
	return ret;
}

SPIN_elem_info * stack1::match_top_name( const SGMLApplication::CharString & name )
{
	if(sp_count >= sp_max) {
		// without this the top might match by chance 
		printf("match_top_name - sp_count(%d) > sp_max\n",sp_count);
		error("stack1::match_top_name");
		return NULL;
	}
DST	printf(" sp = %d \n", sp );
	SPIN_elem_info * elem = item[sp].elem_info ;
	if(! elem ) {
		error("match_top_name - NULL elem_info\n");
		return NULL;
	}
	AppString s = elem->name;
	if(!s) {
		error("match_top_name - NULL name\n");
		return NULL;
	}
	if( match_string( s, name ) )
	{
		Py_INCREF( elem );
		return elem;
	} else {
		error("match_top_name - didn't\n");
		return NULL;
	}

}

stack1::stack1( SP_App * app_) {
	app = app_;
	init();
	item[0].zone = PyList_New(0); // because root tree added to nothing
}
stack1::~stack1( void ) {
	while( sp > 0 ) { // stop on last
		printf("stack::~stack -- warning sp == %d\n", sp );
		pop();
	}
	Py_XDECREF( item[0].zone );
	app = NULL;
}
