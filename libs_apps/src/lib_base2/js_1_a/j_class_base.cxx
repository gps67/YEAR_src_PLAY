
#include "j_class_base.h"
#include "e_print.h"
#include "dgb_fail.h"

#define IFDBG if(0)

namespace JSSM {

/*
	obj_ref_js is the INSTANCE base class,
	j_class_base is the CLASS base, but ...

	the js_class does a lot of the work for each instance,
	eg jsinst . get_prop()
	
*/

#define NULL_JS_FinalizeStub NULL

/*!
	sets the JSClass to almost NULL values, name and spare not set
*/
void j_class_base::init_JSClass()
{
	JSClass & my_class = *this;

	my_class.name = "UNSET";
	my_class.flags 
	= JSCLASS_NEW_RESOLVE 
	+ JSCLASS_HAS_PRIVATE
	;

	my_class.addProperty =   JS_PropertyStub;
	my_class.delProperty =   JS_PropertyStub;
	my_class.getProperty =   JS_PropertyStub;
#warning "was JS_PropertyStub now JSStrictPropertyOp "
	my_class.setProperty =   JS_StrictPropertyStub;
	my_class.enumerate   =   JS_EnumerateStub;
	my_class.resolve     =   JS_ResolveStub;
	my_class.convert     =   JS_ConvertStub;
	my_class.finalize    =   NULL_JS_FinalizeStub;
//	my_class.finalize    = s_JS_Finalise;

// 	my_class.getObjectOps = NULL;	// 1.8.0

#if 0
#endif
//	my_class.reserved0    = NULL;
	my_class.checkAccess  = NULL;
	my_class.call         = NULL;
	my_class.construct    = NULL;
//	my_class.xdrObject    = NULL;
	my_class.hasInstance  = NULL;

//	my_class.mark         = 0;
	// why wssnt this initialised NULL ?
//	my_class.reserveSlots  = 0;	// 1.8.0
//	my_class.reserved1  = 0;
      for(int i = 0; i < 19; i++ )
	my_class.reserved[i]  = NULL;

/* syntax, docs says its an array of 2, jsapi.h says its single jsapi
	my_class.spare[0] = 0 ;
	my_class.spare[1] = 0 ;

	IT HAS DISAPPEARED
	I'M NOW USING JSClass * my_class = this;
*/
}


/*!
	constructor - sets JSClass and keeps js1, not registered
*/
j_class_base::j_class_base(
	const char * class_name,
	JS1 * _js1
)
{
	//! used to plant GC roots
	js1 = _js1;

	init_JSClass(); // raw minimum

	name = class_name;
	// 
	// JSClass & my_class = *this;
	// my_class.name = class_name;

	/*
		OLD WAY: JSClass.spare[0] = this
		NEW_WAY: base class with pointer adjustment

		SPECIAL LURK, link from JS to C++
		jsword in 32 bit on Intel86, 64-bit need to check
		spare is unsed, except by this class as ClientData

		An alterative is to make this struct an extension of class,
		so that JS_GET_CLASS returns this directly!
		NEWS_ALERT that nolonger exists
		bcause the cx parameter is nolonger needed (duh)
		not even for error messages
	*/
//	my_class.spare = (jsword) this;
//	my_class == this !!

	prototype = NULL;

	if(1)
		parent_prototype = JS_NewObject( js1->cx, NULL, NULL, NULL );
	else
		parent_prototype = NULL;
}

/*!
	Return the C++ js_class_base instance of the JSObject, or NULL

	js_class_plus wraps this and casts it, as there is NO OTHER type,
	and  it gets called from a static function, specific to that JSClass.

	Before the call, we cant be sure if cx_obj is a PEER or a PLAIN,
	this class returns NULL if its plain.
*/
// STATIC
j_class_base * j_class_base::get_class_base ( const j_cx_obj & cx_obj )
{
	/*
		RULE: obj.class.spare == C++ instance of j_class_base
		OR: its a plain JS Object (no other library mixes .priv)
	*/

	/*
		every JSObject has a JSClass, get it
	*/
	JSClass * cls = cx_obj.GET_CLASS(); // seems a long way round
	if(!cls) {
		NULL_dgb_fail( "JS_GET_CLASS" );
		return NULL;
	}

	/*
		Our objects have extra C++ j_class_base, get it
	*/
	j_class_base * self = static_cast<j_class_base *>( cls );
	/*
		dynamic_cast would do a type check (it cant be used here)
		static_cast cannot check, but can add the 4 byte offset
		The NULL test ... will only detect NULL input

		there is a 4 byte difference between the base addresses
		this is changing my approach to casts from (PyObject*)
		I prefered it when the VTBL appeared in the MIDDLE
		and the pointer to the start remained exact

		It makes it as bad as subtracting the offset of a field,
		to find the start of a larger (containing) object
		actually that seems more honest now!
	*/
	if(!self) {
		e_print( "get_class_base found null spare - not a JS class\n");
		return NULL;
	}

	/*
		This supprises me because I previously had some test
		that placed VTBL in the middle of the derived class
		not pre-pended at the front.

		Maybe its time to look at all casts from base
		where the base isnt virtual, and beyond my control
	*/

#if 0
	/*
		Our classes use cls->spare to point to the C++ class-inst
		and that also points back to the JSClass. Check it.
	*/
//	j_class_base * self = (j_class_base *) cls->spare;
#endif

#if 0
	error: cannot dynamic_cast ‘cls’ (of type ‘struct JSClass*’)
	to type ‘class JS::j_class_base*’ (source type is not polymorphic)
	j_class_base * self = dynamic_cast<j_class_base *>( cls );
#endif

#if 0
	// tests
	cls = static_cast<JSClass *>( self );
	j_class_base * p3 = static_cast<j_class_base *>( cls );
	JSClass * p4 = static_cast<JSClass *>( self );
	e_print("j_class_base %p JSClass %p p3 %p p4 %x\n", self, cls, p3, p4 );
	exit(1);
#endif

	/*
		The old way was better, as the pointer was actually stored.
		The new way can falsely cast a JSClass pointer to C++
		by simply adding -4 to it.

		Previously I only had C++ JS objects,
		but mixing with other libraries might accidently get confused.
		EG other.spare was *probably* NULL
		The 'correct' way to do it, is to have TWO pointers (NOT DONE)
		if( PTR_ONE == MY_CODES_PTR_ONE ) { // single exact match 
			my_class = PTR_TWO // one of a dozen values
		}

	*/
	if( (j_class_base *) self != (j_class_base *) cls )
	{
		e_print( "get_class() # self != cls # %p %p", self, cls);
		// caution // return NULL;
	}
#if 0
	/* debugging */
	// if( & self->my_class != cls )
	if( (j_class_base *) & self != (j_class_base *) cls )
	{
		NULL_dgb_fail( "get_class() # ( & self->my_class != cls )");
		return NULL;
	}
#endif
	return self;
}

/*!
	define a script->C function in the scope of the script class
*/
bool j_class_base:: Def_Function(
	str0 name,		// has to be statically allocated ?
	JSNative function,
	int nargs
)
{
	return
        JS_DefineFunction(
                js1->cx,
                parent_prototype,
                name,
                function,
                nargs,		// number of args
                0		// extra flags - all deprecated
        );
}

/*!
	JS_DefinePropertyWithTinyId() adds the property with C accessors
*/
bool j_class_base::Def_Prop(
	const char * name,
	int tinyint,
	JSPropertyOp getter,
	JSStrictPropertyOp setter
)
{
	js1->ok = JS_DefinePropertyWithTinyId(
		js1->cx,
		parent_prototype,		// distinguish class and inst
		name,
		tinyint,
		JSVAL_VOID,		// initial value
		getter,
		setter,
		JSPROP_ENUMERATE	// for( key in ary ) ...
	);
	if(! js1->ok ) {
		fprintf(stderr, "#ERROR# Def_Prop \n" );
	}
	return js1->ok;
}

/*!
	JS_DefinePropertyWithTinyId() adds the property with C accessors
*/
bool j_class_base::Def_Prop(
	const char * name,
	JSPropertyOp getter,
	JSStrictPropertyOp setter
)
{
	js1->ok = JS_DefineProperty(
		js1->cx,
		prototype,		// distinguish class and inst
		name,
		JSVAL_VOID,		// initial value
		getter,
		setter,
		JSPROP_ENUMERATE	// for( key in ary ) ...
	);
	if(! js1->ok ) {
		fprintf(stderr, "#ERROR# Def_Prop \n" );
	}
	return js1->ok;
}

/*!
	THIS IS FINAL - DO NOT REPLACE IN SUBCLASS (used by CX.set_ret(jsc,inst))

	Upgrade inst to hold a new peer JSObject.

	Create a new JSObject

	Each obj_ref_js (C++) can hold a JS Object which is its peer.

	This function creates the JSObject and calls set_inst(inst,obj)
	to connect them.
*/
JSObject * j_class_base::mk_js_self(
	obj_ref_js * inst
)
{
	if( inst->js_self )
	{
		return inst->js_self;
	}
	JSContext * cx = js1->cx;
	JSObject * jsob;
	/*
		NewObject() parent==NULL causes JS to lookup JSOBS's
		get-prop 'constructor', so that its parent is usd

		could either allow get-prop, or avoid the call or ...
	*/
	JSClass & my_class = *this;
	jsob = JS_NewObject( cx, &my_class, prototype, NULL );
	if(!jsob)
	{
		JS_ReportError( cx, "FATAL: JS_NewObject failed\n");
		return NULL; 
	}
	// Forge the pair into peers, maybe move the code here ?
	set_inst( inst, jsob );
	return jsob;
}

/*!
	Connect the C++ and JS object pairs in the way that j_class_base
	expects.

	Called by mk_js_self(inst) and also by j_class_plus::new_Thing(args)
*/
void j_class_base::set_inst(
	obj_ref_js * inst,
	JSObject * jsob
)
{
	assert( inst );
	assert( jsob );

	JSClass & my_class = *this;
	IFDBG e_print( "Set_inst() class = %s\n", my_class.name );
	// The C++ object can easily find its JSObject
	inst->js_self = jsob;
	// The JSObject can easily find its C++ object
	// JSOB.priv == jsi
	IFDBG {
		buffer1 tn;
		e_print( "SetPrivate(jsob=%p,inst%p, insttype=%s)\n",
			jsob, inst, inst->type_name(tn) );
	}
	JS_SetPrivate( jsob, inst );
	// Dont delete the C++ object until JS has finished with it
	inst->ref_incr();
}

/*!
	Disconnect the C++ and JS object peers
*/
void j_class_base::split_cls_inst_jsob(
	obj_ref_js * inst,
	JSObject * jsob
)
{
	JS_SetPrivate( jsob, NULL );
	if(inst) inst->ref_decr();
}

bool j_class_base::ret_set_fn( JSContext *cx, JS::Value * vp, obj_ref_js * inst )
{
	JS::Value * retvar = vp; // #warning "LURK deprecated use of header private"
	if(!retvar) {
		js1->ReportError( "CODING ERROR: NULL retvar pointer");
		return false;
	}
	JSObject * jo = inst->js_self;
	if(!jo) jo = mk_js_self(inst);
	*retvar = OBJECT_TO_JSVAL( jo );
	return true;
}

bool j_class_base::ret_set_fn( JS::Value * retvar, obj_ref_js * inst )
{
	if(!retvar) {
		js1->ReportError( "CODING ERROR: NULL retvar pointer");
		return false;
	}
	JSObject * jo = inst->js_self;
	if(!jo) jo = mk_js_self(inst);
	*retvar = OBJECT_TO_JSVAL( jo );
	return true;
}

}; // namespace JSSM
