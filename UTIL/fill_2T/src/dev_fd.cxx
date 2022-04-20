#include "buffer2.h"
#include "dev_fd.h"
#include <linux/fs.h>

bool dev_fd_t:: open_abb( const char * abb_dev_name )
{
	buffer1 _dev_name;
	_dev_name.print("/dev/%s", abb_dev_name );
	if(! open_device( (STR0) _dev_name ))
		return FAIL_FAILED();
	
	buffer1 _restart_file_name;
	_restart_file_name.print("./.fill_2T_%s.restart", abb_dev_name );
	if(! open_restart_file( (STR0) _restart_file_name ))
		return FAIL_FAILED();

	return true;
}

bool dev_fd_t:: open_restart_file( const char * _restart_file_name )
{
	/*
		or split this code into a type of its own
		with limited responsibilites to self
		then combine in over arching admix
	*/
	if(fd_restart_file.open_RW( _restart_file_name )) {
		INFO("opened %s", _restart_file_name );
		if(!fd_restart_file.read( &restart_data, sizeof( restart_data )))
		{
			WARN("suggest delete and restart %s", _restart_file_name);
			return FAIL_FAILED();
		}
	} else {
		bool is_async = false; // it is sync
		if(!fd_restart_file.open_RW_CREATE( _restart_file_name, is_async ))
		{
			return FAIL_FAILED();
		}
		restart_data.zero();
		INFO("opened %s and wrote", _restart_file_name );
		if(!fd_restart_file.write( &restart_data, sizeof( restart_data )))
			return FAIL_FAILED();

	}
	return true;
}

bool dev_fd_t:: open_device( const char * _dev_name )
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
	return true;
}

bool dev_fd_t:: show()
{
	INFO( "DEVICE %s", (STR0) dev_name );
	float size_G = file_size_in_bytes / (1024 * 1024 * 1024);
	INFO( "BLKGETSIZE64 %lld %7.2f G", file_size_in_bytes, size_G );
	INFO( "BLKSSZGET %d", block_size_in_bytes );
	flush_buffer_cache();
	return true;
}


bool dev_fd_t:: flush_buffer_cache()
{
	INFO("this needs root permission");
	if( !fd.do_ioctl( "BLKFLSBUF", BLKFLSBUF, NULL )) {
		FAIL( "BLKFLSBUF %s", (STR0) dev_name );
		return FAIL_clear_error(); // it is silent // and not working
		return false;
	}

	return true;
}
