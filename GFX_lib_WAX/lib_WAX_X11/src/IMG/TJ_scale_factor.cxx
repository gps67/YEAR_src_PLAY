
// .h // #include <turbojpeg.h>
#include "TJ_scale_factor.h"
#include "buffer1.h"
#include "dgb.h"
#include <math.h> // fabs

using namespace TJ;


scale_factor_t:: scale_factor_t()
{
	init_1();
}

bool scale_factor_t:: init_1() {
	scalingFactor = { 1, 1 };
	return true;
}

bool scale_factor_t:: scale_is_1() {
  return
	scalingFactor.num == 1 &&
	scalingFactor.denom == 1;
}

// scale factor is same for (X,Y), but might not be, eg fax
bool scale_factor_t:: apply_to_W_H( int & width, int & height ) {
	int w1 = width;
	int h1 = height;
	width = TJSCALED( width, scalingFactor );
	height = TJSCALED( height, scalingFactor );
	INFO("from (%d,%d) to (%d,%d)", w1, h1, width, height );
	return true;
}

double scale_factor_t:: get_factor()
{
	return 
	  	(double) scalingFactor.num /
		(double) scalingFactor.denom;
}

scale_factors_t:: scale_factors_t()
: numScalingFactors( 0 )
, scalingFactors( NULL )
{
	get_scaling_factors();
}

bool scale_factors_t:: get_scaling_factors() {
	if( numScalingFactors ) {
		WARN("called twice");
		WARN("reporting twice");
		return true;
	}
	scalingFactors = tjGetScalingFactors(&numScalingFactors);
	if(!scalingFactors) {
		return FAIL("tjGetScalingFactors() returned NULL");
	}
	return true;
}

STR0 scale_factors_t:: list_scaling_factors( buffer1 & buf ) {
	const char * sep = "";
	for (int j = 0; j < numScalingFactors; j++) {
		buf.print("%s%d/%d", sep, 
		  scalingFactors[j].num,
		  scalingFactors[j].denom);
		sep = ", ";
	}
	return (STR0) buf;
/*
	  2/1,	16 / 8
	 15/8,	15 / 8
	  7/4,	14 / 8
	 13/8,  14 / 8
	  3/2,  12 / 8
	 11/8,  11 / 8
	  5/4,  10 / 8
	  9/8,   9 / 8
	  1/1,   8 / 8
	  7/8,   7 / 8
	  3/4,   6 / 8
	  5/8,   5 / 8
	  1/2,   4 / 8
	  3/8,   3 / 8
	  1/4,   2 / 8
	  1/8    1 / 8

	Old camera did 1600x1200 0.34 MB 72dpi
	Cant see a way of editing attrs,

	2048x1370 
	1296x1728
	

*/
}

bool scale_factors_t:: set_eighth( int eighth , scale_factor_t & scale_factor ) {
	if( (1 <= eighth ) and ( eighth <= 16 )) {
		buffer1 ratio;
		ratio.print("%d/8", eighth);
		return parse_scale_value( (STR0) ratio, scale_factor );
	} else {
		return FAIL("expected eighth [1..16] got %d", eighth );
	}
}


bool scale_factors_t:: parse_scale_value( const char * str, scale_factor_t & scale_factor ) {
	buffer1 buf1;
	INFO("OPTIONS %s", list_scaling_factors( buf1 ) );

	double wanted_val = 1.0; // safe default

	int temp1 = 0, temp2 = 0, j;
	if (sscanf( str, "%d/%d", &temp1, &temp2 ) == 2) {
		wanted_val = (double) temp1 / (double) temp2;
	} else if (sscanf( str, "%lf", &wanted_val ) == 1 ) {
		// continue with float value
	} else {
		buffer1 buf;
		INFO("OPTIONS %s", list_scaling_factors( buf ) );
		return FAIL("expected 0.375 3/8 got %s", str );
	}

#if 0
	for (j = 0; j < numScalingFactors; j++) {
	  if(temp1 != scalingFactors[j].num) continue;
	  if(temp2 != scalingFactors[j].denom) continue;
	  // we dont support same ratio only exact the same pair
	  // no multiples

	  scale_factor.scalingFactor = scalingFactors[j];
	  return PASS("%s is %d/%d", str, temp1, temp2 );
	  return true;
	}
#endif

	int nearest_idx = 0;
	double nearest_val = 20.0; // max is 16/8 is 2.0
	double nearest_away = 20.0; // far too far // bigger than first item
	for (int row_idx = 0; row_idx < numScalingFactors; row_idx++) {
	  double row_val = (
	  	(double) scalingFactors[row_idx].num /
		(double) scalingFactors[row_idx].denom
	  );
	  double row_away = fabs( row_val - wanted_val );
	  if(0) INFO("want %5.3lf row_val %5.3lf", wanted_val, row_val );
	  if( row_away < nearest_away ) {
	  	nearest_away = row_away;
	  	nearest_val = row_val;
	  	nearest_idx = row_idx;
		scale_factor.scalingFactor = scalingFactors[row_idx];
	  }
	}

	// errrm : nearest would be factor not subtract // but OK
	// TODO permit factors of 2 of 2 // multiple fractions of 2 of 2

	temp1 = scalingFactors[nearest_idx].num ;
	temp2 = scalingFactors[nearest_idx].denom;
	return PASS("want %5.3lf got %5.3lf==%d/%d from '%s'",
		wanted_val, nearest_val, temp1, temp2, str);

	buffer1 buf;
	INFO("got %s OPTIONS %s", str, list_scaling_factors( buf ) );
	return false;
}

