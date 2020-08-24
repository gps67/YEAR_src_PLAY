#ifndef SPOUT_argv_decoder_H
#define SPOUT_argv_decoder_H

#include "buffer1.h"

namespace SPOUT {
struct argv_decoder
{
	int argc;
	char ** argv; // tried cahr * argv[]; // but no
	char ** envp;
	
	argv_decoder( int _argc, char * _argv[], char * _envp[] );

	STR0 get_prog_name( buffer1 & buf ); // helper or entire result or mmore

}; // struct

}; // namespace
#endif
