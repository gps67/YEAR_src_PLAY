#include "w_img_id.h"
#include "w_win_top.h"
#include "w_vbox.h"
#include "w_btn_plus.h"
#include "w_entry.h"
#include "buffer2.h"

#include "img_id_util.h"
#include "w_label.h"
#include "w_table.h"

#include "scan_util.h"

namespace ACCTS {

/*
	'table: TRANS col: img' is col_spec_img_id
	tbl_col_spec.cxx mk_edit creates a button spec_img_edit_btn
	when (Replace) get_edited_value_as_cstr returns the val from the row
	when clicked it calls mk_w_img_id_panel1

	w_img_id_panel1 is a top-level window, and bridges date/cat/
	Img_Id_Util - dos the name mangling, from its copies of date/cat
	Scan_Util - does the actual scanimage to file
	File_Alloc - finds an unused filename (for MMDDX1 prefix)

	The return route for the values is a bit NAFF, as the display doesnt
	get updated, but its there.
*/

/*!
	w_img_id_panel1 is how the scan_img_id is edited,
	and the file scanned. It is a (single global) pop-up
	with buttons.

	The file name is derived from the img_id.

	The file can be processed by external utils (eg thumbnails).

	The loaded ,mm3 doesnt really have place for an img_id,
	other than the REF fields (which is used for the cheque-no).
	so there will be a merge phase.

	BUG:
		this is very specific to my table schema -
		the correct fields must exist (either TRANS or XFER)
		Absent fields will segfault


	w_win_top required - because obj_hold<w_win_top>
	USUALLY accepts one level of guess-resolving,
	but when w_win_top isnt defined, it doesnt

	ditto w_vbox ?
*/

class w_img_id_panel1 : public TOPAPP_Task_Window_Minor
{
 public:

	Img_Id_Util util;

	obj_hold<w_entry> entry_cmnt;
	obj_hold<w_entry> entry_name;

	obj_hold<col_spec_img_id> spec;
	obj_hold<row_inst> row;

	w_img_id_panel1();
	void invoke(
		col_spec_img_id * _spec,
		row_inst * _row
	);
	void CMD1( str0 cmd );
	void get_edited();
	void hide_close();
};

/*!
	btn_CMD1 holds a link back to the panel, and carries a CMD1 string.

	The display string is internationalisationable.
*/
class btn_CMD1 : public w_btn_plus
{
	obj_hold<w_img_id_panel1> panel;
	str1 cmd;
 public:
	btn_CMD1( w_img_id_panel1 * _panel, str0 _cmd, str0 _name )
	: w_btn_plus( _name )
	, panel( _panel )
	, cmd( _cmd )
	{
	}
	void event_clicked()
	{
		panel->CMD1( cmd );
	}
};

	w_img_id_panel1::w_img_id_panel1()
	: TOPAPP_Task_Window_Minor(
		NULL,	// _m_a_h is taken from elsewhere
		"Scan Image For Transaction", // title
		// 300,300, // width height,
		0,0,  // width height,
		NULL, false
	)
	{
		e_print("w_img_id_panel1() constructed\n");
	}

	void w_img_id_panel1::hide_close()
	{
		hide();
		app_area->remove_all();
	}

	void w_img_id_panel1::get_edited()
	{
		util.str_imgid = entry_name->get_text();
		util.str_cmnt  = entry_cmnt->get_text();
		spec->set_from_edit( row, util.str_imgid );
	}

	void w_img_id_panel1::CMD1( str0 cmd )
	{
		Scan_Util scan;
		if( cmd == "SCAN" )
		{
			get_edited();
			hide_close();
			e_print("SCAN imgid    %s\n", (STR0) util.str_imgid );
			e_print("SCAN pathbase %s\n", (STR0) util.path_base() );
			e_print("SCAN comment  %s\n", (STR0) util.str_cmnt );

			e_print("This takes ages - stand-by\n");
			scan.scan_into_file(
				(STR0) util.path_base(),
				(STR0) util.str_cmnt
			);
			return;
		}

		if( cmd == "CANCEL" )
		{
			hide_close();
			return;
		}

		if( cmd == "NAMED" )
		{
			get_edited();
			hide_close();
			return;
		}

		if( cmd == "DISPLAY_PNM" )
		{
			get_edited();
			scan.display( (STR0) util.path_base(), ".pnm" );
			return;
		}

		if( cmd == "DISPLAY_TIFF" )
		{
			get_edited();
			scan.display( (STR0) util.path_base(), ".tiff" );
			return;
		}

		if( cmd == "DISPLAY_PNG" )
		{
			get_edited();
			scan.display( (STR0) util.path_base(), ".png" );
			return;
		}

	}

