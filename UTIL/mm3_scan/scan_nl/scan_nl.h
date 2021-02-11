#ifndef SCAN_NL_H
#define SCAN_NL_H /* getc *P++ buffer that checkcs eof every NL */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#pragma interface
/* file-heap */
#include "dgb_print.h"
#include "dgb_fluff.h"

/*
	I've forgotten why I didn't want to use regexp
	possibly because of 286 target, with no library
	possibly because of speed difference
	possibly to build a flexible library of building blocks
	possibly to have a getc() buffer, also terminated with NL/NUL
	with no need to do anything but a single *P++ upto NL or NUL
	then fixup to test for EOF or read-more or whatever

	Instead I have a word lexer that stops when it sees a LF (NL)
	or a NUL byte. That saves a lot of EOF testing, parameter
	passing, and there is scope for a dynamic buffer, where NUL means
	"load-more-from-disk" (or fifo) That does mean that you may
	not search for NUL or LF without calling the specific function
	that does that. It also means that the file MUST end with a NL,
	so that scanning does stop!

	If the buffer is completely empty (not really supported),
	the general assumption is that accessing (just outside) the buffer
	won't cause a bus error.
	
	If the buffer is properly terminated, you will never access
	outside, however, the last char read (the terminating newline)
	will REMAIN pointed to (to prevent excuberant code overshooting
	the end). In that case a special flag is set, so that the next 
	scan_nl gets eof. (normally P0 <= P, AFTER-EOF: P<P0)

	Hence you can always say: if( *P == 'X' ) P++; provided X is
	not NL or NUL.

	You provide the data, eg if scanning a syslog file, you would
	read a chunk, scan backwards (from the buffer edge) to find the last
	newline, set a p0p2 and scan that.

	In theory, withiin a line you can just pass P, but you would have to
	return the modified P. Hopefully the inline nature would do exactly
	that with register optimisation, but anyway you get the benefit of
	not comparing P < zone.p2 for every char, and things like comparing
	words are easier.


NOTE:
	str_len() does NOT do a strlen()  it does p2-p0
	scan_to_NL does not OWN the buffer it scans
	Y is the number of this line
	P0 points to the first char on this line (X0 might be better?)
	P0 <= P - except after EOF
	P points to the next char to be read
	P is always valid, and "sticks" at end

*/

#include "p0p2.h"

// used for scan_cset only 
#include "cset.h"

class scan_to_NL
{ public:
	u8 * 	P;	// csr
	int	Y;	// counting line numbers
	u8 *	P0;	// eof if P0 == 0 // else first char on line(x)
	p0p2	file_zone;

	// initialise an empty buffer (file_zone initialises itself)
	scan_to_NL( void )
	{
		P = 0 ;
		Y = 1 ;
		P0 = 0 ;
	}
	// init-copy an existing scanner
	scan_to_NL( scan_to_NL & buff )
	{
		P = buff.P ;
		Y = buff.Y ;
		P0 = buff.P0 ;
		file_zone = buff.file_zone ;
	}
	// init over a p0p2 buffer (such as one from mmap)
	scan_to_NL( p0p2 buffer )
	{
		file_zone = buffer;
		P=file_zone.p0;
		Y=1;
		P0=P;
		if( file_zone.str_len() == 0 )
		{
			// fake NL,
			P0++;
		}
	}

u8 *	P_last( void )	{ return file_zone.p2 - 1; }
int	N_left( void )	{ return file_zone.p2 - P; }

bool	check_nl_at_eof()
	{
		u8 clast = *P_last() ;
		if(clast == '\n') return TRUE;
		printf("check_nl_at_eof() found %2XX %c\n",(unsigned)clast,(unsigned)clast);
		if(clast == 0) return TRUE; // allow use on strings ??
		return FALSE;
	}

/* UNUSED --------------------------------------------------------------
int	is_eof()
{
	if( P0 == 0 )
	{
		dgb.fmt("GOOD: found eof indicator in P0\n");
//		assert( N_left == 1 ); // NL repeatedly found
		return TRUE;
	}
	if (P < P_last()) return FALSE;	// well inside // == means last char
	dgb.fmt("OK ONCE: is_eof last reached\n");
	if (P < P0 ) // impossible flag for eof reached,   last eaten
	{
		dgb.fmt("OK ONCE: is_eof P0 seen reached\n");
		return TRUE;
	}
	dgb.fmt("is_eof(not quite read NL\n");
	return FALSE;
}
int	is_last()
{
	return ((P+1) == file_zone.p2);
}
 UNUSED -------------------------------------------------------------- */

/*
	if buff.scan_char( '!' ) ...
*/
int	scan_char( u8 literal )
{
#define allow_bad_usage 1
#ifdef allow_bad_usage
	if( literal == '\n' )
	{
		// maybe the library could allow these, since they are handled
		// for now it's diagnotic (option of removing)
		dgb.fmt("Dont use scan_char(literal) for NL or NULL\n");
		return scan_nl();	/*  ie TRUE  */
	}
	if( literal == 0 )
	{
		dgb.fmt("Dont use scan_char(literal) for NL or NULL\n");
		return scan_nul();	/* want non inline */
	}
#endif
	if( *P != literal ) return FALSE; 
	P++;
	return TRUE;
}

//	P0 was P at BOLN when y==0
//	P0 <= P   ALWAYS EXCEPT AFTER P-- when it went VOID
//	if( P < P0 ) KWOWN("EOF already detected INSIDE scan_nl
//
int	scan_nl( void )
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

