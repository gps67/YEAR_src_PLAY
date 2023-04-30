#include "dgb.h"
#include "audacity_mod_script_pipe.h"

bool bool_main() {

	audacity_mod_script_pipe pair;

//	DONE in CTOR
//	pair.set_filename_PAIR_to_default();

	if(!pair.open_pipe()) return FAIL_FAILED();

        return true;
}

int main( int argc, char ** argv ) {

        for(int i=1; i< argc; i++ ) {
                INFO("ARGV[%d] '%s'", i, argv[i] );
        }

	if(bool_main()) return 0;
	if(errno) return errno;
	return 22;
        return 0;

}



