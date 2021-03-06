#include "EV_argv_decoder.h"
#include "dgb.h"
#include <stdlib.h> // getenv

using namespace EV;

	// ARGV.CTOR // EVAL_EXEC_WAIT_API
	// default is to PROVIDE_NEW_MEM ARGV += argv_decoder_t

	argv_decoder:: 
	argv_decoder( int _argc, char * _argv[], char * _envp[] )
	: argc(_argc)
//	, argv(_argv)
//	, envp(_envp)
	{
		argv = _argv; // fail for * []
		envp = _envp; // pass for **
		
		INFO("CTOR");
	}
	
	STR0 argv_decoder:: get_prog_name( buffer1 & buf ) // helper or entire result or mmore
	{
		// appended to buffer, but only tail returned
		buf.printf("%s", argv[0] );
		return argv[0];
	};

	
	bool argv_decoder:: set_env( STR0 varname, STR0 value ) // 
	{
		static const int overwrite = 1;
		if( 0 == setenv( varname, value, overwrite )){
			return true;
		};
		return FAIL("%s %s", varname, value );
	}

	STR0 argv_decoder:: get_env( STR0 varname ) // 
	{
		STR0 VAL = getenv( varname ); // GLOBAL ignoring envp
		if(!VAL) {
			FAIL("varname %s", varname );
			return VAL;
		}
	if(1)	INFO("varname %s '%s'", varname, VAL );
		return VAL;

		// or do own lookup sweep
		// add #if #else #endif

		int len = strlen( varname );
		// environ == envp
		int pos = 0;
		while(1) {
		 int idx = pos ++;
		 STR0 PAIR = envp[ idx ];
		 if(!PAIR) {
		 	FAIL("%s not found", varname );
			return NULL;
		 }
		 if( strncmp( varname, PAIR, len ) == 0 ) {
		   if( PAIR[len] == '=' ) {
		    STR0 rhs = PAIR + len + 1;
	// dgb // show get_env
	if(1)	    INFO("FOUND %s '%s'", varname, rhs );
		    return rhs;
		  }
		 }
	// every ARGV is "VARNAME=value"
	// could built HT of key -> value
	// or lots of things // set_var

	if(0)	 INFO("ENV[%d] '%s'", idx, envp[ idx ]);
		}
		// appended to buffer, but only tail returned
		return "NULLL";
	};

