/*
	scan_mm3.c	scan mm3 file outputting the data
	Graham Swallow 
*/

// #pragma interface
// #include "scan_nl.h"	// getc buffer with lex matching
// #include "map_file.h"	// mmaps in file (readonly)
// #include "own_str.h"	// own_str

// 2025-06-24 // kludged const char * warnings # borrowed dgb_print

#include "mm3_file_tbl.h"
#include "mm3_file_scan.h"

int main(int argc, char ** argv)
{
	if(argc!=2)
	{
		printf("USAGE: %s filename.mm3\n",argv[0]);
		exit(1);
	}
	mm3_file_data * data;
	mm3_file_tbl dset;
	mm3_file_scan f1;
	data = (mm3_file_data *) & dset;
	if( f1.scan_mm3_table( data, argv[1]) ) {
		dset.print_tbl();
		return 0;
	} else
		return 1;
}
/* -eof- */
