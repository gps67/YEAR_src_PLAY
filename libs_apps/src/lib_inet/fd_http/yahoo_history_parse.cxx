/*
*/
#include "yahoo_history_parse.h"
#include "e_print.h"
#include "dgb_fail.h"
#include "buffer2.h"

yahoo_history_parse_base::yahoo_history_parse_base()
{
	line_1_seen = false;
}

yahoo_history_parse_base::~yahoo_history_parse_base()
{
}


bool yahoo_history_parse_base::got_line_1()
{
/*
Date,Open,High,Low,Close,Volume                                                 
18-Dec-02,1833.80,1833.80,1822.40,1822.50,0                                     
17-Dec-02,1844.90,1845.40,1836.60,1836.60,0                                     
*/
	if( nfield != 6)
	{
		e_print("yahoo_history_parse::got_line_1() - expected 'Date,Open,High,Low,Close,Volume'\n");
		if(1) print_fields();                                                 
		return false;
	}
	if(!line_1_seen)
	{
		if(0) print_fields();                                                 
		line_1_seen = true;
		return true;
	}

	if( !  date_current.set_dd_Jan_yy( fields[0] ) )
	{
		e_print("yahoo_history_parse::got_line_1() - DATE CONV failed\n");
		print_fields();
		return false;
	}

	str0 s;
	s = fields[1];
	if(! s.as_double( f_Open ) ) return FALSE_dgb_fail("float_conv");
	s = fields[2];
	if(! s.as_double( f_High ) ) return FALSE_dgb_fail("float_conv");
	s = fields[3];
	if(! s.as_double( f_Low ) ) return FALSE_dgb_fail("float_conv");
	s = fields[4];
	if(! s.as_double( f_Close ) ) return FALSE_dgb_fail("float_conv");
	s = fields[5];
	if(! s.as_double( f_Volume ) ) return FALSE_dgb_fail("float_conv");                                                 

	if(0) return TRUE; // much faster without fflush(0)

	D_curr = date_current.julian();

	if(!line_2_seen)
	{
		D_lo = D_curr;
		D_hi = D_curr;
	}

	if(!( D_lo < D_curr )) D_lo = D_curr;
	if(!( D_hi > D_curr )) D_hi = D_curr;

	got_row();

	return true;
}


///////////////


void yahoo_history_parse::got_row()
{
	V_Open	.adds( D_curr, f_Open );
	V_High	.adds( D_curr, f_High );
	V_Low	.adds( D_curr, f_Low );
	V_Close	.adds( D_curr, f_Close );
	V_Volume.adds( D_curr, f_Volume );

}

void yahoo_history_parse::prints()
{
	buffer2 out( 2000 );
        str1 date_temp_str; // why must I use str1

	int n = V_Open.N();
	for(int i = 0; i<n; i++ )
	{
		date_current.set_julian( V_Open[i].D );
		date_current.get_year_mm_dd( date_temp_str );
		out.print("%s %s %d %7.2f\n",
			(STR0) tag,
			(STR0) date_temp_str,
			date_current.y2k_days(),
			V_Open[i].V
		);
	}

	e_print("%s", STR0(out) );


	// e_print( "%7.2f ", f_Open );
	// e_print( "%7.2f ", f_High );
	// e_print( "%7.2f ", f_Low );
	// e_print( "%7.2f ", f_Close );
	// e_print( "%7.2f ", f_Volume );
	// e_print( " OK\n");
}

int yahoo_history_parse::parse( str0 _tag, str0 file )
{
	line_1_seen = false; // headlines
	line_2_seen = false; // first date in range
	D_lo = 0;
	D_hi = 0;
	tag = _tag;
	load_file( file );
	prints();
	return 0;
}
