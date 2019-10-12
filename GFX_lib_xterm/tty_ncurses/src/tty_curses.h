extern bool main_test1();

namespace TTY_CURSES {
 class tty_curses {
  public:
	virtual ~tty_curses ();
	tty_curses ();
	// setup caries colour, multi tty, fallbacks
	virtual bool setup();
	
 };
};

