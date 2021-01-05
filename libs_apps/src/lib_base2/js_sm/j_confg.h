#ifndef j_config_H
#define j_config_H

/*
	actually now I think about it this may be a source of strange behavior
	(where prop_name returns a float)
	because the client side is not using the same GCC flags as the DLL

	YOU MUST MANUALLY MAKE THIS CORRECT
	even thoug I am not sure what is what anyway!

	!!! what is the point of
	js-config.h
	js-confdefs.h
	with all the -D definitions in it
	if the defintions are not in it !!!

	also a bit naff to have no prefix to 'DEBUG'
*/

/* OLD
#define DEBUG
#define _DEBUG
#define TRACING
#define USE_SYSTEM_MALLOC 1
#define ENABLE_ASSEMBLER 1
#define ENABLE_JIT 1
#define MOZILLA_CLIENT
OLD */

// borrowed from output from build of mozjs

#define DEBUG 1
#define IMPL_MFBT
#define LZ4LIB_VISIBILITY
#define MOZILLA_CLIENT
#define _FORTIFY_SOURCE 2


#endif
