#include "buffer2.h"
#include "fd_dev.h"
#include <linux/fs.h>

fd_dev_t::
fd_dev_t()
: fd()
, file_size_in_bytes(0)
, block_size_in_bytes(0)
, pair_steps()
{
	i64 dx = 1000 * 1000; // 1 second in microseconds
	i64 dy = 1024 * 1024; // 1 megabyte
	pair_steps.late_init( dx, dy, dx * 1024 );
	fd.ref_static(); // else stack smashing crash
}

bool fd_dev_t:: open_abb( const char * abb_dev_name )
{
	buffer1 _dev_name;
	_dev_name.print("/dev/%s", abb_dev_name );
	if(! open_device( (STR0) _dev_name ))
		return FAIL_FAILED();
	
	if(! fd_restart_file.open_abb( abb_dev_name, file_size_in_bytes ) )
		return FAIL_FAILED();
	
	buffer1 _log_name;
	_log_name.print("./fill_2T_log_data_%s.data", (STR0) abb_dev_name );
	if(! pair_steps.open_data_log( (STR0) _log_name ))
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

	if(!check_sector_zero_of_device()) {
		// checks "fill_2T" but not seek nor fill_fill data
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

bool fd_dev_t:: check_sector_zero_of_device()
{
	if(!fd.seek_SET_64( 0 ) ) return FAIL_FAILED();

	// read it in
	int t=fd.read( (void *) & sect_in, b512_data_t:: N512 );
	if(t != b512_data_t:: N512 ) {
		WARN("fd.read() returned %d # expected 512", t );
		return FAIL_FAILED();
	}

	// fill_2t NUL //
	// this is our safeguard to not destroying other disks
	if(!sect_in.check_name()) {	// error if not "fill_2T" tagged
		return FAIL("check_name failed - you have to tag disk manually" );
	}

	// that's all that is required 
	// printf "fill_2T\0" > /dev/sdx
	// but it is checked again AFTER restart file has been loaded
	// and a READ_SWEEP is done

	// leave seek where you would expect it to be after open
	if(!fd.seek_SET_64( 0 ) ) return FAIL_FAILED();
	return true;
}

bool fd_dev_t:: read_next_sector()
{
	// CODE check
	if( !fd_restart_file.restart_mmap ) {
		return FAIL("CODE ERROR restart_map not set");
	} 

	// read it in
	int t=fd.read( (void *) & sect_in, b512_data_t:: N512 );
	if(t != b512_data_t:: N512 ) {
		WARN("fd.read() returned %d # expected 512", t );
		return FAIL_FAILED();
	}

	// shorter
	u64 seek_pos = fd_restart_file.restart_mmap->seek_rd ;

	if(!sect_in.check_name()) {	// error if not "fill_2T" tagged
		return FAIL("check_name failed in sector 0x%08llX", seek_pos );
	}

	if(!sect_in.check_sector_is(seek_pos)) {
		// check that sector is tagged as itself // no wrap-around
		return FAIL("check_sector failed in sector 0x%08llX", seek_pos );
		return FAIL_FAILED();
	}

	if(!sect_in.check_fill()) {
		// must be 0xFA // or NUL or FF or memcmp
		return FAIL("check_fill failed in sector 0x%08llX", seek_pos );
		return FAIL_FAILED();
	}

	// write modified SEEK after checking it is OK
	// actual write happens much later, thanks to mmap

	fd_restart_file.restart_mmap->seek_rd += b512_data_t:: N512;

 	// tell progress filter
	// obtain TIME in sect_out - as good as any
	// TODO obtain TIME from pair_steps ...
 if(1) {
	sect_out.time_stamp_now(),
	pair_steps.pair_data(
		sect_out.time_stamp_get(),
		fd_restart_file.restart_mmap->seek_rd  // after ++
	);
 }
	return true;
}

bool fd_dev_t:: write_next_sector()
{
	// CODE check
	if(!fd_restart_file.restart_mmap) {
		return FAIL("caller must have mmaped the restart file");
	}

	// caller should have prepared it
	if(!sect_out.check_name())	// error if not "fill_2T" tagged
		return FAIL_FAILED();
	
	// WRITE the OFFS into each sector so that we see duplicate blocks
	sect_out.sector_offset = fd_restart_file.restart_mmap->seek_wr;

	// for decoding later
	sect_out.time_stamp_now();
//	sect_out.time_stamp_set(t64);

	// not called each time, saves some speed, called once setup
	// sect_out.fill_fill()

	// WRITE the DATA into each sector for possible use later
	int t=fd.write( (void *) & sect_out, b512_data_t:: N512 );
	if(t != b512_data_t:: N512 )
		return FAIL_FAILED();

	// retain EA next in restart file
	fd_restart_file.restart_mmap->seek_wr += b512_data_t:: N512;

	// prep sect_out for next write
	sect_out.sector_offset += b512_data_t:: N512;

 if(1)
	pair_steps.pair_data(
		sect_out.time_stamp_get(),
		fd_restart_file.restart_mmap->seek_wr  // after ++
	);

	return true;
}

bool fd_dev_t:: WRITE_SWEEP_RESUME()
{
	if(! fd_restart_file.restart_mmap ) {
		return FAIL("NULL page0 - not setup properly");
	}

	if(! fd.seek_SET_64( fd_restart_file.restart_mmap-> seek_wr ))
	{
		return FAIL_FAILED();
	}

	// do this elsewhere
	pair_steps.Y100 = fd_restart_file.restart_mmap-> seek_eof;

	for( int i=0; i<5000000000; i++ ) {
		if( fd_restart_file.restart_mmap-> seek_wr  
		 >= fd_restart_file.restart_mmap-> seek_eof ) 
		{
		 	return PASS("stopping at seek_eof");
		}
 if(0)
		INFO("seek_wr 0x%08llX us 0x%08llX",
			fd_restart_file.restart_mmap -> seek_wr,
			sect_out.time_stamp_get()
		);
		if(! write_next_sector() )
			return FAIL_FAILED();
		static const int every = 128;
		if( ( i % every ) == 0 ) {

		 if(0)
			pair_steps.pair_data(
				sect_out.time_stamp_get(),
				fd_restart_file.restart_mmap->seek_wr 
				// seek_wr after += N512
			);
		//	INFO("fsync");
			fd.fsync();
		}
	}

	return true;
}

bool fd_dev_t:: READ_SWEEP_RESUME()
{
	// CODE error (check)
	if(! fd_restart_file.restart_mmap ) {
		return FAIL("NULL page0 - not setup properly");
	}

	// seek to where it left off
	if(! fd.seek_SET_64( fd_restart_file.restart_mmap-> seek_rd ))
	{
		return FAIL_FAILED();
	}

	// actually intention is a while loop
	// that calls read_next_sector several times, 
	// 
	// 1 512 = K/2
	// 2 512 = K
	// 2048 = 1M
	// 2048*1024 = 1G
	// 2048*1024*1024 = 1T

	for( int i=0; i<5000000; i++ ) {

	if(0)	if( fd_restart_file.restart_mmap-> seek_rd  
		 >= fd_restart_file.restart_mmap-> seek_wr ) 
		 {
		 	return PASS("stopping at seek_wr");
		 }
		if( fd_restart_file.restart_mmap-> seek_rd  
		 >= fd_restart_file.restart_mmap-> seek_eof ) 
		 {
		 	return PASS("stopping at seek_eof");
		 }

	if(0)	INFO("seek_rd 0x%08llX",
			fd_restart_file.restart_mmap-> seek_rd );
		if(! read_next_sector() )
			return FAIL_FAILED();
	}

	return true;
}


bool fd_dev_t:: SHOW_RESTART()
{
	if(!  fd_restart_file.restart_mmap ) {
		return FAIL("NULL page0 - not setup properly");
	}
	show();
	fd_restart_file.restart_mmap-> show_info();
	return true;
}

