#ifndef w_calendar_H
#define w_calendar_H

#include "tbl_cell_base.h"
#include "obj_list.h"
#include "w_widget0.h"

/*
	glib_date uses Jan==1, calendar doesnt
	Once the value is converted to/from ::ACCTS::cell_date its another problem
*/
#define gtk_inconsitent

/*!
	A calendar displays one month with adjustment buttons
*/
class w_calendar : public w_widget0
{
 public:
	//! return GTK widget
	GtkCalendar * Calendar() { return (GtkCalendar *) w; }

	//! constructor
	w_calendar()
	{
		w_set( gtk_calendar_new() );
	}

	//! set the visible and highlighted date
	void set_date( ::ACCTS::cell_date * date )
	{
		int year, mm, dd;
		if( date )
			date->date.get_year_mm_dd( year, mm, dd );
		else {
			year = 2000;
			mm = 1;
			dd = 1;
		}
#ifdef gtk_inconsitent
		mm--;
#endif
		gtk_calendar_select_month( Calendar(), mm, year );
		gtk_calendar_select_day( Calendar(), dd );
	}

	//! get the cell_date
	void get_date( ::ACCTS::cell_date * date )
	{
		if(!date) return;
		guint year, mm, dd;
		gtk_calendar_get_date( Calendar(), &year, &mm, & dd );
#ifdef gtk_inconsitent
		mm++;
#endif
		date->date.set_year_mm_dd( year, mm, dd );
		
	}

	//! form layout needs this (most others are 1)
	int approx_line_height()
	{
		return 6;
	}

	//! some standard needed ?
	str0 get_edited_value_as_cstr()
	{
		::ACCTS::cell_date date1("2001-01-02");;
		get_date( &date1 );
		return date1.get_str0(); 
	}
};

#endif
