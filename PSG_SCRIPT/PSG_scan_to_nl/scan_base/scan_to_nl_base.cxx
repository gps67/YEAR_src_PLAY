
#include "scan_to_nl_base.h"
#include "str1.h"
#include "p0p2.h"


//	u8 *	P_last( void )	{ return file_zone.p2 - 1; } // rare use
//	int	N_left( void )	{ return file_zone.p2 - P; } // rare use

/*!
	the buffer must end with a decent terminator - return T/F
*/
bool	scan_to_nl_base::check_nl_at_eof()
{
	u8 * P_last = file_zone.p2 - 1;
	u8 clast = *P_last;
	if(clast == '\n') return TRUE;
	e_print("# ERROR # check_nl_at_eof() found %2XX %c\n",
		(unsigned)clast,
		(unsigned)clast
	);
	if(clast == 0) return TRUE; // allow use on strings ??
	return FALSE;
}

/*!
	print the lex position at the start of the report line
*/
void scan_to_nl_base::report_lhs( const char * LHS )
{
	int x_start;
	int y_start;

	if(!LHS) LHS = "LHS";

	get_x_y( x_start, y_start );
	e_print("# %4s %2d %2d ", LHS, y_start, x_start );
}

/*!
*/
void scan_to_nl_base::report1( const char * name )
{
	report_lhs( "got" );
	e_print("# %s \n", name );
}

/*!
*/
void scan_to_nl_base::report2( const char * name, const char * s )
{
	report_lhs( "got" );
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
	here_start would need to store P0 as well as P

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
	get_curr_line_zone( line_zone );	// needs P0
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

// VIRTUAL
/*!
	get the X position within the line (P beyond P0)

	this wont work well with multi-byte utf, or NUL data
	but its enough to make ASCII easier to debug.
	For better, use P, P0 and file_zone.p0

	converting tabs to counted spaces
	might require print to have same alignment
*/
int scan_to_nl_base::get_x()
{
	if( P < P0 ) return 0;
	u8 * P1 = P0;
	int x = 1;
	while( P1 < P )
	{
		u8 ch = *P1++;
		x++;
		if( ch >= ' ' ) continue;
		if( ch == '\t' ) {
			x = (x + 7) & ~7;
		}
		// treat all other chars [0..31] as glyphs
		// should not include CR LF
	}
	return x;
}

// VIRTUAL
/*!
*/
void scan_to_nl_base::get_x_y( int & x, int & y )
{
	x = get_x();
	y = Y;
}

/////////////
// Things that need P0, Y
// specifically NUL and NL and EOF
/////////////


/*!
	If not a NL return false

	If NL, start next line or detect EOF

		scan over the '\n' char
		set P0 to point to X0 of next line
		(That keeps (P0 <= P) in normal use)
		check for EOF
		
	if EOF,
		P is NOT advanced over the NL
		P is left pointing to the last byte (NL or NUL)
		P0 is kept outside so that (P0 <= P) is nolonger true

	If EOF before starting, return FALSE

		scan_nl only detects the NL ONCE!

	If you want a multi-part read-next-block parser,
	its code goes here (but also check for comments
	that cross files)
*/
bool	scan_to_nl_base::scan_nl_fn()
{
	if( *P != '\n' ) return FALSE;

	/*
		the last NL in the buffer is left pointed to
		that prevents overruns. but needs a special "flag"
		to prevent an infinite number of trailing NL's

		The first time the last NL is encounted, return TRUE
		then return FALSE. That will work (hopefully) since nothing
		else matches, and the calling code should exit because it
		can't find anything to do. To diagnose infinite loops
		something is printed
	*/

	if( P < P0 )	/* EOF previously found */
	{
		/*
			you have already matched the last NL and asked again!

			Actually this is OK, instead of matching EOF everywhere,
			try a few more branches, then fall back to EOF.
			Since the NL only matches ONCE, the other brances fail.
		*/
		if(0) {
			e_print("## WARN ## repeat scan_nl() at EOF\n");
		}
		return FALSE;
	}

	/* normal case NL found */

	P++;	// scan over NL // BEFORE P0=p;
	Y++;	// count next line number Y++
	P0=P;	// line start (char * not file-seek)

	/* was that NL inside the text, or the last NL, triggering EOF */
	/* check for the last NL in the buffer, step back one and set flag */

	if( P == file_zone.p2 )	/* weve gone void */
	{
		P--;		// valid(P), ( P < P0 )
		if(0) {
			e_print("## INFO ## detect EOF in scan_nl()\n");
		}
	}
	return TRUE;
}

/*!
	only scan_nul() may step over a NUL byte, else false

	see scan_nl_fn, except NUL doesnt increment Y or P0 (except EOS)
*/
bool	scan_to_nl_base::scan_nul_fn( void )
{
	if( *P ) return FALSE;
	if( P < P0 )		// EOF already detected
	{
		return FALSE;	// dont match EOS twice
	}
	P++; 
	if(0) {
		e_print("## INFO ## detect NUL in scan_nul()\n");
	}
	// do not set P0 or Y++, but enables NUL inside text
	if( P == file_zone.p2 )	// GONE VOID
	{
		P0 = P;		// NL does that automatically
		P--;		// NOW (P<P0)
		if(0) {
			e_print("## INFO ## detect EOF NUL in scan_nul()\n");
		}
	}
	return TRUE;
}

/*!
	match CR and LF without exception (caller applies flexability)
*/
bool scan_to_nl_base::scan_crlf( void )
{
	if( P[0] != '\r' ) return FALSE; // NUL is not an option
	if( P[1] != '\n' ) return FALSE; // NUL is not an option
	P++; // skip the \r
	return scan_nl(); // always returns true, but does the extra code
}

/*!
	return the current line, going back to P0
*/
void	scan_to_nl_base::get_curr_line_zone( p0p2 & line_zone )
{
	// if EOF, return an empty string before the last byte
	if( P < P0 )
	{
		line_zone.p0 = P;
		line_zone.p2 = P;
		return;
	}
	// start with the line so far
	line_zone.p0 = P0;
	line_zone.p2 = P;
	// slide upto NL or NUL
	u8 * P1 = P;
	while( (*P1) && (*P1 != '\n') ) P1++;
	line_zone.p2 = P1;
}
