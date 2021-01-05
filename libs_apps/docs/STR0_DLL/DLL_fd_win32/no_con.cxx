#ifndef WIN32 
#error This is for WIN32
#endif

/*
// no_con.c
// runa a child process without a console
// stops openvpn from showing its console
// but also stops you killing it (this program gets killed)
// it does wait for it to exit though (when not killed)
//
// THIS SNIPPET (C) GPS - freely re-usable
//
// To run without a console, under mingw32, as a WIN32 sort of program
// you use this instead of main()
// it calls no_win_main( argc, argv )
//
// TODO: keep the argv[0] somewhere (if you dont do that with main()))
// TODO: keep the E: drive letter 
// TODO: some first steps, cross_platform, approach
// TODO: code against MAX argc, in a hostile environment, with utf8, etc
//
*/

#include <windows.h>
#include "win_main.h"
// #include "word_splitter.h"
// extern int main( int, char ** );
// extern "C" int no_win_main( int, char ** );
extern "C" int printf(const char*fmt,...);

#define MAXPATHLEN 300
#define ARGC_MAX 33
#define NUL 0 // char byte

#define ZERO_FILL_STRUCT( AVAR ) memset( &(AVAR), 0 , sizeof( (AVAR) ) )

int WINAPI
WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
)
{
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
