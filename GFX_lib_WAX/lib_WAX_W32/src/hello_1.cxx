#include <windows.h>

#include <stdio.h>

#define APPNAME "HELLO_WIN"

char *pWindowText;

HINSTANCE g_hInst = 0;          // current instance of 'module'

void CenterWindow(HWND hWnd);

/*! obj_ref - refcounted object
	This is supposed to look a lot like libs_apps, but avoid using it
*/
struct obj_ref_t {
	virtual ~obj_ref_t() {}
	obj_ref_t() {ref_count = 0;}	// maybe demand an extra loop allocator ?
	int ref_count;

	void ref_incr() {
		ref_count ++;
	}
	void ref_decr() {
		ref_count --;
		if( ref_count == 0 ) {
			delete this;
		}
	}

	virtual
	void on_ref_count_zero() {
		// pre-call // calls DTOR twice !!
		// in C would check this NULL
		// it might decide to ref_incr ?
	}

	static
	void ref_incr(obj_ref_t *& obj) {
		obj->ref_count ++;
	}
	static
	void ref_decr(obj_ref_t *& obj) {
		obj->ref_count --;
		if( obj->ref_count == 0 ) {
			// protect against tree loops
			// ensure PTR left as NULL
			obj_ref_t * obj_2 = obj;
			obj = NULL;
			// OPTION // not inline from here //
			// OPTION // THREE calls per DTOR!
			obj_2 -> on_ref_count_zero(); // add CTXT here ?
			if( obj_2-> ref_count == 0 ) {
				delete obj_2; // call two DTORs inefficient
				// that also deletes memory
				// not same for MMAP tree
				// which W_t never is
			}
		}
	}
	// OK instead of ref_hold<T> use these
	static
	bool set_ptr(obj_ref_t *& ptr2, obj_ref_t * obj) {
		if(!obj) return set_NULL( ptr2 );
		ref_incr( obj );
		obj_ref_t * ptr2_old = ptr2;
		ptr2 = obj;
		if( ptr2_old ) ref_decr( ptr2_old );
		return true;
	}
	static
	bool set_NULL(obj_ref_t *& ptr2) {
		obj_ref_t * ptr2_old = ptr2;
		ptr2 = NULL;
		if( ptr2_old ) ref_decr( ptr2_old );
		return true;
	}

};

/*!	W_t - VTBL defines all that is a widget

	On WIN32 the HWND hwnd holds a lot of XYWH info
	Some extra fields are held here, wich is not efficient

	There is a circular reference, as WHND holds the W_t * self;

	All the different window types will need their own subclass of W_t,
	and they will all need to recognise the same base class API

	To get back to (or near to) the specific W_t subclass,
	we need some design luck. eg a button hooks into the
	on_click() at the BUTTON_t level to activate btn_run() (slow/fast)
	and the specific subclass holds the data that fits btn_run()
	When following that chain of calls, the BASE type magically 
	upgrades from W_t to SLOW_BUTTON_that_starts_a_process_t

	In that example virtual btn_run() is added by the BUTTON_t
	subtree, (SLOW_ means change_FGBG add_lock start_activity change FGBG)

*/
struct W_t : public obj_ref_t{
	virtual ~W_t() {}
	W_t():hwnd(0) {}

	HWND hwnd;

