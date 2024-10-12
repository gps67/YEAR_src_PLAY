#include <windows.h>

#include <stdio.h>

#define APP_NAME "wnd_class"
// ##_MINX # 

// EDIT #define WINDOW_TITLE "Window Title"
#define WINDOW_TITLE "SCRIPT_Window Title" 

// EDIT #define APP_NAME_wnd_class "APP_NAME_wnd_class" 
// STEPS // APP_NAME edit
#define APP_NAME_wnd_class "wnd_class_MINX"

char * window_text;

HINSTANCE h_module = 0;          // current instance of 'module'

void CenterWindow(HWND h_window ); // h_window handle of window WND

//  wnd_proc - windows_event_handler function
// todo OBJ = lookup( h_window )
// on_WM_PAINT


LRESULT CALLBACK wnd_proc ( // all TOPLEVELS type WAX same WNDPROC
  HWND h_window,
  UINT message,
  WPARAM wParam,
  LPARAM lParam
 )
{
/*
	h_window	window handle
	message		WM_MESSAGE
	wParam		eg 'A' when that key pressed
	lParam		eg scancode encoded when that key pressed

	Seems that we will need to do a HT lookup of h_window
	to find the thing that it corresponds to

	Then theres sub-windows, so h_window is a detail with a parent
	Both get recorded
*/

	switch (message)
	{
		// ----------------------- first and last
		case WM_CREATE:
printf( "wnd_proc( W: 0x%X, MSG: 0x%X _WM_CREATE, wP: 0x%X, lP: 0x%X ); WM_CREATE h_module = 0x%X \n", h_window, message, wParam, lParam, h_module );
			CenterWindow( h_window );
			break;

		case WM_DESTROY:
			PostQuitMessage(0); // send the message 0==> WM_QUIT
			break;


		// ----------------------- get out of it...
		case WM_RBUTTONUP:
			DestroyWindow( h_window );
			break;

		case WM_KEYDOWN:
printf( "wnd_proc( %X, %X, %X, %X ); WM_KEYDWN\n", h_window, message, wParam, lParam );
			if (VK_ESCAPE == wParam)
				DestroyWindow( h_window );
			break;


		// ----------------------- display our minimal info
		case WM_PAINT:
		{
printf( "wnd_proc( %X, %X, %X, %X ); WM_PAINT\n", h_window, message, wParam, lParam );
printf( "// # // INDENTED_BLOCK of TEXT calling GDI api\n"); // paint_zone

		// LOCAL PRINT_HEAD_item
		// h_window hdc.CTOR = PAIRED_CALL_begin BegainPaint()
			PAINTSTRUCT PAINT_ZONE;

		// hdc = BeginPaint( window, PAINT_ZONE )
			HDC         hdc; // handle draw context
			// any other CTORS before BeginPaint_ZONE
			hdc = BeginPaint( h_window, &PAINT_ZONE );

		// rect = Client_Rect( window ) // after title, scrolls, menu
			RECT        rect; // h_window . Client_Rect 
			GetClientRect( h_window, &rect );

		// FGBG text.fgbg.text_colour // hdc = this_paint_zone
		// FGBG item = text.fgbg.text_colour // hdc = "TextColor"

			SetTextColor( hdc, RGB(240,240,96) );

		// BACKGROUND TRANSPARENT

			SetBkMode( hdc, TRANSPARENT );

		// DRAW list //

		   //	traverse_tree_calling_DRAW

		// DRAW text // DT_ auto CENTER single LINE

			DrawText(
		 	  hdc,
			  window_text, // as if this is a button sort of hting 
			  -1, // ??
			  &rect, // window where centered
			  // V CENTER ITEM 
			  DT_CENTER | DT_SINGLELINE | DT_VCENTER
			 ) ;

			// LAST_UPTO_EOT
			// BLANK_LINE after indented SCRIPT // EOT
printf( "// # // INDENTED_BLOCK_EDGE\n");
			EndPaint( h_window, &PAINT_ZONE);
			break;

printf( "// # // INDENTED_BLOCK_api\n");
printf( "// # // INDENTED_BLOCK_is_CODE_API_TEXT\n"); // eg DRAW WM_PAINT()
		}

		// ----------------------- let windows do all other stuff
		default:
			return DefWindowProc( h_window, message, wParam, lParam);
	}
	return 0;
}

