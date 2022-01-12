
#include "scan_to_nl_base.h"

/*!
	print the lex position at the start of the report line
*/
void scan_to_nl_base::report_lhs( const char * LHS )
{
	int x_start;
	int y_start;

	if(!LHS) LHS = "LHS";

	get_x_y( x_start, y_start );
	e_print("# %2d %2d # %s  ", y_start, x_start, LHS );
}

/*!
*/
void scan_to_nl_base::report1( const char * name )
{
	report_lhs( "Got" );
	e_print("# %s \n", name );
}

/*!
*/
void scan_to_nl_base::report2( const char * name, const char * s )
{
	report_lhs( "GOT" );
	e_print("# %s('%s')\n", name, s );
}

/*!
*/
void scan_to_nl_base::report2( const char * name, const str1 & s )
{
	report2( name, (STR0) s.str );
}

// #warning "here_back cant move to a previous line"
/*
	here_start would need to store P_X0 as well as P

	The original idea was to never pass LF, then back

	maybe get_curr_line_zone could look backwards in zone
*/

void scan_to_nl_base::report_vprint( const char * err, const char * fmt, va_list args )
{
//	va_list args;
//	va_start( args, fmt );

	// print the source line
	report_lhs( err ); // x,y should NOT change
	p0p2 line_zone;
	get_curr_line_zone( line_zone );	// needs P_X0
	str1 line( line_zone );
	e_print("#\t%s\n", line.str );

	// print the position + message
	report_lhs( err );
	e_print("#\t" );
	int x_start, y_start;
	get_x_y( x_start, y_start );
	for( int i=1; i<x_start; i++ )
		e_print("-");

	e_print( "^--%s-- ", err );
	e_vprint( fmt, args );
	e_print( "\n" );
//	va_end( args );
}

void scan_to_nl_base::report_print( const char * err, const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	report_vprint( err, fmt, args );
	va_end( args );
}

/*!
	print multi-line error message, against source, point to pos
*/
void scan_to_nl_base::report_FAIL( const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	report_vprint( "FAIL", fmt, args );
	va_end( args );
}

/*!
	print warning (info) on the current line
*/
void scan_to_nl_base::report_WARN( const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	report_vprint( "WARN", fmt, args );
	va_end( args );
}

/*!
	print warning (info) on the current line
*/
void scan_to_nl_base::report_OK( const char * fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	report_vprint( "OK", fmt, args );
	va_end( args );
}

