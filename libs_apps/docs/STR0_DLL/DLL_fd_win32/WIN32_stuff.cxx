#include "WIN32_stuff_tester.h"
#include "WIN32_stuff.h"

// #define ZERO_FILL_STRUCT( AVAR ) memset( &(AVAR), 0 , sizeof( (AVAR) ) )
#define ZERO_FILL_STRUCT( AVAR ) ZeroMemory( &(AVAR), sizeof( (AVAR) ) )

// WIN32_CreateProcess_base::
WIN32_CreateProcess_base::WIN32_CreateProcess_base()
{
	lpApplicationName = NULL;
	lpCommandLine = NULL;
	lpProcessAttributes = NULL;
	lpThreadAttributes = NULL;
	bInheritHandles = FALSE;
	dwCreationFlags = 0;

	lpEnvironment = NULL;
	lpCurrentDirectory = NULL;

	ZERO_FILL_STRUCT( startup_info );
	ZERO_FILL_STRUCT( process_info );

	startup_info.cb = sizeof( startup_info );  

	void set_good_defaults();
}

void WIN32_CreateProcess_base::set_good_defaults()
{
	// request busy cursor and some time allocation
	startup_info.dwFlags |= STARTF_FORCEONFEEDBACK; // default anyway ?
}

bool WIN32_CreateProcess_base::call_CreateProcess()
{
	INFO_2("# CreateProcess( %s %s ) ... \r\n",
		lpApplicationName,
		lpCommandLine
	);
	bool ok = CreateProcess(
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
	if(!ok) {
		PERROR("CreateProcess()");
		INFO( lpApplicationName );
		INFO( lpCommandLine );
	}
	return ok;
}

////////////////////////////////////////////

WIN32_stuff_tester * new_WIN32_stuff_tester( app_session_base & that )
{
	return new WIN32_stuff_tester( that );
}

WIN32_stuff_tester::WIN32_stuff_tester( app_session_base & that )
: sess( that )
{
}

bool WIN32_stuff_tester::test1()
{
	return true;
	// WIN32_CreateProcess_vpn	vpn;
	vpn.lpCommandLine = "openvpn keys/DIR_openvpn.ovpn --service AirBookExit 0";
	if(! vpn.call_CreateProcess()) return false;
	vpn.set_no_console_vpn();
	vpn.wait_forever_for_child_exit();
	TRACE("test1");
	return false;
}
