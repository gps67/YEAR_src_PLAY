
#include "dgb.h"
const static char CHAR_DELETE = 22;
#include "word_splitter.h"
#include "ASCII_chars.h"
#include "blk_base64.h"

/*!
*/
word_splitter::word_splitter()
: word_space( 50 )
, parse_error( false )
, remove_newlines_added_with_curlies( false )
{
}

/*!
*/
word_splitter::~word_splitter()
{
}

/*!
*/
void word_splitter::clear()
{
	word_space.clear();
	word_list.clear();
	parse_error = false;
}

/*!
*/
bool word_splitter::split_by_spaces( str0 line, int len )
{
	if(!len) len = line.str_len();
	clear();
	dyn_array<int> word_offs;
	word_offs.rezero();
	// const uchar NUL = 0;
	word_space.get_space( len + 30 );
	int offs = 0;
	const uchar * p1 = line;

	while( *p1 )
	{
		// skip spaces
		while( *p1 && (*p1 <= SP )) p1 ++;
		if(!*p1) break;
		offs = word_space.gap_offs();
		while( *p1 > ASCII_SP )
		{
			word_space.put( *p1++ );
		}
		word_space.put( NUL );
		int n = word_offs.N();
		word_offs.extend_by(1);
		word_offs[n] = offs ;
	}

	int n = word_offs.N();
	word_list.extend_to(n);
	for( int i=0; i<n; i++ )
	{
		word_list[i] = (str0)(STR0)( word_space.buff + word_offs[i] );
	}
	return true;
}

/*!
*/
bool word_splitter::split_by_spaces_and_quotes( str0 line, int len )
{
	if(!len) len = line.str_len();
	clear();
	dyn_array<int> word_offs;
	word_offs.rezero();
	// const uchar NUL = 0;
	word_space.get_space( len + 30 );
	int offs = 0;
	const uchar * p1 = line;

	bool q1 = false;
	bool q2 = false;

	while( *p1 )
	{
		// skip spaces
		while( *p1 && (*p1 <= ASCII_SP )) p1 ++;
		if(!*p1) break;
		offs = word_space.gap_offs();

		bool inword = true;

		while(inword && *p1)
		{
		 switch(*p1) {
		  case ASCII_NUL:
			inword=false;
			break; // impossible here
		  case ASCII_QUOTE_SINGLE:
			if(q1)	// treat as close quote - but stay in word!
			{
				q1 = false;
			}
			else	// treat as open quote - start/stay in word
			{
				q1 = true;
			}
			p1++;
			break;
		  case ASCII_QUOTE_DOUBLE:
			if(q2)	// treat as close quote - but stay in word!
			{
				q2 = false;
			}
			else	// treat as open quote - start/stay in word
			{
				q2 = true;
			}
			p1++;
			break;
		  case ASCII_BACKSLASH:
			if(p1[1]) {
				p1++;
				word_space.put( *p1++ );
			} else {
				p1++;
			//	return FAIL("BACKSLASH NUL - is not good for me");
				// it might be a chunk boundt though
			}
			break;
		  case ASCII_SP:
		  case ASCII_TAB:
		  default:
			if(*p1 <= ASCII_SP)
			{
				if( q1 || q2 )
				{
					// put spaces into word
					word_space.put( *p1++ );
					break;
				} else {
					// spaces break words
					inword = false;
					p1++;
					break;
				}
			} else
				// most plain chars land here
				word_space.put( *p1++ );
		 }
		} // while inword
		// close word and add to output list
		word_space.put( NUL );
		int n = word_offs.N();
		word_offs.extend_by(1);
		word_offs[n] = offs ;
	} // while *p1

	// convert offsets to str0 pointers
	int n = word_offs.N();
	word_list.extend_to(n);
	for( int i=0; i<n; i++ )
	{
		word_list[i] = (str0)(STR0)( word_space.buff + word_offs[i] );
	}
	return true;
}

