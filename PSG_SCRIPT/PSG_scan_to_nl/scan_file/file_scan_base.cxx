#include "file_scan_base.h"	// includeds the following

file_scan_base::file_scan_base()
{
}

bool file_scan_base::scan_Jan( int & mm )
{
	if(LEX.scan_word("Jan")) { mm = 1 ; return TRUE; } else
	if(LEX.scan_word("Feb")) { mm = 2 ; return TRUE; } else
	if(LEX.scan_word("Mar")) { mm = 3 ; return TRUE; } else
	if(LEX.scan_word("Apr")) { mm = 4 ; return TRUE; } else
	if(LEX.scan_word("May")) { mm = 5 ; return TRUE; } else
	if(LEX.scan_word("Jun")) { mm = 6 ; return TRUE; } else
	if(LEX.scan_word("Jul")) { mm = 7 ; return TRUE; } else
	if(LEX.scan_word("Aug")) { mm = 8 ; return TRUE; } else
	if(LEX.scan_word("Sep")) { mm = 9 ; return TRUE; } else
	if(LEX.scan_word("Oct")) { mm = 10 ; return TRUE; } else
	if(LEX.scan_word("Nov")) { mm = 11 ; return TRUE; } else
	if(LEX.scan_word("Dec")) { mm = 12 ; return TRUE; } else
	return FALSE;
}

bool file_scan_base::scan_line( str1 & str )
{
	p0p2 zone;
	LEX.p0p2_start( zone );
	LEX.scan_line(); // cset that excludes NUL NL and CR
	LEX.p0p2_stop( zone );
	str = zone;
	// LURK - this is MM3 only
	// str.no_comma();
	return LEX.scan_crlf();
}

bool file_scan_base::open_file( const u8 * filename )
{
	int t;
	t=f.map_in_file( filename, FALSE );
	if(t) return FALSE;
	if( f.fd_size == 0 ) {
		static u8 fake_buffer[] = {'\n', 0 };
		zone = p0p2( fake_buffer, (uns) 0 ); // see API
		// zone = p0p2( "\n", 0 ); // see API
		f.close();
	} else {
		zone = p0p2( f.page0, f.fd_size );
	}
	LEX.set_file_zone( zone );
	// already thrown
	if(!LEX.check_nl_at_eof())
	{
		e_print("File %s does not end in NL\n",filename );
		return false;
	}
	return true;
}
