// Copyright (c) 2003 Graham Swallow - the SPIN_PY work

#ifdef __GNUG__
#pragma implementation
#endif

// #include "config.h"
#include "SP_App.h"
#include "text_collector_utf.h"

// string comparisons from examples
// used by mk_tokenised_attr

inline
bool operator==(const SGMLApplication::CharString &str1,
		const SGMLApplication::CharString &str2)
{
  return (str1.len == str2.len
	  && (str1.len == 0
	      || memcmp(str1.ptr,
			str2.ptr,
			str1.len*sizeof(SGMLApplication::Char)) == 0));
}

inline
bool operator!=(const SGMLApplication::CharString &str1,
		const SGMLApplication::CharString &str2)
{
  return !(str1 == str2);
}

/*
	forgotten what this wasnt - but linkage error
*/
void SP_App::appinfo(const AppinfoEvent &)
{
}

extern "C" void test_link();

void test_link()
{
	SPIN_top * _top = NULL;
	SP_App * s = new SP_App( _top );
}
