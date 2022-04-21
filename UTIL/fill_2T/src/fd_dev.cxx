#include "buffer2.h"
#include "fd_dev.h"
#include <linux/fs.h>

bool fd_dev_t:: open_abb( const char * abb_dev_name )
{
	buffer1 _dev_name;
	_dev_name.print("/dev/%s", abb_dev_name );
	if(! open_device( (STR0) _dev_name ))
		return FAIL_FAILED();
	
	if(! fd_restart_file.open_abb( abb_dev_name, file_size_in_bytes ) )
		return FAIL_FAILED();

	return true;
}

bool fd_dev_t:: open_device( const char * _dev_name )
{
	file_size_in_bytes = 0;
	block_size_in_bytes = 0;
	dev_name = _dev_name;

	if( !fd.open_RW( (STR0) dev_name ))
		return FAIL_FAILED();

	// BLKGETSIZE64 2000398934016 1863.00 G
	if( !fd.do_ioctl( "BLKGETSIZE64", BLKGETSIZE64, &file_size_in_bytes ))
		return FAIL( "BLKGETSIZE64 %s", (STR0) dev_name );

	// 512 bytes per sector // usb connected SATA drive
	if( !fd.do_ioctl( "BLKSSZGET", BLKSSZGET, &block_size_in_bytes ))
		return FAIL( "BLKSSZGET %s", (STR0) dev_name );

	show();

	// SEEK is currently ZERO
	// read first sector and check it has name8
	// KNOW that restart file will soon be loaded, but is not yet

	// this will be NULL because we have not yet loaded the restart file
	// MAYBE switch to writing restart data at specific times ?

	INFO("NULL fd_restart_file.restart_mmap->zero();");
//	fd_restart_file.restart_mmap->zero(); // fd and data

	if(!read_next_sector()) { // maybe it detects NULL
		return FAIL_FAILED();
	}

	return true;
}

bool fd_dev_t:: show()
{
	INFO( "DEVICE %s", (STR0) dev_name );
	float size_G = file_size_in_bytes / (1024 * 1024 * 1024);
	INFO( "BLKGETSIZE64 %lld %7.2f G", file_size_in_bytes, size_G );
	INFO( "BLKSSZGET %d", block_size_in_bytes );
	flush_buffer_cache();
	return true;
}


bool fd_dev_t:: flush_buffer_cache()
{
	INFO("this needs root permission");
	return fd.fsync();

	if( !fd.do_ioctl( "BLKFLSBUF", BLKFLSBUF, NULL )) {
		FAIL( "BLKFLSBUF %s", (STR0) dev_name );
		return FAIL_clear_error(); // it is silent // and not working
		return false;
	}

	return true;
}

void show_OFFS( const char * seekname, u64 seek_val, const char * descr )
{
}

bool fd_dev_t:: read_next_sector()
{
	int t=fd.read( (void *) & sect_in, b512_data_t:: N512 );
	if(t != b512_data_t:: N512 ) {
		WARN("fd.read() returned %d # expected 512", t );
		return FAIL_FAILED();
	}
	
	if(!sect_in.check_name()) {	// error if not "fill_2T" tagged
		if( !fd_restart_file.restart_mmap ) {
			WARN("seek_rd NULL means early");
		} else {
			WARN("seek_rd 0x%08llX", 
				fd_restart_file.restart_mmap->seek_rd 
			);
		}
		return FAIL_FAILED();
	}

	// write modified SEEK after checking it is OK
	// NULL means we are checking sector zero on open

	if(!fd_restart_file.restart_mmap) {
		return PASS("NULL restart_mmap - hope it is sector zero");
	} else {
		fd_restart_file.restart_mmap->seek_rd += b512_data_t:: N512;
	}
//	fd_restart_file.restart_mmap->zero(); // fd and data
	return true;
}

bool fd_dev_t:: write_next_sector()
{
	if(!fd_restart_file.restart_mmap) {
		return FAIL("caller must have mmaped the restart file");
	}

	// caller should have prepared it
	if(!sect_out.check_name())	// error if not "fill_2T" tagged
		return FAIL_FAILED();
	
	// sector_offset was init 0 // seek for resume // += N512
//	sect_out.sector_offset = fd_restart_file.restart_mmap->seek_wr

	
	int t=fd.write( (void *) & sect_out, b512_data_t:: N512 );
	if(t != b512_data_t:: N512 )
		return FAIL_FAILED();

	fd_restart_file.restart_mmap->seek_wr += b512_data_t:: N512;
	sect_out.sector_offset += b512_data_t:: N512;
	return true;
}

bool fd_dev_t:: WRITE_SWEEP_RESUME()
{
	if(!  fd_restart_file.restart_mmap ) {
		return FAIL("NULL page0 - not setup properly");
	}
	INFO("HOPE have checked first sector");
	for( int i=0; i<5; i++ ) {
		INFO("seek_wr 0x%08llX",
			fd_restart_file.restart_mmap-> seek_wr );
		if(! write_next_sector() )
			return FAIL_FAILED();
	}

	return true;
}

bool fd_dev_t:: READ_SWEEP_RESUME()
{
	return FAIL("TODO");
	return true;
}