	/*!
		ie create the panel (done already) and unhide it
	*/
	void w_img_id_panel1::invoke(
		col_spec_img_id * _spec,
		row_inst * _row
	)
	{
		main_window->raise_window();
		spec = _spec;
		row = _row;
		// main_window->set_title( "OTHER TITLE" );
		app_area->remove_all();

		table * tbl = spec->tbl; // TRANS or XFER or REPORT1

		/*
			get the input data for util to calc the names
		*/
		col_spec_date * spec_date_tax  = (col_spec_date *)tbl->get_col_spec( "date_tax" );
		col_spec      * spec_date_cat  = tbl->get_col_spec( "cat" );
		col_spec      * spec_amnt      = tbl->get_col_spec( "amnt" );
		col_spec      * spec_imgid     = tbl->get_col_spec( "img" );
		col_spec      * spec_desc      = tbl->get_col_spec( "desc" );
		col_spec      * spec_acct      = tbl->get_col_spec( "acct" );
		if(!spec_acct)
			spec_acct      = tbl->get_col_spec( "acct1" );
		if(!spec_date_tax)
			spec_date_tax  = (col_spec_date *)tbl->get_col_spec( "date" ); // xfer

		util.str_date  = spec_date_tax ->get_str2_for_edit( _row ) ->str;
		util.str_cat   = spec_date_cat ->get_str2_for_edit( _row ) ->str;
		util.str_amnt  = spec_amnt     ->get_str2_for_edit( _row ) ->str;
		util.str_desc  = spec_desc     ->get_str2_for_edit( _row ) ->str;
		util.str_acct  = spec_acct     ->get_str2_for_edit( _row ) ->str;

/*
	what if loaded value already set ...
*/
		obj_hold<str2> orig_imgid;
		orig_imgid = spec_imgid    ->get_str2_for_edit( _row );
		if(( *orig_imgid == "-" ) 
		|| ( *orig_imgid == "+" ) 
		|| ( *orig_imgid == "" ) 
		) orig_imgid = NULL;
		

		if( spec_date_tax->get_date( _row, util.date ) ) ; // LURK CHECK

		/*
			compute the filename
		*/
		util.recalc_comment_and_imgid();
		if( orig_imgid )
		{
			// if file actually exists ... TODO
			util.str_imgid = orig_imgid->str;
		}

		entry_cmnt = new w_entry( util.str_cmnt );
		entry_name = new w_entry( util.str_imgid );

		w_table * layout = new w_table( 2, 3 );
		app_area->pack_big( layout );

		int r = 0;

		/*
			interesting .. w_table acts ODD when expanded,
			without any add_expand, the individual extry's
			dont stretch (the overall doesnt), with one,
			it does (but all y expansion goes to that item).

			Sort out later
		*/

		layout->add( 0, r, new w_label( "Img id" ) );
		layout->add( 1, r, entry_name );
		r++;
		layout->add( 0, r,  new w_label( "Comment" ) );
		layout->add( 1, r, entry_cmnt );
		r++;
		layout->add( 0, r, new btn_CMD1( this, "SCAN",   "Scan" ), 2 );
		r++;
		layout->add( 0, r, new btn_CMD1( this, "CANCEL", "Cancel" ), 2 );
		r++;
		layout->add( 0, r, new btn_CMD1( this, "NAMED",  "Named" ), 2 );
		r++;
		layout->add_expand( 0, r, new btn_CMD1( this, "DISPLAY_TIFF",  "Display .tiff" ), 2 );
		r++;
		layout->add( 0, r, new btn_CMD1( this, "DISPLAY_PNG",  "Display .png" ), 2 );

		main_window->show();
	}


void mk_w_img_id_panel1( col_spec_img_id * _spec, row_inst * _row )
{
	/*
		it shouldne be obj_hold as the framework _m_a_h stuff
		should hold it, but I've forgotten what and how
		that works, so its fudged to get a quick result

		The single instance gets reused each time the
		mk_edit button is pressed (col_spec.cxx).
	*/
	static obj_hold<w_img_id_panel1> panel = new w_img_id_panel1();
	panel->invoke( _spec, _row );
}

}; // NAMESPACE