void word_splitter::dgb_show( const char * name )
{
	if(!name) name = "ARGV";
// 	gdb_invoke(true);
	buffer2 buf;
	int n = N();
	if(n)
		buf.print("%s", name); 
	else
		buf.print("%s[N==%d]", name, n); 
	u8 sep='(';
	for(int i=0; i<n; i++ ) {
		if(sep) buf.put_byte(sep);
		sep=',';
		str0 s = word_list[i];
		buf.print("%s", (STR0) s );
	}
	if(sep) buf.put_byte(')');
	INFO("%s", (STR0) buf );
}

bool word_splitter:: split_next_line_from( buffer3 & in_buf )
{
	clear();
	bool line_found = false;

 //	in_buf.dgb_show("PARSE", "BUF");
	// ++ move this to splitter.grab_line_from_buffer ++
	// ++ make line_splitter like word splitter ...++
	// ++ a small parsing world ++
	u8 * P0 = in_buf.circ_addr_rd();
	u8 * P2 = P0 + in_buf.circ_data_avail();
	u8 * P = P0;
	u8 * P1a = P; // BOLN
	while( P < P2 ) {
		u8 * P1b = P; // P1a..P1b..P  back a bit (or two)
		u8 c1 = *P++;
		// INFO("BYTE %c", c1 );

		switch( c1 ) {
		 case ASCII_NUL:
			FAIL("Not supporting NUL in proto");
			parse_error = true;
			// delete input text
			in_buf.circ_clear();
			return false;
		 case ASCII_CR:
			if(*P!=ASCII_LF) {
				FAIL("Not supporting lone CR in proto (when not CRLF)");
				parse_error = true;
				// delete input text
				in_buf.circ_clear();
				return false;
			}
			P++;	// over LF
			break; // stay after switch for actions
		 case ASCII_LF:
			WARN("lone LF not in CRLF");
			break; // stay after switch for actions
		 default:
			continue;
			// do not stay after switch, back to while loop
		}
		{
			// need P ready to be future P1a BOLN
			*P1b = NUL;
			int len = P1b - P1a;
			split_by_spaces_and_quotes( P1a, len );
#warning above split_by_... could actually fail - it should be a bool
			line_found = true;
			// splitter.split_by_spaces_and_quotes( P1a, len );
			// splitter.split_by_spaces( P1a, len );
			// PROCESS THIS LINE // dgb_show(debug_get_obj_name());
			P1a = P; // next line
		}
	 break; // one line only
	}
	int N = P1a - P0;
	if(N) {
		in_buf.circ_used( N );
	//	in_buf.circ_check(); // want this done by surrounding libr
	} else {
	//	WARN("NO PROGRESS"); // part line OK
	}
 //	in_buf.dgb_show("PARSE-RESIDUAL", "BUF");
	return line_found;
	return true;
}

bool word_splitter:: scan_close_brace_line( buffer3 &in_buf )
{
	//
	// DELETE ME
	//
	u8 * P0 = in_buf.circ_addr_rd();
	u8 * P2 = P0 + in_buf.circ_data_avail();
	u8 * P = P0;
	while( P < P2 ) {
		// u8 * P1b = P; // CR or LF on line becomes NUL
		u8 c1 = *P++;
		if( c1 == ASCII_CR) return false;
		if( c1 == ASCII_LF) return false;
		if( c1 <= ASCII_SP) continue; // skip leading indent
		if( c1 != ASCII_BRACE_RIGHT ) {
			// line doesnt match, but probably not error
			// do not advance callers pointers
			return false;
		}
		break;
	}
	while( P < P2 ) {
		// u8 * P1b = P; // CR or LF on line becomes NUL
		u8 c1 = *P++;
		switch (c1) {
		 case ASCII_NUL: 
			// delete input text
			in_buf.circ_clear();
			return FAIL("NUL not supported in proto");
		 case ASCII_TAB:
			continue;
		 case ASCII_SP:
			continue;
		 case ASCII_CR:
			continue;
		 case ASCII_LF: {
			int nn = P - P0;
			in_buf.circ_used( nn );
			STEP("MATCH");
			return TRUE;
		 }
		 default:
			break;
		}
		return false;
	}
	return false;
}

