// #define OKfprintf if(1) fprintf
#include "sar_parser.h"

bool sar_parser::zfile_to_dset(
	const char * site_name,
	const char * cmd_name,		// sar
	const char * zfilename,
	dset_api * dset
) {
	// create the store for the data
	if( !cmd_name )
		cmd_name = "sar";
	tbl_collect _tbl( site_name, cmd_name );

	// create a sar_parser
	sar_parser parser( & _tbl );

	// run the parser over the file, into its tbl_collect
	if(! parser.z_parse( zfilename ) ) return FALSE;

	// send the data downstream
	_tbl.send_to_dset( dset );

	// delete the parser and the tbl_collect (on exit)

	return TRUE;
}



bool sar_parser::z_parse( const char * zfilename )
{
	if( !open( zfilename ) ) return FALSE;
	if(!header()) return FALSE;
	// read each paragraph, stopping at eof
	tbl->prep_next_row();
	while( stanza() ) 
	{
		tbl->prep_next_row();
	}
	tbl->undo_prep_next_row();
	// check trailer text (maybe stanza() stopped on a syntax error
	if( !trailer() ) return FALSE;
	return TRUE;
}

sar_parser::sar_parser(
		tbl_collect * _tbl
)
: sar_parser_temps( _tbl )
, tbl( _tbl )
{
	line = NULL;
	eof = TRUE;	// or initially false ??
}

sar_parser::~sar_parser()
{
	// destroy all data collected (except dset)
}

bool sar_parser::open( const char * filename ) {
	line = NULL;
	if( in.open( filename )) {
		eof = FALSE;
		get_line();
		return TRUE;
	} else {
		eof = TRUE;
		return false;
	}
}

bool sar_parser::nibble_time() {
	if( !get_line() ) {
		fprintf(stderr, "# FAIL # nibble_time - no line \n" );
		return FALSE;
	}
	const char * fmt = "%d:%d:%d ";
	if( 3 != sscanf( line, fmt, &hour, &min, &sec )) {
		fprintf(stderr,"# Expected %s\n#GOT '%s'\n",
			fmt, line );
		return FALSE;
	}
	OKfprintf(stderr,"# OK # nibble_time %2.2d:%2.2d:%2.2d \n",
		hour, min, sec );
	for(int i = 0; i<8; i++ ) {
		line[i] = ' ';
	}
	return TRUE;
}

bool sar_parser::header_line() {

	char * letter;
	if( !get_line() ) return FALSE;
	const char * fmt1 = "HP-UX %20s B.11.00 U 9000/800    %d/%d/%d";
	const char * fmt2 = "HP-UX %20s B.10.20 %c 9000/800    %d/%d/%d";
	if( 4 == sscanf( line, fmt1, hostname, &mm, &dd, &yy )) {
		// OK
	} else if( 5 == sscanf( line, fmt2, hostname, &letter, &mm, &dd, &yy )) {
		// OK
	} else {
		// if there is an OS selector, this is not an error YET
		fprintf(stderr,"# Expected %s\n#GOT '%s'\n",
			fmt1, line );
		return FALSE;
	}
	// LURK // Y2K LINE
	if( yy < 70 ) 
		year = 2000 + yy;
	else
		year = 1900 + yy;
	OKfprintf(stderr,"# OK # HOST %s %d-%2.2d-%2.2d\n",
		hostname, year, mm, dd );
	used_line();
	tbl->set_host_in_all_cols( hostname );
	return TRUE;
}

bool sar_parser::header() {
	if( !blank_line() ) {
		fprintf(stderr, "# FAIL # expected '' got '%s'\n",
			line );
		return FALSE;
	}

	if( !header_line() ) return FALSE;

	if( !blank_line() ) {
		fprintf(stderr, "# FAIL # expected '' got '%s'\n",
			line );
		return FALSE;
	}
	if( !get_line() ) return FALSE;

	if(!nibble_time()) {
		// nibble sets error code
		return FALSE;
	}
	// store time for the first time
	set_time_now();

	// slightly hardcoded - 10 lines of unread header text
	for( int i = 0; i<10; i++ ) {
		if( !get_line() ) return FALSE;
		if( line[0] != ' ' ) {
			fprintf(stderr, "# FAIL # '%s'\n", line );
			return FALSE;
		}
		OKfprintf(stderr, "# OK # %2d # %s", i, line );
		used_line();
	}
	return TRUE;
}

