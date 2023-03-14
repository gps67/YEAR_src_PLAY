#ifndef X_Display_H
#define X_Display_H

#include "X_STUBS.h"

#include "A_Map_W.h"
#include "X_Colours.h"
#include "X_Window.h" // _Root

# if 0
 X_Display_1 	 just the Display * display
 X_Display_2 	 adds cmap and dictionary // you might need to replace DRY
 X_Display_3	 adds extra stuff specific to ME and not you, useful as standard
 X_Display 	 the universal expectation <=== has range of features
 X_Display_UDEF	 your apps extra
#endif


namespace WAX {

struct X_Display_1 { // : public X_Display_OBJ_TYPE // GEN "obj_ref0 * PTR //

	// X_Display_3 is the basic connection to the X server.
	Display * display;

	static const int SCREEN_0 = 0;

	X_Display_1( 
		Display * _display
	)
	: display( _display )
	{
		// INLINE ASSOCIATE WORD VAR IN MEMORY // CTOR_PRE_LOADED //
		// late_init_display is DONE
	}

	virtual ~X_Display_1()
	{
	// virtual base class //
	// simply forget { Display * display } as OLD_VAL // AUTO { lhs rhs }
	// AUTO_ADD_PTR_STEP
	//	do print _t on TYPE_NAME
	//	USAGE { TYPENAME VARNAME } // TYPENAME { typedef lhs * rhs } 
	//	USAGE { %s %d %s } // IDX = N ++ // VAR_NAME of N is "%s"
	//	USAGE { CAP1 * lower } // CODEPOINT ADDS '{ CAP1 * lower }'
	//	USAGE { lhs & rhs }
	}
	
//	virtual bool late_init_display( X_Display * _disp ) = 0;


	/*!
		send all drawing requests
	*/
	void XFlush()
	{
		::XFlush( display );
	}

	/*!
		wait for an event
	
		X11 can open several connections,
		need to get a non-blocking XNextEvent
		and to feed X11 using (end of chapter2)
			XAddConnctionWatch()
			conn_watch_proc()
			CProcessInternalConnection( display, fd )
	*/
	void XNextEvent( XEvent & event )
	{
		// INFO("... %d reasons",reasons_to_stay );
		::XNextEvent( display, & event );
	}



	// vanilla plus // screen 1 fails on my 2 monitor laptop
	// and SCREEN_0 is opening on the second monitor, same as xterm mon
	int get_screen_0() const { return SCREEN_0; }

	Visual * get_Default_Visual() {
		return DefaultVisual( display, SCREEN_0 );
	}

// https://tronche.com/gui/x/xlib/display/display-macros.html
// Other depths may also be supported on this screen (see .PN XMatchVisualInfo ).

	int Default_Depth() { return DefaultDepth( display, SCREEN_0 ); }

	GC Default_GC() { return DefaultGC( display, SCREEN_0 ); }

	// DefaultRootWindow
	// DefaultScreenOfDisplay
	// ScreensOfDisplay
	// DefaultScreen 	// TRY maybe using ssh second screen ? xvnc ?
	// DefaultColormap
	// DisplayCells // of cmap
	// DisplayPlanes // depth ??
	// DisplayString // kept opening string
	// int n = ScreenCount( display ) // 1
	// ServerVendor
	// VendorRelease
	//XExtendedMaxRequestSize
	// XMaxRequestSize
	// WidthOfScreen( Screen * screen )
	// HeightOfScreen( Screen * screen )
	// WidthMMOfScreen
	// HeightMMOfScreen
	// Screen *DefaultScreenOfDisplay(Display *display);

	Screen *Default_Screen_Of_Display(Display *display) {
		return  DefaultScreenOfDisplay(display);
	}
};

struct X_Display_2 : public X_Display_1 { // GEN "obj_ref0 * PTR // WRAPPER


	X_Colours cmap;
	A_Map_W map_W;	// find X_Window from { Window win } // LEAK
        X_Window_Root * root_window;

	// wm items that ALL get
	 static Atom atom_wm_delete_window;
	 static Atom atom_wm_state;
	 static Atom atom_wm_state_above;
	 static Atom atom_wm_state_add;
	 static Atom atom_wm_state_remove;

