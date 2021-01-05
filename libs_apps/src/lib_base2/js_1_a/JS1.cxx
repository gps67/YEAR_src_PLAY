#include "j_confg.h"
//#include "hdrs.h"
#include "jsapi.h"
#include "js/Initialization.h" 	// JS_Init
// JS_NewRuntime
//#include "jsprvtd.h"

#include "JS1.h"
#include "edit_line.h"
#include "j_cx_dbg.h"
// #include "js/AutoByteString.h"


/*
	version 52 - close enough to 63 ?

	https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/How_to_embed_the_JavaScript_engine
*/

/*
 or is it? JSAPI might be the old API SM is crap lke that

https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/JSAPI_User_Guide
*/

using namespace JSSM;

// /home/gps/G/mozjs/include/mozjs-66/js/RootingAPI.h
// objects can move and all POINTERS updated
// except registers, temp stack protection see RootingAPI.h

// warning root
#define JS_AddRoot( cx, varobj ) false
#define JS_RemoveRoot( cx, varobj ) false

#if 0
	// so very unused

	bool JS1::GC_grab( JSObject ** obj )
	{
		if( JS_AddRoot( cx, obj ))
			return true;
		else {
			// ERROR NO MEM
			return false;
		}
	}

	void JS1::GC_drop( JSObject ** obj )
	{
		JS_RemoveRoot( cx, obj );
	}
#endif

/*
	app can switch this separately
*/
extern void my_ErrorReporter(
	JSContext *cx,
	const char *message,
	JSErrorReport *report
);

/*!
	script function print(a,b,c,d,e,f ...) is available for debug
	it adds a space between args and its own newline,
	it doesnt have %3.3d formatting

	TO get more, define your own io = Module.get_stream("STDERR")

	There are no other useful functions, which JS adds for itself
	(as does tclsh), but there are some builtins such as
		Math.sin( 3.14 / 2 )
 */
static
bool
Print(JSContext *cx, uns argc, JS::Value *vp )
{
	/*
		move CallArgs to base of my classes
		add get_String_from_argv( i )
		add cx JS1
	*/
	CallArgs argv = CallArgsFromVp(argc, vp);

	uns i;
	buffer1 buf_tmp;
	buffer1 buf;

	for (i = 0; i < argc; i++) {
		buf.clear();
		if( i ) buf.putc(' ');
		fprintf(
			stdout,
			"%s%s",
			i ? " " : "", 
			(STR0) as_string( argv[i], buf_tmp )
		);
	}
if(1)	fputc('!', stdout);
	fputc('\n', stdout);
	fflush(0);
	return true;
}

void JS1::print_rval()
{
#if 1 // 63
	RootedString strng(cx);
	buffer1 buf;
	// strng = rval.toString();
	// JSAutoByteString str_abs(cx, strng);
	// const char * str = str_abs.ptr();
	// if (!str) return;
	fprintf(stdout, "%s", (STR0) as_string( rval, buf ));
#endif
#if 0 // 185
	JSString *str = JS_ValueToString(cx, rval );
	if (!str) return;
	fprintf(stdout, "%s", JS_EncodeString(cx, str));
#endif
#if 0 // 180
	JS::Value argv[1];
	argv[0] = rval;
	Print( cx, /*globalObj,*/ 1, argv /*, NULL*/ ); /*180*/
#endif
}

/*
	switch debugging trace on
*/
static
bool
Tracer_on(JSContext *cx, uns argc, JS::Value *vp )
{
	gdb_invoke(1);
//	JS::Value *argv = JS_ARGV( cx, vp );
//	JS::Value *rval = JS_RVAL( cx, vp );
	j_cx_dbg jdb( cx ) ; // cant be NULL
	jdb.set_logfp_stderr(); // switched tracing ON // line by line tho ?
	return true;
}

/*
"jspubtd.h" LINE 419 ish
 * Typedef for native functions called by the JS VM.
 *
 * See jsapi.h, the JS_CALLEE, JS_THIS, etc. macros.
 * /

typedef bool
(* JSNative)(JSContext *cx, uns argc, JS::Value *vp);

	ALSO:
https://developer.mozilla.org/en-US/docs/SpiderMonkey/JSAPI_Reference/JSNative
	says that you have to use macros to get the return pointers etc
	Why does a 'minor' version get a major redefinition
	SpiderMonkey - the clue is in the title
*/

static JSFunctionSpec shell_functions[] = {
    {"tracer_on",  Tracer_on,  0},
    {"print",      Print,      0},
    {0}
};

void fail( const char * str )
{
	fprintf(stderr, "fail() %s\n", str );
	fflush(0);
	exit( 1 );
}

// removed but still there
#define xNULL_JS_FinalizeStub NULL

