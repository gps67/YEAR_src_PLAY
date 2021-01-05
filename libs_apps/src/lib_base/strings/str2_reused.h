
#ifndef str2_reused_H
#define str2_reused_H
#include "str2.h"

/*!
	str2_reused - avoid malloc/delete

	Intended for iterating over get_line_from_file, where
	any saved values will have their own copies.

	str2 copies all assigned strings
	which is a waste for ht_lookup
	This class 'breaks' that by splicing the shared pointer in
	ONLY to be used for program strings (which never need free)
	LURK: also add lots of copy constructors (never used)
*/
class str2_reused : public str2
{
 public:
	str2_reused()
	: str2()	// str = NULL
	{
	}

	~str2_reused()
	{
		set_fixed( (uchar*)NULL ); // avoid FREE in str1
	}

	void set_NULL()
	{
		clear_hash();
		str = NULL;
	}

	void set_fixed( const char * s )
	{
		clear_hash();
		str = (uchar *) s;
	}

	void set_fixed( const uchar * s )
	{
		clear_hash();
		str = (uchar *) s;
	}

	void set_fixed( str0 s )
	{
		clear_hash();
		str = s;
	}

};

#endif
