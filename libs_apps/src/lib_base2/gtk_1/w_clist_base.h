#ifndef w_clist_base_H
#define w_clist_base_H

#include "w_widget0.h"
#include "align_val.h"

/*!
*/
typedef
	void (*FNselect_row) (
		GtkWidget      *clist,
		gint            row,
		gint            column,
		GdkEventButton *event,
		w_widget0 		* app	// or subtype
	);

/*!
*/
typedef
	void (FN_select_row) (
		GtkWidget      *clist,
		gint            row,
		gint            column,
		GdkEventButton *event,
		w_widget0 		* app	// or subtype
	);

/*!
	a clist is grid sheet - and also the base of a ctree
*/
class w_clist_base : public w_widget0
{
 public:

	/*!
		return the GTK widget
	*/
	GtkCList * CList() { return GTK_CLIST(w); }

	/*!
		clear all rows
	*/
	void clear()
	{
		gtk_clist_clear( CList() );
	}

	/*!
		calc column width
	*/
	int optimal_column_width( int c )
	{
		return gtk_clist_optimal_column_width( CList(), c ) ;
	}

	/*!
		set column width
	*/
	void set_column_width( int c, int width )
	{
		gtk_clist_set_column_width( CList(), c, width );
	}

	/*!
		align the column to a GTK_JUSTIFY_enum
	*/
	void set_column_align( int c, align_val align )
	{
		set_column_justification(
			c,
			(GtkJustification) align.as_gtk_justification()
		);
	}

	/*!
		align the column to a GTK_JUSTIFY_enum
	*/
	void set_column_justification( int c, GtkJustification just )
	{
		// _LEFT _RIGHT _CENTER _FILL (text)
		gtk_clist_set_column_justification( CList(), c, just );
	}

	/*!
		align left (default)
	*/
	void set_column_align_left( int c )
	{
		set_column_justification( c, GTK_JUSTIFY_LEFT );
	}

	/*!
		align right (decimals already in place)
	*/
	void set_column_align_right( int c )
	{
		set_column_justification( c, GTK_JUSTIFY_RIGHT );
	}

	/*!
		align center
	*/
	void set_column_align_center( int c )
	{
		set_column_justification( c, GTK_JUSTIFY_CENTER );
	}

	/*!
		hide the titles (why)
	*/
	void titles_hide()
	{
		gtk_clist_column_titles_hide( CList() );
	}

	/*!
		show the titles
	*/
	void titles_show()
	{
		gtk_clist_column_titles_show( CList() );
	}
	

};

#endif
