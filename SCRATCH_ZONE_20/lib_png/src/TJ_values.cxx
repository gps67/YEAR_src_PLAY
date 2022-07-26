#include "TJ_values.h"
#include "dgb.h"
// #include "enum_tbl.h"
#include "tbl_enum.h"

 extern
 enum_tbl_t tbl_samp;

using namespace TJ;

	bool quality_t:: parse_quality_value( const char * str ) {
		int q = atoi(str);
		if (q < 1 || q > 100) {
			return FAIL("-q %d # must be 1 to 100", q );
		}
		val = q;
		return true;
	}

//	static
	STR0 subsamp_t:: name_of_val(int intval) {
		return tbl_samp.name_of_value( intval );
	}

	STR0 subsamp_t:: get_name() {
		return name_of_val(get_val()); // convert UNSET to DEFAULT
	}

	bool subsamp_t:: parse_subsamp_value( str0 str ) {
		int idx = 0;
		if(!tbl_samp.lookup_name( str, idx )) {
			return FAIL("expected TJSAMP_444 got %s", (STR0)str );
		}
		val = tbl_samp.rows[idx].val_int;
		return PASS("%s == %d", (STR0) str, val );
	}