bool sar_parser::cpu_line_u() {
	if( !get_line() ) return FALSE;
	const char * fmt1 = "%d %f %f %f %f"; // NB matches leading SP ok
	const char * fmt2 = " system %f %f %f %f";
	float usr, sys, wio, idle;
	if( 5 == sscanf( line, fmt1, &cpu,
		&usr,
		&sys,
		&wio,
		&idle
	) ) {
		sar_u * u = lookup_cpu_u( cpu );
		if(!u) return FALSE;
		* u->usr.ptr() = usr;
		* u->sys.ptr() = sys;
		* u->wio.ptr() = wio;
		* u->idle.ptr() = idle;
		OKfprintf(stderr,"# OK # cpu[%d] ... %s", cpu, line );
		used_line();
		return TRUE;
	}
	if( 4 == sscanf( line, fmt2,
		&usr,
		&sys,
		&wio,
		&idle
	) ) {
		cpu = -1;
		sar_u * u = lookup_cpu_u( cpu );
		if(!u) return FALSE;
		* u->usr.ptr() = usr;
		* u->sys.ptr() = sys;
		* u->wio.ptr() = wio;
		* u->idle.ptr() = idle;
		OKfprintf(stderr,"# OK # cpu[%d] ... %s", cpu, line );
		used_line();
		// fail - so that next line doesnt have to!
		return FALSE;
		return TRUE;
	}
	fprintf(stderr,"# FAIL ' expected %s, got '%s'", fmt2, line );
	return FALSE;
}

bool sar_parser::cpu_line_q() {
	// this is flawed! It only keeps 2 of 4 variables LURK
	if( !get_line() ) return FALSE;
	const char * fmt1 = "%d %f %f %f %f %s "; // NB matches leading SP ok
	const char * fmt2 = " system %f %f";
	// cpu runq-sz %runocc swpq-sz %swpocc

	float runqsz;
	float runocc;
	float swpqsz = 0;
	float swpocc = 0;
	sar_q * q = NULL;

	int t = -1;
	if( 5 == (t = sscanf( line, fmt1, &cpu,
		&runqsz,
		&runocc,
		&swpqsz,
		&swpocc
	 )) ) {
		q = lookup_cpu_q( cpu );
		* q->runqsz.ptr() = runqsz;
		* q->runocc.ptr() = runocc;
		* q->swpqsz.ptr() = swpqsz;
		* q->swpocc.ptr() = swpocc;
		// rare - normally 2 of 4
		fprintf(stderr,"# OK!# CPU[%d] ... %s", cpu, line );
		used_line();
		return TRUE;
	} else if( t == 3 ) {
		q = lookup_cpu_q( cpu );
		* q->runqsz.ptr() = runqsz;
		* q->runocc.ptr() = runocc;
		OKfprintf(stderr,"# OK # CPU[%d] ... %s", cpu, line );
		used_line();
		return TRUE;
	}
	// system line matches any number of %f's
	if( 2 == sscanf( line, fmt2,
		&runqsz,
		&runocc
	) ) {
		cpu = -1;
		q = lookup_cpu_q( cpu );
		* q->runqsz.ptr() = runqsz;
		* q->runocc.ptr() = runocc;
		OKfprintf(stderr,"# OK # CPU--- ... %s", line );
		used_line();
		// fail - so that next line doesnt have to!
		return FALSE;
	}
	fprintf(stderr,"# FAIL ' expected %s, got '%s'", fmt2, line );
	return FALSE;
}

