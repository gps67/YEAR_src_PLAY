#ifndef tty_curses_H
#define tty_curses_H

// #warning YES OK

#include "tty_curses_STUBS.h" // just in case

 struct screen;
 typedef struct screen  SCREEN;


extern bool main_test1();

namespace TTY_CURSES {

 class tty_panel {
  public:
 };

 class tty_curses {
 	SCREEN * SCREEN_HERE;
  public:
	virtual ~tty_curses ();
	tty_curses ();
	// setup caries colour, multi tty, fallbacks
	virtual bool setup();
	virtual bool set_handle_SIGINT(); // endwin() and Q KEY_SIGINT
	bool set_term_global(); // this becomes the one
	
 };

};

#endif
