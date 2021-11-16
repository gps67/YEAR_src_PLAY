#include "SP_App.h"
#include "text_collector_utf.h"
#include "app_link.h"
#include "Python.h"
#include "str_obj.h"                                                                          
#include "app_link.h"
#include "P_ref_base.h"

#include "dtd_tables.h"

/*
	// vret( some-python-call) checks for NULL and does DECREF, report error
*/

/*
	auto_load -- rename to "auto_build_tree without Python Callbacks"
	It simply switches off calls to ElemenEnd Data ... BUT it doesnt
	override vectors held in elem_info
*/

/*!
	Call scripts EH.ElementEnd( info, name, tree )  - or as configured
*/
void app_link::call_ElementEnd( AppElem tree )
{
// 3%	return;
	if(!tree) { vret(0); return; }
	SPIN_elem_obj * elem = (SPIN_elem_obj *) tree; // LURK
	SPIN_elem_info * info = elem->eleminfo.self();
	// info cannot be NULL ??

/*
	If a function has been assigned to the elem_info, use it

	(TODO: Provide a C function that does the tree add per-elem)
*/

	if( info->on_element_end ) {
		PyObject * arglist = Py_BuildValue("(OOO)",
			(PyObject *) elem->eleminfo,
			(PyObject *) elem->eleminfo.self()->name,
			tree
		);
		refhold x1( arglist );
		vret ( PyEval_CallObject(
			info->on_element_end,
			arglist )
		);
		return; 
	}

/*
	if auto_load (generally), and ON_Element_End = not requested
	simply build the tree.

		info.ON_Element_End = 1 # causes standard EH callback
*/

	if( auto_load && ! info->get_on_flag( ON_Element_End ) )
	{
		top->app->stack.add_item( tree );
		return;
	}

/*
	Otherwise invoke the scripts EH.ElementEnd( info, name, tree )
	It has to do the tree add (nb C still does tree push/pop,
	any item added is in the data list)
*/

	vret( PyObject_CallMethod(
		EH(),
		(char *) "ElementEnd",
		(char *) "(OOO)",
		(PyObject *) elem->eleminfo,
		(PyObject *) elem->eleminfo.self()->name,
		tree
	));

}

/*!
	EH.DocumentEnd( tree )
*/
void app_link::call_DocumentEnd( AppElem tree )
{
	if(!tree) { vret(0); return; }
	vret( PyObject_CallMethod(
		EH(),
		(char *) "DocumentEnd",
		(char *) "(O)",
		tree
	));
}

//! utility func
PyObject * app_link::mk_tuple2( PyObject * A, PyObject * B )
{
	PyObject * tpl = PyTuple_New( 2 );
	Py_INCREF( A ); // eaten by SetItem
	Py_INCREF( B ); // eaten by SetItem
        PyTuple_SetItem( tpl, 0, A );
        PyTuple_SetItem( tpl, 1, B );
        return tpl;
}

/*!
	construct an SGML-Attribute = (name,text)
*/
AppAttr app_link::mk_attr( AppString name, AppString text )
{
	return mk_tuple2( name, text );
}

/*!
	look for elem_info by name in the standard dictionary
	else return silent NULL and let caller create or report
*/
SPIN_elem_info * app_link::look_elem_info( AppString name )
{
        SPIN_elem_info * info = top->dtd_tables.self() -> look_elem_info( name );
        if(info) 
                // PyDict_GetItem doesn't do INCREF!
                Py_INCREF( (PyObject *)info );
	else
		{
		// look is milder than find or expect, so no error
		// vret(0);
		// report_error();
		// PyErr_Clear();
		// but do return NULL
	}
	return info;

}

/*!
	Initialise an elem_info, and involve the scripts. This is
	not used when script invokes new eleminfo

	called by  SP_App::find_elem_info
*/
SPIN_elem_info * app_link::make_elem_info(
	AppString name,
	Enum_Element_Type et,
	const char * ctyp,	// ascii enum token
	bool element_breaks_word
)
{
	P_Int etype( et );
	AppString ctype = mk_str( ctyp );

	SPIN_elem_info * 
	info = new_SPIN_elem_info(
		top -> dtd_tables,
		name,
		etype, // etype.py_ptr(),
		ctype,
		element_breaks_word
	); // it keeps in dry and returns a incremented ref
	if(!info)
	{
		raise_error("new_SPIN_elem_info failed");
		vret(0);
		return 0;
	}
        vret( PyObject_CallMethod(
                EH(),
                (char *) "ElementInfoInit",
                (char *) "(OO)",
                name,  // "UL"
                (PyObject *)info
        ));
	return info;
}

