#ifndef X_Window_H
#define X_Window_H

#include "X_STUBS.h"
#include "A_point_plus.h"

// free // #include <stdlib.h>
// strdup // #include <string.h>
#include <X11/Xlib.h>

#include "XFT.h"
#include "X_Drawable_Surface.h"
#include "X_Display.h"

namespace WAX {

/*!
	X_Window is NOT a TOP LEVEL WINDOW
	(X_Window_Root is, and is an X_Window)

	child class must provide behavior functions

	X_Window is BASE_t of API 
	X_WINDOW declares { virtual bool api_event( event ) } // AUTO event_t
	X_WINDOW declares { virtual bool api_group( group_event ) } // AUTO %s
	X_WINDOW declares { virtual bool api_WMAN( WMAN_event_t ) } //
	X_WINDOW declares { virtual bool api_VIEW( EVENT ) } // DRAW 
	X_WINDOW declares { virtual bool api_BEHAVIOUR( EVENT ) } // 
	X_WINDOW declares { virtual bool api_RPC( EVENT ) } // 
	X_WINDOW declares { virtual bool api_BOT( API_EVENT ) } // CALL // IO
	X_WINDOW declares { virtual bool api_GROUP( API_EVENT ) } // CALL // IO
	X_WINDOW declares { virtual bool api( GROUP, API_EVENT ) } // CODE_POINT
	CODE_POINT SCRIPT.LINE.XPOS .DATA { SESS += CTXT }
	SCRIPT must work under simple YACC // but with added MATCH "lhs" rhs
	VIEW "lhs" lhs %s // concurrent interpretations // MULTI_VERSE_VIEWS
	API += GROUP // event framework allows SOFT JOIN of GROUP 
	virtual bool api_LIBR_EVENT( LIBR EVENT ) // LIBR = GROUP // EVENT =
	LIBR.call_api_LIBR_EVENT( LIBR, EVENT ); // bool = IS_OK // not OK0
	bool = FAIL("rejected_data") // bool = PASS("that_went_well");

	RUN FULLRANGE_of_EVENTS over SCRIPT
	GEN CXX for all on_event found within tree
	GEN CXX ADJ splice in edit to GEN minimal MAKE JIT relink or ADJ
	ADJ GEN GXX

	TOP_LEVEL api is a GROUP of signals and events 
	TOP_LEVEL adds new_GROUP("Top_Level") // BASE == X_Window //
	TOP_LEVEL gets CT pre_defined or RT now_defined // int // idx //
	TOP_LEVEL . on_event( group, event ) // FAIL("STR_from_DATA");


*/
struct X_Window : public X_Drawable_Surface // X_Window is a _Drawable_Surface
{
	// Display * display;
	// Drawable drawable; // Window is "drawable" // alias SURFACE 
	// A_WH WH;
	//
	X_Window * parent;
	X_Display * disp;
	const char * name; // debugging name // free never called

//	Drawable get_drawable() { return drawable; } // in BASE_t
	Window get_window_0() { return drawable; } // cast drawable to Window
	Window get_window() {
		check_window();
		return drawable;
	} 
	bool check_window(); // true == OK
	void set_window( Window _window ) { drawable = _window; }

#if 1
	X_Window() {} // permit plain sketch design
#endif

	~X_Window(); // deregister

	/*!
		child classes use this to create the object
		generic CTOR of fields
		NULL_parent
	*/
	X_Window(
		X_Window * _parent,	// possibly NULL
		X_Display * _disp,	// must have display
		Window _window,		// possibly 0 or XXXX
		const char * _name	// usually set
	);


	/*!
		every widget has a (copied string) name for debugging / other
	*/
	void set_name( const char * _name );

	/*!
		create a simple window on the parent
	
		really - should create the local X_Window object
		without a display's window.
		Then do the tree-layout,
		then INSTANCIATE the window
	*/
	X_Window(
		const char * _name,
		X_Window * parent_,
		A_Rectangle xywh,
		int borderwidth
	);

	/*!
		call this once - register ROOT window in our list of items
	static X_Window * register_root(
		X_Display & _disp,
		const char * _name
	);
	*/


/*
		create a child-of-this sub-window
	X_Window * mk_child( const char * _name, A_Rectangle xywh, int borderwidth )
	{
		X_Window * child = new X_Window( name, this, xywh, borderwidth );
		return child;
	}
*/

