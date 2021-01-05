#ifndef zfile_line_in_H
#define zfile_line_in_H

// #include <stdio.h>
#include "misc.h"
#include "blk1.h"
#include "zlib.h"

// old as zlib is, X11R6 uses an even older include file without this!
extern "C" char * gzgets (gzFile file, char *buf, int len);

/*!
	zfile_line_in READS lines from a zlib comressed file

	This COULD then call fd_selecteable().got_line() in a loop
	simply block,
	(as it currently is), as load_file(filename.gz) isnt always
	an NFS file. For when it is, use fd_file_plain() to fetch
	data (packets in arbitrary 4K blocks) that a FILTER reacts to
	bridging with the zlib details that I have so far avoided.
*/
class zfile_line_in : public GRP_lib_base
{
	gzFile f;
	blk1 linebuff;
public:

	 zfile_line_in(); 
	~zfile_line_in() { close(); }
	bool open( const char * filename ); // readonly!
	char * getline(); // NULL means ....
	//! after a getline() return the SAME LINE again
	char * repeat_get_line() { return (char *) linebuff.buff; }
	void close();
};

#endif
