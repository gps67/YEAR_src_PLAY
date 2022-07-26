#ifndef TJ_values_H
#define TJ_values_H

#include "str0.h"
#include "buffer1.h"

namespace TJ {


 struct quality_t 
 {
	int val;
 	static const int DEFAULT_QUALITY = 100;

	quality_t() { val = -1; }
	bool is_UNSET() { return val == -1; }
	bool is_SET() { return val != -1; }
	int get_val() { return is_UNSET() ?  DEFAULT_QUALITY : val; }

	bool parse_quality_value( const char * str ); // val=atoi(argv[i])
 };

 struct subsamp_t
 {
	int val;
 	static const int DEFAULT_SUBSAMP = 100;

	subsamp_t() {
		val = -1; //  UNSET
	}

	bool is_UNSET() { return val == -1; }
	bool is_SET() { return val != -1; }
	int get_val() { return is_UNSET() ?  DEFAULT_SUBSAMP : val; }
	
		// if( pixelFormat == TJPF_GRAY ) // out of reach
        	//	outSubsamp = TJSAMP_GRAY; // TODO caller

 static
	STR0 name_of_val(int intval);
	STR0 get_name();
	bool parse_subsamp_value( str0 str );

 };

}; // namespace

#endif
