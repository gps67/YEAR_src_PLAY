
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

// #include <stdlib.h>
// #include <stdio.h>
// #include <time.h>
// #include <errno.h>

// #include "misc.h"
//#include "array_base_1248.h"

#ifndef VMPARSE_1020_H
#define VMPARSE_1020_H

#include "vm18flt_tbl.h"
#include "zfile_line_in.h"

class vmparse_1020 {
	// simple holder of the parse strings, line matchers
	// should be BOOL though
public:
	int scan_line_1020( vm18flt_row & v, char * line );
	int matches_header1( char * str );
	int matches_header2( char * str );
	char ** colnames();

	// read each line of in
	// build tbl output
	// use info from header line1
	void z_parse( line1 &line, zfile_line &in, vm18flt_tbl & tbl );
	// plough through any errors

};

#endif
