#ifndef DBG_H
#define DBG_H

//! debug level
extern int dbg;

// BAD STYLE macros but who cares

#define DBG0(stmt) if( dbg >= 0 ) {stmt}	/* almost always */
#define DBG1(stmt) if( dbg >= 1 ) {stmt}
#define DBG2(stmt) if( dbg >= 2 ) {stmt}
#endif