bool sar_parser::disk_line() {
	if( !get_line() ) return FALSE;

	sar_d * d = NULL;

	const char * fmt_dev = " %20s ";
	if( 1 != sscanf( line, fmt_dev, &device )) {
		return FALSE;
	}
	if( device[0] <= '9' ) return FALSE;
	d = lookup_disk( device );
	if(!d) return FALSE;

	const char * fmt = " %20s %f %f %f %f %f %f"; // NB matches leading SP ok

	if( 7 == sscanf( line, fmt, &device,
			d->busy.ptr(),
			d->avque.ptr(),
			d->rw.ptr(),
			d->blk.ptr(),
			d->avwait.ptr(),
			d->avserv.ptr()
	)) {
		if( device[0] <= '9' ) return FALSE;
		OKfprintf(stderr,"# OK # DEV[%s] ... %s", device, line );
		used_line();
		return TRUE;
	}
	return FALSE;
}

/*
//1         bread/s lread/s %rcache bwrit/s lwrit/s %wcache pread/s pwrit/s
//2         swpin/s bswin/s swpot/s bswot/s pswch/s
//3         scall/s  sread/s  swrit/s   fork/s   exec/s  rchar/s  wchar/s
//4          iget/s namei/s dirbk/s
//5         rawch/s canch/s outch/s rcvin/s xmtin/s mdmin/s
//6         text-sz  ov  proc-sz  ov  inod-sz  ov  file-sz  ov
//7           msg/s  sema/s
*/

bool sar_parser::sys_line_1() {
	const char * fmt = " %f %f %f %f %f %f %f %f %s"; 

	if( !get_line() ) return FALSE;

	if( 8 == sscanf( line, fmt,
		b.bread.ptr(),
		b.lread.ptr(),
		b.rcache.ptr(),
		b.bwrit.ptr(),
		b.lwrit.ptr(),
		b.wcache.ptr(),
		b.pread.ptr(),
		b.pwrit.ptr()
	)) {
		OKfprintf(stderr,"# OK_1 ## %s", line );
		used_line();
		return TRUE;
	}
	return FALSE;
}

bool sar_parser::sys_line_2() {

	if( !get_line() ) return FALSE;
	const char * fmt = " %f %f %f %f %f %s"; 

	if( 5 == sscanf( line, fmt,
		w.swpin.ptr(),
		w.bswin.ptr(),
		w.swpot.ptr(),
		w.bswot.ptr(),
		w.pswch.ptr()
	)) {
		OKfprintf(stderr,"# OK_2 ## %s", line );
		used_line();
		return TRUE;
	}
	return FALSE;
}

bool sar_parser::sys_line_3() {
	if( !get_line() ) return FALSE;
	const char * fmt = " %f %f %f %f %f %f %f %s"; 

	if( 7 == sscanf( line, fmt,
		c.scall.ptr(),
		c.sread.ptr(),
		c.swrit.ptr(),
		c.fork.ptr(),
		c.exec.ptr(),
		c.rchar.ptr(),
		c.wchar.ptr()
	)) {
		OKfprintf(stderr,"# OK_3 ## %s", line );
		used_line();
		return TRUE;
	}
	return FALSE;
}

bool sar_parser::sys_line_4() {
	if( !get_line() ) return FALSE;

	const char * fmt = " %f %f %f %s"; 

	if( 3 == sscanf( line, fmt,
		a.iget.ptr(),
		a.namei.ptr(),
		a.dirbk.ptr()
	)) {
		OKfprintf(stderr,"# OK_4 ## %s", line );
		used_line();
		return TRUE;
	}
	return FALSE;
}

bool sar_parser::sys_line_5() {
	if( !get_line() ) return FALSE;

	const char * fmt = " %f %f %f %f %f %f %s"; 

	if( 6 == sscanf( line, fmt,
		y.rawch.ptr(),
		y.canch.ptr(),
		y.outch.ptr(),
		y.rcvin.ptr(),
		y.xmtin.ptr(),
		y.mdmin.ptr()
	)) {
		OKfprintf(stderr,"# OK_5 ## %s", line );
		used_line();
		return TRUE;
	}
	return FALSE;
}

