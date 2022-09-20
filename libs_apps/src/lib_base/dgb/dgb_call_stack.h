#ifndef H_dgb_call_stack_H
#define H_dgb_call_stack_H

#include <stdio.h> // FILE
#include "dgb.h"
#include "str0.h"

extern bool dgb_demangle_cpp_symbol( buffer1 & buf2, const char * symb );
extern bool dgb_print_call_stack(FILE *out, int show, int skip );
extern bool dgb_print_call_stack2(int depth = 30, int skip = 0 );
extern bool dgb_print_call_stack(); // stdout

extern bool print_call_stack(FILE *out, int show, int skip );
extern int X_printf( const char * fmt, ... );

#define dgb_FMT_3                       "# %s # %-5s # %-12s # "
#define dgb_FMT_3_2                     "# %-4s # %-12s # "
#define dgb_FMT_WIN_d_s                 "# WIN32 ERRNO # %d # %s"
#define dgb_FMT_UNIX_d_s                "# LIBC ERRNO # %d # %s"
#define dgb_FMT_UNIX_d_s_lf              dgb_FMT_UNIX_d_s "\r\n"
#define dgb_FMT_3_s_s_s                 "# %s # %s # %s: "
#define dgb_FMT_3_s_s_s_par             "# %s # %s # %s(): "


// // CALLER REWRITE TO //
// bool print_backtrace(FILE *out, int show, int skip )
// {
// 	return  dgb_print_call_stack(out, show, skip );
// }


struct dgb_call_stack_t {
	str0* stack_name;

	static const int max_depth = 100;
	size_t stack_depth;
	void *stack_addrs[max_depth]; // max_depth limit
	char **stack_strings;

	/*CTOR*/ dgb_call_stack_t();
	/*DTOR*/ ~dgb_call_stack_t();

	bool print_call_stack_all();
	bool print_call_stack(FILE *out, int show, int skip );
	bool stack_get();
	bool stack_get(const char * stackname );

// './test1_nested_pair.elf(+0x44ce) [0x56065e1b14ce]' 
// FILENAME ( FUNCNAME + 0xOFFS ) [ 0xADDR ]

	bool parse_stack_string( 
		const char * line, // 
		buffer1 & ret_filename,
		buffer1 & ret_funcname,
		i64 & ret_offs,
		i64 & ret_addr
	);

	static
	bool demangle_cpp_symbol( buffer1 & buf2, const char * symb ){
		return dgb_demangle_cpp_symbol( buf2, symb );
	}
};

#endif

