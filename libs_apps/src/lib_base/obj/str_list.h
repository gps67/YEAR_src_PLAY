#ifndef str_list_H
#define str_list_H

#include "obj_list.h"
#include "str2.h"

/*!
	a list of strings, which can also be converted to an argv

	For no good reason this is not derived from dyn_array
*/
class str_list : public GRP_lib_base {
 protected:
	obj_list<str2>	strs;
 public:
	str_list( int n );
	void re_init( int n );
	void extend_to( int n );
	void append( str2 * val );
//	void set( int pos, str0 val );
	void set( int pos, str2 * val );
	str0 get_str( int pos );
//	const char ** get_argv();
	int N();
};

/*!
	a temp argv builder - NOT str0 (just in case it isnt plain)
*/
class str_list_with_argv : public str_list
{
	dyn_array<const char *> argv;
 public:
	str_list_with_argv( int n );
	const char ** get_argv();

};

#endif
