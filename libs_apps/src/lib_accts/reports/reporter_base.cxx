
// #include "rpt_help_RPT.h"
#include "reporter_base.h"

namespace ACCTS {

	reporter_base::
	~reporter_base()
	{
	}

	reporter_base::
	reporter_base( data_set * _dset)
	: dset( _dset)
	, TRANS( _dset )
	, XFER( _dset )
	, ACCT( _dset )
	, CAT( _dset )
	, VAT( _dset )
	, ERA( _dset )
	, REF( _dset )
	{
	}

}; // NAMESPACE


