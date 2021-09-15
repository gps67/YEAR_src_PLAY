
#define _GNU_SOURCE	// RTLD_NEXT // must be early
#include <dlfcn.h> 	// dlsym

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>	// errno perror


#include <stdio.h>


typedef void (* ANY_FUNC)();

void * call_dlsym_fn( const char * sym, ANY_FUNC * EA_FUNC_VAR )
{
	ANY_FUNC PTR = * EA_FUNC_VAR;
	if( PTR ) {
		fprintf(stderr,"Already set FUNC_VAR '%s'\n", sym );
		return PTR; // PASS
		return 0; // FAIL
	}
	PTR = dlsym( RTLD_NEXT, sym ); // RTLD_NEXT
	if( ! PTR ) {
		// BUG-ETTE errno is zero 
		// NOUN = syn
		// VERB = dlsym
		// OPTS == NULL
		// ERRM = msg
		// SUBSYSTEM = CODE
		int e = errno;
		if(!e) {
			fprintf(stderr,"errno is zero, faking ENOENT\n");
			e = errno = ENOENT;
		}
		const char * msg = dlerror() ;
		fprintf(stderr, "dlsym(%s) -- %s\n", sym, msg );
		errno = e;
		return 0;
	}

	* EA_FUNC_VAR = PTR;
	return PTR;
}
// cast any EA_FUNC_VAR to ANY_FUNC
#define call_dlsym( sym, EA_FUNC_VAR ) \
        call_dlsym_fn( sym, (ANY_FUNC *) EA_FUNC_VAR )


static // is default anyway for global to this file
int (* orig_open)( const char * pathname, int flags, ... ) = 0 ; // mode_t mode 

// extern
int open( const char * pathname, int flags, ... ) // mode_t mode 
{
	fprintf( stderr, "open() called \n");

	if(!call_dlsym( "open", & orig_open )) {
		return -1;
	}

	if(!call_dlsym( "open", & orig_open )) {
		fprintf(stderr,"FAIL on SECOND LOAD\n"); // I pass it anyway
	}

	int t = orig_open( pathname, flags ); // mode_t from varargs
	if(t<0) {
		perror(pathname);
	}
	return t ; // fd

//	errno = ENOENT;
//	perror("errno from fake_open says");
//	return -1 ;
}
