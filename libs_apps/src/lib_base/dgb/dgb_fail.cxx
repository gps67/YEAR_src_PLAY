#include "dgb_fail.h"
#include "e_print.h"
#include <stdio.h>
#include "misc.h"


/*!
	constructor - see dgb_fail_base
*/
dgb_fail::dgb_fail( const char * _msg )
: dgb_fail_base( _msg )
{
}

/*!
	destructor - plain
*/
dgb_fail::~dgb_fail()
{
}

///////////////////////////////////////////////////////

/*!
	constructor - exception is a single string message

	Option of calling gdb as well as jumping back.

	note use of e_print and not FAIL etc ....
*/
dgb_fail_base::dgb_fail_base( const char * _msg )
{
	// retain the message (presume static or careful malloc)
	msg = _msg;

	// tell the trace logs
	e_print( "# !!!! # EXCEPTION: %s\n", msg );
	fprintf( stderr, "# !!!! # EXCEPTION: %s\n", msg );
	fflush(0);

	// gdb
	if(1) {	// ALWAYS
		/*
			invoke gdb with the gui
		*/
		bool usegui = true;
		usegui = false;
		gdb_invoke( usegui );
	} else {
		/*
			IF gdb has already been activated
			eg by running from gdb, or from some other
			eg gdb_invoke(),
			stop at this break_point
			(and all future break points).

			REQUIRE export INGDB=notnull // 
		*/
		char * ingdb = getenv( "INGDB" );
		if( ingdb )
			gdb_off = false;
		gdb_break_point();
	}
}

/*!
	tell tracelog that the exception was caught
*/
void dgb_fail_base::caught()
{
	e_print( "CAUGHT: %s\n", msg );
}

/*!
	destructor - msg was never allocated
*/
dgb_fail_base::~dgb_fail_base()
{
	msg = NULL;
}

/*!
	Most get vectored through here
*/
void throw_new_dgb_fail( const char * msg )
{
	bool override = false;				// gdb can change this
	dgb_fail * exception = new dgb_fail( msg );	// invoke gdb
	if( !override ) throw exception;		// C++ throw
}

///////////////////////////////////////////////////////

/*!
	STATIC - create and throw a new exception
*/
void * NULL_dgb_fail( const char * msg )
{
	void * resp = NULL;	// gdb user could put a full value here
	throw_new_dgb_fail( msg );
	return resp;
}

/*!
	STATIC - create and throw a new exception
*/
bool FALSE_dgb_fail( const char * msg )
{
	bool resp = false;	// gdb user could put a full value here
	throw_new_dgb_fail( msg );
	return resp;
}

/*!
	STATIC - create and throw a new exception
*/
void THROW_dgb_fail( const char * msg )
{
	throw_new_dgb_fail( msg );
}


