
#pragma implementation "img_id_util.h"

#include "img_id_util.h"


#include "buffer2.h"
#include "file_alloc.h"
#include "e_print.h"

namespace ACCTS {


	Img_Id_Util::Img_Id_Util(
		str0 d1,
		str0 d2,
		str0 d3,
		str0 d4
	)
	: buf( 80 )
	, dir( "/tmp" ) // The actual value is set in ... CTOR! d1
	// , dir( "/home/gps/Accts_CD_Prep_1/2007-2008/images/tmp" ) // Change ...
	{
		dir_list[0].set( d1 );
		dir_list[1].set( d2 );
		dir_list[2].set( d3 );
		dir_list[3].set( d4 );
		dir.set( d1 );
	// 	if(!dir.str_len())
	// 		dir = "/tmp/img";
	}

	Img_Id_Util::~Img_Id_Util()
	{
	}

	void Img_Id_Util::recalc_imgid()
	{
	 	if(!dir.str_len())
		{
			e_print("Img_Id_Util::recalc_imgid() - NULL dir\n");
		}
		buf.clear();

		int year, mm, dd;
		date.get_year_mm_dd( year, mm, dd );

		// buf.print( "%2.2d%2.2d%s", mm, dd, (STR0)( str_cat ) );
		buf.print( "%4.4d%2.2d%2.2d_%s", year, mm, dd, (STR0)( str_cat ) );

		if( str_cat == "N2" ) {
			int stmt = 99;
			buf.print( "_%s_%d", (STR0) str_acct, stmt );
		}

if(0) gdb_invoke(true);

		str1 pfx = str0( buf );
		File_Alloc_chr file_alloc( dir, pfx );
		int id = file_alloc.find_hi_unused();
		str_imgid = file_alloc.mk_base( id );
	}

	void Img_Id_Util::recalc_comment()
	{
		buf.clear();
		buf.print( "date %s cat %s amnt %s acct %s desc '%s' ",
			STR0( date.tmp_year_mm_dd() ),
			STR0( str_cat ),
			STR0( str_amnt ),
			STR0( str_acct ),
			STR0( str_desc )
		);
		str_cmnt = str0( buf );
	}

	void Img_Id_Util::recalc_comment_and_imgid()
	{
		recalc_comment();
		recalc_imgid();
	}

	str0 Img_Id_Util::path_base()
	{
		buf.clear();
		buf.print( "%s/%s", (STR0) dir, (STR0) str_imgid );
		return buf;
	}

	str0 Img_Id_Util::path_file( str0 dotext)
	{
		buf.clear();
		buf.print( "%s/%s%s", (STR0) dir, (STR0) str_imgid, (STR0) dotext );
		return buf;
	}

}; // NAMESPACE