bool sar_parser::sys_line_6() {
	if( !get_line() ) return FALSE;

	const char * fmt = " %f/%f %f %f/%f %f %f/%f %f %f/%f %f %s"; 

	if( 12 == sscanf( line, fmt,
		v.text.ptr(), v.text_sz.ptr(), v.text_ov.ptr(),
		v.proc.ptr(), v.proc_sz.ptr(), v.proc_ov.ptr(),
		v.inod.ptr(), v.inod_sz.ptr(), v.inod_ov.ptr(),
		v.file.ptr(), v.file_sz.ptr(), v.file_ov.ptr()
	)) {
		OKfprintf(stderr,"# OK_6 ## %s", line );
		used_line();
		return TRUE;
	}

	* v.text.ptr() = -1;
	* v.text_sz.ptr() = -1;
	* v.text_ov.ptr() = -1;

	fmt = " N/A N/A %f/%f %f %f/%f %f %f/%f %f %s"; 
	if( 9 == sscanf( line, fmt,
		v.proc.ptr(), v.proc_sz.ptr(), v.proc_ov.ptr(),
		v.inod.ptr(), v.inod_sz.ptr(), v.inod_ov.ptr(),
		v.file.ptr(), v.file_sz.ptr(), v.file_ov.ptr()
	)) {
		OKfprintf(stderr,"# OK_6_## %s", line );
		used_line();
		return TRUE;
	}

	return FALSE;
}

bool sar_parser::sys_line_7() {
	if( !get_line() ) return FALSE;
	const char * fmt = " %f %f %f"; // used to have trailing %s !!!

	if( 2 <= sscanf( line, fmt,
		m.msg.ptr(),
		m.sema.ptr(),
		m.select.ptr()	// ADDED but only if -s used in sar ... LURK
	)) {
		OKfprintf(stderr,"# OK_7 ## %s", line );
		used_line();
		return TRUE;
	}
	// else try 2 = " %f %f" without select
	return FALSE;
}

bool sar_parser::stanza() {
//	e_print("stanza\n");
	/*
		This allocates space for the next row before we know
		IF there is another row.

		Added columns (new CPU's etc) should copy their neighbours
		so be OK.

		At end N is one too many
	*/
	// tbl->prep_next_row();

	if( !get_line() ) {
		return FALSE; 	// HMM EOF not allowed here
	}
	if( blank_line_peek() ) {
		/*
			GOOD - list of stanzas ends with a blank line
		*/
		return FALSE;
	}

	if( !nibble_time() ) {
		// nibble sets error code
		return FALSE;
	}
	// nibble doesnt eat the line - it nibbles it
	// store updated time and maybe dt for the first+ time
	adj_time_now();
	/*
		adj_time_now() does all the propagation work
		tbl->set_dt_in_all_cols( dt_now );
	*/

	bool ok = FALSE;
	while( cpu_line_u() ) ok = TRUE;
	if( !ok ) {
		fprintf(stderr,"CPU LINE expected, got '%s'\n", line);
		return FALSE;
	}

	ok = FALSE;
	while( disk_line() ) ok = TRUE;
	if( !ok ) {
		fprintf(stderr,"# DISK LINE expected, got '%s'\n", line);
		// possibly all disks idle for 2 minutes ...
		// return FALSE;
	}

	ok = FALSE;
	while( cpu_line_q() ) ok = TRUE;
	if( !ok ) {
		fprintf(stderr,"CPU LINE B expected, got '%s'\n", line);
		return FALSE;
	}

	if(!sys_line_1()) { return FALSE; }
	if(!sys_line_2()) { return FALSE; }
	if(!sys_line_3()) { return FALSE; }
	if(!sys_line_4()) { return FALSE; }
	if(!sys_line_5()) { return FALSE; }
	if(!sys_line_6()) { return FALSE; }
	if(!sys_line_7()) { return FALSE; }

//	tbl->prints( stderr ); // one row only!
	return TRUE;
}

bool sar_parser::trailer() {
	if( !blank_line() ) {
		fprintf(stderr,"trailer - missing blank line\n");
		return FALSE;
	}
	while( get_line() ) {
		char any[100];
		if( 1 <= sscanf( line, "Average %20s ", any )) {
			OKfprintf(stderr,"# OK # %s", line );
			used_line();
			continue;
		} else {
			fprintf(stderr,"trailer - missing Average line, '%s'", line);
			return FALSE;
		}
	}
	return TRUE;
}