namespace WAX {
 /*!
	A Window class provides the message handler function.

	A DLL can create/de-register classes when it is loaded
	CS_GLOBALCLASS
http://msdn2.microsoft.com/en-us/library/ms633574(VS.85).aspx

	An application gets its classes auto deleted - 

	An application (AKA module), usually has ONE class, 
	to handle the top level window (or each of them)

	The class belongs to the hInstance, so when the program exits,
	it should be sure that all windows are closed - isnt that automatic?

	When using a DLL, hook into the DLL unload, and shut all windows.

	So now allocate a pointer to be added to each window,
	or other method to hold ClientData of window
 */
 class wnd_class {

	WNDCLASSEX wc; // wnd_class uses abbr wc = WIN32_WND_CLASS
	/* struct WNDCLASSEX { 
		UINT       cbSize; 		// its own
		UINT       style; 		// CLASS STYLE -vs- WINDOW STYLE -?-
		WNDPROC    lpfnWndProc; 	// wnd_proc function
		int        cbClsExtra; 		// more memory for Cls
		int        cbWndExtra; 		// more memory per Wnd - Object *
		HINSTANCE  hInstance; 		// the APPLICATION instance or DLL
		HICON      hIcon; 
		HCURSOR    hCursor; 
		HBRUSH     hbrBackground; 
		LPCTSTR    lpszMenuName; 
		LPCTSTR    lpszClassName; 
		HICON      hIconSm; 
	}; */
	ATOM wc_atom; // what is this - if I added it ?
	friend class create_window_helper;

  public:
	wnd_class(
		// h_instance =  this DLL or APP
		HINSTANCE h_Instance,	// this APP ID // or that own the class
		// option name == xFFFF byte_A byte_B u16_BA
		// name "Module" // or api_BUTTON
		const char * app_WND_CLASS_name,	// GUI_Name == "MINX" // WAX_
		// pointer to the code that
		WNDPROC wndProc		// the WM_message handler lp_fn
	)
	{
printf( "wnd_class( h_Instance=%X, '%s' ); \n", h_Instance, app_WND_CLASS_name );
		init_null();

		// class belongs to the h_Instance module // app api exe dll
		wc.hInstance = h_Instance;
		// the class is found by name in 3 search places
		wc.lpszClassName = app_WND_CLASS_name;
		// the point of a class, is the handler func
		wc.lpfnWndProc = wndProc;

		// make it all default usable
		default_bg_black();
		default_icon();
		default_cursor();

		default_style(); // accept double clicks
	}

	void init_null()
	{
		ZeroMemory(&wc, sizeof(wc)); // seems to be necessary!
		// DONE // ZeroMemory(&wc, sizeof wc);
		// init NULL
		wc.cbSize = sizeof(wc);
		wc.cbClsExtra = 0;	// must be 40 or less for W98
		wc.cbWndExtra = 0;	// must be 40 or less for W98
		wc_atom = 0;

		// TODO cdSize = sizeof(this) // of derived this

		/*
			T * P = (T*) getWindowLong( h_window, GWL_USERDATA );
 * 		*/
	}

	void default_bg_black()
	{
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	}

	void default_icon()
	{
		// you can use the program instance if its in one of the DLLs
		// from NULL file, load the icon called ... IDI_ ...
		// APPLICATION
		// INFORMATION
		// ASTERISK
		// QUESTION
		// WARNING
		// EXCLAMATION
		// HAND
		// ERROR
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	}
	void default_cursor()
	{
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		// wc.hCursor = LoadCursor(NULL, IDC_WAIT);
	}

	void default_style()
	{
		wc.style =
		  CS_DBLCLKS	// double clicks enabled!
		| CS_VREDRAW	// V
		| CS_HREDRAW;	// H

	//	| CS_SAVEBITS	// backing pixmap of what is BEHIND this
	//	| CS_NOCLOSE	// all TopLevel should have a close button
	//	| CS_OWNDC	// each window gets its own DC
	//	| CS_PARENTDC	// clip is parents window, not own edge!
	//	| CS_GLOBALCLASS // DLL
	//	| CS_DROPSHADOW // enabled for menus, see SPI_SETDROPSHADOW
	//	| CS_BYTEALIGNCLIENT //
	//	| CS_BYTEALIGNWINDOW // the outside not the inside
	}

	bool Register_Class()
	{
		//ExW means WIDE - and extra files required on W98  RTFM
		wc_atom = RegisterClassExA(&wc); // A means ASCII W means WIDE
		if(!wc_atom) {
			 printf("RegisterClass() failed - message?\n");
			 return false;
		}
		printf("RegisterClass() wc_atom=%X\n", wc_atom );
		return true;
	}

