#ifndef fmt_system_H
#define fmt_system_H

#include <stdarg.h>

__attribute__((__format__ (printf, 1, 2))) // fn_WARN(a,b,c,d,fmt,..
extern
bool fmt_system( const char * fmt, ... );

#endif