bool word_splitter:: scan_text_line_from_buffer_to_buffer(  buffer3 &in_buf, blk1 & line )
{
	u8 * P0 = in_buf.circ_addr_rd();
	u8 * P2 = P0 + in_buf.circ_data_avail();
	u8 * P = P0;
	u8 * P1a = P; // BOLN
	while( P < P2 ) {
		u8 * P1b = P; // CR or LF on line becomes NUL
		u8 c1 = *P++;
		// INFO("BYTE %c", c1 );

		switch( c1 ) {
		 case ASCII_NUL:
			FAIL("Not supporting NUL in proto");
			parse_error = true;
			in_buf.circ_clear(); // BRUTAL!
			return false;
		 case ASCII_CR:
			if(*P!=ASCII_LF) {
				FAIL("Not supporting lone CR in proto (when not CRLF)");
				parse_error = true;
				in_buf.circ_clear();
				return false;
			}
			P++;	// over LF
			break; // stay after switch for actions
		 case ASCII_LF:
			WARN("lone LF not in CRLF");
			break; // stay after switch for actions
		 default:
			continue;
			// do not stay after switch, back to while loop
		}
		{
			// need P ready to be future P1a BOLN
			*P1b = NUL;
//			int len = P1b - P1a;
//
//			split_by_spaces_and_quotes( P1a, len );
//
			P1a = P; // next line
		}
	 break; // one line only
	}
	
	int N = P1a - P0;
	line.put_nn_bytes( N, P0 ); // copy to output (caller must add CRLF)
	if(N) {
		in_buf.circ_used( N );
	//	in_buf.circ_check(); // want this done by surrounding libr
	} else {
	//	WARN("NO PROGRESS"); // part line OK
	}
 //	in_buf.dgb_show("PARSE-RESIDUAL", "BUF");
	return true;
}

bool word_splitter:: scan_base64_line_from_buffer_to_blob(  buffer3 &in, blk1 & blob )
{
	buffer2 line;
	if(!scan_text_line_from_buffer_to_buffer(  in, line )) // no CRLF
		return false;
	blk_base64 conv;
	if(!conv.decode( line, blob ))
		return FAIL("blk_base64::decode()");
	return true;
}

