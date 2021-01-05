#ifndef rem_xawtv_cmd_H
#define rem_xawtv_cmd_H

#include "ints.h"

/*!
	rem_xawtv_cmd formats a command line but doesnt send it to xawtv
*/
struct rem_xawtv_cmd : public GRP_lib_media
{
	char * words;
	int len_used;
	int len_alloc;

	~rem_xawtv_cmd();
	rem_xawtv_cmd( const char * line );
	rem_xawtv_cmd( int argc, char ** argv );
	void get_alloc( int len );

	void print() const;
};

#endif
