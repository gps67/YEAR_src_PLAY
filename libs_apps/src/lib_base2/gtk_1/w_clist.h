#ifndef w_clist_H
#define w_clist_H

#include "w_widget0.h"
#include "w_clist_base.h"

/*!
	a clist is grid sheet - but on the wanted list
*/
class w_clist : public w_clist_base
{
 public:

	/*!
		constructor - unused
		w_clist() { }
	*/

	/*!
		constructor - ncol known
	*/
	w_clist( int ncol, char * titles[] )
	{
		w_set( gtk_clist_new_with_titles( ncol, titles));
	}

	/*!
		create a clist, set shadow and leading column width
	*/
 static
	w_clist * new_c_list( int ncol, char * titles[] )
	{
		w_clist * w1 = new w_clist( ncol, titles );
		w1->set_shadow_out();
		w1->set_column_width( 0, 50 );
		return w1;
	}

	/*!
		outline shadow
	*/
	void set_shadow_out()
	{
		gtk_clist_set_shadow_type (CList(), GTK_SHADOW_OUT);
	}

	/*!
		append a row to the bottom of the list
	*/
	void append( char * datarow[] )
	{
		gtk_clist_append( CList(), datarow );
	}

	/*!
		get the text at row, col
	*/
	void get_text( int row, int col, char ** ptext )
	{
		gtk_clist_get_text(CList(), row, col, ptext);
	}

	/*!
		clist should really know who app is
	*/
	void on_select_row_tell( SignalFunc func, widget0 * app )
	{
		connect(
			"select_row",
			func,
			(obj_ref *) app
		);
	}

	/*!
	void recalc_col_widths()
	{
		fprintf(stderr,"CLIST recalc_col_widths()\n" );
	}
	*/

	/*!
		GTK can group multiple changes together (freeze/thaw)
	*/
	void freeze()
	{
		gtk_clist_freeze( CList() );
	}

	/*!
		GTK can group multiple changes together (freeze/thaw)
	*/
	void thaw()
	{
		gtk_clist_thaw( CList() );
	}

};


#endif
