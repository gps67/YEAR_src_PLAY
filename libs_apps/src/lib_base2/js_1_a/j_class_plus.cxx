#include "j_class_plus.h"
#include "misc.h"

// WANT THE struct DEFINITION IN GDB //
// but adds errors
// #include "jsfun.h"

#define IFDBG if(0)

// using namespace JSSM;
namespace JSSM {

/*
	VIRTUAL DESTRUCTOR REQUIRED
*/

/*!
	constructor

	This is not much difference between _base and _plus,
	and maybe the functions should be moved down a bit
*/
j_class_plus::j_class_plus(
	const char * classname,
	JS1 * _js1,
	bool _allow_other_attributes
)
: j_class_base( classname, _js1 )
{
	allow_other_attributes = _allow_other_attributes;
	Init_Class();
}

/*!
	Define a property by NAME that the virtual get/set recognises.

	This specifies the C functions, others could be used elsewhere
*/
bool j_class_plus::def_prop_str( const char * name )
{
	IFDBG e_print("%s::def_prop(%s)\n", class_name(), name );
// TEST TYPE SPEC
        JSPropertyOp getter = 
        (JSPropertyOp)
		s_getProperty;
        JSStrictPropertyOp setter =
        (JSStrictPropertyOp)
		s_setProperty;

	return Def_Prop( name, getter, setter );
	// return Def_Prop( name, s_getProperty, s_setProperty );
}

/*!
	Define a property by id that the virtual get/set recognises.

	This specifies the C functions, others could be used elsewhere
*/
bool j_class_plus::def_prop( const char * name, int id )
{
	IFDBG e_print("%s::def_prop(%s,%d)\n", class_name(), name, id );
	return Def_Prop(
		name,
		id,
(JSPropertyOp)
		s_getProperty,
        (JSStrictPropertyOp)
		s_setProperty
	);
	// return Def_Prop( name, id, s_getProperty, s_setProperty );
}


/*!
	idx is a pre-allocated id, not necessarily what JS uses
*/
bool j_class_plus::def_prop_alloc( const char * name, jsc_prop_list_item * & prop )
{
	int jsid = -1;
	if(! prop_list.alloc_prop( jsid, name, prop ) ) return false;
	return def_prop( name, jsid );
}

/*!
	idx is a pre-allocated id, not necessarily what JS uses
*/
bool j_class_plus::def_prop_alloc( const char * name, int idx )
{
	int jsid = -1;
	if(! prop_list.alloc_prop( jsid, name, idx ) ) return false;
	return def_prop( name, jsid );
}


/*!
	Whatever id is used by JS, this object is the jsc's handler
	for that property (which may be the instances prop value
	when each inst has a class of its own )
*/
bool j_class_plus::def_prop_alloc( const char * name, obj_ref * obj )
{
	int jsid = -1;
	if(! prop_list.alloc_prop( jsid, name, obj ) ) return false;
	return def_prop( name, jsid );
}





/*
	getProperty always returns true, unless an actual error happened.

	If *vp isnt changed, JS notices and continues its search for property
	(ie it uses plain properties)
*/
/*!
	JSOB.attr calls this static C function which calls the virtual function
*/
// static
bool
j_class_plus::s_getProperty( JSContext *cx, JSObject *obj, jsid id, JS::Value *vp )
{
	gdb_invoke(1);
	/*
		repackage the args, use the peering links
	*/
	j_args_prop_get args ( cx, obj, id, vp );

	// debug demo
	buffer1 buf;
	// IFDBG
	INFO("prop_name (%s)", (STR0) args.prop_name(buf) );

	/*
		RULE obj.priv == some instance data (not null)

		Debugging check that inst is there
	*/
	if(!args.inst) {
		/*
			whilst calling NewObject, JS calls back
			asking for the constructor.

			return true without setting any retval
		*/
		return true;
	}

	if(!args.cls) {
		JS_ReportError(cx, "s_getProperty() NULL cls\n");
	}
	/*
		RULE obj.class.spare == class == self

		Get the actual j_class_self (args.cls)
	*/
	j_class_plus * self = args.get_class_plus();
	if(!self) {
		JS_ReportError(cx, "s_getProperty() NULL class\n");
		return false;
	}

	/*
		Call either the by_id or by_str virtual getter
	*/
	if( args.id_int >=0  )
	{
		if(!args.jsc_prop)
		{
			// array overshoot causes crash - damage limitiation
			gdb_invoke(true);
			return true;
			return false;
		}
		return self->j_get_property_by_id( & args );
	}

	// REUSE // buffer1 buf;
	IFDBG INFO("prop_name (%s)", (STR0) args.prop_name(buf) );

	if( args.id_js_str )
		return self->j_get_property_by_str( & args );

	// should never happen
	JS_ReportError(cx, "getProperty() - WHAT\\n");
	return false;
}

/*!
	Find the objects C++ (JS) class and call the virtual get_ function

	NB The function called is the j_class_plus's virtual, not the
	instances virtual function !!!
*/
#warning "TODO bool strict - unused - what is strict anyway? jspubtd.h line 196"
// static
bool
j_class_plus::
	s_setProperty( JSContext *cx, JSObject *obj, jsid id, bool strict, JS::Value *vp )
{
	j_args_prop_set args ( cx, obj, id, vp );
	if( !args.inst ) {
		JS_ReportError(cx, "setProperty() NULL inst\n");
		return false;
	}
	j_class_plus * self = (j_class_plus *) args.cls;
	if(!self) {
		JS_ReportError(cx, "s_setProperty() NULL class\n");
		return false;
	}
	if( args.id_int >=0  )
		return self->j_set_property_by_id( & args );
	if( args.id_js_str )
		return self->j_set_property_by_str( & args );
	JS_ReportError(cx, "setProperty - id not sensible\n");
	return false;
}

/*!
	Find the objects C++ class, and call its j_finalise function.
*/
// static
void
j_class_plus::
s_finalise( JSFreeOp *fop, JSObject *obj )
{
	// JS17 changes this (above) signatire, was 
	// was s_finalise( JSContext *cx, JSObject *obj )
	// now s_finalise( JSFreeOp *fop, JSObject *obj )
	// so where can we get a cx from ? fop->runtime()
#warning NULL cx used to obtain self from object
	JSContext *cx = NULL;
	j_args_delfunc args ( cx, obj );
	j_class_plus * self = (j_class_plus *) args.cls;
	if(!self) {
		JS_ReportError(cx, "s_finalise() NULL class\n");
	}
	if( args.inst ) {
		self->j_finalise( & args );
		args.split_pair();
	}
}

/*!
*/
	

/*!
	YET ANOTHER 1.8.5 stupidity - How do I get clasp ?

		The standard classes use one function per class
		and each is hardwired with clasp

		I *think* I need the PRIVATE of a JSNative

	s_new_Thing creates a new instance of clasp//c++ type
	It receives script parameters in JS_ARGV(cx,vp)
	It does NOT receive JS_THIS_OBJECT XX - it must create it

	The clasp comes from ...

	connected by JS_InitClass, s_construct is used when the script
	requests a new thing, not when embedded creates something and
	passes to the script ..

	static means there is no this-inst

	The script is new Stats_Module() or new Something()
	That is my_JSNative constructor,

	create new_Thing
*/
// static
bool
j_class_plus:: s_new_Thing(
	JSContext *cx, 
	uns argc, 
	JS::Value *vp 
)
{
	// gdb_invoke( false );
	gdb_invoke( true );

	/*
		clasp = ... this works ...
		where is the JSAPI cookbook version of this?
		I think it needs assert(!FUN_INTERPRETED(fun))
		but it is only invoked by bay of InitClass callback

		NOW it doesnt

	*/
	JS::Value jsval0 = *vp;
	JSObject * callee = JSVAL_TO_OBJECT( jsval0 );
	JSFunction * fun = (JSFunction *) callee;
	INFO("P =%p", fun );
//	JSClass * clasp = (JSClass *) fun->u.n.clasp;
	JSClass * clasp = NULL;

	JS::Value *argv = JS_ARGV( cx, vp );


//	see JS_InitClass()
//	Native CTOR must create the NewObject itself 
//	1.8.0 did create the instance jsobj, 1.8.5 is different
//	DO NOT call JS_THIS_OBJECT(cx,vp)
//	DO NOT call IsConstring IsConstructing // irony ?


	/*
		Create the JSObject (this JSNative is a constructor)
		NULL proto and parent get proto from clasp
	*/
	JSObject *newobj = JS_NewObject( cx, clasp, NULL, NULL );
	if (!newobj)
		return false;
	//	newobj needs a C++ object creating and attaching

	/*
		args is a temp utility struct, to call C++ macros (cx)
	*/
	j_args_newfunc args ( cx, newobj, argc, argv );

	/*
		Get the C++ varsion of classp, with its VTBL.
		The actual class will be derived, VTBL does it
	*/
	j_class_plus * self = get_class_plus( args );
	if( !self )
	{
		JS_ReportError(cx, "s_new_Thing() impossible class mismatch\n");
		return false;
	}

	JS::Value val = OBJECT_TO_JSVAL( newobj );
	JS_SET_RVAL(cx,vp,val);

	/*
		The subclass (VTBL) will create an obj_ref_js * inst
		and attach that to the newobj (which is held in args).

		it can also use JS argv from the script,
		but not vp[0] which was the js:classp with a bit set

		Then return TRUE or FALSE and report error
	*/
	return self->new_Thing( & args );
}

/*!
	derived classes may implement this
*/
// virtual
bool
j_class_plus::new_Thing( j_args_newfunc * args )
{
	JS_ReportError( args->cx, "This class doesnt support new\n");
	return false;
}

/*
	// virtual
	bool new_Thing ( j_args_newfunc * args )
	{
		JS_ReportError( args->cx, "This class doesnt support new\n");
		return false;
	}
*/

/*!
*/
// virtual
bool j_class_plus::j_get_property_by_id( j_args_prop_get * args )
{
	if( allow_other_attributes )
		return true;
	buffer1 tn;
	WARN("ATTR %s", (STR0) args->prop_name(tn) );
	args->ReportError_v( "get X.ID = No other attributes allowed class=%s\n",
		args->inst->type_name(tn) );
	return false;
}

/*!
	ATTR was not by 'id', so this particular class needs to fulfil it,
	or let JS search further for a soft attribute.
*/
// virtual
bool j_class_plus::j_get_property_by_str( j_args_prop_get * args )
{
	buffer1 pn;
	INFO("> args.prop_name = %s", (STR0) args->prop_name(pn) );
	if( allow_other_attributes )
		return true;
	WARN( "get X.%s = No other attributes allowed str\n", (STR0) args->prop_name(pn) );
	args->ReportError_v( "get X.%s = No other attributes allowed str\n", (STR0) args->prop_name(pn) );
	return false;
}

/*!
*/
// virtual
bool j_class_plus::j_set_property_by_id( j_args_prop_set * args )
{
	if( allow_other_attributes )
		return true;
	buffer1 buf;
	INFO("args.prop_name = %s",
			(STR0) args->prop_name(buf) );
	buffer1 tn;
	args->ReportError_v( "set X.ID No other attributes allowed class = %s\n",
		args->inst->type_name(tn) );
	return false;
}

/*!
*/
// virtual
bool j_class_plus::j_set_property_by_str( j_args_prop_set * args )
{
	if( allow_other_attributes )
		return true;
	buffer1 buf;
	JS_ReportError( args->cx, "set X.%s No other attributes allowed\n", (STR0) args->prop_name(buf) );
	return false;
}

/*!
*/
// virtual
bool j_class_plus::j_finalise( j_args_delfunc * args )
{
	/*
		The static function does all the work,
		the js_self is detatched and ref_decr()
	*/
	return true;
}

/*!
*/
#define static_props NULL
#define static_methods NULL
#define methods NULL
void j_class_plus::Init_Class()
{
	JSClass &my_class = * this;
	// j_class_base sets defaults in its constructor
#warning TODO CAST JSProprtyOp
	my_class.getProperty = (JSPropertyOp) s_getProperty;
	my_class.setProperty = (JSStrictPropertyOp) s_setProperty;
	my_class.finalize    = s_finalise;

	prototype = JS_InitClass(
		js1->cx,
		js1->globalObj, // clasp->name is obj.func
		parent_prototype, // JS will create a new prototype
		& my_class,

		/* native constructor function and min arg count */
		s_new_Thing, 0,

			    /* prototype object properties and methods -- these
			       will be "inherited" by all instances through
			       delegation up the instance's prototype link. */
		NULL,		// JSPropertySpecs[] ps;
		methods,	// JSFunctionSpecs[] fs; // NULL !

			    /* class constructor properties and methods */
		static_props,	// NULL!
		static_methods	// NULL!
	);
	// if(!prototype) fail(...);

	/*
		JS_InitClass() creates the globalObj.ctorfunc(args)
		which will call s_new_Thing() with no object
		that function must call JS_NewObject()
	*/

	// RULE obj.class.spare == j_class_plus
	// RULE obj.proto.priv == j_class_plus
	// NOT RULE-2 // JS_SetPrivate( prototype, this );

if(0) {
	// getConstructor uses getProperty !
	JSObject  * constructor = JS_GetConstructor( js1->cx, prototype );
	JS_SetPrivate( constructor, this );
}

 /* TEST
	int ncol = 5;
	const char * col_names[5] = {
		"ZER0", "one", "Two", "Three", "Four"
	};
	for(int i=0; i<ncol; i++ )
	{
		Def_Prop( js1, col_names[i], i, s_getProperty, s_setProperty );
	}

	if(0) {
		e_print("JS_InitClass this C++ class %p\n", this);
		e_print("JS_InitClass globalObject %p\n",  js1->globalObj );
		e_print("JS_InitClass globalproto %p\n",  
			JS_GetPrototype( js1->cx, js1->globalObj )
		);
		e_print("JS_InitClass parent_prototype %p\n",  parent_prototype );
		e_print("JS_InitClass prototype %p\n", prototype);
	}
 */
}


}; // NAMESPACE JSSM