	if( P < P0 )		/* already counted final NL */
	{
		// you have already matched the last NL and asked again!
		dgb.fmt("scan_nl P<P0 - you are told here\n");
		return FALSE;
	}

	/* it's either a NL in the text, or the last NL */

	P++;	// scan over NL // BEFORE P0=p;
	Y++;	// count next line number Y++
	P0=P;	// line start (char * not file-seek)

	/* check for the last NL in the buffer, step back one and set flag */

	if( P == file_zone.p2 )	/* weve gone void */
	{
		P--;		// valid(P), ( P < P0 )
		write(1,"(nl-eof)",8); // this is not printed byt scan_mm3?
	}
	return TRUE;
}
int	scan_nul( void )
{
	if( *P ) return FALSE;
	if( P < P0 )		// IMPOSSIBLE flag
	{
		return FALSE;	// dont match EOS twice
	}
	P++; 
	if( P == file_zone.p2 )	// GONE VOID
	{
		P0 = P;		// NL does that automatically
		P--;		// NOW (P<P0)
	}
	return TRUE;
}
/*
	after each scan_nl or scan_nul (or nothing matching)
	you should check with scan_eof
	You should also check before doing anything
*/
int	scan_eof( void )
{
	if( P < P0 )
	{
//		assert( P == file_zone.p2 - 1 );
//		assert( (*P=='\n')||(!*P) );
		return TRUE;
	}
	return FALSE;
}

/*
	If it returns -1, you should feed it more data, that was EOF
	You don't HAVE to check for EOF after NL but the next request
	would probably print a diagnostic message, and irritate the user.
*/
int	get_ch( void )
{
	u8 ch = *P++;
	if( ch == '\n' )
	{
		P--;
		if(scan_nl()) return ch;
		return EOF;		// -1
	}
	if( ch == 0 )
	{
		P--;
		if(scan_nul()) return ch;
		return EOF;		// -1
	}
	return ch;
}

/*
	this is a better getc method, which wont advance over NL or NUL
	you could even base additional code around this model,
	and update P yourself. If it returns NUL it may be a normal NL,
	but since that doesn't appear in "words", that is what you want.

	This will fail for a completely empty p0p2 buffer,
	unless a static area is used for the null case
	Obviously the buffer must be LF terminated
*/
u8	get_ch_fast( void )
{
	u8 ch = *P++;
	if( ch == '\n' || (ch == 0))
	{
		P--;
		return 0;	// goto FAIL
	}
	return ch;
}

// PRESUME a1 EXCLUDES NUL and NL

int	scan_a1( cset_bit_map & a1)
{
	if( a1.get_bit( *P ))
	{
		P++;
		return TRUE;
	}
	return FALSE;
}

int	scan_a1_plus( cset_bit_map & a1)
{
	int found=FALSE;
	while( a1.get_bit( *P ))
	{
		P++;
		found=TRUE;
	}
	return found;
}

int	scan_a1_star( cset_bit_map & a1 )
{
	scan_a1_plus( a1 );
	return TRUE;
}

// repeat for byte_map

};
#endif
