// fill_2T.cxx /dev/sdb1 fill_2T_sdb1.ini // OPT 32K // OPT NUL //
// fill_2T.cxx /* /dev/ */ sdb1 fill_2T_%s.ini 123456 // 
// MAIN.cxx 
#include "dgb.h"
// #include "ints.h"
// #include <stdio.h>
#include "buffer2.h"
#include "fd_dev.h"
#include "fill_2T.h"



/*!
	fill /dev/sdb 
	with 1K blocks
	head {
		u64_MARK // "FILL_2T" <NUL>
		u64_TIME // before write request time in ms // or ZERO
		u64_SEEK // BYTE_POS in file
		u64_HASH // homebrew CRC of this 1K but ZERO here
	}
	body {
		u8 body[ 1024 - sizeof(head) ] // of RANDOM or repeated or
	}

	TODO: detect size of lsblk
	TODO: detect serial of partition / disk
	TODO: exactly NUL as if /dev/zero
	TODO: more random data, not same every time

	fill_2T_sdb1.ini {
#	 use LIBR .ini to save and reload
	 DEVICE /dev/sdb1 # check same on restart
	 SERIAL 1234567890 # check same on restart
 -->	 SEEK pos
	 OPT_NUL_HEAD false # head needed to test
	 OPT_NUL_BODY false # body not zeroed out
	 OPT_repeat_BODY true # same data very block
	 OPT_random_BODY false # AA 55 00 FF # hd notices repeat 16
	}

	GRAPH_DATA {
		u64_time_ms	# or _ns # or binary 2^20 
		u64_seek_pos	# bytes
	}

	OPTION:

		32K block write might help SRAM
		256K write request might help SYNC make sense
		1K block permits checking (but timing not sensible)
	
	HASH CRC

		the hash that HT uses, before truncation to 32 bit
		CRC = INIT 0x1020304050607080
		CRC += BYTE
		CRC *= b10000000010000001000100101
	
	READ BLOCK

		the header block must be there from a different program
		eg echo "SOMETHING" > /dev/sdb

		MAGIC {
			fill_2T NUL	
		}

		printf "fill_2T\0" | hd -c
	
	COMMANDS

		WRITE_SWEEP_RESUME

			store POS in header
			restart from there

		READ_SWEEP_RESUME

			POS in header

		WRITE_TREE_RESUME

			HIGH pos comes from command line
			round up to power of 2
			HALF, QUARTER, sweep
			save pos every 128 writes

		READ_TREE_RESUME

			same route but start at READ_POS and stop at WRITE_POS

*/
bool bool_main( int argc, char ** argv ) {

// actually this is still copy_restart - stolen code :-)

	set_prog_name( argv[0] ); // check this
//	gdb_sigaction( argv[0] ); // check this

	if( argc != 3 ) {
		errno = 22;
		FAIL("USAGE %s sdb WRITE_SWEEP_RESUME # /dev/sdb # argc == %d", get_prog_alias(), argc );
		return false;
	}

//	gdb_invoke(false);

	const char * dev_name_tail = argv[1]; // "sdb";

	fd_dev_t fd_dev;
	if(! fd_dev.open_abb( dev_name_tail )) {
		return FAIL_FAILED();
	}

	str0 arg_opcode = argv[2];
	if( arg_opcode == "WRITE_SWEEP_RESUME" )
	{
		if(! fd_dev.WRITE_SWEEP_RESUME() ) 
			return FAIL_FAILED();
	} else 
	if( arg_opcode == "READ_SWEEP_RESUME" )
	{
		if(! fd_dev.READ_SWEEP_RESUME() ) 
			return FAIL_FAILED();
	} else {
		return FAIL("expected opcode, eg WRITE_SWEEP_RESUME");
	}

	INFO("dev %s", (STR0) dev_name_tail );
	return PASS("STOP");
	return WARN("STOP");


	return true;
}

int main( int argc, char ** argv ) {
	if( bool_main( argc, argv )) {
		PASS("DONE");
		return 0;
	}
	if(errno) 
		return errno;
	WARN("NO ERRNO");
	return 1;
}
