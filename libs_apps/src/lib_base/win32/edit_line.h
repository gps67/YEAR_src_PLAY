#ifndef edit_line_H
#define edit_line_H

#include "buffer2.h"
#include "obj_hold.h"
#include "str2.h"

extern "C" {
//	struct File;
	typedef struct editline EditLine;
	typedef struct history History;
	typedef struct tokenizer Tokenizer;
}

namespace EL {


/*!
 * wraps lib-edit-line, alternative to readline
 * */

class edit_line_base
{
 public:
	edit_line_base();
	~edit_line_base();
	obj_hold<str2>	PS1;	// public but not NULL
	obj_hold<str2>	PS2;	// public but not NULL
	obj_hold<str2>	PromptString;
	buffer2		buff;
	const char * get_prompt(); // private - but not
// interface
//	bool gets_PS1();
//	bool gets_PS2_append();
};

#ifdef WIN32

class edit_line_win32 : public edit_line_base
{
 public:
	edit_line_win32( const char * prog = "progName" );
	~edit_line_win32();

// interface
	bool gets_PS1();
	bool gets_PS2_append();
};

class edit_line : public edit_line_win32
{
 public:
	edit_line( const char * prog = "progName" );
	~edit_line();
};

#else


class edit_line_unix : public edit_line_base
{
 public:

	EditLine	* _editline;
	History		* _history;
	Tokenizer	* _tokenizer;


 public:
	edit_line_unix( const char * prog );
	~edit_line_unix();

	const char * get_prompt();

	bool gets_PS1();
	bool gets_PS2_append();

};

class edit_line : public edit_line_unix
{
 public:
	edit_line( const char * prog = "progName" );
	~edit_line();
};
#endif

}; // namespace
#endif
