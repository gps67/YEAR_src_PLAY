#ifndef SCAN_NL_H
#define SCAN_NL_H /* getc *P++ buffer that checkcs eof every NL */

#include "scan_to_nl_plus.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


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
	scan_nl gets eof. (normally P_X0 <= P, AFTER-EOF: P<P_X0)

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
	scan_nl does not OWN the buffer it scans
	Y is the number of this line
	P_X0 points to the first char on this line (X0 might be better?)
	P_X0 <= P - except after EOF
	P points to the next char to be read
	P is always valid, and "sticks" at end

*/

#include "p0p2.h"

// used for scan_cset only 
// #include "cset.h"

/*!
	scan_nl is a lex-level scanner, that STOPS at newline, \n, LF, 0x0A

	The caller loads a text area, checks that it ends with a LF, then
	uses lexers that stop at LF, and special EOF lexers that step over LF.

	This makes lexers faster as they never need to check for EOF,
	except when stepping over an LF and only need to use one pointer.

	NUL bytes are not really allowed, unless you read the source closely.
	There is an alternative mode that uses NUL exactly like NL. LURK.

	Lex-level items that do contain LF (such as multi-line-comments),
	will need to do so, in multiple line/eoln steps.

	The supplied/example routines are almost as much as you need.

	The cursor pointer P is ALWAYS VALID (if initialy valid),
	so there is a special impossible condition for when EOF happens,
	(P is last NL, *P==NL, P_X0==NULL ).

	If you dont want to load the ENTIRE text in one go (eg 4K chunks),
	this class only needs a few modifications to load more on demand.
	At that point, old bookmarks within the buffer become invalid.
	Each loaded segment would need to locate its LAST NL, and your
	code would still be LINE oriented.

	A line counter is provided to locate syntax errors, and P_X0
	points to the first char of the line (NL if an empty line).

	CR is just-another-char.

	At the end of file, scan_nl() succeeds ONCE, then fails.
*/
struct mm3_file_scan;
class scan_to_nl : public scan_to_nl_plus
{
	friend struct mm3_file_scan; // too lazy to adjust code
 public:
	u8 *	P_start_lex;
	u8 *	P_start_gap;
	u8 *	P_start_next;
	int	x_lex;
	int 	y_lex;
	int	x_gap;
	int	y_gap;
	int	x_next;
	int	y_next;

	bool	GAP_after_LEX_found;

	/*!
		Every LEX item, even semicolon can have trailing GAP.

		That might accidentally eat up a DOC comment, but ... OK ...
	*/
	virtual bool GAP_after_LEX();

	//! constructor - init empty
	scan_to_nl( void )
	: scan_to_nl_plus()
	{
	}

/*!
	// init-copy an existing scanner
	scan_to_nl( scan_to_nl & buff )
	{
		set_file_zone( buff.file_zone );
		P = buff.P ;
		Y = buff.Y ;
		P_X0 = buff.P_X0 ;
	}
*/
	scan_to_nl( p0p2 buffer )
	: scan_to_nl_plus( buffer )
	{
		P_start_lex = P;
		P_start_gap = P;
		P_start_next = P; // most important one to init!
	}


/*!
*/
	bool	lex_char( u8 literal )
	{
		if( *P != literal ) return FALSE; 
		P++;
		GAP_after_LEX();
		return TRUE;
	}

/*!
*/
	bool	lex_char_expect( u8 literal )
	{
		if( *P == literal ) {
			P++;
			GAP_after_LEX();
			return TRUE;
		}
		scan_char_expect( literal );
		return false;
	}

/*!
*/
	bool	lex_chars( u8 c0, u8 c1 )
	{
		if( P[0] != c0 ) return false;
		if( P[1] != c1 ) return false;
		P += 2;
		GAP_after_LEX();
		return true;
	}


/*!
*/
	void	scan_line()
	{
		scan_a1_star( cset_line );
		// you must still skip crlf at eoln
		// return true or void
	}



/*!
*/
	/*
		UNUSED - but some might want it - single byte char
	*/
	int	get_byte( void )
	{
		u8 ch = *P;
		if( ch < ' ' ) return get_byte_fn();
		if( ch > 127 ) return get_byte_fn();
		P++;
		return ch;
	}

	/*!
	*/
	int	get_byte_fn( void )
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

/*!
*/
	/*
		this is a better getc method, which wont advance over NL or NUL
		you could even base additional code around this model,
		and update P yourself. If it returns NUL it may be a normal NL,
		but since that doesn't appear in "words", that is what you want.

		This will fail for a completely empty p0p2 buffer,
		unless a static area is used for the null case
		Obviously the buffer must be LF terminated
	*/
	u8	get_byte_fast( void )
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


/*!
*/
	bool	scan_NOSP()
	{
		if( GAP_after_LEX_found ) return false;
		GAP_after_LEX();
		if( GAP_after_LEX_found ) return false;
		return true;
	}

	/*
		add some reporter vprintf functions
		add some XXX_expect() functions
		return same as XXX
	*/

// EXPECT

	bool	scan_char_expect( u8 literal );
//	bool	scan_char2_expect( u8 c1, u8 * c2 );
	bool	scan_word_expect( const u8 * word );
	bool	scan_word_expect( const char * word )
	{
		return scan_word_expect( (const u8 *) word);
	}

	// repeat for byte_map

};
#endif
