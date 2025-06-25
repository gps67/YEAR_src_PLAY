#ifndef MM3_FILE_TBL
#define MM3_FILE_TBL

#include "mm3_file_data.h"

class mm3_file_tbl : public mm3_file_data
{
 public:
	void decl_tbl( const char * name, const char * list )
	{
		fprintf( stdout, "%s, %s\n", name, list );
	}
	void print_tbl_name( const char * name )
	{
		fprintf( stdout, "%s", name );
	}

	void print_tbl_field( own_str & name, int w = 1 )
	{
		fprintf( stdout, ", %*s", w, name.str );
	}

	void print_tbl_field( const char * name, int w = 1 )
	{
		fprintf( stdout, ", %*s", w, name );
	}

	void print_tbl_eoln()
	{
		fprintf( stdout, "\n" );
	}

	void print_tbl_blank_line()
	{
		fprintf( stdout, "\n" );
	}

	const char * mk_img( int year, int mm, int dd, char * cat );
	void print_transaction_tbl(a_trans & t);
	void print_transfer(a_trans & t);
	void print_transfer_tbl(a_trans & t);

	void print_tbl();

};

#endif
