#ifndef tty_panel_H
#define tty_panel_H

#include "tty_curses.h"

namespace TTY_CURSES {


 class tty_panel {
  public:
	virtual ~tty_panel ();
	tty_panel ();
	// setup caries colour, multi tty, fallbacks
	virtual bool setup();
	
 };

};

#endif
