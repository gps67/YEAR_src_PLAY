#include <string.h>
#include <malloc.h>
#include "rem_xawtv_cmd.h"

#include "w_widget0.h"

/*!
	destructor - release mem
*/
rem_xawtv_cmd::~rem_xawtv_cmd()
{
	if(words) free(words);
}

/*!
	constructor - convert string to NUL terminated word format
*/
rem_xawtv_cmd::rem_xawtv_cmd( const char * line )
{
	/*
		xawtv + X uses len_used, with each word NUL terminated
	*/
	len_alloc = 0;
	len_used = 0;
	words = NULL;
	get_alloc( strlen( line ) + 1 );
const	char * i_pos = line;
	char * o_pos = words;

	*o_pos = 0; // just in case

	// skip leading spaces, tabs,
	while( (*i_pos) && (*i_pos <= ' ') )
		i_pos++;

	// loop over words from i_pos
	while(*i_pos)
	{
		// copy one word
		while( *i_pos > ' ' ) *o_pos++ = *i_pos++;
		// add a null
		*o_pos++ = 0;
		// skip the spaces
		while( (*i_pos) && (*i_pos <= ' ' )) i_pos++;
	}
}

/*!
	constructor - converts format of a single command line
*/
rem_xawtv_cmd::rem_xawtv_cmd( int argc, char ** argv )
{
	len_alloc = 0;
	len_used = 0;
	words = NULL;
	// int len = 0;

	// caluclate byte_len of words mem block (each followed by a nul byte)
	int byte_len = 0;
	for ( int i = 0; i < argc; i++ ) {
		byte_len += strlen(argv[i]) + 1;
	}

	get_alloc( byte_len );
	len_used = byte_len;
	words[0] = 0;

	char * pos = words;
	for ( int i = 0; i < argc; i++ )
	{
		strcpy( pos, argv[i]);
		pos += strlen(argv[i]) + 1;
	}
}

/*!
	allocate block for words
*/
void rem_xawtv_cmd::get_alloc( int len )
{
	len_used = len;
	if( len <= len_alloc ) return;
	if( words )
		words = (char *) realloc( words, len );
	else
		words = (char *) malloc( len );
	len_alloc = len;
}

/*!
	debugging print
*/
void rem_xawtv_cmd::print() const
{
	char buff [len_used + 1 ];
	for(int i=0; i< len_used; i++ )
	{
		char c = words[i];
		if(!c) c = ' ';
		buff[i] = c;
	}
	buff[len_used] = 0;
	e_print("XAWTV_CMD: %s\n", buff );
}
