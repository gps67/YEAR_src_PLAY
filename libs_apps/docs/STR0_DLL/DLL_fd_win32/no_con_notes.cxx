/*

http://msdn.microsoft.com/en-us/library/ms682425.aspx
http://msdn.microsoft.com/en-us/library/ms684863(VS.85).aspx

http://www.gidforums.com/t-15226.html
http://www.catch22.net/tuts/undoc01

STARTUPINFO instance.wShowWindow =SW_HIDE;
STARTF_USESHOWWINDOW in STARTUPINFO::dwFlags.
Alternatively, add CREATE_NO_WINDOW flag to dwCreationFlags parameter of CreateProcess. 

To send a process a window signal try:

	PostThreadMessage( process_info.dwThreadId, WM_QUIT, 0, 0 );  

To wiat for a process to exit try:

	WaitForSingleObject( process_info.hThread, INFINITE );  

To see if handle is stale

	HANDLE hProc = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		process_info.dwProcessId
	);  
	ASSERT( hProc );
	const DWORD RetVal = WaitForSingleObject( hProc, 0 );  
	switch( RetVal ) {
	 case WAIT_FAILED:
		// lacking permission to Wiat for this process!
	 case WAIT_TIMEOUT;
		// process_handle is valid (zero timeout, no event)
	 default
		// event tht happened is EXIT signalled
	}
*/


#define ZERO_FILL_STRUCT( AVAR ) memset( &(AVAR), 0 , sizeof( (AVAR) ) )


struct W32_Create_Process
{
	/*!
	
	startup_info.dwXSize
	startup_info.dwYSize
			size
	startup_info.dwX
	startup_info.dwY
			location
	startup_info.dwXCountChars
	startup_info.dwYCountChars
			buffer size in char cells
	startup_info.dwFillAttribute
			fg/bg colour attributes
				BACKGROUND_	_BLUE
				FOREGROUND_	_BLACK
	startup_info.lpTitle
			display name

	startup_info.lpReserved
	startup_info.lpReserved2
	startup_info.lpDesktop
	startup_info.lpReserved2
	startup_info.wShowWindow ...
        SW_HIDE            0                      // hidden, get file selector
        SW_NORMAL          1  SW_SHOWNORMAL    1  // normal...
        SW_SHOWMINIMIZED   2                      // normal? 
        SW_MAXIMIZE        3  SW_SHOWMAXIMIZED 3  // proc win goes top left
        SW_SHOWNOACTIVATE  4                      // wierd, shows but like 0
        SW_SHOW            5                      // normal?
        SW_MINIMIZE        6                      // minimizes window
        SW_SHOWMINNOACTIVE 7                      // minimizes inactive window
        SW_SHOWNA          8                      // wierd, shows but like 0
        SW_RESTORE         9                      // fairly normal?
        SW_SHOWDEFAULT    10                      // fairly normal?
        SW_FORCEMINIMIZE  11  SW_MAX 11           // hidden, like 0


	startup_info.dwFlags:
		STARTF_USEPOSITION      |
		STARTF_USESHOWWINDOW    |
		STARTF_USECOUNTCHARS    |
		STARTF_USEFILLATTRIBUTE |
		STARTF_USESIZE          |
		STARTF_USESTDHANDLES
		;

	*/
	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;

	const char * dir;
	const char * exe;

	LPCTSTR lpApplicationName;
	LPTSTR lpCommandLine;
	LPSECURITY_ATTRIBUTES lpProcessAttributes;
	LPSECURITY_ATTRIBUTES lpThreadAttributes;
	BOOL bInheritHandles;
	DWORD dwCreationFlags;
	LPVOID lpEnvironment;
	LPCTSTR lpCurrentDirectory;

	W32_Create_Process()
	{
		dir = NULL;
		exe = NULL;

		init_0(); // sets startup_info.cb
	}

	~W32_Create_Process()
	{
	 if( process_info.hProcess ) CloseHandle( process_info.hProcess );
	 if( process_info.hThread  ) CloseHandle( process_info.hThread );
	}
	
	void init_0()
	{
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

		startup_info.cb = sizeof( startup_info );  
	}

	void set_ApplicationName( const char * str )
	{
		lpApplicationName = str;
		/*
			if NULL the first command word is used
			if USED then
				PATH is not used
				. and X: are used
				.exe must be specified (or .com)
				.bat must use "cmd.exe"

			HINT: MAX_PATH length when in CommandLine
			
		*/
	}
	void set_CommandLine( const char * str )
	{
		lpCommandLine = str;
		/*
			32K maximum
			** written if using the 16w varient so no const **
			NULL means reuse ApplicationName

		*/
	}

	void set_CREATE_NO_WINDOW()
	{
		dwCreationFlags |= CREATE_NO_WINDOW;
	}

	void setup_SHOWWINDOW_HIDE()
	{
	 if(1) {
		startup_info.dwFlags |= STARTF_USESHOWWINDOW;
		startup_info.wShowWindow = SW_HIDE;
	 }
	// alternatively
	 if(1) {
		set_CREATE_NO_WINDOW();
	 }
	}

	void setup_title(const char * str )
	{
		startup_info.lpTitle = str;
		startup_info.wShowWindow = SW_HIDE;
	}

	dwCreationFlags |= CREATE_NEW_CONSOLE,


	bool create()
	{
		 CreateProcess(
			NULL,
			exe,
			NULL,
			NULL,
			FALSE,
			0,
			0,
			dir,
			&startup_info,
			&process_info
		 );  
	}
};

void OpenCmdInDir( const char * dir, const char * cmd )  
{  


	// Closing handles since not needed further  
}  

// Usage  
OpenCmdInDir( _T( "C:\\Windows\\System32\\Drivers" ));  
