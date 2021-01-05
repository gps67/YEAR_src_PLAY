
#ifndef VMPARSE_1020_H
#define VMPARSE_1020_H

#include "zfile_line_in.h"
class dset_api;
#include "tbl_collect.h"

/*!
	Parse a zlib comressed ASCII file from vmstat, with an initial line1,
	and send the results to a dset_api (which stores it).
*/
class vmparse_1020 : public GRP_sar_parse
{
	// simple holder of the parse strings, line matchers
	// should be bool though
public:

	/*!
		the collected data (renewed for each input file)
	*/
	tbl_collect tbl;

	~vmparse_1020();

	vmparse_1020(
		const char * site_name, 
		const char * cmd_name,
		const char * hostname,
		time_t t0,
		int dt,
		int nrows
	);

	bool scan_line_1020( char * line );
	bool matches_header1( char * str );
	bool matches_header2( char * str );
	const char ** colnames();

	bool z_parse( zfile_line_in &in );

	static bool zfile_to_dset(
		const char * site_name,
		const char * cmd_name,          // vmstat
		const char * zfilename,
		dset_api * dset
	);                                                                      

};

#endif
