// suddenly SP_App has to be included first - ?
#include "SP_App.h"
#include "text_collector.h"
#include <string.h> // memmove
#include "str_obj.h"

text_collector::text_collector(int N) : buff(N)
{
	wrap = 0;
	putc_prev = ' '; // (once) Shall strip leading SP's if wrap is set
	drop_cr = 0;	// comments get CRLF, text gets CR, into LF
	// set_wrap( 0 ); // NB text_collector is also a local mk_string temp
#ifdef USING_PY_STRINGS
	flags = 0;
#else
	flags = SPIN_string_flags_8859;
#endif
}
text_collector::~text_collector()
{
	// buff1 auto deletes
}

void text_collector::data_str(const char * str )
{
	while( char c = *str++) {
		data_char( c );
	}
}

void text_collector::data_string(const SGMLApplication::CharString & str )
{
	for( size_t i=0; i< str.len; i++)
	{
		data_Char( str.ptr[i] );
	}
}

/*	this is still a bit loose, pystring is needed for simpler scripts
	other string types are needed for non 8859
	using pystring for attrs, gi, sdata names
	They could be forced into utf, but it one method for now
*/

AppString text_collector::mk_string_py(void)
{
	AppString s = PyString_FromStringAndSize(
	(STR0)	buff.buff,
		buff.nbytes_used);
	buff.nbytes_used = 0; //leave putc_prev
	if(!s)
		; // requires app ! // vret(s); // report fail
	return s;
}

AppString text_collector::mk_string(void)
{
#ifdef USING_PY_STRINGS
	return mk_string_py();
#else
	SPIN_string * s = new_SPIN_string(
		     flags, // buff doesn't hold flags or know beyond byte
		buff.nbytes_used,
		buff.buff );
	buff.nbytes_used = 0; //leave putc_prev
	if(!s)
		; // requires app ! // vret(s); // report fail
	return (PyObject *) s;
#endif
}

int text_collector::trim_trailing_space(void)
{
	if(!buff.nbytes_used)	// boolean none found
		return 0;
	if( buff.buff[buff.nbytes_used-1] != ' ' )
		return 0;
	buff.nbytes_used --;
	return 1;
}

int text_collector::trim_leading_space(void)
{
	if(!buff.nbytes_used)
		return 0;
	if( buff.buff[0] != ' ' )
		return 0;
	char * Q =(char *) buff.buff;
	char * P = Q + 1;
	memmove( Q, P, buff.nbytes_used - 1 );
	buff.nbytes_used --;
	return 1;
}
