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
	if( !fd.do_ioctl( "BLKFLSBUF", BLKFLSBUF, NULL )) {
		FAIL( "BLKFLSBUF %s", (STR0) dev_name );
		return FAIL_clear_error(); // it is silent // and not working
		return false;
	}

	return true;
}
