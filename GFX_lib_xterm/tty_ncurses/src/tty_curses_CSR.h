#ifndef tty_curses_CSR_H
#define tty_curses_CSR_H

#include "tty_curses_WIN_BOX.h"
// #include "tty_curses_STUBS.h"
//	#include <stdarg.h> // 
// #include "tty_curses.h"

#ifndef NULL
#define NULL 0L
#endif

#if 0
 struct screen;
 typedef struct screen SCREEN;
#endif

// #include <ncurses.h> // chtype !!!


namespace TTY_CURSES {

 /*!
 */
 class tty_curses_CSR : public tty_curses_WIN_BOX {
 public:
	tty_curses_CSR( tty_curses * _TTY_curses ); // PARENT

 }; // class

}; // namespace

#endif

