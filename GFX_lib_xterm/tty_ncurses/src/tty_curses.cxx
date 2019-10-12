#include <ncurses.h>
#include "tty_curses.h"
#include "dgb.h"
#include "e_print.h"

extern "C" void exit(int);


  bool main_test1()
  {       
	TTY_CURSES::tty_curses tty;
	tty.setup();

        printw("Hello World!");
        refresh();
        getch();

        return true;
  }

#include <signal.h>
  static void on_sig_INT(int sig) {
  	endwin();
	WARN("CTRL-C INTR");
	e_print("===============\n");
	exit(sig);
  }
 #warning this is nevver called ??

namespace TTY_CURSES {

 // ncurses is C and used loads of global names
 // but we could pretend that didn't matter
 // and keep ours in a class in a namespace

  tty_curses:: ~tty_curses () {
        endwin(); 
  }

  tty_curses:: tty_curses () {
  }

  bool tty_curses:: setup () {
  	(void) signal(SIGINT, on_sig_INT); // TTY is bigger than curses

        initscr(); // curses
	nonl();			// TTY NL -> CR LF // OFF
	cbreak();		// char break not line buffered
	raw();			// TTY line buffering mode
	noecho();		// TTY echo off
	keypad(stdscr, true );	// get KEY events

	if(has_colors()) {
	 start_color();
	 // colour pair(0, not available
	 init_pair( 1, COLOR_GREEN, COLOR_BLACK );
	 attrset( COLOR_PAIR(1) );
	}
  }

}; // namespace
