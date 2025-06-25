#ifndef tbl_cell_base_H
#define tbl_cell_base_H

#include "str2.h"
#include "date_glib.h"
#include "amnt_t.h"
#include "per_cent.h"

#include "obj_hold.h"

class w_calendar; // uses date to get ...

namespace ACCTS {

/*!
	a cell of a table - storage not display

	The cell_spec belongs to the table and needs a row object
	where the value held.

	call_base(+) is a stored data value

*/
class cell_base : public obj_ref
{
 public:
	u32 cell_flags;
	virtual str0 get_str0() ;
	virtual str2 * get_str2() ;
	virtual void prints() ;

	void cache_clear() {}

/*
	virtual bool set_str0(str0 s) {
		return FALSE;
	}
	virtual str2 * get_str2() { 
		return new str2( get_str0() );
	}
	virtual bool set_str2(str2 * s) {
		cache_clear();
		return set_str0( s->get() );
	}
*/

};

/*!
	scalar adds a str2 cache for the value, (except for str1/str2)
	eg date convertions cache the returned string value here.
	Only cache the str2 for DISPLAY, file can churn a few if csv needs it
*/
class cell_base_scalar : public cell_base
{
 public:
	obj_hold<str2>	str2_display_cached;
	void cache_clear()
	{
		str2_display_cached = NULL;
	}
};

//! UNUSED INTERFACE All data cells are like this
/*!
	\li Sub-Classes
	\li cell_str1 : public cell_base
	\li cell_date : public cell_base
	\li cell_amnt : public cell_base
	\li cell_percent : public cell_base
	\li Other:
	\li w_calendar;

	Each does what it says (a string, a date, a currency amount, percentage)
	Most of the work is done by the col_spec, which DOES have a commin subclass

	call_str1 doesnt have an extra cached string
	
*/

class cell_str1 : public cell_base
{
	str1 data;
 public:
	cell_str1( str0 s ) ;
	virtual str0 get_str0() ;
	virtual bool set_str0(str0 s) ;
};

enum enum_CELL_STATE {
	cell_state_UNSET = 0,
	cell_state_VALID,
	cell_state_RECALC,
	cell_state_STRING, // eg "-" "default"
	cell_state_NULL, // ie 0L as PTR
	cell_state_EMPTY, // ie ""
};

class cell_date : public cell_base_scalar
{
	friend class col_spec_date;
	friend class col_spec_date_rel;
	friend class ::w_calendar; // uses date to get ...
 protected:
	date_glib	date;
	// class w_calendar;
	// friend w_calendar;
 public:
	virtual bool set_str0(str0 s) ;
	virtual str0 get_str0() ;
	str2 * get_str2() ;
	cell_date( str0 s ) ;
	str2 * get_YEAR_MM() ;
	str2 * get_DD() ;
	IS_DIFF_t cmp( cell_date & date2 ) ;
	IS_DIFF_t cmp( cell_date * date2 ) ;
	void set_date( date_glib _date );
};

class cell_date_rel : public cell_base_scalar
{
	friend class col_spec_date_rel;
 protected:
	date_glib	date_val;
	date_glib	date_rel;
	int		days_diff_cache;
	// class w_calendar;
	// friend w_calendar;
 public:
	cell_date_rel( date_glib & d0, date_glib & d1 ) ;
	virtual bool set_str0(str0 s) ;
	virtual str0 get_str0() ;
	str2 * get_str2() ;
	str2 * get_YEAR_MM() ;
	str2 * get_DD() ;
	IS_DIFF_t cmp( cell_date_rel & date2 ) ;
	IS_DIFF_t cmp( cell_date_rel * date2 ) ;
};

class cell_amnt : public cell_base_scalar
{
	friend class col_spec_amnt;
 protected:
	amnt_t		amnt;
 public:

	virtual bool set_str0(str0 s) ;
	virtual str2 * get_str2() ;
	virtual str0 get_str0() ;
	cell_amnt( str0 s ) ;
	cell_amnt( amnt_t _amnt ) ;
	IS_DIFF_t cmp( const cell_amnt & amnt2 ) ;
	IS_DIFF_t cmp( const cell_amnt * amnt2 ) ;
	void set_amnt( amnt_t _amnt ) ;
};

class cell_percent : public cell_base_scalar
{
	friend class col_spec_percent;
 protected:
	per_cent_t		percent;
 public:

	virtual bool set_str0(str0 s) ;
	virtual str2 * get_str2() ;
	virtual str0 get_str0() ;
	cell_percent( str0 s ) ;
	IS_DIFF_t cmp( cell_percent & amnt2 ) ;
	IS_DIFF_t cmp( cell_percent * amnt2 ) ;
	void set_percent( per_cent_t  amnt2 ) ;
};
}; // NAMESPACE

#endif

