
#include <execinfo.h>
#include <cxxabi.h>
#include "dgb_call_stack.h"
#include "buffer1.h"
#include "p0p2.h"

bool dgb_print_call_stack()
{
	dgb_call_stack_t dgb_call_stack;
	dgb_call_stack.stack_get("call_stack"); // PRINT CALL STACK
	dgb_call_stack.print_call_stack_all();
	return true;
}

bool print_call_stack(FILE *out, int show, int skip )
{
	return  dgb_print_call_stack(out, show, skip );
}


bool dgb_print_call_stack(FILE *out, int show, int skip )
{
	dgb_call_stack_t dgb_call_stack;
	dgb_call_stack.stack_get("call_stack"); // PRINT CALL STACK dgb.backtrace 
	dgb_call_stack.print_call_stack( out, show, skip );
//	dgb_call_stack.DTOR(); // function on object gets API TEMP OBJ SCOPE

//	SIDEWAYS SCOPE mk_var dgb_backtrace_t "dgb_backtrace" // VARNAME //
//	SIDEWAYS SCOPE VARNAME_t VARNAME; // STO += C++ scoped memory
//	SIDEWAYS SCOPE VARNAME_t VARNAME; // STO += nbytes_init_null_CTOR_SCRIPT
//	SIDEWAYS SCOPE "%s_t %s // CMNT" VARNAME VARNAME; // STO += nbytes_init_null_CTOR_SCRIPT
//	SIDEWAYS SCOPE CTOR_SCRIPT SCRIPT = print_call_stack(...)
//	ALLOC VAR { dgb_call_stack dgb_call_stack; // CMNT } // %s backtrace
//	LOGIC dgb_call_stack dgb_backtrace dgb.backtrace //
//	MK_VAR as field of dgb_fieldname == "fieldname" // "backtrace" // tag //
//	


//	CTOR_SCRIPT SCRIPT // called via API 
	return true; // RAN_OK
}

/*CTOR*/
dgb_call_stack_t:: dgb_call_stack_t()
{
	stack_depth = 0;
	stack_strings = NULL;
}

/*DTOR*/
dgb_call_stack_t:: ~dgb_call_stack_t() {
	if(stack_strings) {
		free(stack_strings); // malloc()ed by backtrace_symbols
		stack_strings = NULL;
	}
}

bool dgb_call_stack_t:: stack_get(const char * stackname ) {

	stack_depth = backtrace(stack_addrs, max_depth);
	stack_strings = backtrace_symbols(stack_addrs, stack_depth);
	return PASS("GOT");
	return FAIL("TODO");
}

bool dgb_call_stack_t:: print_call_stack_all() {
	X_printf("inner [0] [%d // stack_depth \n", stack_depth );
	int show = 100;
	int skip = 0;
	return print_call_stack(stderr, show, skip );
	return true; // RAN_OK
}


bool dgb_call_stack_t:: parse_stack_string( 
	const char * line_in, // 
	buffer1 & ret_filename,
	buffer1 & ret_funcname,
	i64 & ret_offs,
	i64 & ret_addr
) {
	ret_filename.clear();
	ret_funcname.clear();
	ret_offs = -1;
	ret_addr = -1;

	buffer1 line_rw;
	line_rw.set( line_in ); // writable buffer

	char * P0 = (char *)line_rw.buff;
//	char * P2 = line_rw.nbytes_used + P0;
	char * P;

	P = strchr( P0, '(' );
	if( !P ) {
		ret_filename.set( line_in );
		return FAIL("no '(' in line '%s'", line_in );
	}
	*P = NUL;
	ret_filename.set(P0);	// ^ filename ( func + 0xOFF ) SP [0xADDR]

	P0 = P+1; 
	P = strchr( P0, '+' );
	if(!P)
		return FAIL("no '+' in line '%s'", line_in );

	*P = NUL;
	if( demangle_cpp_symbol( ret_funcname, P0 ) ) {
		// name = (char *)(STR0) name_buf; // use before free
	} else {
		ret_funcname = P0;
	}

	P0 = P+1;
	P = strchr( P0, ')' );
	if(!P) {
		return FAIL("no ')' in line '%s'", line_in );
	}

	*P = NUL; // 
	str0 str_offs = P0;
//	INFO("str_offs %s from line %s", (STR0) str_offs, line_in );
 //	INFO("str_offs %s", (STR0) str_offs );

	P0 = P+1;
	P = strchr( P0, '[' );
	if(!P)
		return FAIL("no '[' in line '%s'", line_in );
	// SP [
	P0 = P + 1;
	P = strchr( P0, ']' );
	if(!P)
		return FAIL("no ']' in line '%s'", line_in );
	*P = NUL; // 

	str0 str_addr = P0;
//	INFO("str_addr %s from line %s", (STR0) str_addr, line_in );
 //	INFO("str_addr %s", (STR0) str_addr );

	return true;
}