#if 0
// COPY+PASTE from cook book or tutorial
static JSClass x_global_class = { "global", JSCLASS_GLOBAL_FLAGS, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub, JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, NULL, JSCLASS_NO_OPTIONAL_MEMBERS };
#endif


// 
/*
	Theres not much point in defining this as it isnt passed back
	to the defined functions. Maybe a global static with Init() ??
*/

#define STACK_CHUNK_SIZE    8192

JS1::JS1()
: j_cx()
{
	rt = NULL;
//	cx = NULL;
	globalObj = NULL;

	filename = "no_file_script";
	lineno = 1;
}

void JS1::Init(
	int heapsize,
	int stacksize
)
{
	/*
		now the cookbook says it must all be
		one context per runtime

		could still have multiple globalObj or multimple JS1's
		but in that case only one JS_Init ?
	*/
	static bool done_JS_Init = false;
	if( done_JS_Init ) {
		fail("Not My Apps"); 

	} else {
		if (JS_Init())
			fail("JS_Init() failed to initialise the JS engine"); 
		done_JS_Init = true;
	}

#ifdef REMOVED_NOTEXPLAINED
	rt = JS_NewRuntime( heapsize );
	if (!rt)
		fail("can't create JavaScript runtime");
#endif
	// rt NULL

	// changes API, not in online docs
	cx = JS_NewContext( stacksize );
	if (!cx)
		fail("can't create JavaScript context");

#if 0
	JS_SetOptions(cx, JSOPTION_VAROBJFIX | JSOPTION_METHODJIT);
	JS_SetVersion(cx, JSVERSION_LATEST);
	JS_SetErrorReporter(cx, reportError);
#endif

	// OPTN // JS_SetContextPrivate(cx, anydatapointer );


#ifdef REMOVED_NOTEXPLAINED
	JS_SetErrorReporter( rt, my_ErrorReporter );
#endif

#ifdef REMOVED_NOTEXPLAINED
	JS_SetVersion(cx, JSVERSION_LATEST); // WHY ?
#endif

//	static const size_t K500 = 1024 * 500;

//	size_t gMaxStackSize = K500;
//	size_t gScriptStackQuota = JS_DEFAULT_SCRIPT_STACK_QUOTA;
//	ShellOperationCallback = FUNCTION;
/*
static bool
ShellOperationCallback(JSContext *cx)
{
    if (!gCanceled)
        return true;

    JS_ClearPendingException(cx);
    return false;
}

*/

// static void
// SetContextOptions(JSContext *cx)
// {
//	JS_SetNativeStackQuota(cx, gMaxStackSize);
//	JS_SetScriptStackQuota(cx, gScriptStackQuota);
//	JS_SetOperationCallback(cx, ShellOperationCallback);
// }

/*
    if (enableTraceJit)
        JS_ToggleOptions(cx, JSOPTION_JIT);
    if (enableMethodJit)
        JS_ToggleOptions(cx, JSOPTION_METHODJIT);
*/

/*
    JS_SetOptions(cx, JS_GetOptions(cx) | JSOPTION_ANONFUNFIX);
    JS_SetGCParameterForThread(cx, JSGC_MAX_CODE_CACHE_BYTES, 16 * 1024 * 1024);
*/


// 1.8.0
//	globalObj = JS_NewObject( cx, &my_global_class, 0, 0 );
//	JS_SetGlobalObject(cx, globalObj); // NOT NECESSARY

//      
// OR   JS_NewCompartmentAndGlobalObject(cx, &global_class, NULL)


#if 0
	globalObj = JS_NewGlobalObject( cx, &my_global_class );
//	globalObj = JS_NewGlobalObject( cx, &   global_class );
#else
	// got further
	// https://developer.mozilla.org/en-US/docs/SpiderMonkey/JSAPI_Reference/JS_NewCompartmentAndGlobalObject
	// https://bugzilla.mozilla.org/show_bug.cgi?id=755186
	// but URL says dont use it!
	// even though it is in shell/js.cpp
	// wunch of purveyors of financial products
	// OLD  globalObj = JS_NewCompartmentAndGlobalObject(cx, &my_global_class, NULL);
//	// globalObj = JS_NewGlobalObject(cx, &my_global_class, NULL);
#endif

// /home/gps/G/mozjs/mozjs-63.0.3/js/src/shell/js.cpp

   RootedObject globalObj(
  	cx,
	JS_NewGlobalObject(
		cx,
		&my_global_class,
		principals,
		JS::DontFireOnNewGlobalHook,
		options
	)
   );
    if (!glob)
        return nullptr;



	if(!JS_InitStandardClasses( cx, globalObj ))
		fail("!JS_InitStandardClasses(cx,global)");

	// Print
	if (!JS_DefineFunctions(cx, globalObj, shell_functions))
		fail("can't define JavaScript functions");

	// this points to cx which points to this
	JS_SetContextPrivate( cx, this );
}

