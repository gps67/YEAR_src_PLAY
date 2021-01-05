#ifndef CR_LF_NUL_H
#define CR_LF_NUL_H

// #include "CR_LF_NUL.h"

// This does presume ASCII/unicode not ebcdic
//

#ifdef NUL
#undef NUL
#endif
static const char NUL = 0;
static const char TAB = 9;
static const char CR = 13;
static const char LF = 10;
static const char SP = 32;

// static const char ASCII_LF = 10;

// static const char DEL = 127;

#endif
