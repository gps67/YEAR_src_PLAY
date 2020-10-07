
#include "dgb.h"

#include <math.h> // sin cos
#include "cpu_var_Name.h"

using namespace WAX;

	// static aim and init
// OK	bool cpu_var:: init_done;
	bool cpu_var:: init_done = false;

 cpu_var :: cpu_var() {
 	if(!init()) throw "ERR";
 }

 cpu_var :: ~cpu_var() {
 	if(!done()) FAIL_FAILED();
 //	if(!done()) throw "ERR"; // dont throw // 
 }

 bool cpu_var :: init() {
 	if(init_done) {
		WARN("already init_done");
		return true; // but OK
	}
	// LOCK HERE?
	init_done = true;
	return true;
 }

 bool cpu_var :: done() {
 	if(!init_done) {
		WARN("already init_done false");
		return true; // unusual but OK
	}
	// LOCK
	init_done = false;
	PASS("FT_Done_FreeType");
	return true;
 }

 bool cpu_var:: OK_or_ERR( int errr )
 {
  if( errr ) {
  	return WARN("ERR %d - %s", errr, "Error_String( errr )");
  } else {
  	return true;
  }
 }


#define int_from_fixed_6(x) ((x+32)>>6) // +32 rounds to 64
// #define FT_Fixed_from_float(f)( (FT_Fixed)( (f) * (float) 0x10000L )) // << 16
#define FT_Fixed_from_float(f)( (FT_Fixed)( (f) * 0x10000L )) // << 16
#define PI 3.1415926
#define rads_from_degress( degs ) ( 2.0 * PI * degs / 360.0 ) //

  // angle above horizontal, anti-clockwise
  // -ve below horizontal, clockwise
  // float angle = rads_from_degress( -23.0 );

 bool cpu_var :: test1() {

	return PASS("OK");
	return true;
 }


