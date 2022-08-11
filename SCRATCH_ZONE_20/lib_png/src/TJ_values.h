#ifndef TJ_values_H
#define TJ_values_H

#include "str0.h"
#include "buffer1.h"
#include "tbl_enum.h"

// #include <turbojpeg.h> // TJSAMP_444

namespace TJ {

 extern enum_tbl_t tbl_samp;
 extern enum_tbl_t tbl_PF;
 extern enum_tbl_t tbl_CS;


 struct quality_t 
 {
	int val;

	static const int DEFAULT_QUALITY = 95;

	quality_t() { val = -1; }
	bool is_UNSET() { return val == -1; }
	bool is_SET() { return val != -1; }
	int get_val() { return is_UNSET() ?  DEFAULT_QUALITY : val; }


	bool parse_quality_value( const char * str ); // val=atoi(argv[i])
 };

 struct subsamp_t
 {
	int val;

	subsamp_t() {
		val = -1; //  UNSET
	}

	bool is_UNSET() { return val == -1; }
	bool is_SET() { return val != -1; }
	int get_val() { return is_UNSET() ?  get_DEFAULT_subsamp() : val; }
	static
	int get_DEFAULT_subsamp(); // TSAMP_444
	
		// if( pixel_format_tj == TJPF_GRAY ) // out of reach
        	//	outSubsamp = TJSAMP_GRAY; // TODO caller

 static
	STR0 name_of_val(int intval);
	STR0 get_name();
	bool parse_subsamp_value( str0 str );

 };

}; // namespace

#endif
