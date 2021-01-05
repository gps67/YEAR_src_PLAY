
#ifndef yahoo_history_parse_H
#define yahoo_history_parse_H

#include "io_csv_in.h"
#include "date_glib.h"


/*!
	D_V holds a single point (date,value)
	where date is different from a time,
	but can span a wider range

	This introduces a cloned layer like T_V
*/
struct D_V : public GRP_lib_inet
{
	i32	D;	// DATE as julian
	float	V;

	D_V()
	{
		D = 0;
		V = 0;
	}

	D_V( i32 d, float v )
	{
		D = d;
		V = v;
	}
};

// STILL PROBS WITH .append //
/*
	typedef dyn_array<D_V> D_V_array;
../src/lib_base/buffer/dyn_array.h: In member function `bool 
   dyn_array<T>::append(const T&) [with T = D_V]':
../src/lib_inet/fd_http/yahoo_history_parse.cxx:68:   instantiated from here
../src/lib_base/buffer/dyn_array.h:209: error: no match for `* dyn_array<D_V>&' 
   operator
make: *** [yahoo_history_parse.o] Error 1
*/

class D_V_array : public dyn_array<D_V> 
{
 public:
	bool adds( i32 d, float v )
	{
		int i = N();
		if(!extend_by(1)) return false;
		D_V & item = ary_base()[i];
		item.D = d;
		item.V = v;
		return true;
	}
};

/*!
	yahoo_history_parse parses a stock ticker history CSV from yahoo

Date,Open,High,Low,Close,Volume                                                 
18-Dec-02,1833.80,1833.80,1822.40,1822.50,0                                     
17-Dec-02,1844.90,1845.40,1836.60,1836.60,0                                     
16-Dec-02,1849.40,1849.40,1843.40,1845.20,0                                     
13-Dec-02,1871.90,1871.90,1851.60,1852.00,0                                     

*/
class yahoo_history_parse_base : public io_csv_in
{
 public:

	str1 tag;

	bool line_1_seen;	// headers
	bool line_2_seen;	// first date is lo/hi without compare

// parser sets the following

	date_glib date_current;

	u32	D_lo;
	u32	D_hi;
	u32	D_curr;

	double f_Open;
	double f_High;
	double f_Low;
	double f_Close;
	double f_Volume;

	yahoo_history_parse_base();
	~yahoo_history_parse_base();

//
	bool got_line_1();
	//! callback
	virtual void got_row() = 0;
//
//	EOF ??
};


class yahoo_history_parse : public yahoo_history_parse_base
{
 public:
// got row build columns, and prints

	D_V_array V_Open;
	D_V_array V_High;
	D_V_array V_Low;
	D_V_array V_Close;
	D_V_array V_Volume;

	FILE * kept_data;

	int parse( str0 tag, str0 file );
	void prints();
	virtual void got_row();
};

extern int run_YH_parse( str0 tag, str0 file );
int run_YH_parse( str0 tag, str0 file )
{
        yahoo_history_parse parser;
        return parser.parse( tag, file );
}

#endif
