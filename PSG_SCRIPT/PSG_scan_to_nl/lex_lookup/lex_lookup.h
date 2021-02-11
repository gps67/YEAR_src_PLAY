#ifndef lex_lookup_H
#define lex_lookup_H

/*
 * caller uses this class to lookup/add items to one/few HT's
 * It can obtain a soft lock to the table.
 * It can create new keys (proper str2's) from buffers
 * It can even do some utf8 convertion and scan_lex things
*/

#include "HT_table.h"
#include "p0p2.h"
#include "stbl_spelling.h"
#include "buffer2.h"

/*
	CSET should be set PER string + buffer

	an object should KNOW its own cset (poss via variable)

	a parameter should be of SPECIFIED type,
	eg puts_utf8( const char * str_utf8) 
	eg puts_8859( const char * str_8859_9 )

	the caller must specify the cset of its parameter (as above)
	unless that info is known at compile time, or VTBL of well known mix

	I would also like to have BTF8 which is BYTE_utf8 but no convertion
	BYTE<-->UNICODE except u8<-->u32, ie EURO, POUND, etc are not converted

	A pointer to a cset OBJECT would allow a range of operations, queries,
	tables, etc.

	The advantage of using utf8, is that one size force-fits all.
	The disadvantage is when the output cant handle >8 bits
	BUT: already have fonts with missing glyphs, + extra used, so ...

	The advantage of using BYTE, is that a table is a decent size,
	and every read is a single action,
	and can use a few other values (box lines) with care
	and MIGHT be able to 
*/
enum e_cset {
	e_cset_8859,	// -9 is default ?
	e_cset_utf8,
	e_cset_BYTE,	// other 8859 variations, iso646, ... must specify
};

class lex_lookup
{
 public:
	 lex_lookup() {
		 clr_HT();
	 }
	 ~lex_lookup() {
		 /* default */
	 }
	 bool			str_is_utf8;
	 bool			str_is_8859;
	 bool			str_is_BYTE;
	 obj_hold<str2>		str_spelling;
	 p0p2			str_p0p2;
	 buffer1		str_buffer;
	 HT_table_base *	sym_table1;
	 obj_ref *		val_found;
	 obj_hold<obj_ref>	key_found;
	 obj_ref *		key_val_pair_found;

	 void clr_HT()
	 {
		 set_HT( NULL );
	 }
	 void set_HT( HT_table_base * tbl )
	 {
		 sym_table1 = tbl;
		 str_spelling = NULL;
		 str_p0p2.clear();
		 str_buffer.clear();
		 val_found = NULL;
		 key_found = NULL;
		 key_val_pair_found = NULL;
	 }
	 bool look_utf8( const p0p2 & s )
	 {
#warning "set(utf8) will double encode - must rewrite as set_plain()"
		 str_buffer.set( s ); // UTF8
		 return look_buffer();
	 }
	 bool look_8859( const p0p2 & s )
	 {
		 str_buffer.set_8859( s ); // UTF8
	 }
	 bool look_buffer()
	 {
		 str_spelling = STBL::Spelling_Pool_lookup_add( str_buffer );
	 }

	 void set_utf8( p0p2 & s );
//	 bool lookup( p0p2 
};
#endif


