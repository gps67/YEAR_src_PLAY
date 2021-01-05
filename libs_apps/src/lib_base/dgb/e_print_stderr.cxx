#include "e_print_io.h"

class e_print_stderr : public e_print_io_INTERFACE
{
public:
	bool write_UTF8( STR0 str ) {
		fprintf( stderr, "%s", str );
		fflush(0);
	}
};

e_print_io_INTERFACE * new_e_print_stderr()
{
	return (e_print_io_INTERFACE *) new e_print_stderr()
}

