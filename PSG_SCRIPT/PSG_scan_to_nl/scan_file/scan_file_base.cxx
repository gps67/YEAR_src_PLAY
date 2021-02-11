#include "scan_file_base.h"
#include "str1.h"
#include "scan_to_nl.h"

// SHARE ?
bool scan_file_base::open_file( const u8 * filename, scan_to_nl * LEX0 )
{
	int t;
	t=f.map_in_file( filename, FALSE );
	if(t) return FALSE;
	p0p2 zone; // no need to keep ?
	zone = p0p2( f.page0, f.fd_size );
	LEX0->set_file_zone( zone );
	// already thrown
	if(!LEX0->check_nl_at_eof())
	{
		e_print("File %s does not end in NL\n",filename );
		return false;
	}
	return true;
}

