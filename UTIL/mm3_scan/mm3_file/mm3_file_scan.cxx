#define NB(x)
/*
	mm3_file.c	scan mm3 file outputting the data
	Graham Swallow 
*/

#include "mm3_file_scan.h"	// includeds the following

/*
	mm3 imposes a max of 50 categories
	that simply isn't enough!
	particularly for multi-year legacy data-sets
	however a fixed limit is easier than a dyna-ray
*/
#define MAX_CAT 100

bool mm3_file_scan::open_file( u8 * filename )
{
	int t;
	init_csets();
	t=f.map_in_file( filename, FALSE );
	if(t) return FALSE;
	zone = p0p2( f.page0, f.fd_size );
	S = scan_to_NL ( zone );
	if(!S.check_nl_at_eof())
	{
		dgb.fmt("File %s does not end in NL\n",filename );
		return 0;
	}
}

// scan for a returned decimal int
bool mm3_file_scan::scan_int( int & i)
{
	p0p2 zone;
	p0p2_start( zone );
	while(S.scan_char( ' ' )) ;
	if(!S.scan_a1_plus( cset_09 )) return FALSE;
	p0p2_stop( zone );
	// somewhere s = zone causes a copy to be taken (I hate C++ here)
	own_str s( zone) ;
	// own_str s = zone;
	i = atoi( s.str );
	return TRUE;
}

// scan for a constant string
// being constant the word isn't returned
// after the word, the buffer must not contain a longer word
// using strange AZaz here not AZaz09 ?? - make it a parameter?
bool mm3_file_scan::scan_word( u8 * word)
{
	u8 * P0;
	here_start( P0 );
	while( *word && S.scan_char( *word++ ) ) { }
	if ( *word ) goto fail;
	if( S.scan_a1_plus( cset_AZaz ) ) goto fail;
	return TRUE;
fail:
	here_back( P0 );
	return FALSE;
}

// lurk - CR without LF eats the CR, fails and continues!
bool mm3_file_scan::scan_crlf( void )
{
	if(!S.scan_char( '\r' )) return FALSE;
	return S.scan_nl();
}
// lurk - does not roll back properly if MM4 follwed by anything
bool mm3_file_scan::scan_mm4_line( void )
{
	if(!scan_word( "MM4" )) return FALSE;
	if(!scan_crlf() ) return FALSE;
	return TRUE;
}
bool mm3_file_scan::scan_title( own_str & str )
{
	return scan_line( str );
}
bool mm3_file_scan::scan_line( own_str & str )
{
	p0p2 zone;
	p0p2_start( zone );
	S.scan_a1_star( cset_line ); // excludes NUL NL and CR
	p0p2_stop( zone );
	str = zone;
	str.no_comma();
	return scan_crlf();
}
bool mm3_file_scan::scan_category( own_str & abb, own_str & desc )
{
	// these could be made more exact to test for validity of abb and desc
	// two lines
	// returned to caller
	// also added to categories table (global!)
	own_str abb1;
	if(!scan_line( abb1 )) return FALSE;
	rename_cat( abb1.str, abb );
	if(!scan_line( desc )) return FALSE;
	if(!data->cats.add_cat( abb, desc )) return FALSE;
	return TRUE;
}

bool mm3_file_scan::scan_account( own_str & abb, own_str & desc, own_str & amount )
{
	// three lines
	// not much validation here
	if(!scan_line( abb )) return FALSE;
	if(!scan_line( desc )) return FALSE;
	if(!scan_line( amount )) return FALSE;
	if(!data->accts.add_acct( abb, desc, amount )) return FALSE;
	return TRUE;
}

bool mm3_file_scan::scan_vat( own_str & abb, own_str & desc, own_str & amount )
{
	// three lines
	// not much validation here
	if(!scan_line( abb )) return FALSE;
	if(!scan_line( desc )) return FALSE;
	if(!scan_line( amount )) return FALSE;
	if(!data->vats.add_vat( abb, desc, amount )) return FALSE;
	return TRUE;
}

bool mm3_file_scan::scan_note( own_str & abb ,own_str & desc )
{
	if(!scan_line( abb )) return FALSE;
	if(!scan_line( desc )) return FALSE;
	if(!data->notes.add_note( abb, desc )) return FALSE;
	return TRUE;
}

