
#include "scan_to_nl_base.h"
#include "str1.h"
#include "p0p2.h"
#include "dgb.h" // WARN FAIL

/*
	statics are just like "extern" things
	define them in the .CXX corresponding to the .H
*/
int scan_to_nl_base::cset_inited_a = 0; // init defined as = 0;
cset_bit_map scan_to_nl_base::cset_09;
cset_bit_map scan_to_nl_base::cset_09_af_AF;
cset_bit_map scan_to_nl_base::cset_az;
cset_bit_map scan_to_nl_base::cset_AZ;
cset_bit_map scan_to_nl_base::cset_AZaz;
cset_bit_map scan_to_nl_base::cset_AZaz_;
cset_bit_map scan_to_nl_base::cset_AZaz09;
cset_bit_map scan_to_nl_base::cset_AZaz09_;
cset_bit_map scan_to_nl_base::cset_line;
cset_bit_map scan_to_nl_base::cset_ident_a1;
cset_bit_map scan_to_nl_base::cset_ident_a2;
/*!
*/
void scan_to_nl_base::init_csets(void)
{
	// maybe add force_init_csets() which sets cset_inited_a = 0;
	if( cset_inited_a ) return;
	cset_inited_a = 1;

	e_print("##### C init_csets() ###\n");

	// could be done by init constructor!
	cset_az.set_null();
	cset_AZ.set_null();
	cset_09.set_null();
	cset_AZaz.set_null();
	cset_AZaz_.set_null();
	cset_AZaz09.set_null();
	cset_line.set_null();

	cset_az.set_range( 'a', 'z' );
	cset_AZ.set_range( 'A', 'Z' );
	cset_09.set_range( '0', '9' );

	cset_AZaz   |= cset_az;
	cset_AZaz   |= cset_AZ;
	cset_AZaz09 |= cset_AZaz;
	cset_AZaz09 |= cset_09;

	cset_line.set_range( 0, 255 );
	cset_line.reset_bit( 0 ); // not allowed
	cset_line.reset_bit( '\n' ); // not allowed
	cset_line.reset_bit( '\r' ); // not part of line text

	cset_AZaz_ = cset_AZaz;
	cset_AZaz_.set_bit( '_' );

	cset_AZaz09_ = cset_AZaz09;
	cset_AZaz09_.set_bit( '_' );

	cset_09_af_AF.set_null();
	cset_line.set_range( '0', '9' );
	cset_line.set_range( 'a', 'f' );
	cset_line.set_range( 'A', 'F' );

	// default identifier is C
	cset_ident_a1 = cset_AZaz;
	cset_ident_a2 = cset_AZaz09_;
}


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
bool	scan_to_nl_base::scan_LF_fn() // LF is LINE_FEED NEWLINE NL EOLN LF
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

	if( EOF_touched ) {
		if(1) {
			e_print("EOF touched and still looking for LF\n");
		}
		return FALSE;
	}

	if( P < P0 )	/* EOF previously found */
	{
		/*
			P0 is our lex p0p2.P0
		*/
			// STRANGE condition to flag some condition 
			// - move away or fix

		/*
			you have already matched the last NL and asked again!

			Actually this is OK, instead of matching EOF everywhere,
			try a few more branches, then fall back to EOF.
			Since the NL only matches ONCE, the other brances fail.
		*/
		if(1) {
			e_print("## WARN ## P < P0 \n");
		}
		return FALSE;
	}

	/* normal case NL found, possibly last one */

	// step over LF
	P++;	// scan over NL // BEFORE P0=p;
	// count Y++ and keep P0 of Y
	Y++;	// count next line number Y++
	P0 = P;	// line start (char * not file-seek)

	/* was that NL inside the text, or the last NL, triggering EOF */
	/* check for the last NL in the buffer, step back one and set flag */

	if( P == file_zone.p2 )	/* weve gone void */
	{
		// caller must call if(scan_eof())
		if(1) {
			e_print("## WARN ##  P == P2 FIRST DETECT \n");
			INFO("P %s", P );
			INFO("P0 %s", P0 );
		}
		EOF_touched ++; // actually not reported yet //
		// STEP BACK to P2_M1
		P--;		// valid(P), ( P < P0 )
		gdb_invoke(false);
		return true;
	}
	if( P > file_zone.p2 ) {	/* weve gone full void */
		FAIL("WAY PAST");
		// return true EOF
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

bool scan_to_nl_base::scan_eof_fn() 
{
	if( EOF_touched ) {
		WARN("called again");
		return true;
	}

	if( (*P==NUL)) {	// we accept NUL as EOF p0p2 == STR0
		INFO("NUL found for eof");
		EOF_touched ++;
		return true;
	}

	u8 * P2 = file_zone.p2 ;
	u8 * P2_M1 = P2 - 1;

	if( P < P2_M1 ) {
		INFO("excessive eof check (P<P2_M1)");
		return false;
	}

	if( P == P2 ) {
		INFO("exact eof - WHY DIDNT _nl_fn set EOF_touched");
		EOF_touched ++;
		return true;
	}


	if( P == P2_M1 ) {
		EOF_touched ++;
		if( (*P=='\n')  ) {
			INFO("got LF bouncing at EOF_touched %d", EOF_touched );
		} else {
			INFO("expected LF got 0x%2X", *P );
		}
		return true;
	}

	FAIL("this makes no sense");
	EOF_touched ++;
	return true;

	if( P < P0 )
	{
		return TRUE;
//		debugging options
//		assert( P == file_zone.p2 - 1 );
//		assert( (*P=='\n')||(!*P) );
	}
	return FALSE;
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

/* TODO
	{ u32 c3 c2 c1 OP } {
		DECODER MATCH OP
		 OP {
		 	OP is BYTE_A
		 	C1 is BYTE_B	// fast start
			WORD >> 16
		 	c2 is BYTE_A	// == CMP was_not_NUL
		 	C3 is BYTE_B	// == CMP was_not_NUL // detect NUL *P
		 }
	}
	FILTER plays a series of TOKENISED OPCODES MATCH(c1,c2,c3) etc
	PARSER plays against *P into PSG
	PSG uses TOKENISED_SCRIPT_from_GEN

		DECODER OP 
		 OP {
		 	OP is "c3 c2 c1 OP"
			B1 is "goto [B1]
			LO 
			HI

			HAVE { C == *P }
			if( C < LO ) goto NOMATCH
			// OPTION == // if( C == LO ) goto MATCH_LO //
			if( C > HI ) goto NOMATCH
			// OPTION == // if( C == HI ) goto MATCH_HI //
			goto "HERE = MATCH_in_range_LO_HI" // KNOW able

			MATCH "[LO HI]"
		 }

		SUBRANGE {
			used to hold a small number of GOTO_ADDR words

			u32 SUBRANGE[N] // 
			 idx2 = idx1 + SUBRANGE_BASE  // LO
			LO == SUBRANGE_BASE 
			OV == SUBRANGE_BASE + N
			HI == OV -1
			i2 == i1 - LO
			WORD = SUBRANGE[ i2 ] // [0[N // "[LO HI]" // [LO HI]
		}

		REMAP {
			u8 REMAP[range]
			used to hold a small number of GOTO_ADDR words

			u16 REMAP[range]
			used to hold a shared_space of EXPRS

			eg
			LIST_VALS V A L S
			LIST_IDX2 0 1 2 3 // idx2 = RANGE_idx ++ // N == idx
			VECT_EXPR E0 E1 E2 E3 // E2 = SHARED_SPACE[u16_E2]
		}

		SHARED_SPACE {
			EIGHT_t SPACE[u16_idx] // filled low to ROM STO
			u16 MINI_MACHINE_REMAPPE[u8] // why not LO += N
			u16 BIND_TOKEN_REMAPPE[u8] // why not LO += N
		}

		compile FSM to ASM switch_case goto_case[step]
		 NODE + STEPS[+] // STEP += on_MATCH_STEP_STEP_STEP
		  foreach NODE_MATCH_STEP[i] 
		   if MATCH then STEP // GOTO_EXIT_no_return
		  ELSE NO_MATCH



*/
