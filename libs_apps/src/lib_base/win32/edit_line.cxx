
#ifdef WIN32
#include "edit_line_win32.cxx"
#define edit_line_BASE edit_line_win32
#else
#include "edit_line_unix.cxx"
#define edit_line_BASE edit_line_unix
#endif

/*!
 * 	CTOR
 */
edit_line_base::
edit_line_base()
{
	PS1 = new str2("PS1> ");
	PS2 = new str2("PS2> ");
	PromptString = PS2;
}

/*!
 * 	DTOR
 */
edit_line_base::
~edit_line_base()
{
	// defaults
}

edit_line::
edit_line( const char * progname )
: edit_line_BASE( progname )
{
}

edit_line::
~edit_line()
{
}