bool mm3_file_scan::scan_Jan( int & mm )
{
	if(scan_word("Jan")) { mm = 1 ; return TRUE; } else
	if(scan_word("Feb")) { mm = 2 ; return TRUE; } else
	if(scan_word("Mar")) { mm = 3 ; return TRUE; } else
	if(scan_word("Apr")) { mm = 4 ; return TRUE; } else
	if(scan_word("May")) { mm = 5 ; return TRUE; } else
	if(scan_word("Jun")) { mm = 6 ; return TRUE; } else
	if(scan_word("Jul")) { mm = 7 ; return TRUE; } else
	if(scan_word("Aug")) { mm = 8 ; return TRUE; } else
	if(scan_word("Sep")) { mm = 9 ; return TRUE; } else
	if(scan_word("Oct")) { mm = 10 ; return TRUE; } else
	if(scan_word("Nov")) { mm = 11 ; return TRUE; } else
	if(scan_word("Dec")) { mm = 12 ; return TRUE; } else
	return FALSE;
}

bool mm3_file_scan::scan_month_line( int & year, int & mm )
{
	int yy = 0;
	if(!scan_Jan( mm )) return FALSE;
	if(!S.scan_char('-')) return FALSE;
	if(!scan_int(yy)) return FALSE;
	if(!scan_crlf()) return FALSE;
	year = 1900 + yy;
	if( yy < 80 ) year += 100;
	return TRUE;
}

void mm3_file_scan::rename_cat( const char * zone, own_str & cat )
{
	char name[3] = { 0, 0, 0 };
	char c0 = zone[0];
	char c1 = zone[1];
	char NIXOT = '_';
	switch( c0 ) {
		// i4 becomes I4
		 case 'd': 
		 case 'n': 
		 case 'i': 
		 case 'x': 
		 case 'o': 
		 case 't': 
			 NIXOT = 'A' - 'a' + c0;
			name[0] = NIXOT;
		break;
		 default:
			name[0] = c0;
	}
	// convert to upper anyway
	if( ('a'<=c0)&&(c0<='z') ) name[0] = 'A' - 'a' + c0;
	if( c1 == '0' ) 
	{
	}  else {
		name[1] = c1;
	}
	cat = (char *) name;
}


bool mm3_file_scan::scan_c5( own_str & cat )
{
	p0p2 zone;
	p0p2_start( zone );

	if(!S.scan_a1( cset_az )) return FALSE;
	if(!S.scan_a1( cset_09 )) goto fail;	// !! should roll back

	p0p2_stop( zone );
	rename_cat( (char *)zone.p0, cat );
	// cat = zone;		// ! should be copy or a token
	return TRUE;
fail:
	here_back( zone.p0 );
	return FALSE;
}
bool mm3_file_scan::scan_a( u8 & acct )
{
	u8 c1 = *S.P;
	if(S.scan_a1( cset_az )) {
		acct = c1;
		return TRUE;
	} else if(S.scan_char(' ')) {
		acct = '-'; // fixup
		return TRUE;
	} else
		return FALSE;
}
bool mm3_file_scan::scan_dd( int & dd )
{
	cset_bit_map a3;
	a3.set_null();
	a3.set_range( '0', '9' );
	a3.set_bit( ' ' );

	u8 * P0 = S.P;
	u8 c1 = P0[0];
	u8 c2 = P0[1];
	if(S.scan_char( ' ' )) {
		c1 = '0';
	} else if(S.scan_a1( cset_09 )) {
		// c1 = c1;
	} else return FALSE;
	if(!S.scan_a1( cset_09 )) {
		goto fail;
	}
	dd = (c1 - '0') * 10 + (c2 - '0');
	return TRUE;
fail:
	S.P = P0;
	return FALSE;
}

bool mm3_file_scan::scan_fld( own_str & fld )
{
	u8 * P0 = S.P;
	S.scan_a1_star( cset_field );
	p0p2 zone ( P0, S.P );
	fld = zone;
	fld.no_comma();
	return TRUE;
}

// added sp=>'-' to a, so vat and mark become '-' if was ' '
#define scan_v( X ) scan_a( X )
#define scan_m( X ) scan_a( X )

