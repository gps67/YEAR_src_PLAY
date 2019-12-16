#include "tty_curses.h"
#include <errno.h>
#include "argv_decoder.h"

#include "STO_i16_hilo.h"

#include <dgb.h>

// PROBS with ncurses not using fd_sel_says
// bunch of uncalled INIT APP - TODO
// #include "APPS_stubs.h"
// #include "APPS_Holder.h"

extern
bool main_test_3(argv_decoder & ARGS); // ARGS

/*
	WARNING
		int main( int argc, char * argv[], char * envp[] )
		argv_decoder( int _argc, char * _argv[], char * _envp[] )
		char ** argv; // tried cahr * argv[]; // but no
	
	# INFO  # argv_decoder::argv_decoder(int, char**, char**) # CTOR

	
	SO
		simply passing a LOCN type is not OK
		G++ managed to fit it to **, 
		(as you see in the error message, prototype printed as it saw)
		But our GEN has to know to generate a different thing
		Or at least note it must only be used with **
*/

int main( int argc, char * argv[], char * envp[] )
{       
	
	argv_decoder ARGS( argc, argv, envp );
	if(main_test_3(ARGS))
		return 0;
	if(errno)
		return errno;
	return 22;
}

