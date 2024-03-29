#include "dgb.h"
#include "str0.h"
#include "tbl_enum.h"
#include "buffer1.h"
 
bool enum_tbl_t:: dump() {
	INFO("table %s pfx %s N_rows %d desc %s",
		(STR0)tbl_name,
		(STR0)pfx,
		N_rows,
		(STR0)tbl_desc
	);
	for( int i = 0; i< N_rows; i++ ) {
		INFO("%s[%d] { '%s' '%s' %d }",
			(STR0)tbl_name,
			i,
			rows[i].name,
			rows[i].desc,
			rows[i].val_int
		);
	}
	return true;
}

bool enum_tbl_t:: lookup_val( int val, int & idx ) {
	for( int i = 0; i< N_rows; i++ ) {
		if( val == rows[i].val_int ) {
			idx = i;
			return true;
		}
	}
	// caller _expects_ it to be found
	return FAIL("not found val %d tbl name '%s' desc '%s'", val, (STR0) tbl_name, (STR0) tbl_desc );
}

bool enum_tbl_t:: lookup_name( str0 name, int & idx ) {
	// also look for pfx_name // poss waste pfx_pfx_name
	buffer1 buf; buf.print("%s%s", (STR0) pfx, (STR0) name );
	str0 pfx_name = (STR0) buf;
	for( int i = 0; i< N_rows; i++ ) {
		if( (name == rows[i].name )
		 || (name == rows[i].alias ) 
		 || (pfx_name == rows[i].name )  
		) {
			idx = i;
			return true;
		}
	}
	INFO("cant find name(%s) in table(%s)", (STR0)name, (STR0)tbl_name );
	return false;
}

const char * enum_tbl_t:: name_of_value( int val )
{
	int idx = -1;
	if(!lookup_val( val, idx )) {
		FAIL_FAILED();
		return "(ABSENT)";
	}
	return rows[idx].name;
}