bool dgb_call_stack_t:: print_call_stack(FILE *out, int show, int skip ) {

	/*
		INFO FAIL are OK but need parallel world

		hence FILE * out; // stderr 

		GEN similar to INFO FAIL but not _FUNCTION_
		__PRETTY_FUNCTION__
	*/


// dgb_call_stack_t:: { ... } 
//	int max_depth = 100;
//	size_t stack_depth;
//	void *stack_addrs[max_depth]; // max_depth limit
//	char **stack_strings;


// 	bool stack_get(const char * VARNAME) // PRINT CALL STACK dgb.backtrace 
	stack_get("CALL_STACK");
 //	stack_depth = backtrace(stack_addrs, max_depth);
 //	stack_strings = backtrace_symbols(stack_addrs, stack_depth);

	// skip 2 depths
	// show 25 depths // corrupted stack 
	// errno /* libc errno */ 
	// app_err /* libr_error _from_REQ syscall RETVAL errno
	// app_err /* libr_error _from_REQ libr_func RETVAL libr_error_t
	// _t { error_number_t error_number } // enum explain // 

	INFO("show = %d skip = %d errno = %d", show, skip, errno );

	if( show < 1 ) show = 10;
	if( skip < 0 ) skip = 0;

	int maxx = show + skip;

	/*
		TODO - keep original stack depth numbering but only print maxx

		That probably means decoding all (100) levels,
		but only printing 5. Then again, you are not supposed to DUMP

		OR use 1 as the nearest to here not nearest to main
		then the math will work OK (ie need to obtain true stack depth)
	*/

	if( maxx > max_depth ) maxx = max_depth;
// if MAIN_TO_TIP and numbering that way
//	if( maxx < max_depth ) max_depth = maxx;

	/* OBTAIN STACK */
	/*
		https://panthema.net/2008/0901-stacktrace-demangled/

		Stack backtraces can be saved with backtrace(3), resolved
		into symbolic names using backtrace_symbols(3) and printed
		using backtrace_symbols_fd(3). These functions are well
		documented and fairly easy to use.

	
	man 3 backtrace


	*/

	if(!stack_name) stack_get("CALL_STACK");

//	X_printf(out, "# BACKTRACK { # \r\n" );

	const char * NOTE = "STACK";
	// SPLICE // NOTE = "%s" // DEFAULT "STACK" // ANYSTR //
	// CODE_OPTION ANYSTR_t ANYSTR; // ALIAS STR0 CSET_DIALECT 
	X_printf( "# %s # %s # cw--<<<---- PROG STACK  ---------- \n", get_prog_alias(), NOTE );

	// [0] is this function
	// [1] is almost certainly fn_WARN // when we get rid of extra layers
	// [2] is
	// SHOW MAIN to TIP (or TIP to MAIN)

	size_t lo = skip;
	size_t hi1 = lo + show; // 1 after the highest
	if( hi1 > stack_depth) hi1 = stack_depth;

	// [0 [1 [2 [lo [hi // hi is first byte of next zone
	X_printf("lo %d hi1 %d stack_depth %d \n", lo, hi1, stack_depth );

// main() is towards the [stack_depth] end
// tip() is [0] the inner that called backtrace

#define MAIN_TO_TIP 1
#if MAIN_TO_TIP
	for (size_t i = lo; i < hi1; i++)
#else
	for (size_t i = hi1-1; i>=lo; i--)
#endif
	{

	//	static const int main_is_3 = 3;

//     /nfs/md7/GPS/libs_apps/lib/libbase.so(_Z7fn_WARNPKciS0_S0_S0_z+0x115) [0x2b12c67c12de]

	__attribute__((unused))
		bool found = true; // optimistic
		char * line = stack_strings[i];
		char * name = NULL;
// stack_strings[ N ]
// [11] '/lib/x86_64-linux-gnu/libc.so.6(+0x29d90) [0x7f0f7ddf5d90]'
// [10] './test1_nested_pair.elf(+0x3f50) [0x563bfc6f2f50]'
// [9] './test1_nested_pair.elf(+0x4cc2) [0x563bfc6f3cc2]'
// [3] './test1_nested_pair.elf(+0x6986) [0x563bfc6f5986]'
// [-] 'left(+0x%s) [0x%x]' // left OFFS ADDR
// left is filename A.exe B.so C.dll
// left is C++ mangled name
// -rdynamic // 

	//	X_printf("stack_strings[%d] '%s'\n", i, line);

	buffer1 ret_filename;
	buffer1 ret_funcname;
	i64 ret_offs;
	i64 ret_addr;

	if(! parse_stack_string( 
		line, // 
		ret_filename,
		ret_funcname,
		ret_offs,
		ret_addr
	)) return FAIL_FAILED();

	X_printf("# SP[%2.2d] # %s   %s \n",
		i, 
		(STR0) ret_filename,
		(STR0) ret_funcname
	);

	continue;

		char func[20];
		X_printf( dgb_FMT_3_s_s_s_par, get_prog_alias(), NOTE, func );
		if( name )
			X_printf( "%s\r\n", name );
		else
			X_printf( "%s\r\n", line );

	}
	X_printf( "# %s # %s # ---------- \n", get_prog_alias(), NOTE );
//	X_printf(out, "# } # \n" );
	fflush(out);
	fflush(NULL);
	return true; // RAN_OK //
}

