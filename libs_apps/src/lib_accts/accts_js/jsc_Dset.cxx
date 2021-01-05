#include "jsc_Dset.h"
#include "jsc_Table.h"
#include "buffer2.h"
#include "j_misc.h"

#include "rep_pdf_RUN_C.h"

#include "fork_progs.h" // view xdpf ...


///////////////////////////

///////////// jsc_Dset ///////////////////////
struct j_args_func_Dset : public j_args_func_Misc
{
	jsc_Dset * jsc_dset;
	data_set * dset;

	j_args_func_Dset(
		JSContext *	_cx,
                uns           _argc,
                jsval *         _vp,
		str0		_func_name = "Function",
		int		nargs = 0
	)
	: j_args_func_Misc( _cx, _argc, _vp, _func_name, nargs )
	, jsc_dset( NULL )
	, dset( NULL )
	{
		if(insane) return;
		jsc_dset = (jsc_Dset*) cls; // already NULL checked
		dset = (data_set*) inst; // already NULL checked
	}

};


///////////////////////////

// static
jsc_Dset * jsc_Dset::mk_jsc_Dset( str0 _classname, JS1 * js1, data_set * dset )
{
	if(!_classname)
	{
		// name of class is an obscure string
		static int class_inst_counter = 0;
		buffer2 class_name_1;
		class_name_1.print( "DSET_%d", class_inst_counter++);
		_classname = JS_strdup( js1->cx, str0(class_name_1) );
	}

	// create a new class for DSET
	jsc_Dset * jsc = new jsc_Dset( _classname, js1 ); // NEVER DELETED!
	// other ops here before InitClass
	jsc->Init_Class(); // jsc->prototype now exists

	jsc->Def_Function(
		"load_file",
		f_load_file,
		1		// number of args
	);

	jsc->Def_Function(
		"save_file",
		f_save_file,
		1		// number of args
	);

	jsc->Def_Function(
		"add_table",
		f_add_table,
		0		// number of args
	);

	jsc->Def_Function(
		"hide_table",
		f_hide_table,
		2		// number of args
	);

	jsc->Def_Function(
		"run_reports",
		f_run_reports,
		0		// number of args
	);

	jsc->Def_Function(
		"rep_pdf",
		f_rep_pdf,
		2		// number of args
	);

	jsc->reregister_tables( dset );
	/*
		dset is not actually held by the jsc,
		dset is held by the inst
	*/
	return jsc;
}

void jsc_Dset::reregister_tables( data_set * dset )
{
	// create each table as a property
	// there is no way to create new tables though !
	// shouldnt this code be in jsc_Dset ?
	int ntbl = dset->tbl_list->tbl_list.N();
	for( int i=0; i<ntbl; i++ )
	{
		table * tbl =  dset->tbl_list->tbl_list[ i ];
		str0 tbl_name = * tbl->name;
		def_prop_alloc( tbl_name, tbl );
	}
}

//  virtual
JSBool jsc_Dset::j_get_property_by_str( j_args_prop_get * args )
{
	data_set * dset = (data_set *) args->inst;
	if(!dset) return JS_FALSE;

	/*
		tables are supposed to be accessed by_id,
		but a new table might come here
	*/
	reregister_tables( dset );

	buffer1 pn;
	buffer1 cn;
	table * tbl =  dset->get_table( args->prop_name(pn) );

	if( !tbl )
	{
		return args->ReportError(
			"no table '%s' in dset class %s",
			(STR0) args->prop_name(pn),
			class_name()
		);
	}
	if(!tbl->get_js_self())
	{
		/* The table needs to be upgraded to an object,
		each table has a jsc and jsi of its own,
		the jsc lives forever and leaks */
		jsc_Table * jsc = jsc_Table::mk_jsc_Table( js1, tbl );
		jsc->mk_js_self( tbl ); // one class per inst !!!
	}
	args->set_ret( tbl->get_js_self() );
	return JS_TRUE;
}

//  virtual
JSBool jsc_Dset::j_get_property_by_id( j_args_prop_get * args )
{
	data_set * dset = (data_set *) args->inst;
	if(!dset) return  args->ReportError("NULL dset in j_get_property_by_id");
	if(!args->jsc_prop)
	{
		return args->ReportError(
			"NULL jsc_prop in class %s",
			class_name()
		);
	}

	/*
		if dsets table list has changed - rebuild that
		but no detection mechanism
		reregister_tables( dset );
	*/
	obj_ref * obj = args->jsc_prop->obj;
	table * tbl = (table *)obj;

	buffer1 pn;
	if( !tbl )
	{
		return args->ReportError(
			"no table '%s' in dset class %s",
			(STR0) args->prop_name(pn), // NOT SURE IF ITS SET by_id
			class_name()
		);
	}
	if(!tbl->get_js_self())
	{
		/* The table needs to be upgraded to an object,
		each table has a jsc and jsi of its own,
		the jsc lives forever and leaks */
		jsc_Table * jsc = jsc_Table::mk_jsc_Table( js1, tbl );
		jsc->mk_js_self( tbl ); // one JSC per INST
	}
	args->set_ret( tbl->get_js_self() );
	return JS_TRUE;
}

