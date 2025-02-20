
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h> // flock
#include <fcntl.h>

#include <stdio.h>
#include <errno.h>


int main( int argc, char ** argv )
{
	// open accepts 2 or 3 args
	// if we dont provide it, the stack provides some random
 //	int mode = 04755; // suid and 755
	int mode = 00640; // rw-r-----
	int open_flags = 0;
	open_flags += O_RDONLY;
	open_flags += O_CREAT;
	int flock_op = 0;
	flock_op |= LOCK_EX;
	flock_op |= LOCK_NB; // dont wait

	const char * filename = "myfilename";

	fprintf( stderr, "%s: run this in 2 windows\n", argv[0] );
	int t = open( filename, open_flags, mode );
	if(t<0) {
		// FAIL("myfilename");
		fprintf( stderr, "%s: got %d from open(%s) %m\n",
		argv[0], t, filename );
		return errno;
	}
	int fd = t;
	fprintf( stderr, "%s: fd = %d = open() \n", argv[0], t);
	t = flock( fd, flock_op ); 
	if(t<0) {
		// FAIL("myfilename");
		fprintf( stderr, "%s: got %d from flock(%s) %m\n",
		argv[0], t, filename );
		return errno;
	}
	fprintf( stderr, "%s: t = %d = flock() \n", argv[0], t);
	sleep(10);
	return 0 ;
}