	const char * get_ClassName() const
	{
		return wc.lpszClassName;
	}
 };

 // CTOR_HELPER // DECL_MODULE LOAD_MODULE INIT_MODULE RESUME_THREAD // 

 struct create_window_helper // helps you create window frames outer_window
 {
	create_window_helper()
	{
		init(); // CTOR from dirty memory // DTOR to NULL INDEXES
	}

	/*!
		const char * 		// STR0 // UTF8_and_ASCII
		window_class_name	// APP_NAME or
					// LIBR_NAME or
					// WINDOW_CLASS == _NAME

				aside	// The usual PREFIX_SPLITTING_MARKERS
					// LEX_GAP += "-" += "_" -= SP_or_GAP
					// ie a gap in a lex NOT SP 
					// HEURISTIC += LEX_GAP += ":" /
					// used in time
					// used in DTE_MAC u48_as_six_bytes
					// BYTE // as 2 hex_digits %02X // UPPER
					// $1:$2:$3:$4:$5:$6 // HILO_or_LOHI
					// thats a SWITCH to PICK GEN VIEW etc

		WIN32 works by each APP creating their own window_class_name

		multiple programs running are independent,
		but use the same event_decoder_API on_EVENT(ARGS) SESS_CTXT

		DUNNO what happens on EDIT and start a new PROCESS and WINDOW
		GUESS
		PREFER
		MAYBE obtain HWND of APP_GENRE_WC_genre // already updated

	*/
	const char * window_class_name; // of special type atomic ROM no free
	// IE the default malloc rule for STR0 might be ROM STR0 // do not free
	// WIN32 requires a window_class _name // "window_class" "window" "app_name"
	// ADD // wc_atom
	HWND hwnd_parent; // ?? ROOT or WMAN _HWND
	HMENU hwnd_menu; // window.menu.windows
	int X,Y,W,H; // zone // 
	long style;

	void init()
	{
		hwnd_parent = 0;
		hwnd_menu = 0;
		X = CW_USEDEFAULT;
		Y = CW_USEDEFAULT;
		W = CW_USEDEFAULT;
		H = CW_USEDEFAULT;
		style = 0;
		window_class_name = NULL; // LURK not str1 malloc
	}

	void init_default()
	{
		init();
		style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		W = 360;
		H = 240;
	}

	void set_window_class_name( const char * name )
	{
		window_class_name = "BUTTON";
		window_class_name = "EDIT";
		window_class_name = name;
	}

	void set_window_class_name( const wnd_class & window_class )
	{
		// The NAME has already been set in the SESS Module global CFG
		// this is when we tell WIN32 
		// The wnd_class cindow_class // is our GDI_API_SESS_TOKEN
		// each LIBR declares UNIQUE WIN32_NAME_wnd_class
		// OFFER Window_Class window_class // WIN32_LIBR //
		// wc_atom = window_class.wc_atom;
		// MACRO window_class.get_ClassName() { // EDIT_FOLD // 
		//  return wc.lpszClassName; // VIEW_CODE_INLINED //
		// VIEW_DATA_ACCESS GET class.name // class == window_class
		// GETTER is return wc.
		// }
		set_window_class_name( window_class.get_ClassName() );
		if(1) // try it
		if( window_class.wc_atom ) {
			/*
				so on WINE WIN32 AMD64
				32 bit int handle can be put in 64 bit register
				and is found
				without this, it still runs BUT
				window title bar not found from class
				window title appears in body
				window appears white not black
				text appears black not yellow
				text appears top left not centralised
				TODO: clear this down

				WORSE: when _name was "BUTTON" that happened
				presumable because

			-- after set STR from wc_atom
			printf( "window_class_name == '%s' \n", window_class_name );
			window_class_name == 'wine: Unhandled page fault on read access to 0000C027 at address F7B856BA (thread 0009), starting debugger...

			*/
			window_class_name = (const char *)  window_class.wc_atom;
			// warning cast u32 to u64_PTR_t PTR;
		}
	// FIXME unravel this TYPE UNION // SEE_AS PRE_TOKEN_ISED "STR"
	// FIXME unravel this TYPE UNION // SEE_AS PRE_TOKEN_ISED_PSG "SCRIPT"
	// FIXME unravel this TYPE UNION // SEE_AS PRE_TOKEN_ISED_TEXT "SCRIPT"
	// FIXME PRE_TOKEN_ISED_TEXT "{ SCRIPT }" // PSG_sublex
	// FIXME PARSED_TEXT "{ SCRIPT }" // PSG_sublex
	// FIXME PARSED_TEXT "{ DATA_DESC_SCRIPT }" // PSG_mini_machines
	//
	//	if( window_class.wc_atom ) {
	//		window_class_name = (const char *)  window_class.wc_atom;
	//		STRING != integer handle
	//	} else
	//		set_window_class_name( window_class.get_ClassName() );
	}

