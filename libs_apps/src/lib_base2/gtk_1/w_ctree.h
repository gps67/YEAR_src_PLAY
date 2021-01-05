#ifndef w_ctree_H
#define w_ctree_H

class w_ctree;

#include "str_list.h"

// #include "win_top.h"
#include "w_clist_base.h"
#include "d_colour.h"

/*!
	The prototype of C_event_tree_select_row(...)
*/
typedef void (FN_tree_select_row) (
        GtkWidget       * ctree,
        GList           * row,
        gint              col,
        w_widget0          * _app
);                                                                               


/*!
	a ctree is a grid sheet with a tree-key

	some things could mve down to the base

	there are extra features to help setup new rows, or selected ones
*/
class w_ctree : public w_clist_base {

	/*
	 	CTree is named to inherit from CList
		except for using NODE * instead of (int row).
		But is is NOT clear how true that is.
		Here common functions have been deemed, experimentally.
	 */

 protected:
	int ncol;
 
 public:
	GtkCTreeNode	* new_node;		//!<

	GtkCTreeNode 	* parent;		//!<
	GtkCTreeNode	* sibling;		//!<
	str_list_with_argv	  strs;		//!<
//	temp_argv_str_list temp_argv;
//	gchar 		* text[];
	guint8		  spacing;		//!<
	GdkPixmap 	* pixmap_closed;		//!<
	GdkPixmap 	* mask_closed;		//!<
	GdkPixmap 	* pixmap_opened;		//!<
	GdkPixmap	* mask_opened;		//!<
	int		  TREE_COLUMN;		//!<
	gboolean	  is_leaf;		//!<
	gboolean	  expanded;		//!<

	//! return the GTK widget
	GtkCTree * CTree() { return (GtkCTree *) w ; }


	//!
	static void C_event_tree_select_row(
		GtkCTree * tree,
		GtkCTreeNode * node,
		gint col,
		w_ctree * app
	);

	//!
	static void C_event_click_column(
		GtkCTree * tree,
		gint col,
		w_ctree * app
	);

	// sel_X is only valid WITHIN the event callback
	void init_sel();
	// selected uses some parameters
	int			sel_col;		//!<
        GtkCTreeNode    *	sel_row_gtk;		//!<
        obj_hold<obj_ref>	sel_row_data;		//!<

	//

	/*!
		Constructor - set ncols for late_init
	*/
	w_ctree( int _ncol )
	: strs( _ncol )
	{
		ncol = _ncol;
		init();
	}

	//!
	void init();

	//!
	w_ctree();
	//!
	~w_ctree();
	//!
	w_ctree( int n, int treecol, const char * titles[] );
	//!
	void late_init( int n, int treecol, const char * titles[] );

	// ctree lacks what ctree_tbl_base provides
	// pending the new wigget rewrite
	//!
	bool do_insert_1();
	//!
	bool do_update_1();
	//!
	void on_select_row_tell( FN_tree_select_row * func, w_widget0 * app );
	//!
	void recalc_col_widths();
	//!
	void recalc_col_width( int col );

	//!
	void set_row_data_full( GtkCTreeNode * node, obj_ref * obj );
	//!
	void set_row_data( GtkCTreeNode * node, obj_ref * obj ); // same
	//!
	void re_select_by_data( obj_ref * data );
	//!
	void make_row_visible( obj_ref * data );
	//!
	obj_ref * get_row_data( GtkCTreeNode * node ); 

	//!
	void set_bg( d_colour * col );
	//!
	void set_bg( GtkCTreeNode *node, d_colour * col );

	//!
	virtual void event_tree_select_row() = 0;
	//!
	virtual void event_click_column() = 0;


};

#endif
