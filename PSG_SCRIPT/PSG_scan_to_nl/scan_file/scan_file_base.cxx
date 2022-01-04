#include "scan_file_base.h"
#include "str1.h"
#include "scan_to_nl.h"
#include "dgb.h"

// SHARE ?
bool scan_file_base::open_file( const u8 * filename, scan_to_nl * LEX0 )
{
	bool writable = false;
	int t; // errno or zero
	t = mmap_fd.map_in_file( filename, writable );
	if(t) return FALSE; // FAIL("%s", filename );

	/*
		take P0P2 from MMAP give it to scan_nl
	*/
	if(!mmap_fd.check_nl_at_eof()) {
		return FAIL("need nl");
	}
	LEX0 -> set_file_zone( p0p2(mmap_fd)  );
	// already thrown
	if(!LEX0 -> check_nl_at_eof())
	{
		e_print("File %s does not end in NL\n",filename );
		return false;
	}
	return true;
}

