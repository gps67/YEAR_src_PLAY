#ifndef io_zlib_out_H
#define io_zlib_out_H

#include <zlib.h>
#include <stdio.h> // FILE
#include "ints.h"
#include <GLOBAL.H> // str0

class data_set;


/*!
	io_zlib_out does binary, zlib compression, output. 
	Text is binary too.

	It might be an idea to make this a varient of the mem buffer?
	Or a downstream fd.
*/
class io_zlib_out : public GRP_lib_base
{
	gzFile f;
	FILE * f_plain;
 protected:
	bool io_error;
 public:
	virtual ~io_zlib_out();
	io_zlib_out();
	bool close();
	bool open( str0 filename, int compression_level );
	
	bool puts( str0 s );
	bool puts_n_spaces(int nsp );

	virtual bool io_err( str0 s ); //!< always returns false
};

#endif
