#include "file_scan_base.h"

class scan_c1 : public file_scan_base
{
 public:

	scan_c1()
	: file_scan_base()
	{
	}

	~scan_c1()
	{
	}

	bool scan_file( const u8 * filename );

	int x_start;
	int y_start;

	void get_x_y_start();

bool scan_typeexpr();

	bool builtin_type_name( str1 & name );
	bool known_type_name( str1 & name );
	bool type_expr_left( str1 & expr );

	// these should return what they find ? or send to STBL ?
	bool var_decl();
	bool cpp_hash_line(); //#define #if #endif ...
	bool scan_pp_include_( str1 & filename, bool & local );

};
