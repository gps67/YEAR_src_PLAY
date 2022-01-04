#ifndef scan_file_base_H
#define scan_file_base_H

#include "map_file.h"
#include "scan_to_nl.h"

class scan_file_base
{
 public:
 	// MMAP of FILE.txt that we are Parsing
	mmap_file mmap_fd;

	// MMAP in entire file // RO //
	// P0 == mmap_fd.page0 
	// P2 == P0 + mmap_fd.nbytes
	// give MMAP_P0P2 to LEX0 
	bool open_file( const u8 * filename, scan_to_nl * LEX0 );

	// no derived class for LEX, that is in the CTOR

};


#endif
