#include "fd_restart_file.h"
#include "buffer1.h"
#include "mmap_file.h"

void restart_data_t:: zero() {
	memset( (char*) abb, 0xFF, sizeof(abb) );
	seek_eof = 0;
	seek_wr = 0;
	seek_rd = 0;
}

void restart_data_t:: init(const char * _abb, u64 dev_size) { 
	zero();
	snprintf( (char*) abb, sizeof(abb), "%s", _abb );
	seek_eof = dev_size;
}

bool restart_data_t:: show_info()
{
	INFO("abb      '%s'", (char *) abb );
	INFO("seek_eof %12llX", seek_eof );
	INFO("seek_wr  %12llX", seek_wr );
	INFO("seek_rd  %12llX", seek_rd );
	return true;
}


/////////////////

bool fd_restart_file_t:: open_abb( const char * _abb, u64 dev_size ) {

	u8 abb[8]; // sdb
	snprintf( (char*) abb, sizeof(abb), "%s", _abb );

	buffer1 full_file_name;
	full_file_name.printf("./fill_2t_restart_%s.data", _abb );

	if(! open_restart_file( _abb, (STR0) full_file_name, dev_size ))
		return FAIL_FAILED();
	
	return true;
}

bool fd_restart_file_t:: mmap_file_data()
{
	return true;
}

bool fd_restart_file_t:: grow_file_write_data(
	const char * _abb,
	u64 dev_size
){
	INFO("sz_data %d fd_size %d", sz_data, fd_size );
	if(! grow_file_16( (u16) sz_data )) 
		return FAIL_FAILED();
	restart_mmap = (restart_data_t  *) page0;
	if(! restart_mmap ) return FAIL("NULL MMAP page0");
	restart_mmap->init(_abb, dev_size );
	return true;
}

bool fd_restart_file_t:: false_report_mismatch()
{
	restart_mmap->show_info();
	FAIL("restart file for a different device or a different size");
	FAIL("delete restart file and try again from zero");
	return false;
}

bool fd_restart_file_t:: open_restart_file(
	const char * _abb,
	const char * _restart_file_name,
	u64 dev_size
) {
	fd_grumble_size = 200; // bytes

	if( mmap_in_file_RW( _restart_file_name )) { 
		// already exists
	} else if( open_RW_CREATE( _restart_file_name )) {
		// just created
		if(!grow_file_write_data(_abb,dev_size))
			return FAIL_FAILED();
	} else {
		return FAIL("%s", _restart_file_name );
		return FAIL_FAILED();
	}

	if( !fd_size  || ( !sz_data != fd_size ))
	{
		FAIL("file wrong size - delete it");
		return FAIL_FAILED();
		if(!grow_file_write_data(_abb, dev_size))
			return FAIL_FAILED();
	}
	restart_mmap->show_info();
	if( restart_mmap->seek_eof != dev_size ) {
		false_report_mismatch();
		return FAIL_FAILED();
	}
	INFO("opened %s", _restart_file_name );
	return true;
}
