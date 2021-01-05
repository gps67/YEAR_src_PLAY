#ifndef WIN32_stuff_H
#define WIN32_stuff_H

// to use ATTACH_PARENT_PROCESS, ... or later version ...
#define _WIN32_WINNT 0x0501

//	INTEGRATE THIS WITH GLIB EVENT LOOP
//	WAIT FOR CHILD PROCESS
//	PIPE TO
//	PIPE FROM

#include "WIN32_stuff_tester.h"
#include "MY_WIN32.h"

// WIN32 is #defined as a CPP macro  '1' or something
namespace win32 {
	/*!
		dgb would probably prefer to connect to a console
		if the app is compiled with -mwindows there isnt one (yet)
		one good idea is to redirect via e_print()
		one good idea is to attach to the parents console
		one good idea is to attach to a desktop debugging console
		one good idea is to create a console 
		one good idea is to attach to a desktop debugging pipe
	*/
	struct ConsoleUtils 
	{
//		bool call_AttachConsole( DWORD process_id )
//		{
//			bool ok = AttachConsole( process_id );
//			if( ok ) return ok;
//			// how can you report this ?
//			PERROR( "AttachConsole() - presumably not avail" );
//			return false;
//		}

// when not in CPP - order of .h
//		bool call_AttachConsole_Parent()
//		{
//			return call_AttachConsole( ATTACH_PARENT_PROCESS );
//		}

		/*!
			detatch from the console - closing it if last user
		*/
		bool call_FreeConsole()
		{
			return FreeConsole();
		}

		/*!
			create a console and attach to it
			(call free / detatch first)
		*/
		bool call_AllocConsole()
		{
			return AllocConsole();
		}

		/*
			To find all users of this console use:
			n = GetConsoleProcessList( & ary, nmax )
			each ary is a ProcessId
		*/

	};
}

class WIN32_CreateProcess_base
{
 public:
	// http://msdn.microsoft.com/en-us/library/ms682425(VS.85).aspx

	LPCTSTR lpApplicationName;
	LPTSTR lpCommandLine;
	LPSECURITY_ATTRIBUTES lpProcessAttributes;
	LPSECURITY_ATTRIBUTES lpThreadAttributes;
	BOOL bInheritHandles;
	DWORD dwCreationFlags;
	LPVOID lpEnvironment; // dwCreationFlags.CREATE_UNICODE_ENVIRONMENT
	LPCTSTR lpCurrentDirectory;
	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;

	WIN32_CreateProcess_base();

	void set_good_defaults();

	void set_pos_pixels( int x, int y ) {
		startup_info.dwFlags |= STARTF_USEPOSITION;
		startup_info.dwX = x;
		startup_info.dwY = y;
	}

	void set_size_pixels( int w, int h ) {
		startup_info.dwFlags |= STARTF_USESIZE;
		startup_info.dwXSize = w;
		startup_info.dwYSize = h;
	}

	void set_size_chars( int w, int h ) {
		startup_info.dwFlags |= STARTF_USECOUNTCHARS;
		startup_info.dwXCountChars = w;
		startup_info.dwYCountChars = h;
	}

	void set_CREATE_NEW_CONSOLE()
	{	
		// do not inherit parents console (nor lack of)
		dwCreationFlags |= CREATE_NEW_CONSOLE;
		// this func does not UNDO any contradiction in calls!
	}

	// DETACHED_PROCESS
	// http://msdn.microsoft.com/en-us/library/ms682528(VS.85).aspx

	void set_CREATE_NO_WINDOW()
	{	
		// vpn flashed a console which then disappeared
		dwCreationFlags |= CREATE_NO_WINDOW;
	}

	void set_HIDE()
	{	
		// vpn didnt even flash the console
		startup_info.dwFlags |= STARTF_USESHOWWINDOW;
		startup_info.wShowWindow = SW_HIDE;
	}

	void set_MINIMIZED()
	{	
		// console will be available for debug, but not obvious
		// ShowWindow() is used by app for own windows
		startup_info.dwFlags |= STARTF_USESHOWWINDOW;
		startup_info.wShowWindow = SW_SHOWMINIMIZED;
	}

	bool AttachConsole_Parent()
	{
		return false; // AttachConsole( ATTACH_PARENT_PROCESS );

	}
	
	bool call_GetStartupInfo() // of our own context
	{
		// dwFlags |= STARTF_USESTDHANDLES; // to get them ?
		// http://msdn.microsoft.com/en-us/library/ms686331(VS.85).aspx
		GetStartupInfo( & startup_info );
		return true;
	}

	~WIN32_CreateProcess_base()
	{
	 if( process_info.hProcess ) CloseHandle( process_info.hProcess );
	 if( process_info.hThread  ) CloseHandle( process_info.hThread );
	}

	void wait_forever_for_child_exit()
	{
	//	WaitForSingleObject( process_info.hThread, INFINITE );  
		WaitForSingleObject( process_info.hProcess, INFINITE );  
	}


	bool call_CreateProcess();
};

class WIN32_CreateProcess_vpn : public WIN32_CreateProcess_base
{
 public:
	WIN32_CreateProcess_vpn()
	: WIN32_CreateProcess_base()
	{
	}

	void set_no_console_vpn()
	{
		if(1) set_CREATE_NO_WINDOW();
		if(0) set_HIDE();
	}
};

// this was a stub class in the above, for no reason
class WIN32_stuff_tester
{
	app_session_base & sess;
 public:

	WIN32_CreateProcess_vpn	vpn;

	WIN32_stuff_tester( app_session_base & that );
	bool test1();
};

#endif
