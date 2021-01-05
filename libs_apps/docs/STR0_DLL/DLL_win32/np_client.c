#include <windows.h>
#include <stdio.h>
#define BUFSIZE 1024
#define PIPE_TIMEOUT 5000

BOOL write_some( HANDLE fd, const char * s1 )
{
	BOOL flg;
	int len = strlen( s1 );
	DWORD dwWrite;
	flg = WriteFile( fd, s1, len, &dwWrite, NULL);
	if (FALSE == flg)
	{
		printf("WriteFile failed for Named Pipe client\n");
		CloseHandle( fd );
		return FALSE;
	}
	else
	{
		printf("WriteFile succeeded for Named Pipe client\n");
		return TRUE;
	}
}

int main()
{
	HANDLE hFile;
	const char * str = "Data from Named Pipe client for createnamedpipe";
	hFile = CreateFile(
		"\\\\.\\pipe\\SamplePipe",
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed for Named Pipe client\n" );
		return 11;
	}

	if(!write_some( hFile, str )) return 12; // OK 
	if(!write_some( hFile, str )) return 12; // FAIL
	CloseHandle(hFile);
	return 0;
}