	/*!
	*/
	bool map();

	/*!
		set all (of mask) events to cause a callback

		ExposureMask - so redraw
		KeyPressMask - so act
		KeyReleaseMask - act after thought
		ButtonPressMask - mouse

	NO:

		ResizeRequest - only called when window is resized by another

	https://tronche.com/gui/x/xlib/event-handling/XSelectInput.html
	https://tronche.com/gui/x/xlib/events/mask.html

	*/
	void XSelectInput( long event_mask )
	{
		::XSelectInput( display, get_window(), event_mask );
	}

	void XSelectInput_mask_one(); // default setting // editable
	//	mask |= ExposureMask ;
	//	mask |= KeyPressMask ;
	//	mask |= ButtonPressMask ;
	//	mask |= ButtonReleaseMask; // Pointer button up
	//	mask |= ResizeRedirectMask; // Redirect resize of this window

// TOP	void set_title( const char * name );


        X_Window * find_top_Window()
        {
                X_Window * csr = this;
                while( csr -> parent ) {
                        csr = csr -> parent;
                }
                return csr;
        }

        bool call_XDestroyWindow_top() {

                int t = XDestroyWindow( display, find_top_Window()->get_window() );
		INFO("returned %d", t);
                return true;
        }

	bool X_WMProtocols_add_WM_DELETE_WINDOW();



// virtuals

	virtual void event_expose( A_Rectangle & xywh ) = 0;

};

// ------------------- Frame (rename from X_Window to X_Window_Frame) -------- handles X button and ...

struct X_Window_SubFrame : public X_Window
{
	X_Window_SubFrame()
	: X_Window()
	{
	}

