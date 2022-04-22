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
	if(!remap()) return FAIL_FAILED();;
	restart_mmap = (restart_data_t  *) page0;
	if(! restart_mmap ) return FAIL("NULL MMAP page0");
	// write the new restart data // zero + device abb + size
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
		restart_mmap = (restart_data_t  *) page0;
		// TODO pass &VAR to mmap_in_file_RW
		// so that mmap updates the types PTR
		INFO("LOADED");
		restart_mmap->show_info();
	} else {
		UNFAIL("Creating %s", _restart_file_name );
		if( open_RW_CREATE( _restart_file_name )) {
			// just created
			// remap is in grow_... as is ... page0
			// restart_mmap = (restart_data_t  *) page0;
			//
			if(!grow_file_write_data(_abb,dev_size))
				return FAIL_FAILED();
		} else {
			return FAIL("%s", _restart_file_name );
			return FAIL_FAILED();
		}
	}

	// all of the above set restart_mmap from page0
	if( ! restart_mmap ) {
		FAIL("CODE error NULL restart_mmap");
		return FAIL_FAILED();
	}

	// look for some checks, CODE error, changes in restart data size
	if( !fd_size  || ( sz_data != fd_size ))
	{
		FAIL("file wrong size - delete it");
		false_report_mismatch();
		return FAIL_FAILED();
	}

	// check device abb is "sdb" (it always will be)
	if( 0!=strcmp( (char*) restart_mmap->abb, _abb )) {
		FAIL("device abb wrong - delete it");
		false_report_mismatch();
		return FAIL_FAILED();
	}

	// this will detect a change in USB device
	// another idea would be to write it to the device itself
	// BUT not update it every sector write / read
	// check device size is what it used to be
	if( restart_mmap->seek_eof != dev_size ) {
		false_report_mismatch();
		return FAIL_FAILED();
	}

	// restart_mmap->show_info();
	INFO("opened %s", _restart_file_name );
	return true;
}
