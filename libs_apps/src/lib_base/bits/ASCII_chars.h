#ifndef ASCII_chars_H
#define ASCII_chars_H

// #include "CR_LF_NUL.h"

// This does presume ASCII/unicode not ebcdic
//

#ifdef NUL
#undef NUL
#endif

static const char ASCII_NUL = 0;
static const char ASCII_TAB = 9;
static const char ASCII_CR = 13;
static const char ASCII_LF = 10;
static const char ASCII_SP = 32;

static const char ASCII_BS =  8;
static const char ASCII_DEL = 127;

static const char ASCII_QUOTE_SINGLE = '\'';
static const char ASCII_QUOTE_DOUBLE = '"';
static const char ASCII_QUOTE_BACK = '`';

static const char ASCII_BRACE_LEFT = '{';
static const char ASCII_BRACE_RIGHT = '}';

static const char ASCII_PAREN_LEFT = '(';
static const char ASCII_PAREN_RIGHT = ')';

static const char ASCII_BACKSLASH = '\\';

/*
	too many to mention
	auto add to local diary
	docs suggest man ascii # -k # man -k ASCII #

	ASCII_chars.h
	ASCII_Table_chars.h
	 CTRL { 0 <= SP }
          NUL {
       000   0     00    NUL '\0' (null character)   100   64    40    @
	  }
       001   1     01    SOH (start of heading)      101   65    41    A
       002   2     02    STX (start of text)         102   66    42    B
       003   3     03    ETX (end of text)           103   67    43    C
       004   4     04    EOT (end of transmission)   104   68    44    D
       005   5     05    ENQ (enquiry)               105   69    45    E
       006   6     06    ACK (acknowledge)           106   70    46    F
	 }
	 Glyphs
	 	# dammn need a vt100 manual with ASCII names
		# search?q=ascii+table

*/


// static const char DEL = 127;

#endif