JS1::~JS1()
{
	shutdown();
}

void JS1::shutdown()
{
	/* For each context created: */
	if(cx) JS_DestroyContext(cx);
	cx = NULL;
	if(rt) {
		JS_DestroyRuntime(rt);
		rt = NULL;
	}
	JS_ShutDown();                                                              
	// LEAK filename
}

bool JS1::call_func(
	const char * func_name,
	int argc,
	JS::Value * argv
)
{
	ok = JS_CallFunctionName(
		cx,
		globalObj,
		func_name,
		argc,
		argv,
		&rval
	);
	return ok;
}

void JS1::ClearPendingException()
{
	JS_ClearPendingException( cx );
}

bool JS1::IsCompilable( buffer1 & buff )
{
	return JS_BufferIsCompilableUnit(
		cx,
		true, // UTF-8 SUPPORT
		globalObj,
	(STR0)	buff.buff,
		buff.nbytes_used
	);
}

JSScript * JS1::CompileScript( buffer1 & buff )
{
	const char * filename = "STRING";
	int lineno = 1;
	JSScript * script = (JSScript*) JS_CompileScript(
		cx,
		globalObj,
	(STR0)	buff.buff,
		buff.nbytes_used,
		filename,
		lineno
	);
	if( !script ) {
		fprintf(stderr,"CompileScript returned NULL\n");
	}
	return script;
}

JSScript * JS1::CompileScript( const char * str )
{
	const char * filename = "STRING";
	int lineno = 1;
	JSScript * script = (JSScript*) JS_CompileScript(
		cx,
		globalObj,
		str,
		strlen( str ),
		filename,
		lineno
	);
	if( !script ) {
		fprintf(stderr,"CompileScript returned NULL\n");
	}
	return script;
}

JSScript * JS1::CompileFile( const char * filename )
{
	JSScript * script = (JSScript*) JS_CompileUTF8File(
		cx,
		globalObj,
		filename
	);
	if( !script ) {
		fprintf(stderr,"CompileFile returned NULL\n");
	}
	return script;
}

JSScript * JS1::CompileFile_hash( const char * filename )
{
	JSScript *script;
	FILE *file;

	file = fopen(filename, "r");
	if (!file) {
	/* LATER
		Number selects a fmt string - maybe LANG ?

		JS_ReportErrorNumber(
			cx,
			my_GetErrorMessage,
			NULL,
			JSSMSG_CANT_OPEN,
			filename,
			strerror(errno)
		);

		Using simpler fn, but what happens when the
		installed routine gets called

		Easy - but there is not CURRENT filename
		unless file loaded from ...
	... */
		// gExitCode = EXITCODE_FILE_NOT_FOUND;
		JS_ReportError( cx,
			"CompileFile_hash::fopen( %s )\n",
			filename
		);
		return NULL;
	}

        int ch = fgetc(file);
        if (ch == '#') {
            while((ch = fgetc(file)) != EOF) {
                if (ch == '\n' || ch == '\r')
                    break;
            }
        }
        ungetc(ch, file);
        script = (JSScript*) JS_CompileUTF8FileHandle( cx, globalObj, filename, file );
	// close ?
        return script;
}

bool JS1::ExecuteScript( JSScript * script )
{
	return JS_ExecuteScript(
		cx, 
		globalObj,
//  (JSObject*)
		script,
		& rval
	);
}

void JS1::DestroyScript( JSScript * script )
{
	// let GC do its thing?
	// JS_DestroyScript( cx, script );
}


int JS1::one_edit_line(EL::edit_line & E) // exit code
{
	ClearPendingException();
	if(!E.gets_PS1()) {
		return 1;
	}
	while(! IsCompilable( E.buff ) ) {
		if(!E.gets_PS2_append()) {
			return 2;
		}
	}
	JSScript * script = NULL;
	script = CompileScript( E.buff );
	if(!script) {
		// reportError DONE
		return 3;
	}
	if(! ExecuteScript( script ) ) {
		DestroyScript( script );
		script = NULL;
		// break;
	} else {
		print_rval();
	}

	if(script) {
		DestroyScript( script );
		script = NULL;
	}
	return 0; // CTRL-D, CTRL-C all exit 0
}

int JS1::loop_edit_line() // exit code
{
	EL::edit_line E( "js" );
	while(1) {
		int t = one_edit_line( E );
		switch(t) {
		 case 0:
		 break;
		 case 1:
		 	return 1;
		 break;
		 case 2:
		 	return 2;
		 break;
		}
	}
	return 0; // CTRL-D, CTRL-C all exit 0
}
