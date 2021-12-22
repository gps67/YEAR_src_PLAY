#ifndef file_alloc_H
#define file_alloc_H

#include "buffer1.h"
#include "dir_list.h"

/*!
	Scan a dir for the HIGHEST numbered file plus one.

	Retain the val and calculate filenames, (single instance etc)

	Gaps are never reused, and hi is only reserved by actually
	creating the file or any deletable).

	EG pfx == "scan_" will find the following and suggest 1236 as next
	NB _orig must not start with a digit

		scan_ 1234 _orig .pnm
		scan_ 1235 _nail .png

	File_Alloc_chr does similar with 0="A" 1="B" (fails at "Z")
	it DOESNT YET detext PFX [_] [A] -file with no trailing _A or A

*/
class File_Alloc_base : public GRP_lib_base
{
 public:
	dir_reader_base d;
	int hi;
	buffer1 buf;
	// int val;

	const char * dir;
	const char * pfx;
	int d_fmt;		// 0 means use char, >0 means use n digits

	~File_Alloc_base();
	File_Alloc_base(
		const char * _dir,
		const char * _pfx,
		int _d_fmt
	);
	int find_hi_unused();
	const char * mk_base( int val );
	const char * mk_pathbase( int val );
	int find_hi_unused_chr();
	int find_hi_unused_int();
	const char * mk_base_int( int val );
	const char * mk_base_chr( int val );
};

class File_Alloc_int : public File_Alloc_base
{
 public:
	~ File_Alloc_int() {}
	File_Alloc_int(
		const char * _dir,
		const char * _pfx,
		int _d_fmt
	)
	: File_Alloc_base( _dir, _pfx, _d_fmt )
	{
	}
};

class File_Alloc_chr : public File_Alloc_base
{
 public:
	~ File_Alloc_chr() {}
	File_Alloc_chr(
		const char * _dir,
		const char * _pfx
	)
	: File_Alloc_base( _dir, _pfx, 0 )
	{
	}
};

#endif
