#ifndef io_csv_in_H
#define io_csv_in_H

#include "zfile_line_in.h"
#include "dyn_array.h"                                                          
#include "str2.h"                                                          

/*!
	reads CSV lines from a zlib file (can be uncompressed), 
	and sends them to (virtual) got_line_1() which has
	public partially-writable access to support structures -

	The (single) input line has been split into fields. The
	text is held in the zfile_line_in input line buffer, and
	may be overwritten. Each field already has a NUL byte
	and text within them can be overwritten or truncated,
	but not expanded, and will often be recycled for each got_line_1()

	Theres an adjustment to be made to the source, but THIS accepts
	packets of CTXT=self CTXT.got_line_1()

	The
*/
class io_csv_in : public GRP_lib_base
{
 public:
	zfile_line_in z;
	dyn_array<char *> fields;
	int nfield;

	io_csv_in();
 virtual
	~io_csv_in();
	bool load_file( str0 filename );
	bool load_file( str2 * filename );
	bool load_line( char * line ); // line is overwritten with NUL's
	bool load_line_x( str0 line ); // line is overwritten with NUL's
	bool load_text( char * text ); // text is overwritten with NUL's
	void print_fields();

 virtual
	void got_blank_line();
 virtual
	void got_comment();
 virtual
	bool got_line_1();
};

#endif
