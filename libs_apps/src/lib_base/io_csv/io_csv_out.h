#ifndef io_csv_out_H
#define io_csv_out_H

#include "zlib.h"
#include "ints.h"
#include "io_zlib_out.h"

#include "align_val.h"

class data_set;


class io_csv_out : public io_zlib_out
{
	bool boln;
	str0 comma;
	int w_residue;
 public:
	io_csv_out();
	~io_csv_out();
	bool open( str0 filename, bool compressed );
	bool put_field( str0 value, bool right=FALSE, int width = 0 );
	bool put_field( str0 value, align_val align, int width = 0 );
	bool eoln();
	bool close();
};

#endif