/*
	transactions are numbered from 1
	trans_id is the number of trans's seen
*/
int trans_id = 0;
/*
	scan and print one transaction line
	(because it's easier than defining a storage structure)
*/
bool mm3_file_scan::scan_transaction( int year, int mm, a_trans & t )
{
	if(*S.P == '\r') return FALSE; // ! blank line terminates list
	u8 * P0 = S.P;

	own_str line;
	t.year = year;
	t.mm = mm;
	if(!scan_c5( t.cat )) goto fail;
	if(!scan_a( t.acct )) goto fail;
	if(!scan_dd( t.dd )) goto fail;
	if(!scan_v( t.mark0 )) goto fail;
	if(!scan_m( t.mark )) goto fail;
	if(!scan_fld( t.ref )) goto fail;
	if(!S.scan_char( 0373 )) goto fail;
	if(!scan_fld( t.desc )) goto fail;
	if(!S.scan_char( 0374 )) goto fail;
	if(!scan_fld( t.amnt )) goto fail;
	if(!S.scan_char( 0375 )) goto fail;
	if(!scan_fld( t.vat2 )) goto fail;
	if(!scan_crlf()) goto fail;
	t.year2 = t.year;
	t.mm2   = t.mm;
	t.dd2   = t.dd;

	if(0==t.desc.str_len())
	{
		t.desc=data->cats.key_to_desc( t.cat );
	}

	t.trans_id = trans_id++;
	t.trans_id *= 16;
	return TRUE;
fail:
	own_str did, left;
	p0p2 zone (P0,S.P);
	did = zone;
	printf("DID {%s}\n",did.str);
	scan_line( did );
	printf("NOT {%s}\n",did.str);
	S.P = P0;
	return FALSE;
}


