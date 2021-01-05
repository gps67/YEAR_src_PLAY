/*
	replaces c_tree_tbl_two
*/

#include <gtk/gtk.h>

#include "grid_base.h"
#include "table.h"
#include "nb_page_tbl.h"
#include "w_win_scrolled.h"

/*
*/

// namespace GRID {
using namespace GRID;


	/*!
	*/
	grid_col_info::grid_col_info()
	{
		width = 10;
	}

	/*!
	*/
	grid_row_info::grid_row_info()
	{
		height = 10;
		data = NULL;
	}

// grid area

	/*!
	*/
	w_grid_area::w_grid_area( grid_window_0 * _up )
	: w_drawing_area()
	, draw( this )
	, draw_white( this )
	{
		up = _up;
		nrows = 0;
		ncols = 0;
	}

	/*!
	*/
	void w_grid_area::pixmap_reallocated()
	{
		draw.set_drawable( pixmap );
		draw_white.set_drawable( pixmap );
		text1.set_drawable( pixmap );
		e_print("void w_grid_area::pixmap_reallocated()\n");
		draw_white.check_gc();
		draw_white.gc->set_fg("white");
		draw_some();

	}

	/*!
		when a cell has a different bg colour, you will need a
		prepared d_draw (for each style!)
	*/
	void w_grid_area::fill_cell( d_draw & draw_colr )
	{
		bool fill = true;
		draw_colr.rectangle_edge(
			fill,
			cell_x0,
			row_y0,
			cell_x3,
			row_y3
		);
	}

	void w_grid_area::draw_cell()
	{
		char buf[20];
		sprintf( buf, "%d,%d", cell_col, cell_row );
		if( ( cell_row==1 ) && ( cell_col==1 ) ) fill_cell( draw_white );
		text1.goto_y_top( row_y1 );
		text1.draw_x_str( cell_x1, buf );
	}

	/*!
	*/
	void w_grid_area::draw_some()
	{
		int h_line = 1;
		int w_line = 1;
		int h_gap = 1;

		e_print("# exp # %d %d\n", width(), height() );
		if(!draw.check_gc())
		{
			e_print("# FAIL # w_grid_area::event_expose() - no gc\n");
			return;
		}
	//	if(!text1.check_gc())
	//	{
	//		e_print("# FAIL # w_grid_area::event_expose() - no gc\n");
	//		return;
	//	}
		row_y0 = h_line;
		int gap = 1;
		int w1 = 40; // cell width
		row_y2 = row_y1;
		for( cell_row = 0; cell_row<nrows; cell_row++ )
		{
			row_y1 = row_y0 + h_gap;
			row_y2 = row_y1 + text1.get_step_height();
			row_y3 = row_y2 + h_gap;

			cell_x0 = w_line;
			for( cell_col = 0; cell_col<ncols; cell_col++ )
			{
				cell_x1 = cell_x0 + gap;
				cell_x2 = cell_x1 + w1;
				cell_x3 = cell_x2 + gap ;

				draw.line( cell_x0, row_y3, cell_x3, row_y3 ); // bottom
				draw.line( cell_x3, row_y0, cell_x3, row_y3 ); // right
				draw_cell();
			if(0) { // diagonals
				draw.line( cell_x0, row_y0, cell_x3, row_y3 ); // TL \ BR
				draw.line( cell_x0, row_y3, cell_x3, row_y0 ); // BL / TR
			}
				cell_x0 = cell_x3 + w_line;
			}
			row_y0 = row_y3 + h_line;
		}
		draw.line( 0, 0, 0, row_y3 ); // left
		draw.line( 0, 0, cell_x3, 0 ); // top
	}

// titles

	/*!
	*/
	w_grid_titles::w_grid_titles( grid_window_0 * _up )
	: w_grid_area( _up )
	{
		nrows = 1;
		ncols = 10;
		connect_event_size_request();
	}

	/*!
	*/
	void w_grid_titles::event_size_request( GtkRequisition *requisition )
	{
		requisition -> height = 25;
		set_bg("green");
	}

// cells

	/*!
	*/
	w_grid_cells::w_grid_cells( grid_window_0 * _up )
	: w_grid_area( _up )
	{
		nrows = 10;
		ncols = 10;
	}

	/*!
	*/
	void w_grid_cells::event_size_request( GtkRequisition *requisition )
	{
		int min_w = 150;
		int min_h = 50;
		int req_w = min_w * 10;
		int req_h = min_h * 10;
		// canvas width causes widget to push others out
		// set the screen size only
		// requisition -> height = 1025;
		// requisition -> width = 1025;
		if( requisition -> height < min_h )
			requisition -> height = req_h;
		if( requisition -> width < min_w)
			requisition -> width = req_w;
		set_bg("yellow");
	}

	/*!
	*/
void C_on_scrolled
(	GtkScrolledWindow *scrolledwindow,
	GtkDirectionType arg1,
	gpointer user_data)
{
	// grid_window_0 * grid = (grid_window_0 *) user_data;
	// int sy = 0;
	// int sx = grid->area_cells
	// gdk_window_scroll( grid->area_titles->w, sx, sy )
	e_print( "Scrolled\n");
	
}

// grid_window

	/*!
	*/
	grid_window_0::grid_window_0()
	: w_table(2,3,false)
	{
		TREE_COLUMN = -1;

		vadj = new g_adjustment( 0, 0, 10, 1, 2, 2 );
		hadj = new g_adjustment( 0, 0, 10, 1, 2, 2 );

		hscroll = new w_scale( true,  hadj );
		vscroll = new w_scale( false, vadj );

		area_titles = new w_grid_titles( this );
		area_cells  = new w_grid_cells( this );

/*
		w_win_scrolled * area_cells_scrolled = new w_win_scrolled();
		area_cells_scrolled->pack_unscrollable( area_cells );

		pack_asis( area_titles );
		pack_big( area_cells_scrolled );

		gtk_signal_connect(
			GTK_OBJECT(area_cells_scrolled->w),
			"scroll_child",
			(GtkSignalFunc) C_on_scrolled,
			this
		);

		gtk_signal_connect(
			GTK_OBJECT(area_cells_scrolled->w),
			"scroll-child",
			(GtkSignalFunc) C_on_scrolled,
			this
		);

		gtk_signal_connect(
			GTK_OBJECT(area_cells_scrolled->w),
			"scroll_event",
			(GtkSignalFunc) C_on_scrolled,
			this
		);

		gtk_signal_connect(
			GTK_OBJECT(area_cells->w),
			"scroll_event",
			(GtkSignalFunc) C_on_scrolled,
			this
		);
*/


		add( 0, 0, area_titles );
		add( 0, 1, area_cells );
		add( 0, 2, hscroll, 1, 1  );
		add( 1, 0, vscroll, 1, 2 );
		add( 1, 2, new w_btn_base("ABC") );
	}

// }; // namespace
