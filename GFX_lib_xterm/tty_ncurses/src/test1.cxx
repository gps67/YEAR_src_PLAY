#include "tty_curses.h"
#include <errno.h>



int main()
{       
	if(main_test1())
		return 0;
	if(errno)
		return errno;
	return 22;
}

