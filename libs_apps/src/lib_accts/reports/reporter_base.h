#ifndef report_base_H
#define report_base_H

#include "obj_hold.h"
#include "dset.h"
#include "tbl_col_spec_plus.h"
#include "dgb_fail.h"
#include "tbl_col_spec_date_rel.h"

#include "rpt_help_tbl.h"
#include "rpt_help_TX.h"

namespace ACCTS {

class reporter_base : public obj_ref
{
 public:
	data_set * dset;

	help_TRANS	TRANS;
	help_XFER	XFER;
	help_ACCT	ACCT;
	help_CAT	CAT;
	help_VAT	VAT;
	help_ERA	ERA;
	help_REF	REF;

	~reporter_base();

	reporter_base( data_set * _dset);
};

}; // NAMESPACE
#endif