	/*!
		child classes use this to create the object
		generic CTOR of fields
		NULL_parent

		By now you should have upgraded to X_Display * disp_one_plus;

			It's not immediately clear how ALIAS --> UDEF("ANYSTR")
			ALIAS == "ANYSTR"
			UDEF X_Display // triggers BASE_CLASS generic_name 
			UDEF SUBTYPE of X_Display
			RT { X_DISPLAY * UDEF ; // CMNT // CODE } STO RUN CODE
			SESS STO CODE sess // INST == "sess" // CODE_POINT //
			GEN CODE for SESS RT UDEF "sess" // %s // cident // IDX //
			RT IDX = { IDX idx ; PAIR cident "sess" } // SCRIPT //
			SCRIPT += { 
			 LOCAL DIALECT script // var of TYPE // script phrase_book
			 LOCAL RUNTIME STR nbytes text PSG {
			  RT {
			  	VAR_NAME "nbytes"
				VAR_TYPE TOKEN_for_known_ITEM PGX SUBLEX SESS
				SCRIPT {
					RT SESS STO += "VAR_POOL[var_name] CODE"
					RT SESS STO += "CALL EVAL ARG" HERE nbytes
					"[var_name] CODE" {

						STR0 CODE_BLOCK_t // IDX += idx
						ADD_ATTR "var_name"
						ADD_ATTR "IDX_t"
						AUTO_MATCH "IDX_t" "%s_t" "LHS_SPEC DATA ARGV
						FOUND_VAR SESS.idx("SESS.idx")

					}

					var_name = NEW_VAL // SET_VAR VAL
					// { %s VAR } // { VAR var } // ALIAS %s.var
					// { VAL %s } // nbytes // STO_API

				ITEM_T == "ITEM" STO += { VARNAME VARTYPE STODATA }
				ITEM_NAME == "VARNAME" // GEN
				ITEM_TYPE == "VARNAME_t" // GEN 
				hakuna mataka == "tanzania phrase"
				ITEM_DATA == "STO_%s" // permit parameter_pcs
				pcs_percent_s_t == "nbytes"
				ITEM_SPEC += "nbytes_t nbytes MK_VAL // fetch_api
				ITEM_API += ""ITEM_t" // STO.ITEM // "ITEM" //
				ITEM_CODE += "SCRIPT"
				ITEM_SESS += "{ SESS_t SESS }" // GEN // BENCH //
				ITEM_BENCH += "SESS" // PICK token from POOL
				{ SESS nbytes_t } // { nbytes_t DATA }
				{ ALIAS DATA }
				{ ALIAS STO }
				{ API CALL CODE ARGS }
				{ ARGS_PSG += "{ ITEM_T ITEM }" } // MK_VAR //
				{ ARGS += "{ %s_t %s }" } // DIALECT lower _t
				{ ARGS += "{ %s_T %s }" } // DIALECT UPPER _T
				{ VAR_POOL += { %s } // PSG LEX TOKEN "%S"
					"VAR_POOL += '{ LHS RHS } // CODE_POINT'"
					"VAR_POOL += '{ LHS } // CODE_POINT'"

					AUTO GEN { DECL_VAR SCRIPT } API += SCRIPT_API
						// NB AUTO_MATCH_LEX_EXPR
						// NB AUTO_MATCH PSG EXPR
						//
						// PSG += { AUTO_MATCH %s } 
						// DECL += { MK_VAR GEN }
						// PARSE_LEX += "{ lhs rhs }" // CODE_POINT
						// CODE_POINT += SCRIPT.HERE.CODE_POINT
						// DIALECT HERE // AUTO_DETECT LEX
						// SCRIPT // MK_VAR // UDEF // EXPR // API

					AUTO MK_VAR simply from "mention"
						VAR_type == "VAR_t" // FROM

					FROM "VAR" "%s_t" VARNAME == "{ _t %s }"
					FROM "VAR" "%s_t" VARNAME += "{ _t %s }"

						DIALECT STR0 "STR0"
						DIALECT "{ nbytes_t nbytes }"
						DIALECT "{ ITEM_t ITEM }"
						DIALECT "{ item %s }" // NAME_or_VAL
						NAME "{ item %s }" // NAME_or_VAL
						VAL "{ idx %s }" // NAME_or_VAL



				{ ARGS += "{ ITEM_T ITEM }" } // MK_VAR
				 MK_VAR += "VAR_NAME"
				 MK_VAR += "VAR_TYPE"
				 MK_VAR += "VAR_STR0"
				 MK_VAR += "VAR_DATA"
				 MK_VAR += "VAR_VAL"
				 MK_VAR += "VAR_STO"

				 	A running SCRIPT triggers MATCH
					SCRIPT.STEP.CMD0 == "ARGV argv[argc]"
					MK_VAR argc argv api

					A running script triggers { SCRIPT }
					LEX cident // ITEM_NAME STR0 //
					LEX_cident knows name_in_POOL "cident_t cident"
					BENCH COMPILER adds _t from { %s }
					typedef "{KEY %s } %_t ; // CMNT"
			  }
			 }
	*/
	X_Window_SubFrame(
		X_Window * _parent,	// possibly NULL
		X_Display * _disp,	// must have display
		Window _window,		// possibly 0 or XXXX
		const char * _name	// usually set
	) : X_Window( _parent, _disp, _window, _name )
	{}

};

struct X_Window_Frame : public X_Window_SubFrame
{
	X_Window_Frame()
	: X_Window_SubFrame()
	{
	}

	/*!
		child classes use this to create the object
		generic CTOR of fields
		NULL_parent
	*/
	X_Window_Frame(
		X_Window * _parent,	// possibly NULL
		X_Display * _disp,	// must have display
		Window _window,		// possibly 0 or XXXX
		const char * _name	// usually set
	) : X_Window_SubFrame( _parent, _disp, _window, _name )
	{}
};



// XXXX

// ------------------- Top_Level -------- handles X button and ...

struct X_Window_Top_Level : public X_Window_Frame 
{
	X_Window_Top_Level()
	: X_Window_Frame ()
	{
	}

	/*!
		create a simple window, NB-TODO some unsigned work 
	*/
	X_Window_Top_Level(
		const char * _name,
		X_Display & _disp,
		A_Rectangle xywh,
		int borderwidth
	);

	// Top_Level // api

	void set_title( const char * name );
	bool X_Raise_Window(); // NO it doesn't - within siblings only
	bool set_always_on_top(); // no off switch !!

	// TODO 
	// set always on top
	// to_icon // from_icon // to_top // full_screen { to from }
	// track previous XYWH // restore // 

	// Top_Level // api events //
	// on_EVENT 
	// on_%s

};
// ------------------- X_Window_Root -------- desktop root window

/*
	The desktop ROOT WINDOW for this display
*/
struct X_Window_Root : public X_Window
{
	virtual void event_expose( A_Rectangle & xywh )
	{
		INFO("UNUSED");
		// we provide an X_Window based class wrap
		// and it requires some virtual functions
		// they never get called!
	}

#define NULL_parent NULL
#define ZERO_window 0