	// CURRENTLY UNUSED // !!
};


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
/*
	hwnd		<== window handle
	message		WM_MESSAGE
	wParam		eg 'A' when that key pressed
	lParam		eg scancode encoded when that key pressed

	Seems that we will need to do a HT lookup of hwnd
	to find the thing that it corresponds to

	Then theres sub-windows, so hwnd is a detail with a parent
*/

	switch (message)
	{
		// ----------------------- first and last
		case WM_CREATE:
printf( "WndProc( W: %X, MSG: %X, wP: %X, lP: %X ); WM_CREATE g_hInst = %X \n", hwnd, message, wParam, lParam, g_hInst );
			CenterWindow(hwnd);
			break;

		case WM_DESTROY:
			// FREE USER DATA etc
			PostQuitMessage(0); // send the message 0==> WM_QUIT
			break;


		// ----------------------- get out of it...
		case WM_RBUTTONUP:
			DestroyWindow(hwnd);
			break;

		case WM_KEYDOWN:
printf( "WndProc( %X, %X, %X, %X ); WM_KEYDWN\n", hwnd, message, wParam, lParam );
			if (VK_ESCAPE == wParam)
				DestroyWindow(hwnd);
			break;


		// ----------------------- display our minimal info
		case WM_PAINT:
		{
printf( "WndProc( %X, %X, %X, %X ); WM_PAINT\n", hwnd, message, wParam, lParam );
			PAINTSTRUCT ps;
			HDC         hdc;
			RECT        rc;
			hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rc);
			SetTextColor(hdc, RGB(240,240,96));
			SetBkMode(hdc, TRANSPARENT);
			DrawText(hdc, pWindowText, -1, &rc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

			EndPaint(hwnd, &ps);
			break;
		}

		// ----------------------- let windows do all other stuff
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
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
	WNDCLASSEX wc;
	ATOM wc_atom;
	friend class create_window_helper;
/*

struct WNDCLASSEX { 
	UINT       cbSize; 		// its own
	UINT       style; 		// CLASS STYLE -vs- WINDOW STYLE -?-
	WNDPROC    lpfnWndProc; 	// WndProc function
	int        cbClsExtra; 		// more memory for Cls
	int        cbWndExtra; 		// more memory per Wnd - Object *
	HINSTANCE  hInstance; 		// the APPLICATION instance or DLL
	HICON      hIcon; 
	HCURSOR    hCursor; 
	HBRUSH     hbrBackground; 
	LPCTSTR    lpszMenuName; 
	LPCTSTR    lpszClassName; 
	HICON      hIconSm; 
};
*/

  public:
	wnd_class(
		HINSTANCE hInstance,	// the application that own the class
		const char * app_name,	// CLASS of main window
		WNDPROC wndProc		// the WM_message handler
	)
	{
printf( "wnd_class( hInstance=%X, '%s' ); \n", hInstance, app_name );
		init_null();
		// this class belongs to the hInstance module / app
		wc.hInstance = hInstance;
		// the class is found by name in 3 search places
		wc.lpszClassName = app_name;
		// the point of a class, is the handler
		wc.lpfnWndProc = wndProc;

		default_bg_black();
		default_icon();
		default_cursor();

		default_style();
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

		/*
			T * P = (T*) getWindowLong( hwnd, GWL_USERDATA );

		 64 bit not 32 bit (as well as)
			SetWindowLongPtr(hWnd, GWLP_USERDATA, this);

		SetProp/RemoveProp/

https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowlongptra

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

 struct create_window_helper
 {
	create_window_helper()
	{
		init();
	}

	const char * window_class_name; // of special type atomic ROM no free
	int X,Y,W,H;
	long style;
	HWND hwnd_parent;
	HMENU menu;

	void init()
	{
		hwnd_parent = 0;
		X = CW_USEDEFAULT;
		Y = CW_USEDEFAULT;
		W = CW_USEDEFAULT;
		H = CW_USEDEFAULT;
		style = 0;
		menu = 0;
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
		window_class_name = name;
		window_class_name = "EDIT";
		window_class_name = "BUTTON";
	}

	void set_window_class_name( const wnd_class & window_class )
	{
		if( window_class.wc_atom ) {
			window_class_name = (const char *)  window_class.wc_atom;
			// CAST // putting a smaller INT inside a larger INT
			// CAST // BUT it is NOT a (const char *)
		} else
			set_window_class_name( window_class.get_ClassName() );
	}

	bool create_window(
	 HINSTANCE g_hInst,          // current instance MODULE
	 HWND & hwnd_new_window,
	 const char * border_title
	)
	{
	// create the browser
	 hwnd_new_window = CreateWindow(
		window_class_name,
		border_title,
		style,
		X, Y, W, H,
		hwnd_parent,
		menu,
		g_hInst,
		0		// MDI would add a menu + subid
	 );
printf( "create_window( %X, '%s' ); => %X \n", g_hInst, border_title, hwnd_new_window );
	 return hwnd_new_window != 0;
	}
 };

 struct hwnd_base
 {
	HWND hwnd;
	hwnd_base( HWND h )
	{
		hwnd = h;
	}
 };

}; // namespace WAX
using namespace WAX;

int APIENTRY WinMain(
	HINSTANCE hInstance,	// of the program
	HINSTANCE hPrevInstance, // ignore
	LPSTR     lpCmdLine,	// command line
	int       nCmdShow)
{

/*
	should I keep the hInstance with
	g_hInst = hInstance;
	because it is otherwise unset
*/

	MSG msg;

	wnd_class window_class(
		hInstance,
		"app_name",
		(WNDPROC)WndProc
	);
	window_class.Register_Class();

	HWND hwnd;

	create_window_helper CW;
	CW.init_default();	// specific to window type
	CW.set_window_class_name( window_class );

	if( !CW.create_window(
		 g_hInst,          // current instance
		 hwnd,
		 "SECOND TITLE"
	) ) {
		return 0;
	}

	/*
		TODO	associate hwnd with OBJ *
			deep so that WndProc is OOP
	*/

// TUTOR says these are needed .. but apparently done by messages
// 	ShowWindow(hWnd, SW_SHOWNORMAL);
// 	UpdateWindow(hWnd);
//

	pWindowText = lpCmdLine[0] ? lpCmdLine : (char *)"Hello Windows!";

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
	RECT rw_self, rc_parent, rw_parent; HWND hwnd_parent;
	hwnd_parent = GetParent(hwnd_self);
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
