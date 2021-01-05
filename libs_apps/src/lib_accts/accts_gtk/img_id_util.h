#ifndef img_id_util_H
#define img_id_util_H

#include "date_glib.h"
#include "str1.h"
#include "buffer2.h"

namespace ACCTS {

/*!
	Img_Id_Util is how the filename and comment comply

	There is also an issue with the dir ... using global
*/
class Img_Id_Util : public GRP_lib_accts
{
	buffer2 buf;
 public:

	date_glib date;
        str1 str_date;
        str1 str_cat;
        str1 str_amnt;
        str1 str_desc;
        str1 str_acct;

        str1 str_imgid;
        str1 str_cmnt;

	str1 dir;
	str1 dir_list[4];

	~Img_Id_Util();
	///// LURK - fixed path name - should use year_start-+1
	Img_Id_Util(
	//	str0 d1 = "/tmp/img", // where images are kept
		str0 d1 = "/home/gps/Accts_CD_Prep_1/2007-2008/images/tmp",
		str0 d2 = "./IMG/Receipts",
		str0 d3 = "./IMG/BankStatements",
		str0 d4 = "./IMG/CertsDocs"
	);
	void recalc_imgid();
	void recalc_comment();
	void recalc_comment_and_imgid();

	str0 path_base();
	str0 path_file( str0 dotext);
	str0 path_file_pnm() { return path_file( ".pnm" ); }
	str0 path_file_png() { return path_file( ".png" ); }
};

}; // NAMESPACE

#endif
