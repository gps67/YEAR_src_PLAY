#ifndef SCAN_NL_P_H
#define SCAN_NL_P_H 

// p0p2 is used as a parameter, not expanded
struct p0p2;
// #include "p0p2.h"

/*
	cset_bit_map is used a lot (but could rework with [a-z][A-Z] ...
	This is used for BYTE/8859 but utf8 fits into zones anyway
*/
#include "cset_bit_map.h"

/*!
	scan_to_nl_P is a single pointer ONLY (sub-classes add)

	It can scan/parse simple lexical things, ints, strings, ....

		if( scan_char('&') ) { ...got_ampersand...  }

		if( scan_chars('?','>') ) { ...got_PI_end...  }

		if( scan_chars('<','<','=') ) { ...got_operator("<<=")...  }

		if( scan_word("include") ) { ...rw_include_found... }

		if(!scan_hex_digits(val)) { ... }

	TODO:

		move more simple lex things (ints) here
		add some floats (etc)

	THINK:
		single input state, multiple output
		eg is it a decimal, a thing_long or a float
			ie return value, and type_tag
		this is really a case for expanding LEX items
		to a higher layer, rather than here.
		For now try the hardest first, and fail most,
		or fail if an item could be a longer more complex
		(effectively the FSM for all of them, but each tried in turn)

	It wont step over a NUL or NL (except specifically)

	Sub-classes are needed to:

		mark and detect EOF
		step over NUL / NL (detecting EOF)
		multi-line comments (etc)
		roll-back
		report warnings / messages (hence expect_ is subclass)
		adjust a1a2 for identifiers of language, and not_a2 of words
		do case-ignore stricmp of words
		gather text (input buffer is in its CSET)

	MAYBE TODO:

		fixup utf8 double encoding, as its very special
*/
class scan_to_nl_P
{
 protected:
	//! pointer to the position in the buffer
	u8 * 	P;

 public:

// statics:
	static int cset_inited_a; // init defined as = 0;
	static cset_bit_map cset_09;
	static cset_bit_map cset_09_af_AF;
	static cset_bit_map cset_az;
	static cset_bit_map cset_AZ;
	static cset_bit_map cset_AZaz;
	static cset_bit_map cset_AZaz_;
	static cset_bit_map cset_AZaz09;
	static cset_bit_map cset_AZaz09_;
	static cset_bit_map cset_line;
	static cset_bit_map cset_ident_a1;
	static cset_bit_map cset_ident_a2;

// init
	void init_csets(void);

	scan_to_nl_P( void )
	{
		P = 0 ;
	}

	bool	peek_nl( void )
	{
		return( *P == '\n' );
	}

	bool	peek_nul( void )
	{
		return( *P == 0 );
	}

/*
	now for the basic scanning functions
*/

	/*!
		check for a punctuation bytes
	*/
	bool	scan_char( u8 literal )
	{
#define NOT_allow_bad_usage
#ifdef allow_bad_usage
#warning "this blows up with subclasses scan_nl anyway"
		if( literal == '\n' )
		{
			// maybe the library could allow these, since they are handled
			// for now it's diagnotic (option of removing)
			e_print("## WARN ## Dont use scan_char(literal) for NL or NULL\n");
			return scan_nl();	/*  ie TRUE  */
		}
		if( literal == 0 )
		{
			e_print("## WARN ## Dont use scan_char(literal) for NL or NULL\n");
			return scan_nul();	/* want non inline */
		}
#endif
		if( *P != literal ) return FALSE; 
		P++;
		return TRUE;
	}

	/*!
		check for a SP or TAB plus
	*/
	bool	scan_spaces_and_tabs()
	{
		if(*P > ' ') return false;
		return scan_spaces_and_tabs_fn();
	}

	bool scan_spaces_and_tabs_fn();

	/*!
		check for two adjacent punctuation bytes
	*/
	bool	scan_chars( u8 c0, u8 c1 )
	{
		if( P[0] != c0 ) return false;
		if( P[1] != c1 ) return false;
		P += 2;
		return true;
	}

	/*!
		check for three adjacent punctuation bytes
	*/
	bool	scan_chars( u8 c0, u8 c1, u8 c2 )
	{
		if( P[0] != c0 ) return false;
		if( P[1] != c1 ) return false;
		if( P[2] != c2 ) return false;
		P += 3;
		return true;
	}

	/*!
		check for a punctuation byte sequence
	*/
	bool	scan_chars( const u8 * str, int len )
	{
		for( int i=0; i<len; i++ )
		{
			if( P[i] != str[i] ) return false;
		}

		P += len;
		return true;
	}

	/*!
		scan upto Q2 or ) or ...
	*/
	bool	scan_upto_char( u8 c1 )
	{
		u8 * P0 = P;
		while( (*P) && ( *P != c1 ) && ( *P != '\n' ) ) P++;
		if( *P == c1 )
			return true;
		P = P0;
		return false;
	}

	/*!
		scan upto Q2 or ) or ... EOLN

		HMMM maybe this shoudl always suceed, depending on whether
		you WANT to accept NUL in a multi-line text as NL or C or BAD
	*/
	bool	scan_upto_char_or_eoln( u8 c1 )
	{
		u8 * P0 = P;
		while( (*P) && ( *P != c1 ) && ( *P != '\n' ) ) P++;
		if( (*P == c1) || (*P == '\n') )
			return true;
		P = P0;
		return false;
	}

	/*!
		sweep upto the eoln
	*/
	void	scan_to_eoln()
	{
		while( *P != '\n' ) P++;
	}

// scan - using bitmaps

	/*!
		scan_a1( setofchars ) but dont advance P
	*/
	bool	peek_a1( const cset_bit_map & a1)
	{
		return( a1.get_bit( *P ));
	}

	/*!
		scan a char (byte) that is in a1 (wont work with utf8)
	*/
	bool	scan_a1( const cset_bit_map & a1)
	{
		if( a1.get_bit( *P ))
		{
			P++;
			return TRUE;
		}
		return FALSE;
	}

	/*!
		scan chars (byte) that are in a1
	*/
	bool	scan_a1_plus( const cset_bit_map & a1)
	{
		bool found=FALSE;
		while( a1.get_bit( *P ))
		{
			P++;
			found=TRUE;
		}
		return found;
	}

	/*!
		scan chars (byte) that are in a1, always return true
	*/
	bool	scan_a1_star( const cset_bit_map & a1 )
	{
		while( a1.get_bit( *P )) P++;
		return TRUE;
	}

	/*!
		scan identifier
	*/
	bool scan_a1_a2_star(
		p0p2 & zone,
		const cset_bit_map & a1,
		const cset_bit_map & a2
	)
	{
		if(!peek_a1( a1 ) ) return false;
		return scan_a1_a2_star_fn( zone, a1, a2 );
	}

	bool scan_a1_a2_star_fn(
		p0p2 & zone,
		const cset_bit_map & a1,
		const cset_bit_map & a2
	);

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

	bool scan_word( const u8 * word, const cset_bit_map & a2 );

	/*!
		scan over a fixed "word" using standard a2
	*/
	bool scan_word( const u8 * word )
	{
		// return scan_word( word, cset_AZaz09_ );
		return scan_word( word, cset_ident_a2 );
	}

	/*!
		scan over a fixed "word" using standard a2
	*/
	bool scan_word( const char * word )
	{
		return scan_word( (const u8 *) word);
	}


	bool peek_hex_digit();
	bool scan_hex_digit();
	bool scan_hex_digit( int & dgt );
	bool scan_hex_digits( int & dgt );

// NICE EXTRAS

};
#endif
