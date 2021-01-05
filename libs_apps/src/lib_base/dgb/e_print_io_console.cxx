#include "e_print.h"
#include "e_print_io.h"
#include "obj_hold.h"
extern obj_hold<e_print_io_INTERFACE> e_print_io;

#include "obj_ref.h"
#include "blk1.h"
#include "buffer2.h"

#ifdef WITH_GTK2
#include <glib.h>
#endif

// ## INCLUDE _PRIV - manually pasted
// 

class e_print_io_console : 
public e_print_io_INTERFACE
{
 public:
	e_print_io_console() {
		app_prefers_8859( true );
	}
	~e_print_io_console() { }
	bool write_BYTES( blk1 & blk )
	{
		blk.trailing_nul();
		fprintf( stderr, "%s", (STR0) blk.buff );
		fflush(0);
		blk.clear();
		return true;
	}
};


// ########################
extern
void e_print_init_console()
{
	e_print_io_console * io = new e_print_io_console();
	io->set_as_glib_print_handler();
}