/*!
	Tell app about sub-zone starting
*/
void app_link::call_ElementStart( SPIN_elem_info * info, AppAttrList atts )
{
 /*
	If script set a callback, call it
 */
	if( info->on_element_start ) {
		PyObject * arglist = Py_BuildValue("(OOOO)",
			(PyObject *)info,
			(PyObject *) info->name,  // "HTMLURL"
			atts,  	// as below
			(PyObject *) info->ctype  // "empty"
		);
		refhold x1( arglist );
		vret ( PyEval_CallObject(
			info->on_element_start,
			arglist )
		);
		return; 
	}
 /*
	if auto_load generally, (remove this ON_element_rubbish soon, 
	set a specific info.on_element_start = EH.ElementStart)
 */

	if( auto_load && !info->get_on_flag( ON_Element_Start ) ) {
		// no action since elem is ALWAYS added to stack
		return;
	}

 /*
	call the general EH handler
 */
	vret( PyObject_CallMethod(
		EH(),
		(char *) "ElementStart",
		(char *) "(OOOO)",
		(PyObject *)info,
		(PyObject *) info->name,  // "HTMLURL"
		atts,  	// list [ (name,info,data) (name,info,data) ...
		(PyObject *) info->ctype  // "empty"
        ));
}

/*!
	The prolog is the per-document extension to the DTD
*/
void app_link::call_PrologEnd()
{
	vret( PyObject_CallMethod(
		EH(),
		(char *) "PrologEnd",
		(char *) "()"
	));
}

/*!
*/
void app_link::call_ExternalDataEntityRef(AppEntity obj )
{
	vret( PyObject_CallMethod(
		EH(),
		(char *) "externalDataEntityRef",
		(char *) "(O)",
		obj
	));
}

AppExternalId app_link::mk_ExternalId( const char * syspub, AppString addr )
{
        AppString obj0 = mk_str( syspub );
	refhold x1( obj0 );
        return mk_tuple2( obj0, addr );
}

/*!
	how a location (line number) is structured
*/
AppLocation app_link::mk_location( 
	AppString entityName,
	AppString filename,
	int _lineNumber,
	int _columnNumber,
	int _byteOffset,
	int _entityOffset,
	int _other
)
{
        AppInt lineNumber   = PyInt_FromLong( _lineNumber );
        AppInt columnNumber = PyInt_FromLong( _columnNumber );
        AppInt byteOffset   = PyInt_FromLong( _byteOffset );
        AppInt entityOffset = PyInt_FromLong( _entityOffset );
        AppInt other        = PyInt_FromLong( (long) _other ); // void *
        refhold x2( lineNumber );
        refhold x3( columnNumber );
        refhold x4( byteOffset );
        refhold x5( entityOffset );
        refhold x6( other );

        PyObject * tpl = Py_BuildValue(
                (char *) "(OOOOOOO)",
                entityName,
                filename,
                lineNumber,
                columnNumber,
                byteOffset,
                entityOffset,
                other
        );
        if(!tpl) {
                vret(0);
        }
        return tpl;
}

/*!
	getting and setting the location (line number) is slow,
	so it is done at specific times, and reported to the
	scripts at specific times.
*/
void app_link::call_set_location( AppLocation locn )
{
        vret( PyObject_CallMethod(
                EH(),
                (char *) "set_location",
                (char *) "(O)",
                locn
        ));
}

/*!
	Tell the scripts about a list of comments 
*/
void app_link::call_Comments( AppStringList ary )
{
	vret( PyObject_CallMethod(
		EH(),
		(char *) "Comments",
		(char *) "(O)",
		ary
	));
}

