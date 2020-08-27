
// #include <stdlib.h>
#include <unistd.h> // write
// #include <string.h>

#include <stdarg.h> // ...

/*
	EV is event
	api_ev is an event as a class with a virtual function

	Each api_ev is very lightweight DATA
	CTXT_EVENT_DATA  adds a load of 

	TODO: remove all SPOUT GEN_PS stuff 

	api_event is an EXIST thing
	as it passes a TREE, it instanciateso
	wave_of_event signal propagation tree queue local sto expands by page


*/

#include "dgb.h"
#include "buffer2.h"

#include "util_buf.h" // blk_write_to_file( out, filename )
#include "dir_name_ext.h" // dir_name_ext( filename ) .ext = ; mk_near_path_name

#include "EV_STUBS.h"
#include "EV_argv_decoder.h"

#define IF_NOT if(0) 

using namespace EV;

struct ink_rgb
;

/*!
*/
class test_EV // : public obj_ref
{
 public:
	buffer2 out;

	test_EV()
	{
		out.get_space( 1024 * 32 );
	}

	bool test1()
	{
		PASS("BUILT OK");
		return true;
	} // test1
};

bool EVER_test1( argv_decoder & ARGS )
{
 	test_EV EVER;
	if(!EVER.test1()) 
		return FAIL_FAILED();;
	return PASS("OK");
}

int main_FAILED() {
	FAIL_FAILED();
	if(errno) return errno;
	return 1;
}

int main( int argc, char ** argv, char ** envp ) {
	argv_decoder ARGS( argc, argv, envp );
	if(! EVER_test1( ARGS ) ) return main_FAILED();
	return 0;
}