// virtual
//	JSBool j_get_property_by_str( j_args_prop_get * args );
// virtual
//	JSBool j_set_property_by_id( j_args_prop_get * args );
// virtual
//	JSBool j_set_property_by_str( j_args_prop_get * args );
// virtual
//	JSBool j_finalise( j_args_delfunc * args );

// static
PROTO_JS_FUNC( jsc_Dset::f_load_file )
{
	J_ARGS_FUNC_Dset( CX, "load_file", 1 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf;
	str0 filename = CX.get_single_string_arg(buf);

	if( !CX.dset ) return JS_FALSE; // already checked
	if( !CX.dset->load_file( filename ) )
	{
		return CX.ReportError_v("load_file %s failed", (STR0)filename );
	}
	CX.jsc_dset->reregister_tables(CX.dset);
	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Dset::f_save_file )
{
	J_ARGS_FUNC_Dset( CX, "save_file", 1 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf;
	str0 filename = CX.get_single_string_arg(buf);
	if( !CX.dset->file_save( filename ) ) // not compressed
	{
		return CX.ReportError_v("save_file %s failed", (STR0)filename );
	}
	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Dset::f_add_table )
{
	J_ARGS_FUNC_Dset( CX, "add_table", 1 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf1;
	str0 name = CX.as_string( J_ARGV0, buf1 );

	table * tbl = CX.dset->find_add( name );
	if( !tbl )
		return CX.ReportError_v("add_table %s failed", (STR0)name );

	/*
		return value is not vital, but might be used
	*/
	jsc_Table * jsc_table = jsc_Table::mk_jsc_Table( CX.get_js1(), tbl );
	if( !jsc_table )
	{
		// delete before throw, but keep for diagnostics !
		THROW_dgb_fail("NULL jsc_table");
		delete tbl;
	}
	CX.set_ret( jsc_table->mk_js_self( tbl ) );
	CX.jsc_dset->reregister_tables(CX.dset);
	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Dset::f_hide_table )
{
	J_ARGS_FUNC_Dset( CX, "hide_table", 2 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf1;
	str0 name = CX.as_string( J_ARGV0, buf1 );

	bool hide;
	if(!CX.ok_as_bool( J_ARGV1, hide ))
	{
		e_print("hide_table( table, bool ) - not bool\n" );
		return JS_FALSE;
	}

	obj_hold<str2> name2 = new str2( name );
	CX.dset->hide_table( name2, hide );
	return JS_TRUE;
}



// static
PROTO_JS_FUNC( jsc_Dset::f_run_reports )
{
	J_ARGS_FUNC_Dset( CX, "run_reports", 0 );
	if(CX.insane) return JS_FALSE;

	bool ok = true;

	ok = ok && run_report_1( CX.dset );
	ok = ok && run_report_2( CX.dset );
	ok = ok && run_report_3( CX.dset );
	ok = ok && run_report_4( CX.dset );

	if( ok )
		return JS_TRUE;
	else {
		return CX.ReportError_v("run_reports() _[1234] failed" );
	}
}

// static
PROTO_JS_FUNC( jsc_Dset::f_rep_pdf )
{
	J_ARGS_FUNC_Dset( CX, "rep_pdf", -2 );
	buffer2 buf1;
	buffer2 buf2;
	buffer2 buf3;
	buffer2 buf4;
	if(CX.insane) return JS_FALSE;
	str0 name = CX.as_string( J_ARGV0, buf1 );
	str0 filenamepdf = CX.as_string( J_ARGV1, buf2 );
	bool ok = true;

	/*
		all sort sof things should change
		The output filename
		The C++/names
		The ...
	*/

	if( name == "VatTrack" ) {
		ok = run_pdf_VatTrack( CX.dset, filenamepdf );
	} else if( name == "Cash_Cat" ) {
		ok = run_pdf_Cash_Cat( CX.dset, filenamepdf );
	} else if( name == "CH" ) {
		// different dset ;-)
		if( CX.argc != 4 ) {
			// changed to dset ... back again ?? LURK
			return CX.ReportError("usage: DSET.f_rep_pdf( 'filename.pdf', 'Y2005_06', 'Y2004_05'");
		}
		j_cx_obj_inst CX_dset_curr( CX.cx, JSVAL_TO_OBJECT( J_ARGV2 ));
		j_cx_obj_inst CX_dset_prev( CX.cx, JSVAL_TO_OBJECT( J_ARGV3 ));
		str0 year_curr = CX.as_string( J_ARGV2, buf3 );
		str0 year_prev = CX.as_string( J_ARGV3, buf4 );
		ok = run_pdf_CH(
			CX.dset,
			filenamepdf,
			(data_set *) CX_dset_curr.inst,
			(data_set *) CX_dset_prev.inst
		);
	} else {
		ok = false;
	}

	if(!ok) {
		return CX.ReportError_v("rep_pdf(%s,%s)", (STR0)name, (STR0)filenamepdf );
	}
	return JS_TRUE;
}

