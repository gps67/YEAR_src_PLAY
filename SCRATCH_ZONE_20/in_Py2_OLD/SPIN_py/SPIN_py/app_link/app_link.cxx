
#include "SP_App.h"
#include "text_collector_utf.h"
#include "app_link.h"
#include "Python.h"
#include "str_obj.h"

app_link::app_link(SPIN_top * _top)
{
	Py_INCREF( _top );
	top = _top;
	auto_load = false;
#ifdef APP_LINK_OWN_COPY_OF_EH
	_EH = top->EH;
	_dry = top->elem_info;
#endif
}

app_link::~app_link(void)
{
	Py_XDECREF( top );
}

AppObject app_link::raise_error(const char * msg) // create new one?
{
	if(PyErr_Occurred()) { // one already detected ...
		report_error(); // so report both!
	}
	// maybe use spin.elem_info error instead of TypeError
	PyErr_SetString( PyExc_TypeError, msg );
	report_error();
	return NULL;
}
AppObject app_link::report_error(void)
{
	if(!PyErr_Occurred()) { // Not all code is clever
		PyErr_SetString( PyExc_TypeError, "AN ERROR OCCURED" );
	}
	/*
		Python protocol to propagate an error to a specific handler
		fetch the values for the script and call the function
	*/
	PyObject * type = 0;
	PyObject * value = 0;
	PyObject * traceback = 0;
	PyErr_Fetch( &type, &value, &traceback);
	if(!value) {
		value = Py_None;
		Py_INCREF( value );
	}
	if(!traceback) {
		traceback = Py_None;
		Py_INCREF( traceback );
	}
	if(PyErr_Occurred())  // msg might be irrel
		PyErr_Print();
		      
	PyObject * ret = PyObject_CallMethod(
		EH(),
		(char *) "report_error",
		(char *) "(OOO)",
		type,
		value,
		traceback
	);
	if(PyErr_Occurred())  { // error in error handler
		// print the error that happenned during the above call
		PyErr_Print();
		// read up - set same error as arrival
		// core // PyErr_Restore( type, value, traceback);
		// core // PyErr_Print();
	}
	Py_XDECREF( ret );
	/*
		actually, if user forgot to define "EH.report_error()"
		maybe it should be printed here ...
		But above tells user that report_error wasn't found
		so thats sufficient
	*/
	// PyErr_Restore( type, value, traceback);
	Py_XDECREF( type );
	Py_XDECREF( value );
	Py_XDECREF( traceback );

	return 0;
}

int app_link::vret(AppObject ret)
{
	/*
		if(vret( Py_Some_call() ) return FAILED;
		Auto reports the error for you
		Auto DECREF the return value for you
		Value was PyObject or NULL
	*/
	if( ret ) {
		Py_DECREF( ret );
		return 0;
	} else {
		report_error();
		// Py_XDECREF( app->mk_location() );
		return 1;
	}
}

/*
	The mk_string family has been a fountain of confusion
	In future there will be a single switch for names
		Names are 8859
		Names are UTF8 
	const	Names are ASCII - builtin labels like "Tokenised" "cdata"
	and various switches for text.
		Text is 8859
		Text is UTF8
		Text is UNI16_in_some_container
		Text is some_new_type
		Text +- can be ANY of above or must be ONE of above
	There are also options for elem_info having several representations
	(that would re-optimise match_stack_top against SP_string), then
	adding a names pool for attribute names.

	You can have aditional flags, such as for when you are processing
	DHTML/TCL/NROFF and you want to say "already backslash decoded
	(or encoded) for transmission through URL to GROFF via TCL via
	SGML &amp; encoded SGML (using prorietry macro bytes extension 4)"

	The Names will each have their own routes (useful for hash tables)
	mk_string_elem_name() which is an inline alias for mk_string..

	For now most things go through the utf encoder, to detect activity
	but many go through a randomly accumulated lagacy route

	IDEA: "string" is name, "text" is text ???

	Note that match_string must follow same encoding, to match stack top
*/

AppString
app_link::mk_str( const char * str8 )
{
#ifndef allowing_old_path
	// force down common path - all strings can become internlised
	tmp_buff.data_string( str8 );
	AppString str = tmp_buff.mk_string_py();
	if(!str) vret(str);
	return str;
#else
	AppString str = PyString_FromString( str8 );
	if(!str) vret(str);
	return str;
#endif
}

AppString
app_link::mk_string( const SGMLApplication::CharString & s16 )
{
	// text_collector tmp_buff(s16.len); // + 50
	tmp_buff.data_string( s16 );
	AppString str = tmp_buff.mk_string();
	if(!str) vret(str);
	return str;
}

AppString
app_link::mk_string_py( const SGMLApplication::CharString & s16 )
{
	// text_collector tmp_buff(s16.len); // + 50
	tmp_buff.data_string( s16 );
	AppString str = tmp_buff.mk_string_py();
	if(!str) vret(str);
	return str;
}

AppString
app_link::mk_string_attr( const SGMLApplication::CharString & s16 )
{
	text_collector tmp_buff(s16.len); // + 50
	tmp_buff.data_string( s16 );
	AppString str = tmp_buff.mk_string_py();
	if(!str) vret(str);
	return str;
}

AppString
app_link::mk_string_elem( const SGMLApplication::CharString & s16 )
{
	text_collector tmp_buff(s16.len); // + 50
	tmp_buff.data_string( s16 );
	AppString str = tmp_buff.mk_string_py();
	if(!str) vret(str);
	return str;
}

AppString
app_link::mk_string_drop_lf( const SGMLApplication::CharString & s16 )
{
	// text_collector tmp_buff(s16.len); // + 50
	tmp_buff.drop_cr = 1; // LURK CRLF from DOS?
	tmp_buff.data_string( s16 );
	AppString str = tmp_buff.mk_string();
	if(!str) vret(str);
	return str;
}

/*
	the idea of match_string was to be quick
	now need to move CharString over to text_collector ...
	or fetch SPIN_string back here
	or ...
*/

bool match_string(AppString obj, const SGMLApplication::CharString & s16)
{
	char * s8 = 0;
	unsigned s8_len = 0;
#ifndef USING_PY_STRINGS
	if( is_SPIN_string( obj ) ) {
		s8 = ((SPIN_string *)obj) -> str;
		s8_len = ((SPIN_string *)obj) -> len;
#else
	if(0) {
#endif
	} else {
		s8 = PyString_AsString( obj ); // checks is string
		s8_len = PyString_GET_SIZE( obj );
	}
	if( !s8 ) {
		printf( "-((-match_string: null s8))-");
		return 0;
	}
	if( s8_len != s16.len ) {
		printf( "-((-match_string: len))-");
		printf( "\ns8 = %s\n", s8 );
		{
			text_collector_utf tmp_buff;
			tmp_buff.data_string( s16 );
			tmp_buff.data_byte(0);
			printf( "\ns16= %s\n", tmp_buff.buff.buff );
		}
		printf( "  s8_len = %d\n", s8_len );
		printf( "  s16.len = %d\n", (int) s16.len );
		return 0;
	}
	size_t i;
	for( i = 0; i < s16.len; i++ ) 
	{
		if( !s8[i] )
			return 0;
		SGMLApplication::Char c2 = s16.ptr[i];
		char c1;
		if(c2>255) { // unsigned
			printf( "-((-match_string: c2>255))-");
			return 0;
		} else 
			c1 = (char) c2 ;
		if( c1 != s8[i] ) {
			printf( "\n-((-match_string: strings differ ))-");
			printf( "\ns8 = %s\n", s8 );
			return 0;
		}
	}
	return (s8[i] == 0);
}


