#ifndef A_matrix_2x2_H
#define A_matrix_2x2_H

	// FcMatrix // a 2x2 matrix used by xft
	// PKGS += fontconfig

#include <X11/Xft/Xft.h>

namespace WAX {

  struct A_matrix_2x2 {
	FcMatrix matrix;
	// i32 : FT_Fixed : 16.16 fraction [-32K [+32K grain 1/(64K)
	// xx xy // fttypes.h
	// yx yy

	A_matrix_2x2();

	bool set_to_unit();
	bool set_to_rotate( double angle ); // anti clockwise
	bool rotate_by( double angle ); // anti clockwise
	bool scale_by( double sx, double sy );
	bool scale_by( double s ) {return scale_by( s, s ); }
	bool shear_by( double sx, double sy );
	bool move_point( double & x, double & y );

  };

}; // namespace

#endif

