#ifndef SPOUT_xy_H
#define SPOUT_xy_H

/*
	float_32 gives 23 bits mantissa // +1.23  // 8M 
	C uses double_64 for operations float_32 for storage and AVAR

	page 2048 x 1024 is 2^11 x 2^10	 SQRT of f32

 unused FP8
  E4M3
 	3: mantissa
	4: exponent
 	1: sign
  E5M2
 	2: mantissa
	5: exponent
 	1: sign
 unused FP16
  FP16 # _1_5_10
       10: mantissa
        5: exponent
        1: sign
  BF16 # _1_8_7
        23: mantissa
        8: exponent
        1: sign

 F32
 	23
 #
 unused FP4_homebrew # 1 2 1 # f4
  -12 -8 -6 -4 -3 -2 -1 -0
                          +0 1 2 3 4 6 8 12
*/

namespace SPOUT {

struct XY_t
{
	float x;
	float y;

	XY_t &  set_zero()
	{
		x = 0.0;
		y = 0.0;
		return * this;
	}

	XY_t()
	{
		set_zero();
	}

	XY_t( float _x, float _y )
	: x(_x)
	, y(_y)
	{
	}

	XY_t & set( float _x, float _y )
	{
		x = _x;
		y = _y;
		return *this;
	}

	bool xy_print_3d( buffer2 & out )
	{
		// NB leading space
		// NB trailing space
		return out.print( " %3d %3d ", (int) x, (int) y );
	}

}; // class
}; // namespace

#endif
