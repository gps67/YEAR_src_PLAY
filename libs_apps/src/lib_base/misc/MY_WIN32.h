#ifndef MY_WIN32_H
#define MY_WIN32_H

#ifdef WIN32
// #define WIN32_LEAN_AND_MEAN
// loses lots of socket definitions
// #undef WIN32_LEAN_AND_MEAN
// gains those socket definitions
#include <windows.h>
// UNDO if changed above // #undef WIN32_LEAN_AND_MEAN
#endif

#include "sleep_ms.h"

// extern void sleep_ms( int ms );
// extern void sleep_secs( int s );

#include "dgb.h"

// also included by dgb_fail.h
// provides:
// INFO("fmt %s",str); // vprintf 
// WARN("something");
// return FAIL("clue");

#endif
