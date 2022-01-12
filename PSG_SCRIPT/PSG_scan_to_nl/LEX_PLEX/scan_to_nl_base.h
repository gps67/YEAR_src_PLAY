#ifndef SCAN_NL_BASE_H
#define SCAN_NL_BASE_H 

#ifndef EOF
#define EOF (-1)
#endif

struct str1; // only used by report2

#include "scan_to_nl_P.h"
#include "p0p2.h"
// #include <stdarg.h>

/*!
	scan_to_nl_base is a sublexer, that pauses at every NL (or NUL)
	It allows simple comparisons of *P, except for NUL and NL

	There is a NL or NUL at the EOF, so it will stop.
	NUL will print a warning, but work OK.

	There might be a 'read more' system, but that will pause at NL
	and might have problems with back-tracking, and retained pointers
	to a resizzed/relocated buffer

	_base is the core of the derived class, but there are additional
	stages that add almost-lex items for C for XML for ...
	but they are added in stages.

	scan_ means that the characters are scanned, but no LEX GAP

	Theres a lot of inlining, so that the compiler can keep
	P and this, then only write P back, when really needed.

*/

class scan_to_nl_base : public scan_to_nl_P
{
 protected:
	//! pointer to the start of the line, eof if 0
	u8 *	P_X0;
	//! line number
	int	Y;
	//! the entire zone of the file (ending in nl or NUL)
	p0p2	file_zone;
	//! zero until eof
	int EOF_touched;
 public:

	scan_to_nl_base( void )
	: scan_to_nl_P()
	{
		// P = 0 ;
		Y = 1 ;
		P_X0 = 0 ;
		EOF_touched = 0; // count 
	}

	// init over a p0p2 buffer (such as one from mmap)
	scan_to_nl_base( p0p2 buffer )
	: scan_to_nl_P()
	{
		set_file_zone( buffer ); // can throw
	}

	bool set_file_zone( const p0p2 & buffer );
	bool set_file_zone( const char * text_string );

// statics:
	static int cset_inited_a; // init defined as = 0;
	static cset_bit_map  cset_09;
	static cset_bit_map  cset_09_af_AF;
	static cset_bit_map  cset_az;
	static cset_bit_map  cset_AZ;
	static cset_bit_map  cset_AZaz;
	static cset_bit_map  cset_AZaz_;
	static cset_bit_map  cset_AZaz09;
	static cset_bit_map  cset_AZaz09_;
	static cset_bit_map  cset_line;

// not statics // can vary per class or instance
	cset_bit_map  cset_ident_a1;
	cset_bit_map  cset_ident_a2;
// init
	void init_csets(void);
static	void init_csets_statics(void);


	/*!
		different LEX'ers setup different csets
	*/
	bool scan_ident( p0p2 & zone )
	{
		return scan_a1_a2_star(
			zone,
			cset_ident_a1,
			cset_ident_a2
		);
	}

	/*!
		scan over a fixed "word" using standard a2
	*/
	bool scan_word( const char * word )
	{
		return scan_word( (const u8 *) word);
	}

	/*!
		scan over a fixed "word" using standard a2
	*/
	bool scan_word( const u8 * word )
	{
		return scan_word_a2( word, cset_ident_a2 );
	}

	/*!
		#define has hash at boln
	*/
	bool	scan_hash_at_boln()
	{
		if( *P != '#' ) return false;
		if( P != P_X0 ) return false;
		P++;
		return true;
	}

	u8 *	P_last( void )	{ return file_zone.p2 - 1; } // rare use
	int	N_left( void )	{ return file_zone.p2 - P; } // rare use
	int	P_as_OFFS()	{ return P - file_zone.p0; } // rare use

	bool	check_nl_at_eof();
	bool	check_nul_at_eof(); // for STR0 not LF
/*
	terminator characters must only be matched by these (inline) functions
*/

	bool	scan_nl_fn() { return scan_LF_fn(); }
	bool	scan_LF_fn();	// make inline smaller
	bool	scan_nul_fn();	// make inline smaller
	bool	scan_eof_fn(); // does any EOF in one place

	/*
		scan_eof depends on scan_nl() (scan_nul) to 
		detect P reaching file_zone.p2, and change P_X0, P

		after each sucessful scan_nl or scan_nul (or nothing matching)
		you should check with scan_eof (also at start of file)

		you only need to test for eof after the end of line
		
		MACRO: return ( P < P_X0 );
	*/
	bool	scan_eof( void )
	{
		return	scan_eof_fn(); // 
	}

		// you should never call this, except LAST
		// after SCAN_LINE SCAN_WORD SCAN_PUCT have all failed
		// after SCAN_LF has failed
		// so no great optiumisation inline
		// but is first that detects P == file_zone.p2 or P2-1

	/*!
		only scan_nul() may step over a NUL bytes, else false
	*/
	bool	scan_nul( void )
	{
		if( *P ) return FALSE;
		return scan_nul_fn();
	}

	/*!
		only scan_nl() may step over a NL bytes, else false
	*/
	bool	scan_nl()
	{
		if( *P != '\n' ) return FALSE;
		return scan_nl_fn();
	}

	// used ? allow LF ?
	bool	scan_crlf( void );

/*
	This class keeps an over-simplistic position counter
	A derived class might add multi-segments, backtracking.
	Hence virtual, eg to reset P_X0,Y if Y<1 or something?
*/
	virtual void	get_x_y( int & x, int & y );
	virtual int	get_x();
	void	get_curr_line_zone( p0p2 & line_zone );

// GCC Function-Attributes to check for fmt
// GXX increases parameter numbers by 1 == this
// #define FMT0 __attribute__((__format__ (printf, 1, 2)))  
#define FMT1 __attribute__((__format__ (printf, 2, 3)))  
#define FMT2 __attribute__((__format__ (printf, 3, 4)))  

/*
	Simple error reports, that show the error message under the
	line where it was detected. NB another system, might collect
	several failed paths, at the furthest points (before backtracking)
	Maybe these should be virtual? Maybe another system. const?
*/
	void report_lhs( const char * LHS = NULL );
	void report1( const char * name );
	void report2( const char * name, const char * s );
	void report2( const char * name, const str1 & s );
	void report_vprint( const char * err, const char * fmt, va_list args );
FMT2	void report_print( const char * err, const char * fmt, ... );
FMT1	void report_FAIL( const char * fmt, ... );
FMT1	void report_WARN( const char * fmt, ... );
FMT1	void report_OK( const char * fmt, ... );

};

#endif
