#include <ncurses.h>
#include "tty_curses.h"
#include "dgb.h"
#include "e_print.h"
#include "stdio.h"
#include <locale.h>

extern "C" void exit(int);

static WINDOW *
statusline(void)
{
    WINDOW * win = stdscr;
    wmove(win, LINES - 1, 1);
    wclrtoeol(win);
    // CSR at start of last line idx of LINES // counting from ZERO
    // +1 SP 
    return win;
}

#include <signal.h>
  static void handle_SIGINT(int sig) {

  	if(TTY_CURSES:: tty_curses:: tty_curses_global)
		TTY_CURSES:: tty_curses:: tty_curses_global -> call_endwin();
		// restart = resume = by next action
	if( sig != 2 ) {
		FAIL("SIGNAME expected %d got %d", SIGINT, sig );
	}
	/*
		nb app must handle EINTR in the REPL loop
		call_again immediately or fail + retry
	*/

	WARN("CTRL-C INTR exi4(%d)", EINTR);
	exit(EINTR);
  }

#if 0
  static
  void handle_SIGWINCH( int signal ) {
	// best to not handle it, let curses handle it, get KEY_RESIZE
	call_endwin();
	refresh(); // picks WH from xterm's ENV somehow // LINES COLS
        INFO("LINES = %d COLS = %d", LINES, COLS );
	// now compute new layout and draw it
	FAIL("unwritten");
  }
#endif

namespace TTY_CURSES {

 // ncurses is C and uses loads of global names
 // but we could pretend that didn't matter
 // and keep ours in a class in a namespace

  tty_curses:: ~tty_curses () {
	INFO("END TTY // _ncurses");
	setup_mouse(false);
	def_prog_mode(); // before shell out
	INFO("END TTY // 2");
	// def_shell_mode(); also exists
        call_endwin(); 
	delscreen( SCREEN_HERE );
	SCREEN_HERE = NULL;
	// hope next refresh sets itself as global ?
	// hope other fallback is stdscr
  }


  // static 
  tty_curses * tty_curses:: tty_curses_global = NULL;

  tty_curses:: tty_curses () {
   // if(!tty_curses_global) tty_curses_global = this;
   SCREEN_HERE = NULL;
   endwin_called = false;
  }

  void tty_curses:: call_endwin () {
  	endwin_called = true;
  	endwin();
  };

  bool tty_curses:: set_term_global () {
   INFO("TTY global");
   if( SCREEN_HERE ) {
    SCREEN * prev = set_term( SCREEN_HERE ); // all ncurses funcs to this
    INFO("LINES = %d COLS = %d", LINES, COLS );
    //NOR // COLS = 20;
    return true;
   } else {
    return FAIL( "curses SCREEN_HERE NULL");
    return false;
   }
  }
/* PANEL
  tty_curses:: go_xy ( int x, int y ) {
  	go_x( x );
  	go_y( y );
  }
  tty_curses:: go_y ( int x ) {
  	go_x( x );
  	go_y( y );
  }
*/
 
 // virtual
  bool tty_curses:: set_handle_SIGINT() 
  {
  	(void) signal(SIGINT, handle_SIGINT); // be brief on stack + time
	 // call_endwin() and Queue KEY_SIGINT
	 // can always restart after endin(); by next action
	return true;
  }

  bool tty_curses:: endwin_resume()
  {
	if(!endwin_called)  return true;
	if( mouse_on ) {
		mouse_on = false;
		setup_mouse( true );

	}
	return true;
  }

  bool tty_curses:: setup_mouse_xterm (bool on) 
  {
	 if( on )
	  printf("\033[?1003h\n"); // XTERM - report mouse movements
	 else
	  printf("\033[?1003l\n"); // Disable mouse movement events, as l = low
	  return true;
  }

  bool tty_curses:: setup_mouse (bool on) {

	if(on == mouse_on) { return true; }

	mouse_on = on;
	if( mouse_on ) {
	  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	} else {
	  // switch off
	}
	setup_mouse_xterm (mouse_on);
	 // mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	return true; // OK
	/*
		endwin lost mouseomaybe create a bool endwin_called
	*/
	
  }

  bool tty_curses:: setup () {

	// mouse_xterm_endwin_recover_bug 2020-05-01 // checked in
	tty_curses_global = this;

	 // TODO wbkgdset RTFM wresize( win, n_Height, n_Width )
	 // on resize, new area is painted background-on-background
	 // so already OK, leave CSR in a decent state
	 //
	 // nb invokes as a signal, with brief stack + time
	 // nb ncurses handles it, converting to KEY_RESIZE
	 //
  	if(!set_handle_SIGINT() ) return FAIL_FAILED();
	//
// best to not handle it, let curses handle it, get KEY_RESIZE
//  	(void) signal(SIGWINCH, handle_SIGWINCH ); // TTY is bigger than curses

	setlocale(LC_ALL, "");
	setlocale(LC_ALL, "en_GB");

	// alternate to initscr();

	SCREEN_HERE = newterm( NULL, stdin, stdout );
	set_term_global();

        // initscr(); // curses
	cbreak();		// char break not line buffered
	noecho();		// TTY echo off
	// halfdelay
	// // raw();			// TTY line buffering mode
	intrflush(stdscr, false ); // dont drop screen data on CTRL-C
	keypad(stdscr, true );	// get KEY events (otherwise get ESC X X)
	meta(stdscr, true );	// get 8 bits
	// NO WORKING HERE as expected tho
//	nodelay(stdscr, true); // nonblocking getch returns ERR // zooms to exit
//	nodelay(stdscr, false); // blocking getch waits for key
	// man curs_inopts says dont mix raw/cbreak ? 
if(0) {
 if(0) {
	noraw();	// CTRL-C == INTR
 }	// noraw() loses BUFFER battle
} else {
 if(0) {
	// raw() loses BUFFER battle
	// raw(); // dont interpret INTERRUPT QUIT SUSPEND XONN/XOFF
	// permits their use as commands
 }
}
	// tabs as left by tput init
	nonl();			// TTY NL -> CR LF // OFF // LF does LF

	if(has_colors()) {
	 start_color();
	 // colour pair(0, not available
	 init_pair( 1, COLOR_GREEN, COLOR_BLACK );
	 // app start on default from setup
	 attrset( COLOR_PAIR(1) );

	}
	setup_mouse();
	return true;
  }

}; // namespace
