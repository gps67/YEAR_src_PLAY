#ifndef TJ_scale_factor_H
#define TJ_scale_factor_H

#include <turbojpeg.h>
#include "buffer1.h"

namespace TJ {

 struct scale_factor_t {
 	tjscalingfactor scalingFactor;

	scale_factor_t();
	bool init_1();
	bool scale_is_1();
	bool apply_to_W_H( int & width, int & height );
	double get_factor();
 };

 struct scale_factors_t {

	int numScalingFactors;
	tjscalingfactor *scalingFactors;

	scale_factors_t();
	bool get_scaling_factors();

	const char * list_scaling_factors( buffer1 & buf );
	bool set_eighth( int eighth , scale_factor_t & scale_factor );
	bool parse_scale_value( const char * str, scale_factor_t & scale_factor );
 }; // struct


}; // namespace TJ
#endif
