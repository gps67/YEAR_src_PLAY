#ifndef int_time_val_H
#define int_time_val_H

#warning is this UNIX specific ?
#include <time.h>

struct int_time_val {

	time_t t;		// 32 or 64 bit time pas 2038

	int_time_val()
	{
		set_zero();
	}
	void set_zero()
	{
		t = 0;
	}
	bool is_zero()
	{
		return t == 0;
	}
	void add_seconds( int s )
	{
		t += s;
	}
	void set_to_current_time()
	{
		time( & t );
	}
	void set_to_current_time_plus_seconds( int s )
	{
		set_to_current_time();
		add_seconds( s );
	}
	bool operator < ( const int_time_val & rhs ) const
	{
		return t < rhs.t;
	}
	bool after_now( const int_time_val & rhs ) const
	{
		return t > rhs.t ;
	}
	bool before_now( const int_time_val & rhs ) const
	{
		return t < rhs.t ;
	}
};

struct int_time_due
: public int_time_val
{
	int_time_due()
	{
		set_never();
	}
	void set_never()
	{
		set_zero();
	}
};

#endif
