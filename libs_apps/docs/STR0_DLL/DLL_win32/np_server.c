/*
http://www.codersource.net/win32_createnamedpipe.html
http://www.codersource.net/win32_named_pipe_client.html
*/

#include <windows.h>
#include <stdio.h>
#define BUFSIZE 1024
#define PIPE_TIMEOUT 5000
	
#define PIPENAME_SamplePipe "\\\\.\\pipe\\SamplePipe"

BOOL np_create_one(HANDLE * retfd, const char * pipename )
{
	/*
		pipename must be \\.\pipe\PIPENAME
		with \ doubled up for CPP
		total 256 limit
	*/
	DWORD open_mode = PIPE_ACCESS_DUPLEX;	// read/write access
	DWORD pipe_mode = 0;
	pipe_mode |= PIPE_TYPE_MESSAGE;		// message type pipe
	pipe_mode |= PIPE_READMODE_MESSAGE;	// message-read mode
	pipe_mode |= PIPE_WAIT;			// blocking mode
	*retfd = CreateNamedPipe(
		pipename,
		open_mode,
		pipe_mode,
		PIPE_UNLIMITED_INSTANCES,	// max. instances
		BUFSIZE,	// output buffer size
		BUFSIZE,	// input buffer size
		PIPE_TIMEOUT,	// client time-out
		NULL		// security attributes
	);
	if (*retfd == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	return TRUE;
}

BOOL np_connect( HANDLE fd )
{
	if( ConnectNamedPipe ( fd , NULL) ) return TRUE;
	if( GetLastError() == ERROR_PIPE_CONNECTED) return TRUE;
	return FALSE;
}

int
main ()
{
	HANDLE fd;

	CHAR chRequest[BUFSIZE];
	DWORD cbBytesRead;
	BOOL fSuccess;

	if(!np_create_one( & fd, PIPENAME_SamplePipe )) return 11;

	for (;;)
	{

		// Trying connectnamedpipe in sample for CreateNamedPipe
		// Wait for the client to connect; if it succeeds,
		// the function returns a nonzero value. If the function returns
		// zero, GetLastError returns ERROR_PIPE_CONNECTED.

		if(!np_connect( fd )) {
			// client could not connect 
			// so close the pipe. and exit
			CloseHandle ( fd );
			return 12;
		}
		BOOL is_open = TRUE;
		while(is_open) {

			fSuccess = ReadFile (
				fd,		// handle to pipe
				chRequest,	// buffer to receive data
				BUFSIZE,	// size of buffer
				&cbBytesRead,	// number of bytes read
				NULL		// not overlapped I/O
			);

			chRequest[cbBytesRead] = '\0';
			printf ("Data Received: %s\n", chRequest);

			if (!fSuccess || cbBytesRead == 0)
				is_open = FALSE;
		}

		FlushFileBuffers (fd);
		DisconnectNamedPipe (fd);
	}
	CloseHandle (fd);
	return 1;

}


/*
The above program waits infinitely for Client Pipes to connect from \\. \ pipe
\ SamplePipe.When a client is connected and data is received,
it is printed on to the console.
###########################################################
*/
