#ifndef align_val_H
#define align_val_H

#include "str1.h"
#include "amnt_t.h"

namespace BASE1 {

/*!
	integer values for left-right alignment
*/
// typedef
enum align_enum
{
	align_none,
	align_left,
	align_right,
	align_center

	// _FILL is a GtkThing
};

/*!
	An interger align_val val, and associated functions
*/
struct align_val : GRP_lib_base
{
	align_enum val;

	//! constructor - default left
	align_val() { set_align_left(); }

	//! copy constructor
	align_val( align_enum _val )
	{
		val = _val;
	}

	//! copy constructor
	align_val( const align_val & v2 )
	{
		val = v2.val;
	}


	//! =
	align_val & operator=( const align_val & v2 )
	{
		val = v2.val;
		return *this;
	}

	//! function as well as enum symbol ?
	void set_align_left()   { val = align_left; }

	//! function as well as enum symbol ?
	void set_align_right()  { val = align_right; }

	//! function as well as enum symbol ?
	void set_align_center() { val = align_center; }


	/*
		GtkJustification is an enum defined in gtkenums.h
		but its an int here, so you dont need the header file
	*/
	//! convert align_val to gtk Value - ...
	// GtkJustification as_gtk_justification();
	int as_gtk_justification();

	//! convert name to val (and assign)
	bool set_by_name( str0 name );

	str0 get_val_name();
};

}; // NAMESPACE

using namespace BASE1;

#endif
