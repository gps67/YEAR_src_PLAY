#ifndef MM3_FILE_SCAN
#define MM3_FILE_SCAN
/*
	mm3_file.h	scan mm3 file outputting the data
	Graham Swallow 
*/

#include <stdio.h> // fprintf

#include "scan_nl.h"	// getc buffer with lex matching
#include "map_file.h"	// mmaps in file (readonly)
#include "own_str.h"	// own_str

#include "mm3_file_data.h"

#define m3_static /* noop or static */ // static just isn't happening

extern int trans_id;

class mm3_file_scan
{ public:
	map_file f;
	scan_to_NL S;
	p0p2 zone;
	mm3_file_data * data;

	/*
		statics can cause a lot undefined references
		unless one file takes responsibility for defining the store!
	*/
	m3_static int cset_inited; // init defined as = 0;
	m3_static cset_bit_map cset_09;
	m3_static cset_bit_map cset_az;
	m3_static cset_bit_map cset_AZ;
	m3_static cset_bit_map cset_AZaz;
	m3_static cset_bit_map cset_AZaz09;
	m3_static cset_bit_map cset_line;
	m3_static cset_bit_map cset_field;

	void init_csets(void) {
		// if( cset_inited ) return;
		cset_inited = 1;

		// could be done by init constructor!
		cset_az.set_null();
		cset_AZ.set_null();
		cset_09.set_null();
		cset_AZaz.set_null();
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

		cset_field.set_range( 0, 255 );
		cset_field.reset_bit( 0 ); // not allowed
		cset_field.reset_bit( '\n' ); // not allowed
		cset_field.reset_bit( '\r' ); // not part of line
		cset_field.reset_bit( 0373 );
		cset_field.reset_bit( 0374 );
		cset_field.reset_bit( 0375 );
	}

	mm3_file_scan(void) {
		init_csets();
	}
	bool open_file( u8 * filename );
	bool open_file( char * filename ) { return open_file( (u8 *) filename ); }
	bool scan_mm3_table( mm3_file_data * _data, u8 * filename );
	bool scan_mm3_table( mm3_file_data * _data, char * filename ) { return scan_mm3_table( _data, (u8 *) filename ); }
	bool scan_mm4_line( void );
	bool scan_line( own_str & title );
	bool scan_title( own_str & title );
	bool scan_category( own_str & abb ,own_str & desc );
	bool scan_account( own_str & abb ,own_str & desc, own_str & amount );
	bool scan_note( own_str & abb ,own_str & desc );
	bool scan_vat( own_str & abb ,own_str & desc, own_str & amount );
	bool scan_month_line( int & yy, int & mm );
	bool scan_Jan( int & mm ); // any Mon - English with exact Cap's
	void rename_cat( const char * zone, own_str & cat );
	bool scan_c5( own_str & cat );
	bool scan_a( u8 & acct );
	bool scan_dd( int & dd );
	bool scan_fld( own_str & fld );
	bool scan_transaction(int yy, int mm, a_trans & t);

 //	void print_transaction(a_trans & t);

	bool scan_word( const u8 * word);
	bool scan_word( const char * word) { return scan_word( (const u8 *) word); }
	bool scan_int( int & i);
	bool scan_crlf( void );
	void p0p2_start( p0p2 & P0 ) {  P0.p0 = P0.p2 = S.P ; }
	void p0p2_stop( p0p2 & P0 ) {  P0.p2 = S.P; }
	void here_start( u8 * & P0 ) {  P0 = S.P; }
	void here_back( u8 * & P0 ) {  S.P = P0; }
};

#endif
