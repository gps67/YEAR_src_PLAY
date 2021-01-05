#include "jsc_Table.h"

#include "dset.h"
#include "table.h"
#include "jsc_Row.h"
#include "jsc_Table.h"
#include "j_misc.h"
#include "buffer2.h"
#include "tbl_col_spec_plus.h"

// #include "rep_pdf_0.h"
#include "rep_pdf_RUN_C.h"

///////////// jsc_Table ///////////////////////
struct j_args_func_Table : public j_args_func_Misc
{
	jsc_Table * jsc_table;
	table * tbl;

	j_args_func_Table(
		JSContext *	_cx,
                uns           _argc,
                jsval *         _vp,
		str0		_func_name = "Function",
		int		nargs = 0
	)
	: j_args_func_Misc( _cx, _argc, _vp, _func_name, nargs )
	, jsc_table( NULL )
	, tbl( NULL )
	{
		if(insane) return;
		jsc_table = (jsc_Table*) cls; // already NULL checked
		tbl = (table*) inst; // already NULL checked
	}

};


///////////// jsc_Table //////////

// STATIC
jsc_Table * jsc_Table::mk_jsc_Table( JS1 * js1, table * tbl )
{
	buffer2 name;
	name.print( "TABLE_%s", (STR0)*tbl->name );
	jsc_Table * jsc = new jsc_Table( JS_strdup( js1->cx, (str0)name), js1, tbl );
	jsc->Init_Class();

	jsc->Def_Function(
		"add_col",
		f_add_col,
		1		// number of args // nam
	);

	jsc->Def_Function(
		"add_type_col",
		f_add_type_col,
		2		// number of args // name type
	);

	jsc->Def_Function(
		"add_type_col_after",
		f_add_type_col_after,
		3		// number of args // name type
	);

	jsc->Def_Function(
		"add_row",
		f_add_row,
		1		// number of args // key
	);

	jsc->Def_Function(
		"hide_col",
		f_hide_col,
		1		// number of args // tablename
	);

	jsc->Def_Function(
		"pdf_doc",
		f_pdf_doc,
		1		// number of args // filename
	);

	jsc->Def_Function(
		"has_row",
		f_has_row,
		1		// number of args // filename
	);

	jsc->Def_Function(
		"del_row",
		f_del_row,
		1		// number of args // filename
	);

	return jsc;
}

jsc_Table::jsc_Table( str0 classname, JS1 * js1, table * tbl )
: j_class_plus( classname, js1 )
{
	/*
		using classname requires mk_jsc_Table()
	*/
	row_class = jsc_Row::mk_jsc_Row( js1, tbl );
	def_prop_alloc( "rowlist", FT_rowlist );
	def_prop_alloc( "name", FT_name );
	def_prop_alloc( "dset", FT_dset );

}

/*!
	table.rowlist - comes here
*/
JSBool jsc_Table::j_get_property_by_id( j_args_prop_get * args )
{
	table * tbl = (table *) args->inst;
	if(!tbl) return JS_FALSE;
	if( !args->jsc_prop )
	{
		gdb_invoke( true );
		return JS_TRUE;
	}
	switch( args->jsc_prop->idx ) {
	 case FT_rowlist:
	 {
		int n = tbl->rows_list.N();
		jsval vals[n];
		for(int i=0; i<n; i++ )
		{
			row_inst * r = tbl->rows_list[i];
			JSObject * r_js = r->get_js_self();
			if(!r_js)
			{
				r_js = row_class->mk_js_self( r );
			}
			vals[i] = OBJECT_TO_JSVAL( r_js );
		}
		args->set_ret( JS_NewArrayObject(args->cx, n, vals ) );
	 }
	 break;
	 case FT_name:
		args->set_ret( args->mk_str( tbl->name->str ));
	 break;
	 case FT_dset:
		args->set_ret( tbl->dset->get_js_self() );
	 break;
	 default:
		// without set_ret its OTHER(str) or undefined
		return JS_TRUE;
	}
	return JS_TRUE;
}

