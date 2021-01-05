#ifndef str_builder_H
#define str_builder_H

#include "blk1.h"
#include "str2.h"
#include "ints.h"

/*!
	Everyone writes their own string classes.

	Mine does not have 'append', thats a string_builder function.
	A string is a storage device, not a manipulation thing.

	This str_buffer adds dynamic allocation

	TODO: a new blk1 class that has a STACK allocated buffer of (80)
	which does not free that buffer, but can expand. That makes
	it a lot easier to use str_builder in every function, without
	(often) invoking malloc/free, but watch stack overflow on DOS
*/
class str_builder : public GRP_lib_base
{
	blk1 buff;
 public:
	~str_builder();
	str_builder( int sz = 50 );
	void append( str2 * s );
	void append( str0 s );
	void append( char c );
	void add_nul();
	str2 * get_str2();
	uchar * get();
	char * sget() { return (char *) get(); }
	void clear();
	int str_len() { return buff.nbytes_used; }
	bool is_empty() { return !buff.nbytes_used; }
	bool isnt_empty() { return buff.nbytes_used; }
};

#endif
