#include "TJ_values.h"
#include "dgb.h"
// #include "enum_tbl.h"
#include "tbl_enum.h"
#include "tbl_enum_MACROS.h"
#include <turbojpeg.h>

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
	int subsamp_t:: get_DEFAULT_subsamp() {
 		return TJSAMP_444;
	}

//	static
	STR0 subsamp_t:: name_of_val(int intval) {
		INFO("HERE");
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

namespace TJ {

	// declared class member functions are OK woth "using namespace TJ;"
	// declared extern vars are NOT OK with using .. these need to be inside
	// hence above line

 ENUM_TBL( "TJSAMP_", tbl_samp, "output resample", rows_samp, { 

	ITEM2( TJSAMP_444,  "4:4:4" )
	ITEM2( TJSAMP_422,  "4:2:2" )
	ITEM2( TJSAMP_420,  "4:2:0" )
	ITEM2( TJSAMP_GRAY, "Grayscale" )
	ITEM2( TJSAMP_440,  "4:4:0" )
	ITEM2( TJSAMP_411,  "4:1:1" )

 })
	// #define TJ_NUMSAMP  6

 ENUM_TBL( "TJPF_", tbl_PF, "pixel format", rows_PF, { // #define TJ_NUMPF  12

	ITEM3( TJPF_RGB,  "RGB", "descr" )
	ITEM3( TJPF_BGR,  "BGR", "descr" )
	ITEM3( TJPF_RGBX, "RGBX", "RGB X bytes order ABCD lohi" )
	ITEM3( TJPF_BGRX, "BGRX", "descr" )
	ITEM3( TJPF_XBGR, "XBGR", "descr" )
	ITEM3( TJPF_XRGB, "XRGB", "descr" )
	ITEM3( TJPF_GRAY, "GRAY", "descr" )
	ITEM3( TJPF_RGBA, "RGBA", "RGB A==0xFF ABCD lohi" )
	ITEM3( TJPF_BGRA, "BGRA", "descr" )
	ITEM3( TJPF_ABGR, "ABGR", "descr" )
	ITEM3( TJPF_ARGB, "ARGB", "descr" )
	ITEM3( TJPF_CMYK, "CMYK", "descr" )
	ITEM3( TJPF_UNKNOWN,"unset", "default for accept INPUT" )

 })


 // CS color space
 ENUM_TBL( "TJCS_", tbl_CS, "pixel format", rows_CS, { // #define TJ_NUMCS  5

	ITEM3( TJCS_RGB,   "rgb",   "descr" )
	ITEM3( TJCS_YCbCr, "ycbcr", "descr" )
	ITEM3( TJCS_GRAY,  "gray",  "descr" )
	ITEM3( TJCS_CMYK,  "cmyk",  "descr" )
	ITEM3( TJCS_YCCK,  "ycck",  "descr" )

 } )
}; // namespace 
