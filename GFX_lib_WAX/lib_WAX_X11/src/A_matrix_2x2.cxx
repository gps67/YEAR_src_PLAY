#include "A_matrix_2x2.h"

#define _USE_MATH_DEFINES
#include <math.h>

static const double PI = M_PI; // M_PI from math,h with #define

inline double rads_from_degs( double degs )
{
	// 180 degs is one PI
	return degs / 180.0 * PI; // left to right
}

inline double degs_from_rads( double rads )
{
	// two PI rads is 2 half circles
	return rads / PI * 180.0; // left to right
}

/*
./man_k_matrix
FcMatrixCopy (3)     - Copy a matrix
FcMatrixEqual (3)    - Compare two matrices
FcMatrixInit (3)     - initialize an FcMatrix structure
FcMatrixMultiply (3) - Multiply matrices "{ B2 = A * B1 }" daft way
FcMatrixRotate (3)   - Rotate a matrix
FcMatrixScale (3)    - Scale a matrix
FcMatrixShear (3)    - Shear a matrix
FcPatternAdd-Type (3) - Add a typed value to a pattern
FcPatternGet-Type (3) - Return a typed value from a pattern
*/


WAX::
A_matrix_2x2::
A_matrix_2x2()
{
	set_to_unit();
}

bool
WAX::
A_matrix_2x2::
set_to_unit()
{
	FcMatrixInit( &matrix );
	return true;
}

bool
WAX::
A_matrix_2x2::
set_to_rotate( double angle )
{
	double c = cos(rads_from_degs(angle));
	double s = sin(rads_from_degs(angle));
	FcMatrixRotate( &matrix, c, s );
	return true;
	matrix.xx = c;
	matrix.xy = -s;
	matrix.yx =  s;
	matrix.yy = c;
	return true;
	// xx xy
	// yx yy
}

bool
WAX::
A_matrix_2x2::
rotate_by( double angle )
{
	double c = cos(rads_from_degs(angle));
	double s = sin(rads_from_degs(angle));
	FcMatrixRotate( &matrix, c, s );
	return true;
	// c -s
	// s  c
}


bool
WAX::
A_matrix_2x2::
scale_by( double sx, double sy )
{
	FcMatrixScale( & matrix, sx, sy );
	return true;
	// sx 0
	// 0 sy
}


bool
WAX::
A_matrix_2x2::
shear_by( double sx, double sy )
{
	FcMatrixShear( & matrix, sx, sy );
	return true;
	// 1  sx
	// sy 1
}

bool
WAX::
A_matrix_2x2::
move_point( double & x, double & y )
{
	double x2 = matrix.xx * x + matrix.xy * y;
	double y2 = matrix.yx * x + matrix.yy * y;
	x = x2;
	y = y2;
	return true;
}
