#ifndef dgb_H
#define dgb_H

extern void set_prog_alias( const char * s );
extern const char * get_prog_alias();

extern int complain_depth;

extern bool fn_WARN(
	const char * note,
	int depth,
	const char * file,
	const char * func,
	const char * fmt,
	...
);
extern bool fn_COMPLAIN1( const char * fmt, ... );
extern bool fn_COMPLAIN2( const char * fmt, ... );
extern bool fn_COMPLAIN3( const char * fmt, ... );

#define COMPLAIN1 fn_COMPLAIN1
#define COMPLAIN2 fn_COMPLAIN2
#define COMPLAIN3 fn_COMPLAIN3

#define V_PERROR(one)  fn_WARN( "PERROR", 0, __FILE__, __FUNCTION__, (const char *)one 
#define V_FAIL(one)    fn_WARN( "FAIL",   1, __FILE__, __FUNCTION__, (const char *)one 
#define V_WARN(one)    fn_WARN( "WARN",   2, __FILE__, __FUNCTION__, (const char *)one 
#define V_STEP(one)    fn_WARN( "STEP",   3, __FILE__, __FUNCTION__, (const char *)one 
#define V_INFO(one)    fn_WARN( "INFO",   3, __FILE__, __FUNCTION__, (const char *)one 
#define V_TRACE(one)   fn_WARN( "TRACE",  4, __FILE__, __FUNCTION__, (const char *)one 
#define V_TODO(one)    fn_WARN( "TODO",   4, __FILE__, __FUNCTION__, (const char *)one 

#ifndef stop_allowing_old_code_through
/*
#define PERROR(one)  V_PERROR(one))
#define FAIL(one)    V_FAIL(one))
#define WARN(one)    V_WARN(one))
#define STEP(one)    V_STEP(one))
#define INFO(one)    V_INFO(one))
#define TRACE(one)   V_TRACE(one))
*/

#define PERROR2(one,two)  V_PERROR(one),two)
#define FAIL2(one,two)    V_FAIL(one),two)
#define WARN2(one,two)    V_WARN(one),two)
#define STEP2(one,two)    V_STEP(one),two)
#define INFO2(one,two)    V_INFO(one),two)
#define TRACE2(one,two)   V_TRACE(one),two)

#define PERROR3(one,two,three)  V_PERROR(one),two,three)
#define FAIL3(one,two,three)    V_FAIL(one),two,three)
#define WARN3(one,two,three)    V_WARN(one),two,three)
#define STEP3(one,two,three)    V_STEP(one),two,three)
#define INFO3(one,two,three)    V_INFO(one),two,three)
#define TRACE3(one,two,three)   V_TRACE(one),two,three)

#define PERROR4(one,two,three,four)  V_PERROR(one),two,three,four)
#define FAIL4(one,two,three,four)    V_FAIL(one),two,three,four)
#define WARN4(one,two,three,four)    V_WARN(one),two,three,four)
#define STEP4(one,two,three,four)    V_STEP(one),two,three,four)
#define INFO4(one,two,three,four)    V_INFO(one),two,three,four)
#define TRACE4(one,two,three,four)   V_TRACE(one),two,three,four)
#endif

// this counts the number of variable above the fmt
// this uses _ and so should you ;-)

#if 0
#define PERROR(fmt)  PERROR_1("%s",fmt)
#define FAIL(fmt)    V_FAIL(fmt))
#define WARN(fmt)    V_WARN(fmt))
#define STEP(fmt)    V_STEP(fmt))
#define INFO(fmt)    V_INFO(fmt))
#define TRACE(fmt)   V_TRACE(fmt))
#else
#define PERROR(fmt)  V_PERROR(fmt))
#define FAIL(fmt)    V_FAIL(fmt))
#define WARN(fmt)    V_WARN(fmt))
#define STEP(fmt)    V_STEP(fmt))
#define INFO(fmt)    V_INFO(fmt))
#define TRACE(fmt)   V_TRACE(fmt))
#define TODO(fmt)    V_TODO(fmt))
#endif

#define PERROR_1(fmt,one)  V_PERROR(fmt),one)
#define FAIL_1(fmt,one)    V_FAIL(fmt),one)
#define WARN_1(fmt,one)    V_WARN(fmt),one)
#define STEP_1(fmt,one)    V_STEP(fmt),one)
#define INFO_1(fmt,one)    V_INFO(fmt),one)
#define TRACE_1(fmt,one)   V_TRACE(fmt),one)
#define TODO_1(fmt,one)    V_TODO(fmt),one)

#define PERROR_2(fmt,one,two)  V_PERROR(fmt),one,two)
#define FAIL_2(fmt,one,two)    V_FAIL(fmt),one,two)
#define WARN_2(fmt,one,two)    V_WARN(fmt),one,two)
#define STEP_2(fmt,one,two)    V_STEP(fmt),one,two)
#define INFO_2(fmt,one,two)    V_INFO(fmt),one,two)
#define TRACE_2(fmt,one,two)   V_TRACE(fmt),one,two)
#define TODO_2(fmt,one,two)    V_TODO(fmt),one,two)

#define PERROR_3(fmt,one,two,three)  V_PERROR(fmt),one,two,three)
#define FAIL_3(fmt,one,two,three)    V_FAIL(fmt),one,two,three)
#define WARN_3(fmt,one,two,three)    V_WARN(fmt),one,two,three)
#define STEP_3(fmt,one,two,three)    V_STEP(fmt),one,two,three)
#define INFO_3(fmt,one,two,three)    V_INFO(fmt),one,two,three)
#define TRACE_3(fmt,one,two,three)   V_TRACE(fmt),one,two,three)
#define TODO_3(fmt,one,two,three)    V_TODO(fmt),one,two,three)

// #define NULL 0L;

#endif