// -----------------------------------------


bool dgb_demangle_cpp_symbol( buffer1 & buf2, const char * symb )
{

	// { buffer1 LHS } = demangle { STR0 RHS }

	// CANT use FAIL etc ... because they call this

	// WANT optimised recycle of buffer1 // get_space( 2049 ) // 2K + 1
	// USE a carousel of buffer1's of get_space( 3K )
	// ON RELEASE put { wrapped buffer1 LHS } in recycle queue
	// overwrite optimised part of BUFF // or all of it // CTOR THEORY //
	// ON CLAIM { mark item as IN_USE CLAIMED BY AS UDEF }
	// UDEF means "SCRIPTED_TYPE" // eg "%s_%s" lhs rhs // lhs == "UDEF"
	// rhs == "{ FUNC NAME ARGS }" // NS CLS METH ARG[v]
	// lhs == "{ SPEC NAME DECL } " // DECL == SCRIPT


#ifdef WIN32
	buf.print( "%s()", symb ); // is it a function? eg an exception
	return true;
	
	const char * ret = 0;
#else

#if 0
#warning LOCK buf here
	static
#endif

	buffer1 buf;
	buf.clear();
	/*
		buf can get itself remalloced which is a problem.
		BY THE CALL TO abi::__cxa_demangle ????
		Setting it excessive is not OK,
		as a larger symbol will realloc the buf
		(though this is what buffer1 does, but ... )
		2049 must be above the 60 that is allocated without malloc

		YES the design of abi::__cxa_demangle is mental.
		I dont want it to malloc buffer, just fill upto limit.
		I dont want to malloc just to feed it, I want buffer1

		SO use a static buffer1, and LOCK this function (somehow)
	*/
	if(!buf.get_space( 2049 )) {
		// CANT CALL FAIL // even though get_space probably did !!
		e_print("#### demangle_cpp_symbol buf.get_space()) failed\n");
		return false;
	}
	int status = -1;
	const char * ret = abi::__cxa_demangle(
		symb,
		(char *) buf.buff,
		(size_t*) & buf.nbytes_alloc,	// cannot avoid malloc/free
		&status
	);
	if( status == 0 ) {
		// copy the static string (at the start of buf) to buf2
		// which is also in the buf
		buf2.print( "%s", ret );
		return true;
	}

	if((status==-2)&&(!ret)) {
		/*
		This is called by fn_WARN which is using symb = prototype
		and not symb = mangled elf name eg:
static int JS::j_class_plus::s_getProperty(JSContext *, JSObject *, long, long *
		so no not complain, and copy symb through
		Adding the () is ... for ... dunno
		*/
		if(0) e_print("#### symb %s not valid under the C++ ABI mangling rules.", symb );
		// do for C what C++ gets
		// the difference between __FUNCTION__ and __PRETTY_FUNCTION__
		// is that NS, class, args and () get added
		// we should also do NAMESPACE prefix stripping
		bool already_as = strchr( symb, ')' );
		if(already_as)
			buf2.print( "%s", (STR0) symb );
		else
			buf2.print( "%s()", (STR0) symb );
		return true;
	}
	if(!ret) e_print("#### ret==NULL status = %d symb = %s \n", status, symb);
	if( status == -2 ) {
	  if(0)	e_print("#### symb %s not valid under the C++ ABI mangling rules.", symb );
		buf2.put_plain( (str0) "()" ); // do for C what C++ gets
		return true;
	}
	if( status == -1 ) { e_print("#### malloc error");  return false; }
	if( status == -3 ) {
	  if(1)	e_print("#### symb '%s' but other parameter error", symb );
		return false;
	}
	e_print("#### return code %d", status );
	return false;
#endif // WIN32/ UNIX
}
