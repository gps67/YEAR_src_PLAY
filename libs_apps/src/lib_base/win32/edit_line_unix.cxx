
#include "edit_line.h"
#include <histedit.h>
#include "dgb_fail.h"

using namespace EL;

const char * C_get_prompt(EditLine * e)
{
	edit_line * ed = NULL;
	// (edit_line_unix *)
	el_get( e, EL_CLIENTDATA, & ed );
	if(!ed) return "NULL-E";
	return ed->get_prompt();
}


/*!
 * 	CTOR
 */
edit_line_unix::
edit_line_unix( const char * prog )
{
	FILE * fin = stdin;
	FILE * fout = stdout;
	FILE * ferr = stderr;
	_editline = NULL;
	_history = NULL;
	_tokenizer = NULL;
	PS1 = new str2("PS1> ");
	PS2 = new str2("PS2> ");
	PromptString = PS2;
	_editline = el_init( prog, fin, fout, ferr );
	if( !_editline ) {
		THROW_dgb_fail("new editline");
	}
	HistEvent history_ev;
	_history = history_init();
	history( _history, &history_ev, H_SETSIZE, 800);
	el_set( _editline, EL_HIST, history, _history);
	el_set( _editline, EL_CLIENTDATA, this );
	el_set( _editline, EL_PROMPT, C_get_prompt );
	el_set( _editline, EL_EDITOR, "emacs" );
	el_source( _editline, NULL ); // now use ~/.editrc
}

/*!
 * 	DTOR
 */
edit_line_unix::
~edit_line_unix()
{
	if( _editline ) {
		el_end(_editline) ;
		_editline = NULL;
	}
	_history = NULL;
	_tokenizer = NULL;
}

/*!
 * 	gets a line
 */
bool
edit_line_unix::
gets_PS1()
{
	HistEvent history_ev;
	if( !_editline ) {
		return false;
	}
	PromptString = PS1;
	int count = 0;
	const char * s = el_gets( _editline, & count );
	if(!count) return false; // CTRL-D
	if(!s) return false;
	buff.set( s );
	buff.trim_trailing_gaps();
	s = (str0) buff;
	if( buff.nbytes_used )
		history( _history, &history_ev, H_ENTER, s);
	return true;
}

/*!
 * 	gets a continuation line
 */
bool
edit_line_unix::
gets_PS2_append()
{
	HistEvent history_ev;
	if( !_editline ) {
		return false;
	}
	PromptString = PS2;
	int count = 0;
	const char * s = el_gets( _editline, & count );
	if(!count) return false; // CTRL-D 
	if(!s) return false;

	if( buff.nbytes_used )
		buff.put_plain( (str0)"\n" );
	buff.put_plain( s );
	buff.trim_trailing_gaps();
	// leave s as the single line input - with trailing spaces though
	// s = (str0) buff;
	if( buff.nbytes_used )
		history( _history, &history_ev, H_ENTER, s);
	return true;
}


/*!
 * 	gets the prompt string - internal
 */
const char *
edit_line_unix::
get_prompt()
{
	return (STR0) * PromptString;
}

