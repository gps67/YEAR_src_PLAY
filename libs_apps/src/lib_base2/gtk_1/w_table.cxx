
#include "w_table.h"

#include "obj_list.h"

#define default_gap 2
// #define default_gap 0


	/*!
		constructor
	*/
	w_table::w_table(
		int n_cols,	// gtk is backwards row,col this is w,h
		int n_rows,
		bool equal
	)
	{
		x_max = n_cols;
		y_max = n_rows;
		w_set( gtk_table_new( n_rows, n_cols, equal ));
	}

	w_table::~w_table()
	{
	}

	/*!
		at (col,row) add widget with default (w,h) of 1,1
	*/
	void w_table::add_expand( int col, int row, w_widget0 * A, int ncol, int nrow )
	{
		if(!A) return;
		attach(
			A,
			col,
			col+ncol,
			row,
			row+nrow,
			GTK_EXPAND,
			GTK_EXPAND,
			default_gap,
			default_gap
		);
	}

	/*!
		at (col,row) add widget with default (w,h) of 1,1
	*/
	void w_table::add( int col, int row, w_widget0 * A, int ncol, int nrow )
	{
		if(!A) return;
		attach(
			A,
			col,
			col+ncol,
			row,
			row+nrow,
			GTK_FILL,
			GTK_FILL,
			default_gap,
			default_gap
		);
	}

	/*!
		base function used by add(), it auto resizes
	*/
	void w_table::attach( 
		w_widget0 * A,
		int col0, int col1,
		int row0, int row1,
		GtkAttachOptions opts_x,
		GtkAttachOptions opts_y,
		int padx, int pady
	) {
		if(!A) return;
		if( (col1 > x_max) || (row1 > y_max) ) {
			if( col1 > x_max ) x_max = col1;
			if( row1 > y_max ) y_max = row1;
			resize( x_max, y_max );
		}
		// e_print(" %d-%d %d-%d\n", col0, col1, row0, row1 );
		gtk_table_attach(
			Table(),
			A->w,
			col0,
			col1,
			row0,
			row1,
			opts_x,
			opts_y,
			padx,
			pady
		);
		packed( A ); // container_N adds to keeper list
		A->show();
	}

	/*!
		change the number of cols and rows
	*/
	void w_table::resize( int ncol, int nrow )
	{
		if( !ncol ) ncol = 1 ;
		if( !nrow ) nrow = 1 ;
		if(0) e_print("resize(%d,%d)\n", ncol, nrow );
		x_max = ncol;
		y_max = nrow;
		gtk_table_resize( Table(), ncol, nrow );
	}

	/*!
	*/
	void w_table::clear()
	{
		// keep_list.rezero(); // temp leak holding all old items
		resize(1,1); // gtk2 doesnt like (0,0)
	}

