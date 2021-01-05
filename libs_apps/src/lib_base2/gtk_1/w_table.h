#ifndef w_table_H
#define w_table_H

#include "w_container_.h"

/*!
	w_table wraps GtkTable to layout child widgets in X,Y (col,row) format

	This doesnt delete over-placed sub-widgets, but GTK will,
	however I dont do that (currently).
*/
class w_table : public w_container_N
{
 public:
	int x_max, y_max;

	GtkTable * Table() { return (GtkTable *) w; }

	~w_table();

	w_table(
		int n_cols,	// gtk is backwards row,col this is w,h
		int n_rows,
		bool equal = FALSE
	);
	void add_expand( int col, int row, w_widget0 * A, int ncol=1, int nrow=1 );
	void add( int col, int row, w_widget0 * A, int ncol=1, int nrow=1 );
	void attach( 
		w_widget0 * A,
		int col0, int col1,
		int row0, int row1,
		GtkAttachOptions opts_x,
		GtkAttachOptions opts_y,
		int padx, int pady
	) ;
	void resize( int ncol, int nrow );
	void clear();
};

#endif
