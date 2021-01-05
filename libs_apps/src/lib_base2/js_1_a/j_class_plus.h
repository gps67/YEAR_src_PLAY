#ifndef j_class_plus_H
#define j_class_plus_H

#include "j_class_base.h"
#include "j_args.h"
#include "obj_ref_js.h"
#include "jsc_prop_list.h"

#if 1
/*
	JSNative functions get called with these args (called these)
*/
#define PROTO_JS_FUNC( name ) \
	bool name(				\
		JSContext *	_cx,		\
                uns           _argc,		\
                JS::Value *         _vp		\
	)
/*
	So store the CALL args as CX

	These no-longer exist:
	_instobj
	_argv
	_retvar 
*/
#define J_ARGS_FUNC( CX ) j_args_func CX( _cx, _argc, _vp );
#define J_ARGS_FUNC3( CX, ignore2, ignore3 ) J_ARGS_FUNC( CX ) 

#define J_ARGV0 JS_ARGV(_cx,_vp)[0]
#define J_ARGV1 JS_ARGV(_cx,_vp)[1]
#define J_ARGV2 JS_ARGV(_cx,_vp)[2]
#define J_ARGV3 JS_ARGV(_cx,_vp)[3]
#else
#define PROTO_JS_FUNC( name ) \
	bool name(				\
		JSContext *	_cx,		\
		JSObject  *	_instobj,	\
                uns           _argc,		\
                JS::Value *         _argv,		\
                JS::Value *         _retvar		\
	)

#define J_ARGS_FUNC( CX ) j_args_func CX( _cx, _instobj, _argc, _argv, _retvar );
#define J_ARGS_FUNC3( CX, ignore2, ignore3 ) J_ARGS_FUNC( CX ) 
#endif

namespace JSSM {

/*!
	some objects have a JSClass of their own but no individual instances.
	IE ONE dset class per dset inst, its all in the class
*/
class no_priv : public obj_ref_js
{
 public:
	no_priv() {}
	~no_priv() {}
};

/*!
	C++ class for a JS class, with virtual "get_property"

	There is a JSClass base class, but this adds a VTBL,
	so they may be at different base addresses (-4bytes).
	Casting one way is easy, the other is a trust issue.
	
	The s_ static functions, are called by JS
	s_setProperty( cx, JSObject *obj, jsid id, bool strict, JS::Value *vp ) ;
	s_getProperty( cx, JSObject *obj, jsid id, JS::Value *vp ) ;
	s_finalise( fop, JSObject *obj );
	s_new_Thing( cx, argc, *vp );

	The v
*/
class j_class_plus : public j_class_base
{
 public:
	//! probably shadows parent class's prototype
// base //	JSObject * parent_prototype;
	//! allow script to set normal JS attributes (with OTHER names)
	bool allow_other_attributes;

	jsc_prop_list prop_list;

	//! destructor - simple
	~j_class_plus() {}

	j_class_plus(
		const char * classname,
		JS1 * _js1,
		bool _allow_other_attributes = false
	);

static
	j_class_plus * get_class_plus( j_cx_obj & cx_obj )
	{
		// PRESUME its of _plus type
		return (j_class_plus *) get_class_base ( cx_obj );
	}

	bool def_prop_str( const char * name );
	bool def_prop( const char * name, int id );
	bool def_prop_alloc( const char * name, jsc_prop_list_item * & prop );
	bool def_prop_alloc( const char * name, int idx );
	bool def_prop_alloc( const char * name, obj_ref * obj );

	void Init_Class() ;

///// JS calls these which call the instance objects thing

static
	bool
	s_getProperty( JSContext *cx, JSObject *obj, jsid id, JS::Value *vp ) ;

/*
	1.8.0 -> 1.8.5 adds bool strict
	also JS::Value id; ==> jsid id
	"jspubtd.h" line 196
*/

static
	bool
	s_setProperty( JSContext *cx, JSObject *obj, jsid id, bool strict, JS::Value *vp ) ;

static
	void
	s_finalise( JSFreeOp *fop, JSObject *obj );

static
	bool
	s_new_Thing(
		JSContext *cx, 
		uns argc, 
		JS::Value *vp 
	) ;

/////////// sub-classes do this /////////////

 virtual
	bool new_Thing ( j_args_newfunc * args );
 virtual
	bool j_get_property_by_id( j_args_prop_get * args );
 virtual
	bool j_get_property_by_str( j_args_prop_get * args );
 virtual
	bool j_set_property_by_id( j_args_prop_set * args );
 virtual
	bool j_set_property_by_str( j_args_prop_set * args );
 virtual
	bool j_finalise( j_args_delfunc * args );

};
}; // namespace JSSM
#endif
