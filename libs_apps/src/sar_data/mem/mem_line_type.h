#ifndef line_type_H
#define line_type_H

#include "e_print.h" // e_print
#include <string.h> // strcmp

#include "str0.h"
#include "ints.h"

class mem_line_type : public GRP_sar_data
{
 public:

	enum Point_or_Series {
		M1=-1,
		series,
		point_date,
		point_time,
	};
	
	Point_or_Series val;

	mem_line_type()
	{
		val = M1;
	}

	void init_M1()
	{
		val = M1;
	}

	void set( str0 str ) { set( str.sget() ); }
	void set( const char * str )
	{
		if( !str ) str = "series";
		if(( 0==strcmp( str, "series" )) 
		 ||( 0==strcmp( str, "S" ) ))
			val = series;
		else
		if(( 0==strcmp( str, "point_time" ) )
		 ||( 0==strcmp( str, "P" ) ))
			val = point_time;
		else
		if(( 0==strcmp( str, "point_date" ) )
		 ||( 0==strcmp( str, "D" ) ))
			val = point_date;
		else {
			e_print("# ERROR mem_line_type::set('%s')\n", str );
			gdb_break_point();
			val = series;
		}
	}

	const char * get1() const
	{
		switch( val ) {
		 case point_time: return "P";
		 case point_date: return "D";
		 case series: return "S";
		 case M1: 
		 break;
		}
		e_print("# ERROR mem_line_type::get() val==%d\n", val );
		gdb_break_point();
		return "X";
	}

	const char * get() const
	{
		switch( val ) {
		 case point_time: return "point_time";
		 case point_date: return "point_date";
		 case series: return "series";
		 case M1:
		 break;
		}
		e_print("# ERROR mem_line_type::get() val==%d\n", val );
		gdb_break_point();
		return "M1_ERROR";
	}

	bool operator!=( const mem_line_type & v2 ) const
	{
		return val != v2.val;
	}
	bool operator==( const mem_line_type & v2 ) const
	{
		return val == v2.val;
	}
};

#endif