	bool create_window(
	 HINSTANCE h_module,          // current instance MODULE
	 HWND & hwnd_new_window,
	 const char * window_border_title
	)
	{
	// create the TOPLEVEL_api WINDOW_api // enable WMAN api
	 hwnd_new_window = CreateWindow( // "/* WIN32_api */" // CALLING NOW
		window_class_name,	// "/* Default */" // STR1 AppName
		window_border_title,    // "%s" "${APP_name}" // "UDEF"
		style, // double_click_opts WIN32,SYMBOLS { eg RTFM }
		X, Y, W, H, // position of window XYWH
		hwnd_parent, // parent surface api
		hwnd_menu, // is this a WIN32 api shortcut
		h_module, // WIN32 api module // ?? //
		0		// MDI would add a hwnd_menu + subid
	 );
printf( "create_window( %X, '%s' ); => %X \n", h_module, window_border_title, hwnd_new_window );
	 return hwnd_new_window != 0;
	}
 };

//	 hwnd_new_window = eg_CreateWindow( // "/* WIN32_api */" // CALLING NOW
//	 "/* WIN32_api */" 
//	 "/* %s */" // LIBR_ITEM // import LIBR in CODE_ZONE // data_SPEC


 struct hwnd_base
 {
	HWND h_window;
	hwnd_base( HWND h )
	{
		h_window = h;
	}
 };

};
using namespace WAX;

// like main( ARGV ) but WinMain( w_id, w_prev, line_str, nCmdShow

int APIENTRY WinMain(
	HINSTANCE h_Instance,	// of the program
	HINSTANCE h_Instance_prev, // ignore
	LPSTR     CMD_ARGS_str,	// command line // char* args; not progname
	int       nCmdShow)
{

/*
	should I keep the hInstance with
	h_module = h_Instance;
	because it is otherwise unset
*/

	MSG msg; // local MSG

	wnd_class window_class(
		h_Instance,	// of the program or PID or THREAD_MUX
		APP_NAME_wnd_class, // "wnd_class_MINX" // app name for the WND
		(WNDPROC)wnd_proc // API connector
	);
	// your edits here, then
	window_class.Register_Class();

	HWND h_window; // window_handle 
	// top_level window // no need to record id // or tokenised

	create_window_helper CW;
	CW.init_default();	// specific to window type
	CW.set_window_class_name( window_class );

	if( !CW.create_window(
		 h_module,	// current instance
		 h_window,	// RETVAR
		 WINDOW_TITLE // "window_title"
	) ) {
	// eg	return FAIL( "explain" ); // MATCH (bool) == false == explain
		return 1; // get error code
	}

// TUTOR says these are needed .. but apparently done by messages
// 	ShowWindow( h_window, SW_SHOWNORMAL);
// 	UpdateWindow( h_window );
//

	window_text = CMD_ARGS_str[0] ? CMD_ARGS_str : (char *)"Hello Windows!";

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//+---------------------------------------------------------------------------

//+---------------------------------------------------------------------------

void CenterWindow(HWND hwnd_self)
{
	RECT rw_self, rc_parent, rw_parent;
	// HWND	// handle of window // EA() { return hwnd_PICK } // EA = EXPR //
	// PICK == hwnd_parent
	// EA // AUTO_VAR_AS_FUNC_inline_MACROS // EA = GETTER_EXPR ARGS //
	HWND hwnd_parent = GetParent(hwnd_self);
	if (NULL==hwnd_parent) hwnd_parent = GetDesktopWindow();
	GetWindowRect(hwnd_parent, &rw_parent);
	GetClientRect(hwnd_parent, &rc_parent);
	GetWindowRect(hwnd_self, &rw_self);
	SetWindowPos(hwnd_self, NULL,
		rw_parent.left + (rc_parent.right + rw_self.left - rw_self.right) / 2,
		rw_parent.top  + (rc_parent.bottom + rw_self.top - rw_self.bottom) / 2,
		0, 0,
		SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE
		);
}

//+---------------------------------------------------------------------------
