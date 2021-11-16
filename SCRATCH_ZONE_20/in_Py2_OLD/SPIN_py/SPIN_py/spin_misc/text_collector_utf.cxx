
// suddenly SP_App has to be included first - ?

#include "SP_App.h"
#include "text_collector_utf.h"
#include <string.h> // memmove
#include "str_obj.h"

/*
	this shares confusion with app_link::mk_string
	In future, this will do as its told, and mk_string
	will request what to do, by class or by parameter

	currently SPIN_STRING is macrod out to plain PyString
*/

text_collector_utf::text_collector_utf(int N) : text_collector(N)
{
}
text_collector_utf::~text_collector_utf()
{
	// buff1 auto deletes
}

void text_collector_utf::data_str(const char * str )
{
	while( char c = *str++) {
		data_char( c );
	}
}

void text_collector_utf::data_string(const SGMLApplication::CharString & str )
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

AppString text_collector_utf::mk_string_py(void)
{
	PyObject * s = PyString_FromStringAndSize(
	(STR0)	buff.buff,
		buff.nbytes_used);
	buff.nbytes_used = 0; //leave putc_prev
	if(!s)
		; // requires app ! // vret(s); // report fail
	return s;
}

AppString text_collector_utf::mk_string(void)
{
	return mk_string_py();

/* this is troublesome
	SPIN_string * s = new_SPIN_string(
		     flags, // buff doesn't hold flags or know beyond byte
		buff.nbytes_used,
		buff.buff );
	buff.nbytes_used = 0; //leave putc_prev
	if(!s)
		; // requires app ! // vret(s); // report fail
	return (PyObject *) s;
*/
}
