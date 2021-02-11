#ifndef MM3_FILE_DATA
#define MM3_FILE_DATA

#define MAX_CAT 100
#define MAX_ACCT 20
#define MAX_NOTE 10
#define MAX_INFO 10
#define MAX_VAT  10
#define MAX_ERA  20
#define MAX_TRANS  945

#include <stdio.h> // fprintf

#include "scan_nl.h"	// getc buffer with lex matching
#include "map_file.h"	// mmaps in file (readonly)
#include "own_str.h"	// own_str

#define m3_static /* noop or static */ // static just isn't happening

extern int trans_id;

struct a_trans {
//
	int trans_id;
	int year;
	int mm;
	int dd;
	int year2;
	int mm2;
	int dd2;
	own_str cat;
	u8 acct;
	u8 acct2;
	u8 mark0;
	u8 mark;
	own_str ref,desc,amnt,vat2;

// contructor must reset acct2 = '-'
	a_trans( void ) {
		reset();
	}
//
// Copy constructor - should be non pointer ?
	void set_from( a_trans & t1 );
//
	void reset(void);
};

struct trans_list {
	const static int tl_max = MAX_TRANS;
	int N;
	a_trans * list[tl_max];

	trans_list() { N = 0 ; }
	~trans_list() {
		N = 0;
		// ignore lost bits !
	}
	void append( a_trans * t ) {
		if( N >= tl_max ) {
			fprintf(stderr, "trans_list limit %d\n", N );
			exit(27);
		}
		a_trans * t2 = new a_trans();
		t2->set_from( *t );
		list[ N++ ] = t2;
	}
};

struct a_cat
{
	own_str	key;
	own_str	path;
	own_str desc;
	a_cat( const char * _key, const char * _path, const char * _desc )
	{
		key = _key;
		path = _path;
		desc = _desc;
	}
	a_cat( own_str & _key, const char * _path, own_str & _desc )
	{
		key = _key;
		path = _path;
		desc = _desc;
	}
};

struct a_era
{
	own_str	key;
	own_str desc;
	own_str path;
	// own_str start;
	// own_str end;
	a_era(
		own_str & _key,
		own_str & _desc,
		own_str & _path // ,
		// own_str & _start, // unused
		// own_str & _end
	)
	{
		key = _key;
		desc = _desc;
		path = _path;
		// start = _start;
		// end = _end;
	}
};

struct a_acct
{
	own_str	key;
	own_str desc;
	own_str amnt;
	a_acct( own_str & _key, own_str & _desc, own_str & _amnt )
	{
		key = _key;
		desc = _desc;
		amnt = _amnt;
	}
};

struct a_note
{
	own_str	key;
	own_str desc;
	a_note( own_str & _key, own_str & _desc )
	{
		key = _key;
		desc = _desc;
	}
};

struct a_vat
{
	own_str	key;
	own_str desc;
	own_str amnt;
	a_vat( own_str & _key, own_str & _desc, own_str & _amnt )
	{
		key = _key;
		desc = _desc;
		amnt = _amnt;
	}
};

struct a_info
{
	own_str	key;
	own_str desc;
	a_info( own_str & _key, own_str & _desc )
	{
		key = _key;
		desc = _desc;
	}
};

class cat_list
{ public:
	int n_cat;
	a_cat * list[MAX_CAT];

	cat_list( void );
	bool add_cat( own_str & abb, own_str & d );
	bool lookup_key( own_str & k, int & pos );
	char * key_to_desc( own_str & k );
};

class vat_list
{ public:
	int n_vat;
	a_vat * list[MAX_VAT];

	vat_list( void ) {
		n_vat = 0;
	}
	bool add_vat( own_str & k, own_str & d , own_str & v ) {
		if( n_vat >= MAX_VAT ) return FALSE;
		list[ n_vat++] = new a_vat( k, d, v  );
		return TRUE; // presume allocs worked
	}
};

class era_list
{ public:
	int n_era;
	a_era * list[MAX_ERA];

	era_list( void ) {
		n_era = 0;
	}
	bool add_era( const char * _k, const char * _d , const char * _v ) {
		own_str k; k = _k;
		own_str d; d = _d;
		own_str v; v = _v;
		if( n_era >= MAX_ERA ) return FALSE;
		list[ n_era++] = new a_era( k, d, v  );
		return TRUE; // presume allocs worked
	}
};

class acct_list
{ public:
	int n_acct;
	a_acct * list[MAX_ACCT];

	acct_list( void ) {
		n_acct = 0;
	}
	bool add_acct( own_str & k, own_str & d , own_str & v ) {
		if( n_acct >= MAX_ACCT ) return FALSE;
		list[ n_acct++] = new a_acct( k, d, v  );
		return TRUE; // presume allocs worked
	}
};

class note_list
{ public:
	int n_note;
	a_note * list[MAX_NOTE];

	note_list( void ) {
		n_note = 0;
	}
	bool add_note( own_str & k, own_str & d ) {
		if( n_note >= MAX_NOTE ) return FALSE;
		list[ n_note++] = new a_note( k, d  );
		return TRUE; // presume allocs worked
	}
};

class info_list
{ public:
	int n_info;
	a_info * list[MAX_INFO];

	info_list( void ) {
		n_info = 0;
	}
	bool add_info( own_str & k, own_str & d ) {
		if( n_info >= MAX_INFO ) return FALSE;
		list[ n_info++] = new a_info( k, d  );
		return TRUE; // presume allocs worked
	}
	bool add_info( const char * _k, const char * _d ) {
		own_str k; k = _k;
		own_str d; d = _d;
		return add_info( k, d );
	}
	bool add_info( const char * _k, own_str & d ) {
		own_str k; k = _k;
		return add_info( k, d );
	}
};

class mm3_file_data
{
 public:
	int year_start;
	own_str title;
	cat_list cats;
	era_list eras;
	acct_list accts;
	info_list infos;
	note_list notes;
	vat_list vats;;
	trans_list trans;
	trans_list xfers;

	void fixup_merge_xfer( a_trans & xfer );
};

#define SPL_cat      "CAT"
#define SPL_acct     "ACCT"
#define SPL_note     "NOTE"
#define SPL_info     "INFO"
#define SPL_vat      "VAT"
#define SPL_xfer     "XFER"
#define SPL_trans    "TRANS"

#ifdef WITH_MARKS
#define HDR_MARKS "mark1 mark2 "
#else
#define HDR_MARKS 
#endif
extern void print_sp_str(int fmtw, own_str & str);

#endif

