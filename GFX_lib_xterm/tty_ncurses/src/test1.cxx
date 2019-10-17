#include "tty_curses.h"
#include <errno.h>

// PROBS with ncurses not using fd_sel_says
// bunch of uncalled INIT APP - TODO
// #include "APPS_stubs.h"
// #include "APPS_Holder.h"

int main()
{       
	if(main_test1())
		return 0;
	if(errno)
		return errno;
	return 22;
}

