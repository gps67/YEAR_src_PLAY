#ifndef JS1_H
#define JS1_H

#include "j_confg.h" // -DDEBUG=1 // fg spelling

// this was with mozill js 17 (not 1.8.5)
// porting to js 66 is a hassle
// even the recomended sample files fails
// the working sample that makes bin/js is 10,000 lines
// that IS mozilla

// #define JS17 1

#include "j_cx.h"

//#include "hdrs.h"
#include "jsapi.h"
#include "ints.h"
#include "buffer1.h"

extern "C" {
	struct JSScript; // added for 1.8.5 not needed for 1.8.0


#if 0
	extern JS_PUBLIC_API(bool)
	JS_AddRoot(JSContext *cx, void *rp);

extern JS_PUBLIC_API(bool)
JS_RemoveRoot(JSContext *cx, void *rp);

extern JS_PUBLIC_API(void *)
X_JS_GetPrivate(JSContext *cx, JSObject *obj);


extern JS_PUBLIC_API(bool)
X_JS_SetPrivate(JSContext *cx, JSObject *obj, void *data);
#endif
}



// extern
// bool
// Print(JSContext *cx, JSObject *obj, uns argc, JS::Value *argv, JS::Value *rval);
//

namespace EL {
	class edit_line;
};

namespace JSSM {

/*
	Theres not much point in defining this as it isnt passed back
	to the defined functions. Maybe a global static with Init() ??
*/

#define JS_DEFAULT_STACK_CHUNK_SIZE    8192
#define JS_DEFAULT_HEAP_SIZE	1000000L

/*!
 * MAIN JavaScript interpreter machine context
*/
class JS1 : public j_cx
{
 public:
	JSRuntime * rt;
//	JSContext * cx;		// inherited from j_cx
	JSObject  * globalObj;

	// error messages report this
	const char * filename;
	// uns lineno;
	uns lineno;

	// function call return val
	JS::Value rval;
	bool ok;

	bool mk_my_global(); // put in a seperate file

 public:
	~JS1();
	JS1();
	void Init(
		int  heapsize = JS_DEFAULT_HEAP_SIZE,
		int stacksize = JS_DEFAULT_STACK_CHUNK_SIZE
	);
	void shutdown();
	bool call_func(
		const char * func_name,
		int argc,
		JS::Value * argv
	);

	int loop_edit_line(); // exit code
	int one_edit_line(EL::edit_line & E); // exit code
	JSScript * CompileScript( const char * str );
	JSScript * CompileScript( buffer1 & buff );
	bool IsCompilable( buffer1 & buff );
	JSScript * CompileFile( const char * filename );
	JSScript * CompileFile_hash( const char * filename );
	bool ExecuteScript( JSScript * script );
	void DestroyScript(  JSScript * script );
	void ClearPendingException();
	void print_rval();
	/*!
		The private data of a context is the JS1 which created it.

		If you want your ClientData, create a subclass of JS1.
	*/
 static
	JS1 * get_JS1( JSContext * cx )
	{
		return (JS1*) JS_GetContextPrivate(cx);
	}
 /* ---------------------------------------------------------- */
 /*
	The global context is often as good as the local context,
	there is only one thread.
 */

	bool GC_grab( JSObject ** obj );
	void GC_drop( JSObject ** obj );

	void   set_priv( JSObject * obj, void * ptr )
	{
		// this should be the ONLY vector ?
	//	JS_SetPrivate( obj, ptr );
// https://github.com/mozilla-spidermonkey/spidermonkey-embedding-examples/blob/esr115/docs/Migration%20Guide.md

	//	JS::PrivateValue() - where void * is ptr is held
	//	JS::SetReservedSlot() - where That JS item is held
		const int slot_idx_obj = 1;
		JS_SetReservedSlot(obj, slot_idx_obj, JS_PrivateValue(ptr));
	}
	void * get_priv( JSObject * obj )
	{
		const int slot_idx_obj = 1;
		return JS_GetMaybePtrFromReservedSlot<void>(obj, slot_idx_obj);
	//	return JS::GetMaybePtrFromReservedSlot<void>(obj, slot_idx_obj);

	//	return JS_GetPrivate( obj );
	}

};

}; // NAMESPACE
#endif
