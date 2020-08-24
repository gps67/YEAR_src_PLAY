
// #include <stdlib.h>
#include <unistd.h> // write
// #include <string.h>

#include "dgb.h"
#include "buffer2.h"

#include "SPOUT_STUBS.h"
#include "SPOUT_argv_decoder.h"

using namespace SPOUT;

/*!
	test_SPOUT is not on the heap
*/
class test_SPOUT // : public obj_ref
{
 public:
	buffer2 out;

	test_SPOUT()
	{
		out.get_space( 1024 * 32 );
	}
	bool test1()
	{
		if(! out.print("THIS IS TEXT\n") ) return FAIL_FAILED();
		write(1, out.buff, out.nbytes_used);
		return true;
	}
};

int main( int argc, char ** argv, char ** envp ) {
	argv_decoder ARGS( argc, argv, envp );
 	test_SPOUT SPOUTER;
	SPOUTER.test1();
	return 0;
}

