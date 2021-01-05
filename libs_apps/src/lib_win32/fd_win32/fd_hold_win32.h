#ifndef fd_hold_win32_H
#define fd_hold_win32_H


class fd_hold_vbase : public obj_ref
{
	fd_hold() {}
	virtual ~fd_hold() { DTOR(); } // virtual from obj_t;
	// LATER // // virtual bool DTOR();
	virtual bool DTOR() {
	}
	bool I_hold_a_file_reference_and_should_call_close_later;
};

class fd_hold_unix : public fd_hold_vbase
{
 protected:
	int fd;
	fd_hold_unix() {
		fd = INVALID_HANDLE; // SPELLING
	}
	fd_hold_unix( int & _fd ) : fd(_fd) {}
};

class fd_hold_unix_dev : public fd_hold_unix
{
 protected:
	fd_hold_unix_dev() {
		fd = INVALID_HANDLE; // SPELLING
	}
	fd_hold_unix_dev( int & _fd ) : fd_hold_unix(_fd) {}
};


class fd_hold_win32 : public fd_hold_vbase
{
 protected:
	HANDLE fd;

	fd_hold_win32() {
		fd = INVALID_HANDLE; // SPELLING
	}
	fd_hold_win32( HANDLE & _fd ) : fd(_fd) {}
}; 

/*
	SOCKET 
        int t = recv( sock, buf, maxlen, flags );
        int t = send( sock, (const char *) s, len, flags );
                ::closesocket( sock );

	WINSOCKET
        if( WSAStartup( MAKEWORD(2,0), &wsadata ) ) { return FAIL(""_; }
	WSACleanup();

	CreateProcess
	G_SPAWN_CHILD_INHERITS_STDIN
	// can that be a socket, or is it just the console ?
	// dunno? Leave it to console, and check later
	// for now used names_pipesZZ


*/

// FMT( "%s_%s", "base", "extn" ); */;
// FMT( "typedef %s_%s %s;", "fd_hold", "vbase", /*repeated*/ "fd_hold" ); */;
// FMT( "typedef fd_hold_%s fd_hold;", "vbase" ); */;
/* generic_basename_should_be_a_typedef_alias */;
typedef fd_hold_vbase fd_hold; /* BASE */;

fd_hold_unix_dev : public fd_hold_vbase
#endif