/*!
	Parse a single (multiline) line, into word_list[], and return TRUE.

	Return FALSE to indicate a part-line, { unbalenced 'quotes

	TODO: move word_space to be reused
	NOTE: cannot be NUL terminated must be CRLF then NUL
*/
bool word_splitter::split_by_spaces_and_quotes_multi_line( buffer3 & buf )
{
 // gdb_invoke();
	buf.trailing_nul(); // copy_down now preserves this ...
	int len = buf.circ_data_avail();
	const uchar * P0 = buf.circ_addr_rd();
	const uchar * p1 = P0;

	/*
		currently the words are copied to another "word_space" area
		but the original area is already writable
		The copied words do not have individually malloced memory
		Each word has word_offs[idx] where it starts
	*/
	clear();
	dyn_array<int> word_offs;
	word_offs.rezero();
	// const uchar NUL = 0;
	word_space.get_space( len + 30 );
	int offs = 0;

	bool in_line = true;
	bool q1 = false;
	bool q2 = false;
	int curlies_depth = 0; // curlies

	while( *p1 && in_line )
	{
		// skip spaces
		while( (*p1==SP) || (*p1==TAB) ) p1++;
		
		// end of line ?
		if(*p1 == ASCII_NUL ) break;
		if(*p1 == ASCII_CR ) break;
		if(*p1 == ASCII_LF ) break;

		offs = word_space.gap_offs();

		bool inword = true;

		while(inword)
		{
		 switch(*p1) {
		  case NUL:
			inword=false;
			in_line=false;
			WARN("NUL found as terminator");
			if( q1 ) return WARN("Unclosed single quote");
			if( q2 ) return WARN("Unclosed double quote");
			if( curlies_depth ) return WARN("Unmatched curly brace");
			return false;
			break; // impossible here
		  case ASCII_BRACE_LEFT:
			if( curlies_depth || q1 || q2 ) {
				word_space.put( *p1++ );
			} else {
				p1++;
				if( remove_newlines_added_with_curlies ) {
					if(*p1 == ASCII_CR ) p1++;
					if(*p1 == ASCII_LF ) p1++;
				}
				// TODO the tail end ...
				// but this saves a copydown
				// TODO the same for quoted blocks
			}
			curlies_depth++; // impossible to quote these, use CHR
			break;
		  case ASCII_BRACE_RIGHT:
			curlies_depth--;
			if( curlies_depth || q1 || q2 ) {
				word_space.put( *p1++ );
			} else {
				p1++;
			}
			break;
			// all of these use the following space NUL or CR
			// to end the word
		  case ASCII_QUOTE_SINGLE:
			if( q2 || curlies_depth ) {
				word_space.put( *p1++ );
				break;
			}
			if(q1)	// treat as close quote - but stay in word!
			{
				q1 = false;
			}
			else	// treat as open quote - start/stay in word
			{
				q1 = true;
			}
			p1++;
			break;
		  case ASCII_QUOTE_DOUBLE:
			if( q1 || curlies_depth ) {
				word_space.put( *p1++ );
				break;
			}
			if(q2)	// treat as close quote - but stay in word!
			{
				q2 = false;
			}
			else	// treat as open quote - start/stay in word
			{
				q2 = true;
			}
			p1++;
			break;
		  case ASCII_BACKSLASH:
			switch(p1[1]) {
			 case ASCII_NUL: {
				WARN("BACKSLASH NUL - chunk boundry?");
				return false;
			 }
			 default:
				p1++; 
				word_space.put( *p1++ );
			}
			break;
		  case CR:
		  case ASCII_LF:
		  case ASCII_SP:
		  case ASCII_TAB:
		  default:
			if(*p1 <= ASCII_SP)
			{
				if( q1 || q2 || curlies_depth )
				{
					// put spaces into word
					// put LF into word
					// do not end line
					if( *p1 == CR ) {
						// drop all CR 
						// (simple convertion)
						p1++;
					} else
						word_space.put( *p1++ );
					break;
				} else {
					// spaces break words
					inword = false;
					switch(*p1) {
					 case ASCII_CR:
					 case ASCII_LF:
					  in_line = false;
					  if(p1[1]==ASCII_LF) p1++; // both of them
					}
					p1++;
					break;
				}
			} else
				// most plain chars land here
				word_space.put( *p1++ );
		 }
		} // while inword
		// close word and add to output list
		word_space.put( NUL );
		int n = word_offs.N();
		word_offs.extend_by(1);
		word_offs[n] = offs ;
	} // while *p1

	// do not advance buffer - maybe more data will solve the problem
	if( q1 ) return FAIL("Unclosed single quote");
	if( q2 ) return FAIL("Unclosed double quote");
	if( curlies_depth ) return FAIL("Unmatched curly brace");

	// BUG
	// BUG // broken fragment will succeed without seeing EOLN
	// BUG

	if(!word_offs.N()) return WARN("blank line not skipped, no words");
	// might be WARN OK

	// committed to returning true
	buf.circ_used( p1 - P0 );

	// convert offsets to str0 pointers
	int n = word_offs.N();
	word_list.extend_to(n);
	for( int i=0; i<n; i++ )
	{
		word_list[i] = (str0)(STR0)( word_space.buff + word_offs[i] );
//		INFO("arg[%d] = %s", i, (STR0) word_list[i] );
	}
 if(0)	buf.dgb_show("SPLITTER","BUF");
	return true;
}
