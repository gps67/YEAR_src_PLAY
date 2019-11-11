#ifndef tty_curses_CSR_H
#define tty_curses_CSR_H

#include <ncurses.h> // to make inline functions work
#include <stdarg.h> // 
#include "tty_curses.h"

#ifndef NULL
#define NULL 0L
#endif

 struct screen;
 typedef struct screen  SCREEN;

 // NO struct WINDOW; // ncurses stubs
 typedef struct _win_st WINDOW;


namespace TTY_CURSES {

 class tty_curses_CSR {
 public:
	tty_curses * TTY_curses; // PARENT
	WINDOW * win; // = stdscr;

	tty_curses_CSR( tty_curses * _TTY_curses ); // PARENT
	void set_stdscr( WINDOW * W = NULL );

	int y_was;
	int x_was;

	void get_yx() {
		get_yx_was();
	}
	void get_yx_was();

	void move( int y, int x ) {
		wmove(win,y,x);
	}

	void printf( const char * fmt, ... );
	void print( const char * str) {
		printf( "%s", str );
	}

	// attr_on is a CURSES MACRO
	void fgbg_on( int attr ) { attron( attr ); }
	void fgbg_off( int attr ) { attroff( attr ); }

	void clear_to_eol();
	void move_status_line(); // and clear it

	int get_ch();


 };

};

#endif

