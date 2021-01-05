#ifndef ftp_sess_H
#define ftp_sess_H

class ftp_sess;

#include "ftp_sel_ftp_data.h"
#include "ftp_sel_ftp_ctrl.h"

class ftp_sess : obj_ref
{
 public:
	obj_hold<ftp_sel_ftp_ctrl>	ctrl;

	ftp_sess()
	{
	}
	~ftp_sess()
	{
	}

	void test1()
	{
	}

};


#endif
