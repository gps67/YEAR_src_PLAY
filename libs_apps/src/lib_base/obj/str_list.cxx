#include "str_list.h"

void str_list::extend_to( int n )
{
	strs.get_space( n - strs.N() );
	for( int i = strs.N(); i<n; i++ ) {
		append( NULL );
	}
	// THIS is only any good for set(pos,val)
}

void str_list::re_init( int n )
{
	extend_to( n );
}

str_list::str_list( int n )
{
	re_init( n );
}

void str_list::append( str2 * val )
{
	static obj_hold<str2> str_list_null = new_str2_static("<NULL>/*str_list*/");
	if( !val ) {
		val = str_list_null;
	}
/*
	str0 s = NULL;
	if( val ) {
		s = val->get();
	}
	// e_print("str_list::append(%s)\n", val->get() );
*/
	strs.append( val );
}
void str_list::set( int pos, str2 * val )
{
	if( !val ) {
		val = new str2("<NULL>");
	}
/*
	str0 s = NULL;
	if( val ) {
		s = val->get();
	}
*/
	strs.set( pos, val );
}

str0 str_list::get_str( int pos )
{
	str2 * s = strs[pos];
	if(!s) throw "str_list::get_str() - NULL";
	return *s;
}

int str_list::N()
{
	return strs.N();
}

//////////////////////////////

str_list_with_argv::str_list_with_argv( int _n )
: str_list( _n )
, argv()
{
	argv.extend_to( _n );
}

const char ** str_list_with_argv::get_argv()
{
	int n = N();
	argv.extend_to( n );
	for( int i = 0; i<n; i++ ) {
		argv[i] = (STR0) get_str( i );
	}
	return argv.ary_base(); // LURK lose const ?
}
