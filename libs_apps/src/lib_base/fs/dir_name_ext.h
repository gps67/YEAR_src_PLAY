#ifndef dir_name_ext_H
#define dir_name_ext_H

#include "str2.h"
#include <unistd.h>
#include "buffer2.h"

/*!
	C++ equivalent to shells dirname, basename, etc
	(or maybe not ... but it should be ... )

	Also a rel-path calculator ...

	This would be better with a list of dir-steps
*/
class dir_name_ext  : public GRP_lib_base
{
 public:

	buffer2 temp;
	buffer2 cwd;
	buffer2 dir;
	buffer2 name;
	buffer2 name_ext;
	buffer2 ext;
	buffer2 orig_path_name;	// the original string //
	buffer2 full_path_name;
	buffer2 near_path_name;

	bool get_cwd();

	bool decode_filename( str0 path );
	bool dos_drive_prefix( str0 path );
	void test_print();
	bool test_on( str0 path );

	~ dir_name_ext() {}

	dir_name_ext( str0 filename )
	{
		decode_filename( filename );
	}

	dir_name_ext()
	{
	}

	void mk_BOTH_path_name(); // some component changed
	void mk_full_path_name();
	void mk_near_path_name();


 static
	void test1();
 static
	str0 abs_filename( str0 filename );
};

#endif
