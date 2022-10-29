/*	PRE_LOAD_one.c - example of LD_PRELOAD in action

	gcc -pedantic -c -o PRE_LOAD_one.o -fPIC  PRE_LOAD_one.c
	(MAIN_one.exec is plain, it calls open())
	LD_PRELOAD=./PRE_LOAD_one.so ./MAIN_one.exec // does the load
*/

#define _GNU_SOURCE	// RTLD_NEXT // must be early
#include <dlfcn.h> 	// dlsym

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>	// errno perror
#include <stdio.h>
#include <stdarg.h>	// va_start etc

// main() does nothing other than call open("myfilename",O_RDONLY)
// LD_PRELOAD=./PRE_LOAD_one.so ./MAIN_one.exec // does the load
// that installs the PRE_LOAD_one:open() as open()
// this then finds the original open() to call that (without syscall stub)


// VOID * // is broken //
// (VOID *) is wrong because FUN_PTR size != data ptr size
// FUNC_PTR_t // pointer to a func // used within PTR to PTR to a func //
// man dlopen says C is stuffed up (as per usual)
// gcc -pedantic permits use of void * into prototyped though
// *(void **) (&cosine) = dlsym(handle, "cos");

typedef void * (* ok_FUNC_PTR_t)(); // a good thing but C-ISO says no PTR sz
typedef void * FUNC_PTR_t; // works with gcc -pedantic // because dlsym does
// typedef *(void **) (& FUNC_PTR_t)(); // strange mess // never valid

FUNC_PTR_t call_dlsym_fn( const char * sym, FUNC_PTR_t * EA_FUNC_VAR )
{
	// user wants to write a variable that looks like original fn prototype
	// so type cast it on the way in
	// call_dlsym /* MACRO with TYPE CAST */ call_dlsym_fn
	// call_dlsym_fn wraps dlsym with a fn
	// dlsym looks up STR0 "open" and returns its ADDR PTR

	// RTLD_NEXT // is how to resolve layer on layer

	// repeat lookup // if you reuse a loop var, you must clear it to NULL
	FUNC_PTR_t PTR = * EA_FUNC_VAR;
	if( PTR ) {
		fprintf(stderr,"Already set FUNC_VAR '%s'\n", sym );
		return PTR; // PASS
		return 0; // FAIL
	}

	// lookup the "open" symbol
	PTR = dlsym( RTLD_NEXT, sym ); // RTLD_NEXT

	// if not found // diagnostic message // return NULL
	if( ! PTR ) {
		// BUG-ETTE errno is zero 
		// NOUN = syn
		// VERB = dlsym
		// OPTS == NULL
		// ERRM = msg
		// SUBSYSTEM = CODE
		int e = errno;
		if(!e) {
			// this always happens in sym not found
			fprintf(stderr,"errno is zero, faking ENOENT\n");
			e = errno = ENOENT;
		}
		const char * msg = dlerror() ;
		fprintf(stderr, "dlsym(RTLD_NEXT,'%s') -- %s\n", sym, msg );

// note how dlerror reported the .so that tried the lookup
// msg == "./PRE_LOAD_one.so: undefined symbol: absent_open"

		errno = e;
		return 0; // NULL => FUNC
	}

	* EA_FUNC_VAR = PTR; // RET_VAL //
	return PTR;
}

// MACRO requires a VAR to hold RET_VAL = PTR => FUNC
// MACRO adds cast to any EA_FUNC_VAR to FUNC_PTR_t
// gcc -pedantic permitted cast from FUNC_PTR to VOID_STAR

#define call_dlsym( sym, EA_FUNC_VAR ) \
        call_dlsym_fn( sym, (FUNC_PTR_t *) EA_FUNC_VAR )


// KEEP POINTER TO ORIGINAL "open" // not the one in this file.so

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

	if(!call_dlsym( "absent_open", & orig_open )) {
		fprintf(stderr,"GOOD FAIL on absent lookup\n"); 
	}

	if(!call_dlsym( "open", & orig_open )) {
		return -1;
	}

	if(1) // TEST a second lookup // get warning it has already been set
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
