#include "X_FontStr.h"

using namespace WAX;

 void
 X_FontStr ::
 init_all( const char * val) {

	 full_name . clear();
	 foundry = val;
	 family = val;
	 weight = val;
	 slant = val;
	 set_width = val;
	 unknown = val;
	 pixels = val;
	 point_10 = val;
	 dpi_h = val;
	 dpi_v = val;
	 spacing = val;
	 width_10 = val;
	 cset = val;
 }

 const char *
 X_FontStr ::
 join()
 {
	full_name . clear();
 	full_name.print("-%s", (STR0) foundry );
 	full_name.print("-%s", (STR0) family );
 	full_name.print("-%s", (STR0) weight );
 	full_name.print("-%s", (STR0) slant );
 	full_name.print("-%s", (STR0) set_width );
 	full_name.print("-%s", (STR0) unknown );
 	full_name.print("-%s", (STR0) pixels );
 	full_name.print("-%s", (STR0) point_10 );
 	full_name.print("-%s", (STR0) dpi_h );
 	full_name.print("-%s", (STR0) dpi_v );
 	full_name.print("-%s", (STR0) spacing );
 	full_name.print("-%s", (STR0) width_10 );
 	full_name.print("-%s", (STR0) cset );
	return full_name;
 }

 void
 print_field( buffer1 & buf, const char * name )
 {
 	printf(" %-12s %s \n", name, (STR0) buf );
 }

#define PRINT_FIELD( name ) print_field( name, #name )

 void
 X_FontStr ::
 print_fields()
 {
 	PRINT_FIELD( foundry );
 	PRINT_FIELD( family );
 	PRINT_FIELD( weight );
 	PRINT_FIELD( slant );
 	PRINT_FIELD( set_width );
 	PRINT_FIELD( unknown );
 	PRINT_FIELD( pixels );
 	PRINT_FIELD( point_10 );
 	PRINT_FIELD( dpi_h );
 	PRINT_FIELD( dpi_v );
 	PRINT_FIELD( spacing ); // m == monospace ?p == prop
 	PRINT_FIELD( width_10 );
 	PRINT_FIELD( cset );
 }

 // not in the class
 bool parse_upto_dash (
 	const char ** s,
	buffer1 & buf
 )
 {
 	bool ok = true;
 	const char * s0 = *s;
 	const char * s1 = s0;
	while((*s1) && ( *s1 != '-' ) ) s1 ++;
	int len = s1 - s0;
	// parse upto dash - and eat the dash
	if( *s1 == '-' ) s1++;
	else if(len==0) {
		ok= FAIL("end of font str");
	}
	buf.set( s0, len );
	*s = s1;
	return ok;
 }

 bool
 X_FontStr ::
 parse( const char * str ) {
	const char * s = str;
	if( *s == '-' ) s++; else { return FAIL("expected -"); }
 	if(!parse_upto_dash( &s, foundry   ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, family    ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, weight    ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, slant     ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, set_width ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, unknown    ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, pixels    ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, point_10  ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, dpi_h     ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, dpi_v     ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, spacing   ) ) return FAIL_FAILED();
 	if(!parse_upto_dash( &s, width_10  ) ) return FAIL_FAILED();
 	//parse_upto_dash( &s, cset ); may contain -
	cset = s;
	join();
	return true;
 }

 void 
 X_FontStr ::
 set_courier()
 {
 	init_all("");
	foundry = "adobe";
	family = "courier";
	set_weight_medium();
	set_slant_roman();
	set_width_normal(); // helv + courier need it
	set_cset_latin1();
	set_point("18");
 }

 void 
 X_FontStr ::
 set_helvetica()
 {
 	init_all("");
	foundry = "adobe";
	family = "helvetica";
	set_weight_medium();
	set_slant_roman();
	set_width_normal(); // helv + courier need it
	set_cset_latin1();
	set_point("18");
 }


 void 
 X_FontStr ::
 set_usable_defaults()
 {
  if(0) {
	set_dpi_150();
	set_dpi_100();
	set_dpi_75();
	set_point( "72" ); // 
	set_slant_roman();
	set_slant_italic();
	set_weight_bold();
	set_weight_normal(); // NO NO
  }
 }


