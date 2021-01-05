#ifdef WIN32

#include "dgb.h"
#include "MY_WIN32.h"

/*
	WIN32 loads socket functions from a DLL

	The linker and runtime can still load without the runtime,
	but gerenate errors. This loads the DLL and incr/decr its ref.

	This is static to the program, and I am not sure what happens
	if the static DTOR is not run. Does it leak? (Of course I care!)

// SEE ALSO: WSAGetLastError() // GetLastError seems OK, eg connection refused

*/

void win_sock_init()
{
        STEP("win_sock_init()");
        WSADATA wsadata;
        if( WSAStartup( MAKEWORD(2,0), &wsadata ) ) {
                FAIL("WSAStartup");
        }
}


void win_sock_shutdown()
{
        STEP("win_sock_shutdown()");
        WSACleanup();
}

class fd_win32_WSA_ref
{
 public:
        fd_win32_WSA_ref()
        {
                win_sock_init();
        }
        ~fd_win32_WSA_ref()
        {
                win_sock_shutdown();
        }
};

static fd_win32_WSA_ref static_ref_WSA;

#endif


