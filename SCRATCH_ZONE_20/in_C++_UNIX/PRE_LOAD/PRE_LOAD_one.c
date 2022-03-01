
#define _GNU_SOURCE	// RTLD_NEXT // must be early
#include <dlfcn.h> 	// dlsym

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>	// errno perror


#include <stdio.h>
#include <stdarg.h>	// va_start etc

// man dlopen says C is stuffed up (as per usual)
// and the (VOID *) is wrong because FUN_PTR size != data ptr size
// gcc -pedantic permits use of void * into prototyped though
// *(void **) (&cosine) = dlsym(handle, "cos");

typedef void * (* ok_FUNC_PTR_t)(); // a good thing but C-ISO says no PTR sz
typedef void * FUNC_PTR_t; // works with gcc -pedantic // because dlsym does
// typedef *(void **) (& FUNC_PTR_t)(); // strange mess // never valid

FUNC_PTR_t call_dlsym_fn( const char * sym, FUNC_PTR_t * EA_FUNC_VAR )
{
	// dlsym looks up STR0 "open" and returns its ADDR PTR
	// call_dlsym_fn wraps it with a fn
	// call_dlsym /*MACRO */ calls _fn

	// RTLD_NEXT // is how to resolve layer on layer

	FUNC_PTR_t PTR = * EA_FUNC_VAR;
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

// cast any EA_FUNC_VAR to FUNC_PTR_t
// gcc -pedantic permitted cast from FUNC_PTR to VOID_STAR
#define call_dlsym( sym, EA_FUNC_VAR ) \
        call_dlsym_fn( sym, (FUNC_PTR_t *) EA_FUNC_VAR )


static // is default anyway for global to this file
int (* orig_open)( const char * pathname, int flags, ... ) = 0 ; // mode_t mode 

// extern
int open( const char * pathname, int flags, ... ) // mode_t mode 
{
	// LD PRELOAD means calling open() calls this open() from this .so

	fprintf( stderr, "This is the PRE_LOAD_ONE version of open() \n");

	// simulating open( "filename", flags, ... ) needs VAR ARGS
	// we lookup the original open() and call it
	// actually the var args is not done fully

	va_list args;
	va_start( args, flags );
	int mode = va_arg( args, int ); // touch wood it was provided // ...
	va_end( args );

	if(!call_dlsym( "open", & orig_open )) {
		return -1;
	}

	if(0) // TEST a second lookup // though above would exit 
	if(!call_dlsym( "open", & orig_open )) {
		fprintf(stderr,"FAIL on SECOND LOAD\n"); // I pass it anyway
	}

	fprintf(stderr, "call /* orig_open */ open( %s, %d, %3.3o )\n",
		pathname, flags, mode );

	int t = orig_open( pathname, flags, mode ); // mode_t from varargs

	if(t<0) {
		perror(pathname);
	}
	return t ; // fd

}