/*
xlsfonts | grep -i courier

*	-adobe
*	-courier
*	-bold	-medium
*	-i -r -o
	-normal
	--0-0-0-0
	-p	-m
	-0
*	-iso8859-1 -2 -15

		-1	Latin1
		-2	Latin2
		-15	Latin9 ?? not -1-15

*	-bitstream
*	-courier 10 pitch
*	-bold -medium
*	-i -r
	-normal
	--0-0-0-0
	-m 
	-0
*	-adobe-standard

		-adobe-standard
		-ascii-0
		-iso10646-1
		-iso8859-1

-adobe-courier-bold-i-normal--0-0-0-0-p-0-iso8859-1
-adobe-courier-bold-i-normal--0-0-0-0-p-0-iso8859-15
-adobe-courier-bold-i-normal--0-0-0-0-p-0-iso8859-2
-adobe-courier-bold-o-normal--0-0-0-0-p-0-iso8859-1
-adobe-courier-bold-o-normal--0-0-0-0-p-0-iso8859-15
-adobe-courier-bold-o-normal--0-0-0-0-p-0-iso8859-2
-adobe-courier-bold-r-normal--0-0-0-0-p-0-iso8859-1
-adobe-courier-bold-r-normal--0-0-0-0-p-0-iso8859-15
-adobe-courier-bold-r-normal--0-0-0-0-p-0-iso8859-2
-adobe-courier-medium-i-normal--0-0-0-0-p-0-iso8859-1
-adobe-courier-medium-i-normal--0-0-0-0-p-0-iso8859-15
-adobe-courier-medium-i-normal--0-0-0-0-p-0-iso8859-2
-adobe-courier-medium-o-normal--0-0-0-0-p-0-iso8859-1
-adobe-courier-medium-o-normal--0-0-0-0-p-0-iso8859-15
-adobe-courier-medium-o-normal--0-0-0-0-p-0-iso8859-2
-adobe-courier-medium-r-normal--0-0-0-0-p-0-iso8859-1
-adobe-courier-medium-r-normal--0-0-0-0-p-0-iso8859-15
-adobe-courier-medium-r-normal--0-0-0-0-p-0-iso8859-2
-bitstream-courier 10 pitch-bold-i-normal--0-0-0-0-m-0-adobe-standard
-bitstream-courier 10 pitch-bold-i-normal--0-0-0-0-m-0-ascii-0
-bitstream-courier 10 pitch-bold-i-normal--0-0-0-0-m-0-iso10646-1
-bitstream-courier 10 pitch-bold-i-normal--0-0-0-0-m-0-iso8859-1
-bitstream-courier 10 pitch-bold-r-normal--0-0-0-0-m-0-adobe-standard
-bitstream-courier 10 pitch-bold-r-normal--0-0-0-0-m-0-ascii-0
-bitstream-courier 10 pitch-bold-r-normal--0-0-0-0-m-0-iso10646-1
-bitstream-courier 10 pitch-bold-r-normal--0-0-0-0-m-0-iso8859-1
-bitstream-courier 10 pitch-medium-i-normal--0-0-0-0-m-0-adobe-standard
-bitstream-courier 10 pitch-medium-i-normal--0-0-0-0-m-0-ascii-0
-bitstream-courier 10 pitch-medium-i-normal--0-0-0-0-m-0-iso10646-1
-bitstream-courier 10 pitch-medium-i-normal--0-0-0-0-m-0-iso8859-1
-bitstream-courier 10 pitch-medium-r-normal--0-0-0-0-m-0-adobe-standard
-bitstream-courier 10 pitch-medium-r-normal--0-0-0-0-m-0-ascii-0
-bitstream-courier 10 pitch-medium-r-normal--0-0-0-0-m-0-iso10646-1
-bitstream-courier 10 pitch-medium-r-normal--0-0-0-0-m-0-iso8859-1

*/