	// LURK no check on DefaultRootWindow() - see syntax

	X_Window_Root(
		X_Display * _disp, // has root_window
		const char * _name
	);

};

	// NOTE name in wrong place in args list

	// MATCH { TYPENAME %s_t } in DOCUMENT = SCRIPT = HERE = CSR { DOC }
	// kebab <KEYWORD> in { virtual bool on_api_event( event ) }
	// KEYWORD event // MATCH SUBCT lhs rhs //  ITEM ITEM // EXPR EXPR //
	// EXPR = ITEM // SCRIPT is a VIEW over DOC = SCRIPT //
	// SCRIPT LEX ITEM // %s cident // 
	// SCRIPT_LAYER // SUBST_SCRIPT // a_mini_machine //
	// MINI_MACHINE // 256 register // each a script function + data //
	// MINI_MACHINE // GET SET REF EXPR(ITEM) // SESS += CTXT //
	// MINI_MACHINE // EA_EXPR // OPCODE_DECODER // %s == %s_t //
	// MINI_MACHINE // SCRIPT += SCRIPT // CXX_SCRIPT
	// CXX_SCRIPT // whatever we can cobble together under YACC
	// each VAR has a GETTER that generates ASM JIT or DATA = LINK_to_VAR
	// VIEW adds FILTER = VIEW // push on a kebab stack //
	// VIEW as { ITEM_t ITEM = ZERO ; // CMNT } { ZERO = INIT }
	// VIEW as { ITEM } // ITEM = NAME = cident = "ANYSTR"
	// VIEW as // trigger explain in the BENCH panel of its choice
	// VIEW as // expand generate ASM // SSTEP though DATA // ERA //
	// ERA RT_CT RT CT // group of siblings peers nears pool_of_item { eg }
	// eg "RT" // eg "CT" // eg ONE of { RT CT RTCT ERA  %s(cident) }
	// eg // %s(%s) "%s" "cident" // %s(cident) // MATCH ALIAS %s //
	// 

	// virtual bool on_api_event( event ); // deduce TYPENAME %s_t // in
	// virtual bool on_api_event( event = initval() ); // profile initval 
	// virtual bool on_api_event( event_t event );

	// api( event ) // universal template //
	// bool on_event( event ) // += CTXT // RETVAL means OK else FAIL
	// bool IS_OK else /* see api_error_stack // stacktop.api_errno // OK0
	// int // ZERO_else_ERRNO // API = api // tbl_ERROR[int] => { EXPLAIN }
	// EXPLAIN_enum group item explain // explains ERROR // { int ERROR_t }
	// SYNTAX VIEW // { int ERROR } // "ERROR_t ERROR = VAL; // CMNT"
	// SYNTAX FILTER // VAL // "cident_of_enoum_group_item" // cident //
	// DECL EVENT // API event_t event // init_event( event, SCRIPT, DATA )
	// EVENT {
	// // event_t == api_event_t // FILTER has each as "%s" || TREE_NODE
	// // %s == %s // "event" // "api_event_t" // %s_t // %s_%s_t //
	// union event_t { api_event_t wm_event_t usb_event_t api_event_t }
	// union event_t += wm_event_WMAN_API_t // %s_%s // WMAN API //
	// WMAN // one_of //
	// WMAN // BTN_X_DESTROY_SIGNAL BTN_MINIMISE BTN_SIGNALS RESIZE_SIGNALS
	// WMAN // COPY_PASTE // DRAG_DROP // LAYOUT_BENCH //
	// API // union EVENT_t { EVENT_message_struct } // FIELDS manager
	// API // existing libxlib has own thought out union of message types
	// API // uses 48 byte event message // or xlib nn byte fixed size
	// API WIRE fixed_size_message_nbytes // alternative memory PTR * STRUCT
	// API H(X) of event_t // base_t == "event_t" // NEW DEL API //
	// API class event_group_t : event_t { SCRIPT } //
	// SCRIPT += CTOR { PTR = CT_EXPR } //
	// RT_EXPR += SCRIPT += CTXT += CODE += DATA
	// }

}; // NAMESPACE
#endif