	~X_Display_2()
	{
		// DTOR // CODE HERE // simply forgets reachability of nbytes
		// VIRTUAL INLINE means inner class implements all the way down
		// INLINE simply "RUNS" the SCRIPT through COMPILER
		// ADJACENT PHRASES CONNECTED
		// OPTIMISER reduces this SCRIPT to NULL CODE to run
		// VIRTUAL += UDEF FEATURE
		// X_Display_3 += SCRIPTS_use_of_X_Display 
		// X_Display += SCRIPTS_use_of_X_Display "ALIAS"
		// ALIAS == "X_Display"
		// LIBR += UDEF
		// DTOR = lookup( OBJ.VTBL[VTBL_%s] ) // VTBL_DTOR //
	}

	// X_Display 
	// CTOR PTR == VAL
	//
	X_Display_2() // VARNAME == "display" //
	: X_Display_1( NULL )
	, cmap( NULL ) // what is the correct C++ of doing this ?
	{
//		late_init_display( display );
//	virtual bool late_init_display( X_Display * _disp ) = 0;
	}

///	X_Display * display = new X_Display( "{ MATCH SCRIPT_VAR_NAME %s }" )
///	X_Display * display = new X_Display( "{ PTR }" ) // nbytes
///	X_Display( X_Display * display = "VARNAME" ) // VARNAME == "display" //
///	Display * display;

	/*
		X_Display_1 

			trying to do nothing more than STO PTR

			KDB needs DERIVE_TYPE { class UDEF is X_Display { SCRIPTS }

			trying to be VAR_NAME and nothing more

			That is a TOKEN in SCRIPT INTERPETER
			No need for a VALUE, pass the TOKEN
			CACHE [ KEYS ] { VARNAME VARTYPE VARDATA VAREXPR }
			CACHE VAR_EXPR {
				GETTER for %s.rhs is { SCRIPT }
				 SCRIPT
				  PAGE
				   GETTER for "VAR_EXPR" { pcts GETTER }
				
				PRE-RUNNING SCRIPT 

					worked example
					CACHE += EXPR

						idx = $1
						expr = "%s_%s_t" 
						 eg VARNAME_%s_t

					known value
					CACHE += TOKEN

				SETTER for %s.rhs is { SCRIPT }

			}
	*/


};

/*!
	X_Display_3 is the basic connection to the X server.
	There is no default constructor, to prevent accidental creates,
	but NULL uses the default display_name.

	Copy constructor is C++ default
*/
struct X_Display_3 : public X_Display_2
{

 static int reasons_to_stay;

	bool reasons_to_stay_incr() { reasons_to_stay++; return true; }
	bool reasons_to_stay_decr() { reasons_to_stay++; return reasons_to_stay > 0; }
	bool reasons_to_stay_test() {return reasons_to_stay > 0; }

	/*!
		create a STRUCT ready for a future connection
	*/
	X_Display_3();

	/*
		sub_windows appear in events (I think)
		top_levels are reasons to stay
	*/

	void add( X_Window * W )
	{
		map_W.add( W );
		reasons_to_stay_incr();
	}

	void del( X_Window * W )
	{
		map_W.del( W );
		reasons_to_stay_decr();
	}

	X_Window * find_Window( Window w )
	{
		return map_W.find( w ); // DO CHECK NULL
	}

	bool guess_screen_size( A_WH & WH ); // pair of side by side = double
	bool test_list_depths(); //

	bool process_event( XEvent & report );
	void process_events_forever();
};

struct X_Display : public X_Display_3 {

	str1 display_name;

	// _PLUS is builtin, _PLUS_LIBR, useful for UDEF
	// _UDEF is extra, so  pick _PLUS as APPARENT

	// should throw if the display is not openable
	X_Display( const char * _display_name )
	: X_Display_3()
	, display_name( _display_name )
	{
	}

	bool open_display()
	{
		if(!X_Open_Display( display_name ) ) {
			 return FAIL_FAILED();
		}
		return true;
	}

	// odd but help provide at least an X_Display
	// base class needs derived to pass to X_Window_Root CTOR

	virtual bool late_init_display( X_Display * _X_Display );
	// CTOR calls X_Open_Display( display_name )
	bool X_Open_Display( const char * display_name );
};


}; // NAMESPACE
#endif
