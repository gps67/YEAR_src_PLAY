#ifndef help_RPT_plus_H
#define help_RPT_plus_H

#include "rpt_help_RPT_base.h"
#include "rpt_tag.h"

namespace ACCTS {

	class help_RPT_plus : public help_RPT_base 
	{
	 public:
		help_RPT_plus(
			data_set * dset,
			reporter_base * rpt1,
			str0 tablename
		);
	};

}; // NAMESPACE
#endif
