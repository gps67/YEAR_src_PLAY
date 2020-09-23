#ifndef EV_argv_decoder_H
#define EV_argv_decoder_H

#include "buffer1.h"

namespace EV {
struct argv_decoder
{
	int argc;
	char ** argv; // tried cahr * argv[]; // but no
	char ** envp;
	
	argv_decoder( int _argc, char * _argv[], char * _envp[] );

	STR0 get_prog_name( buffer1 & buf ); // helper or entire result or mmore
	STR0 get_env( STR0 varname );

}; // struct

}; // namespace
#endif
