#ifndef WIN32 
#error This is for WIN32
#endif



#include <windows.h>
#include "win_main.h"
#include "dgb.h"
// #include "word_splitter.h"
// extern int main( int, char ** );
// extern "C" int no_win_main( int, char ** );
extern "C" int printf(const char*fmt,...);

#define MAXPATHLEN 300
#define ARGC_MAX 33
#define NUL 0 // char byte

#define ZERO_FILL_STRUCT( AVAR ) memset( &(AVAR), 0 , sizeof( (AVAR) ) )

// #define TRACE(msg) printf("%s\n", msg )
// #define PERROR(msg) {TRACE(msg); return false; }

struct DGB_name
{
	const char * dgb_name;
	DGB_name()
	: dgb_name(0)
	{
		set_dgb_name( 0 );
	}
	DGB_name( const char * _dgb_name )
	: dgb_name( 0 )
	{
		set_dgb_name( _dgb_name );
	}

	void set_dgb_name( const char * _dgb_name )
	{
		if(!_dgb_name)
			dgb_name = "(Null)";
		else
			dgb_name = _dgb_name;
	}
};

class WIN32_Handle : public DGB_name
{
 protected:
        HANDLE handle;
 public:
	WIN32_Handle()
	: handle(0)
	{
	}
	WIN32_Handle( HANDLE _handle )
	: handle( _handle )
	{
	}
	bool is_open()
	{
		return handle != 0;
	}
	bool call_CloseHandle()
	{
		bool ok = true;
		if(is_open()) {
			ok = CloseHandle( handle );
		}
		handle = 0;
		return ok;
	}
        bool call_WaitForSingleObject( int ms = INFINITE ){
		TRACE_1("WIN32_WaitForSingleObject(%s)",dgb_name);
		WaitForSingleObject( handle, ms );  
		TRACE_1("WIN32_WaitForSingleObject(%s) DONE",dgb_name);
		return true; // or if it timed out ?
	}
};

class WIN32_Event : public WIN32_Handle
{
 public:
 // these are not supposed to be kept - setup parameters
	LPSECURITY_ATTRIBUTES lpEventAttributes;
	BOOL bManualReset;
	BOOL bInitialState;
	LPCTSTR lpName;

	WIN32_Event( const char * name )
	{
		lpEventAttributes = 0;
		bManualReset = false;
		bInitialState = false;
		lpName = (LPCTSTR) name;
		// set_dgb_name("fd_name") called with all above vars set
		set_dgb_name( name ); // dgb suddenly sees pre_registered_token
	}

	bool call_CreateEvent()
	{
		TRACE_1("WIN32_CreateEvent(%s)", (const char *)lpName );
		handle = CreateEvent(
			lpEventAttributes,
			bManualReset,
			bInitialState,
			lpName
		);
		if( GetLastError() == ERROR_ALREADY_EXISTS ) {
			INFO("OK NOT AN ERROR it already exists");
			SetLastError(0);
			return true;
		}
		if( handle ) {
			return true;
		} else {
			return PERROR_1("WIN32_CreateEvent(%s)", lpName);
		}
	}

        bool call_SetEvent()
	{
		TRACE_1("WIN32_SetEvent(%s)",lpName);
		if( SetEvent( handle )) return true;
		return PERROR_1("WIN32_SetEvent(%s)",lpName);
	}

        bool call_ResetEvent()
	{
		TRACE_1("WIN32_ResetEvent(%s)",lpName);
		if( ResetEvent( handle )) return true;
		return PERROR_1("WIN32_ResetEvent(%s)",lpName);
	}
};

int WINAPI
WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
)
{

	HANDLE H_IN = GetStdHandle( STD_INPUT_HANDLE );
	if( H_IN == INVALID_HANDLE_VALUE ) {
		return 22;
	}
	DWORD console_mode = 0;
	if(! GetConsoleMode( H_IN, & console_mode ) ) {
		// this is not a console
		// cannot printf!
		return 23;
	}
	TRACE("Yes I have a console");


	///////////////
        WIN32_Event W32_event( "AirBookExit");
        W32_event.call_CreateEvent();
        W32_event.call_ResetEvent();
        W32_event.call_WaitForSingleObject();

	TRACE("OK");

	return 0;
}

#if 0



	
	
	int argc = 0;
	int argc_limit = ARGC_MAX;
	char * argv[ argc_limit + 1 + 2 ]; // trailing NULL plus XS of 2

	// argv[0] = F:\somewhere\bin\progname.exe
        TCHAR program[MAXPATHLEN];
        unsigned int l = GetModuleFileName( hInstance, program, sizeof(program) );
	if( (l == 0 ) || ( l >= sizeof( program ))) {
		printf("# WinMain( %s %s ) ERROR filename too long\r\n", program, lpCmdLine );
		return 22; // usage
		
	}
	argv[argc++] = program;

	STARTUPINFO startup_info = {0};
	PROCESS_INFORMATION process_info = {0};
	startup_info.cb = sizeof( startup_info );  

	// fill in other parameters from own startup!
	GetStartupInfo( & startup_info );

	LPCTSTR lpApplicationName;
	LPTSTR lpCommandLine;
	LPSECURITY_ATTRIBUTES lpProcessAttributes;
	LPSECURITY_ATTRIBUTES lpThreadAttributes;
	BOOL bInheritHandles;
	DWORD dwCreationFlags;
	LPVOID lpEnvironment;
	LPCTSTR lpCurrentDirectory;

	// init_0
	ZERO_FILL_STRUCT( startup_info );
	ZERO_FILL_STRUCT( process_info );

	lpApplicationName = NULL;
	lpCommandLine = NULL;

	// child processes dont get sibling handles access
	lpProcessAttributes = NULL;
	lpThreadAttributes = NULL;

	bInheritHandles = FALSE;
	dwCreationFlags = 0;
	lpEnvironment = NULL;
	lpCurrentDirectory = NULL;


	printf("# WinMain( %s %s ) ... \r\n", program, lpCmdLine );

	// something seemed to flicker (a console appeared _ went)
	if(0) dwCreationFlags |= CREATE_NO_WINDOW;

	startup_info.dwFlags |= STARTF_USESHOWWINDOW;
	startup_info.wShowWindow = SW_HIDE;

	lpCommandLine = lpCmdLine;
	CreateProcess(
		lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		& startup_info,
		& process_info
	);

	
	WaitForSingleObject( process_info.hThread, INFINITE );  

	if( process_info.hProcess ) CloseHandle( process_info.hProcess );
	if( process_info.hThread  ) CloseHandle( process_info.hThread );
	return 22;
};
#endif