bool mm3_file_scan::scan_mm3_table( mm3_file_data * _data, u8 * filename )
{
	data = _data;
	int n_cat = 0;
	int n_acct = 0;
	int n_note = 0;
	int n_vat = 0;

	// this program is a bit quiet if it doesn't work!
	if(!open_file( filename )) return FALSE;
	if(!scan_mm4_line()) return FALSE;

	if(!scan_title( data->title )) return FALSE;
	// data->infos.add_info( "title", data->title );

	if(!scan_int( n_cat )) return FALSE;
	if(!scan_crlf()) return FALSE;

	own_str abb, desc, amount;
	for ( int i = 0; i<n_cat; i++)
	{
		if(!scan_category( abb, desc )) return FALSE;
	}

	if(!scan_int( n_acct )) return FALSE;
	if(!scan_crlf()) return FALSE;
	for ( int i = 0; i<n_acct; i++)
	{
		if(!scan_account( abb, desc, amount )) return FALSE;
	}

	if(!scan_int( n_note )) return FALSE;
	if(!scan_crlf()) return FALSE;
	for ( int i = 0; i<n_note; i++)
	{
		if(!scan_note( abb, desc )) return FALSE;
	}

	if(!scan_int( n_vat )) return FALSE;
	if(!scan_crlf()) return FALSE;
//	printf("\ndata_table %s {\n", SPL_vat);
	for ( int i = 0; i<n_vat; i++)
	{
		if(!scan_vat( abb, desc, amount )) return FALSE;
	}

// forgotten - is this a blank line to mark the end of the standard entries?
	scan_line( desc ); NB(desc.str);
// AARRGGHH
// standard entries syntax unknown 
// too lazy to guess, or add some and debug
// ditto for macros

int year=0;
int mm=0;
int qpos=0;
int q_no = 0;

	q_no = 0; // start in Q0
/*
	q_no = -1; //start with Q0 not Q1
	q_no = 0; // start in Q0
	qpos = 2; // 3-2= 1 months to go
	{
		// repeated code
		char QN[3+10];
		sprintf( QN, "Q%d", q_no );
		data->eras.add_era( QN, "Quarter", "-" ); // key,desc,path
	}
*/

	char * MM_Names[1+12+0] = {
		"Zero",
		"Jan", "Feb", "Mar",
		"Apr", "May", "Jun",
		"Jul", "Aug", "Sep",
		"Oct", "Nov", "Dec",
	};

	// trans_list xfers;

	for( int i=0; i<12; i++ )
	{
		if(scan_crlf()) NB("Skipping blank_line\n");
		if(!scan_month_line( year, mm )) return FALSE;

		char QN[7+10];
		char YEAR_MM[7+10];
		// sprintf( QN, "%4.4d_Q%d", year, q_no );
		sprintf( YEAR_MM, "%4.4d-%2.2d", year,mm );
		qpos--;
		if( qpos<=0 )
		{
		 q_no ++;
		 int m_lo = mm;
		 int m_hi = mm + 3;
		 if( year <= 2001 )
		 {
			switch( mm )
			{
			        case 0:
			 break; case 2:
					m_lo=0; q_no--;
			 break; case 3:
			        case 4:
			        case 5:
					m_lo=3;
			 break; case 6:
			        case 7: 
			        case 8:
					m_lo=6;
			 break; case 9:
			        case 10:
			        case 11:
					m_lo=9;
			 break; case 12:
			        case 1: // thankfully this never lands
					m_lo=12;
			 ;
			}
			m_hi = m_lo + 3;
		 } else if( year == 2002 )
		 {
			switch( mm )
			{
			 case 2:
				m_lo = 0;
				m_hi = 3;
				q_no--; // zero not one
			 break;
			 case 3:
				m_hi = m_lo + 2;
			}
		 } else {
			qpos=3;
		 }
		 qpos = m_hi - mm;
		 sprintf( QN, "%4.4d_Q%d", year, q_no );
		 char buff[88];
		 char * p = buff;
		 buff[0] = 0;
		 sprintf( buff, "VAT Q%d ", q_no );
		 const char * sep = "";
		 for( int m=m_lo; m<m_hi; m++ )
		 {
			int m2 = m;
			if( m2 < 1 ) m2 += 12;
			if( m2 >12 ) m2 -= 12;
			p = buff + strlen(buff);
			sprintf( p, "%s%s", sep, MM_Names[m2] );
			sep = "&comma; ";
			sep = " ";
		 }
		 p = buff + strlen(buff);
		 // sprintf( p, "%s", " )" );
		 data->eras.add_era( QN, buff, "year" );
		}
		data->eras.add_era( YEAR_MM, MM_Names[mm], QN );

		if(!i)
		{
			/*
				year start MUST be first of month
			*/
			char YEAR_MM_DD[10+1+10];
			sprintf( YEAR_MM_DD, "%s-01", YEAR_MM );
			data->infos.add_info( "year_start", YEAR_MM_DD );
			data->year_start = year;
		}

		a_trans t;
		while( scan_transaction(year,mm,t) ) {
		  if( t.cat.str[0] == 'X' ) {
			a_trans t2;
			if(!scan_transaction(year,mm,t2) ) {
				data->trans.append(&t);
				printf("ERROR: second trans failed!\n"); 
				return FALSE;
			}
			if(0!=strcmp(t.cat.str, t2.cat.str) ) {
				data->trans.append(&t);
				data->trans.append(&t2);
				printf("ERROR: cat MIS MATCH!\n"); 
				return FALSE;
			} 
			// second trans_id gets un-done
			trans_id--;
			// debug // print_transaction(t) ; // debug
			// printf("# XFER:\n"); 
			// lurk - this is confusing - check it!
			if( t.amnt.str[0] == '-' ) {
				t.amnt = t2.amnt; // same but not -
				// t.acct = t.acct // out of first acct
				t.acct2 = t2.acct; // into second acct
			} else {
				// t.amnt = t.amnt
				t.acct2 = t.acct; // + into acct2
				t.acct = t2.acct; // - from acct
			}
			data->fixup_merge_xfer( t );
			// data->xfers.append( &t );
			// print_transfer(t) ;
			// debug // print_transaction(t2) ; // for debug
			
		  } else {
			// print_transaction_tbl(t) ;
			data->trans.append(&t);
		  }
		  t.reset();
		}
	}

	if(!S.scan_eof())
	{
		NB("Skipping MACROS from here to eof\n");
		return TRUE;
		// that actually means that I don't scan for the EOF
		// I previously tested for EOF detection
		// but then I detached this code from the library
	}

	return TRUE;
}