/*!
	table.rowkey - comes here
*/
JSBool jsc_Table::j_get_property_by_str( j_args_prop_get * args )
{
	table * tbl = (table *) args->inst;
	if(!tbl) return JS_FALSE;
	row_inst * r = NULL;
	buffer1 pn;

	bool auto_create_row = true;
	if(  auto_create_row )
	{
		r = tbl->find_row_add_fake( args->prop_name(pn) );
	} else {
		r = tbl->find_row( args->prop_name(pn) );
	}

	if( ! r )
	{
		return args->ReportError_v( "Table[%s] has no row[%s]\n",
			(STR0) tbl->name->str,
			(STR0) args->prop_name(pn)
		);
	}

	args->set_ret( row_class, r );

///	JSObject * r_js = r->get_js_self();
///	if(!r_js)
///	{
///		r_js = row_class->mk_js_self( r );
///	}
///	args->set_ret( r_js );

	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Table::f_add_col )
{
	J_ARGS_FUNC_Table( CX, "add_col", 1 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf;
	str0 name = CX.as_string( J_ARGV0, buf );

	CX.tbl->add_col( name );
	CX.jsc_table->row_class->reregister_columns(CX.tbl);

	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Table::f_add_type_col )
{
	J_ARGS_FUNC_Table( CX, "add_type_col", 2 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf1;
	buffer2 buf2;
	str0 type = CX.as_string( J_ARGV0, buf1 );
	str0 name = CX.as_string( J_ARGV1, buf2 );

	CX.tbl->add_type_col( type, name );
	CX.jsc_table->row_class->reregister_columns(CX.tbl);

	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Table::f_add_type_col_after )
{
	J_ARGS_FUNC_Table( CX, "add_type_col_after", 3 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf1;
	buffer2 buf2;
	buffer2 buf3;
	str0 type = CX.as_string( J_ARGV0, buf1 );
	str0 name = CX.as_string( J_ARGV1, buf2 );
	str0 after = CX.as_string( J_ARGV2, buf3 );

	CX.tbl->add_type_col_after( type, name, after );
	CX.jsc_table->row_class->reregister_columns(CX.tbl);

	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Table::f_add_row )
{
	J_ARGS_FUNC_Table( CX, "add_row", 1 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf;
	str0 name = CX.as_string( J_ARGV0, buf );

	row_inst * row = CX.tbl->find_row_add_fake( name );
	if( !row )
		return CX.ReportError_v("find_row_add_fake %s failed", (STR0)name );

	JSObject * r_js = row->get_js_self();
	if(!r_js)
	{
		r_js = CX.jsc_table->row_class->mk_js_self( row );
	}
	CX.set_ret( r_js );
	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Table::f_hide_col )
{
	J_ARGS_FUNC_Table( CX, "hide_col", 1 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf;
	str0 name = CX.as_string( J_ARGV0, buf );

	col_spec * spec = CX.tbl->get_col_spec( name );
	if( spec )
		spec -> is_hidden = true;
	return JS_TRUE;
}

PROTO_JS_FUNC( jsc_Table::f_del_row )
{
	J_ARGS_FUNC_Table( CX, "del_row", 1 );
	if(CX.insane) return JS_FALSE;

	// row can be specified as inst or key ? KEY ONLY
	buffer2 buf;
	str0 name = CX.as_string( J_ARGV0, buf );

	CX.tbl->del_row( name );

	return JS_TRUE;
}

PROTO_JS_FUNC( jsc_Table::f_has_row )
{
	J_ARGS_FUNC_Table( CX, "has_row", 1 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf;
	str0 name = CX.as_string( J_ARGV0, buf );

	bool found = CX.tbl->find_row( name );
	CX.set_ret_bool( found );

	return JS_TRUE;
}


PROTO_JS_FUNC( jsc_Table::f_pdf_doc )
{
	J_ARGS_FUNC_Table( CX, "pdf_doc", 1 );
	if(CX.insane) return JS_FALSE;
	buffer2 buf;
	str0 name = CX.as_string( J_ARGV0, buf );

	// gdb_invoke( false ); // text
	// gdb_invoke( true ); // gui

	if(!run_pdf_Table( CX.tbl, name ))
	{
		return CX.ReportError_v("pdf_doc(%s) failed", (STR0)name );
	}

	return JS_TRUE;
}

