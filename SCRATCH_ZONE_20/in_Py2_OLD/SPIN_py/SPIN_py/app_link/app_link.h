#ifndef APP_LINK_H
#define APP_LINK_H

#include "SP_App.h"	// Char and CharString
#include "Python.h"	
#include "text_collector_utf.h"
#include "elem_info.h"
#include "EventGenerator.h"
#include "spin_top.h"

#include "dtd_tables.h"

/*
	The python fieldname
*/
#define FIELDNAME_elem_info "elem_info"

/*
	TO compare a stored elem_info->name (top of stack)
	with the incoming ElementEnd from SP.
*/
bool match_string(AppString obj, const SGMLApplication::CharString & s);


/*!
	app_link calls scripted python functions. It also coverts some values.
	It is (a part of) the link from C++ to Python.

	So for now think of "app_link" as "python_related_utilities"
*/
class app_link {
 public:

	~app_link();
	app_link(SPIN_top * _top);

	//! uplink
	SPIN_top * top;
	AppObject EH() { return top->EH;}


	//! used by mk_str and others
	text_collector_utf tmp_buff;

	//! build tree without callbacks
	bool auto_load;
	void set_auto_load( bool on ) { auto_load = on; };

	int vret( AppObject obj ); // checks for null
	AppObject report_error(void); // always returns NULL
	AppObject raise_error(const char * msg); // always returns NULL

	AppString mk_string_drop_lf(const SGMLApplication::CharString & s);
	AppString mk_string_py(const SGMLApplication::CharString & s);
	AppString mk_string_attr(const SGMLApplication::CharString & s);
	AppString mk_string_elem(const SGMLApplication::CharString & s);
	AppString mk_string(const SGMLApplication::CharString & s);
	AppString mk_str(const char * s);

// different kinds of attr are flattened - you can easily add code
	AppAttr mk_attr( AppString name, AppString text );
	PyObject * mk_ExternalId( const char * name, AppString text );
	PyObject * mk_tuple2( PyObject * obj1, PyObject * obj2 );

	AppLocation mk_location( 
		AppString entityName,
		AppString filename,
		int lineNumber,
		int columnNumber,
		int byteOffset,
		int entityOffset,
		int other
	);
	SPIN_elem_info * look_elem_info(
		AppString name
	);
	SPIN_elem_info * make_elem_info(
		AppString name,
		Enum_Element_Type et,
		const char * ctyp,
		bool elem_breaks_word = true
	);
	void call_ElementEnd( AppElem tree );
	void call_DocumentEnd( AppElem tree );
	void call_ElementStart( SPIN_elem_info*, AppAttrList ); //AppContentType 
	void call_Comments( AppStringList );
	void call_Data( text_collector_utf & buff ); // clears buff
	void call_Data( AppString );
	void call_DataSpace();
	void call_PrologEnd();
	void call_ExternalDataEntityRef(AppExternalDataEntityRef);
	void call_GeneralEntitySdata(AppEntity);
	void call_IgnoredChars(AppString);
	void call_MarkedSectionStart(
		AppString status,
		AppStringList params
	);
	void call_MarkedSectionEnd();
	void call_PI(AppString,AppString);
	void call_sdata(AppString name,AppString text);
	void call_DtdStart(
		AppString name,
		AppString syspub,
		AppString path,
		AppString decl,
		AppExternalId eid
	);
	void call_DtdEnd( AppString name );
	void call_SubdocEntityRef(AppEntity SubdocEntityRef);
	void call_set_location( AppLocation locn );

	void call_ErrorEvent(
		AppLocation locn,
		AppString etype,
		AppString msg 
	);
};

#endif
