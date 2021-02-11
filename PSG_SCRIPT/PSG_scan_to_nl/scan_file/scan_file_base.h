#ifndef scan_file_base_H
#define scan_file_base_H

#include "map_file.h"
#include "scan_to_nl.h"

class scan_file_base
{
 public:
	map_file f;
	// derived clss must do soomthing LIKE:
	// scan_nl_XML_LEX LEX;

	bool open_file( const u8 * filename, scan_to_nl * LEX0 );

/*
 NO LEX AVAILABLE YET
	void report0()	{ LEX.report0();} 
*/

};


#endif