//! overload
void app_link::call_Data( text_collector_utf & buff ) // clears buff
{
	// tracer // 
	// buff.data_string("!!");
        if(!buff.buff.nbytes_used) { return; }
	AppString str = buff.mk_string();
	call_Data( str );
	Py_DECREF(str);
}
/*!
	EH.Data( text ) # but nothing if auto-load - wait for on_element_end
*/
void app_link::call_Data( AppString str )
{
// 3% return;

	if( auto_load ) {
		// self.top.tree_add( tree )
		top->app->stack.add_item( str );
		return;
	}

	vret( PyObject_CallMethod(
		EH(),
		(char *) "Data",
		(char *) "(O)",
		str
	));
}

/*!
	less used - a space in the text
*/
void app_link::call_DataSpace()
{
	vret( PyObject_CallMethod(
		EH(),
		(char *) "DataSpace",
		(char *) ""
	));
}

/*!
	Not implemented yet
*/
void app_link::call_GeneralEntitySdata(AppEntity)
{
	raise_error("call_GeneralEntitySdata()");
}

/*!
	is this marked section where mark is IGNORE ??
*/
void app_link::call_IgnoredChars(AppString str)
{
        vret( PyObject_CallMethod(
                EH(),
                (char *) "ignoredChars",
                (char *)
                "(O)",
                str
        ));
}

/*!
	EH.MarkedSectionStart( status, params )
*/
void app_link::call_MarkedSectionStart(
	AppString status,
	AppStringList params
)
{
	vret( PyObject_CallMethod(
		EH(),
		(char *) "MarkedSectionStart",
		(char *) "(OO)",
		status,
		params
	));
}
/*
	data inside a marked section is passed though (as Data() ??)
	so if you wanted it processed, you might have to
	clear the data at start (save it) then find
	the data at end. 

	Or put another way, maybe Marked section should be an element
	on the stack, so that it is separately gathered.

	-- create a new boolean - marked section data - for auto_load ??

	Sometime there will be a pseudo-elem-info for marked sections
	and PI's and comments (etc) to allow auto_load to return
	as much of the tree, as close to the source as SP-generic-API allows
	(ie you wont get DTD/Prolog information)
*/
void app_link::call_MarkedSectionEnd()
{
	vret( PyObject_CallMethod(
		EH(),
		(char *) "MarkedSectionEnd",
		(char *) "()"
	));
}

/*!
	EH.PI( data, entityname )
*/
void app_link::call_PI(AppString data,AppString entityName )
{
	vret( PyObject_CallMethod(
		EH(),
		(char *)"PI",
		(char *)"(OO)",
		data,
		entityName
	));
}

/*!
	EH.sdata( name, text ) - maybe optimise common ones?
*/
void app_link::call_sdata(AppString name,AppString text)
{
	// .1% for non sgml sites: return;
        vret( PyObject_CallMethod(
                EH(),
                (char *) "sdata",
                (char *) "(OO)",
                name,
                text
        ));
}

/*!
	You need the DtdStart to rebuild the DOCTYPE line
*/
void app_link::call_DtdStart(
	AppString name,
	AppString syspub,
	AppString path,
	AppString decl,
	AppExternalId eid
)
{
  vret (PyObject_CallMethod (
	EH(),
	(char *) "DtdStart",
	(char *) "(OOOOO)",
	name,
	syspub,
	path,
	decl,
	eid
  ));
}

//! Interesting ;-)
void app_link::call_DtdEnd( AppString name )
{
	vret (PyObject_CallMethod (
		EH(),
		(char *) "DtdEnd",
		(char *) "(O)",
		name
	));
}

/*!
	The entity is auto handled, but you get to know about it.
*/
void app_link::call_SubdocEntityRef( AppEntity entity )
{
	vret( PyObject_CallMethod(
		EH(),
		(char *) "subdocEntityRef",
		(char *) "(O)",
		entity
	));
}

/*!
	EH.ErrorEvent(locn, errortype, message )
*/
void app_link::call_ErrorEvent(
	AppLocation locn,
	AppString etype,
	AppString message 
)
{
	vret( PyObject_CallMethod(
		EH(),
		(char *) "ErrorEvent",
		(char *) "(OOO)",
		locn,
		etype,
		message
	));
}

