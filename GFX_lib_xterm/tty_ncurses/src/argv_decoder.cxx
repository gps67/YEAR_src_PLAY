#include "argv_decoder.h"
#include "dgb.h"

	
	argv_decoder:: 
	argv_decoder( int _argc, char * _argv[], char * _envp[] )
	: argc(_argc)
//	, argv(_argv)
//	, envp(_envp)
	{
		argv = _argv; // fail for * []
		envp = _envp; // pass for **
		
		// INFO("CTOR");
	}
	
	STR0 argv_decoder:: get_prog_name( buffer1 & buf ) // helper or entire result or mmore
	{
		// appended to buffer, but only tail returned
		buf.printf("%s", argv[0] );
		return argv[0];
	};

