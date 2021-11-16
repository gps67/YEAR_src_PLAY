// generalEntity.c
// this file is very Python specific
// rewrite using new types - or generalise the plastic Dictionary type

#define XX if(0)
/*
	These ONLY happen during DTD startup
	giving general sdata
	and sgml doctypes
*/

AppExternalId SP_App::mk_externalId(const ExternalId & eid )
{
	const char * syspub = "unset";
	AppString obj1 = 0;
	if( eid.haveSystemId ) {
		syspub = "system";
		obj1 = app.mk_string( eid.systemId );
	} else if( eid.havePublicId ) {
		syspub = "public";
		obj1 = app.mk_string( eid.publicId );
	} else if( eid.haveGeneratedSystemId ) {
		syspub = "generated";
		obj1 = app.mk_string( eid.generatedSystemId );
	} else {
		syspub = "missing";
		obj1 = app.mk_str( "" );
	}
	refhold x2( obj1 );
	return app.mk_ExternalId( syspub, obj1 );
}

AppNotation SP_App::mk_notation(const Notation &note )
{
//	AppString obj0 = app.mk_str( "note.name" );
//	AppString obj1 = app.mk_str( "note.externalId" );
	AppString obj0 = 0;
	AppExternalId obj1 = 0;
	if(! note.name.len )
	{
		raise_error("mk_notation with NULL name");
		// crashes in mk_externalId
		obj0 = mk_str( "mk_notation(not_crashing!)" );
if(0) {
		obj1 = Py_None;
		Py_INCREF( obj1 );
} else {
		const ExternalId & eid = note.externalId;
		const char * syspub = "unset";

		if( eid.haveSystemId ) {
			syspub = "system";
// a bug in SP ??
// triggered by a named entity = subdoc
// avaoidable - if name len is zero - the systemId is not valid
// it IS reported elsewhere,
// but SP has haveSystemId == true ...
// avoid by caller checking ?

			gdb_break_point();
			obj1 = app.mk_string( eid.systemId );
		} else if( eid.havePublicId ) {
			syspub = "public";
	//		obj1 = app.mk_string( eid.publicId );
		} else if( eid.haveGeneratedSystemId ) {
			syspub = "generated";
	//		obj1 = app.mk_string( eid.generatedSystemId );
		} else {
			syspub = "missing";
	//		obj1 = app.mk_str( "" );
		}
		obj1 = app.mk_str( syspub );

}
	} else {
		obj0 = app.mk_string( note.name );
		obj1 = mk_externalId( note.externalId );
	}
	AppNotation obj2 = PyTuple_New( 2 );
	PyTuple_SetItem( obj2, 0, obj0 ); // steals a REF count
	PyTuple_SetItem( obj2, 1, obj1 );
	return obj2;
}

AppEntity SP_App::mk_entity(const Entity &ent)
{
/*
  struct Entity {
    CharString name;
    enum DataType { sgml, cdata, sdata, ndata, subdoc, pi };
    enum DeclType { general, parameter, doctype, linktype };
    DataType dataType;
    DeclType declType;
    bool isInternal;
    // Following valid if isInternal is true
    CharString text;
    // Following valid if isInternal is false
    ExternalId externalId;
    size_t nAttributes;
    const Attribute *attributes;
    Notation notation;
  };
*/
	PyObject * dict = PyDict_New();
	if(!dict) { vret(0); return 0; }
	AppString obj = app.mk_string( ent.name );
	PyDict_SetItemString(dict, (char *)"name", obj );
	Py_DECREF( obj );
	const char * s = "unset";
	switch( ent.dataType ) {
	/* */  case Entity::sgml:   s = "sgml";
	break; case Entity::cdata:  s = "cdata";
	break; case Entity::sdata:  s = "sdata";
	break; case Entity::ndata:  s = "nadata";
	break; case Entity::subdoc: s = "subdoc";
	break; case Entity::pi:     s = "pi";
	}
	obj = app.mk_str( s ); s = "unset";
	PyDict_SetItemString(dict, (char *)"dataType", obj );
	Py_DECREF( obj );
	switch( ent.declType ) {
	/* */  case Entity::general:   s = "general";
	break; case Entity::parameter: s = "parameter";
	break; case Entity::doctype:   s = "doctype";
	break; case Entity::linktype:  s = "linktype";
	}
	obj = app.mk_str( s ); s = "unset";
	PyDict_SetItemString(dict, (char *)"declType", obj );
	Py_DECREF( obj );
	if( ent.isInternal ) {
		obj = app.mk_string( ent.text );
		PyDict_SetItemString(dict, (char *)"text", obj );
		Py_DECREF( obj );
	} else {
		obj = mk_externalId( ent.externalId );
		PyDict_SetItemString(dict, (char *)"externalId", obj );
		Py_DECREF( obj );

		if (ent.notation.name.len) {
			obj = mk_notation( ent.notation );
			PyDict_SetItemString(dict, (char *)"notation", obj );
			Py_DECREF( obj );
		} else {
			PyDict_SetItemString(dict, (char *)"notation", Py_None );
		}

		obj = PyInt_FromLong( (long) ent.nAttributes );
		PyDict_SetItemString(dict, (char *)"nAttributes", obj );
		Py_DECREF( obj );

		// LURK:- List of attributes to be added somewhere
		// LURK:- mk_notation crashed on included subdoc
		// with null name and infinite eID string (unset)
	}

	return dict;
}

void SP_App::generalEntity(const GeneralEntityEvent &event)
{
	data_not();
//	keep_pos( event.pos ); // no such field
	keep_pos(0);

	AppEntity obj = mk_entity( event.entity );
	const char * fn = "generalEntity";
	if( event.entity.declType == Entity::general ) // they all are ?
	{
		// call different fn, delete known fields, script could do
		if( event.entity.dataType == Entity::sdata ) {
			fn = "generalEntity_sdata";
			PyDict_DelItemString( obj, (char*)"declType" );
			PyDict_DelItemString( obj, (char*)"dataType" );
		} else if( event.entity.dataType == Entity::sgml ) {
			fn = "generalEntity_sgml";
			PyDict_DelItemString( obj, (char*)"declType" );
			PyDict_DelItemString( obj, (char*)"dataType" );
		}
	}

	vret( PyObject_CallMethod(
		app.top->EH,
//		app.obj,
		(char *) fn,
		(char *) "(O)",
		obj
	));

}

