
#include "dset.h"
#include "table.h"
#include "jsc_Row.h"
#include "jsc_Table.h"
#include "j_misc.h"
#include "buffer2.h"
#include "tbl_col_spec_plus.h"
#include "j_args.h"

using namespace ACCTS;

///////////// - ///////////////////////
struct j_args_func_Row : public j_args_func_Misc
{
	jsc_Row * jsc_row;
	row_inst * row;

	j_args_func_Row(
		JSContext *	_cx,
                uns           _argc,
                jsval *         _vp,
		str0		_func_name = "Function",
		int		nargs = 0
	)
	: j_args_func_Misc( _cx, _argc, _vp, _func_name, nargs )
	, jsc_row( NULL )
	, row( NULL )
	{
		if(insane) return;
		jsc_row = (jsc_Row*) cls; // already NULL checked
		row = (row_inst*) inst; // already NULL checked
	}

};

///////////// jsc_Row //////////
// static
jsc_Row * jsc_Row::mk_jsc_Row( JS1 * js1, table * tbl )
{
	/*
		Two DSETS might name similar table names with different structures

		This jsc is the type of every row of this table
	*/
	buffer2 name;
	static int counter = 0;
	// name.print( "TABLE_%s_%d", (STR0)*tbl->name, counter++ );
	name.print( "ROWofTABLE_%d_%s", counter++, (STR0)*tbl->name );
	// e_print( "counter now %d\n", counter );
	jsc_Row * jsc = new jsc_Row( JS_strdup( js1->cx, (str0) name), js1, tbl );
	jsc->Init_Class();
	return jsc;
}

jsc_Row::jsc_Row( str0 classname, JS1 * js1, table * _tbl )
: j_class_plus( classname, js1 )
, tbl( _tbl )
{
	reregister_columns( _tbl );
}

void jsc_Row::reregister_columns( table * tbl )
{
	STR0 nm = (STR0) tbl-> name->str;
	gdb_invoke(1);
	int n = tbl->col_specs.N();
	for(int i=0; i<n; i++ )
	{
		// should modify def_prop_alloc to not reregister name
		// just update idx 
		def_prop_alloc( (STR0) tbl->col_specs[i]->name->str, i );
		INFO( "'%s.%s'", nm, (STR0) tbl->col_specs[i]->name->str, i );
	}
}

/*!
	This misses the point somehow,
	a cell might be a row link to another table ... or a ...
*/
// virtual
JSBool jsc_Row::j_get_property_by_id( j_args_prop_get * args )
{
	// get the row 
	row_inst * row = (row_inst *) args->inst;
	// get the spec of the property (col of row) - check idx range
	if( args->jsc_prop->idx >= tbl->col_specs.N() )
	{
		return args->ReportError( "idx>N" );
	}
	col_spec * spec =  tbl->col_specs[ args->jsc_prop->idx ];
	if(!spec)
	{
		return args->ReportError_v( "spec is NULL" );
	}

	// T_field.h
	switch( spec->get_field_type() )
	{

         case T_link_grp:
         case T_link_row:
	 {
	// e_print(" link_grp/row \n");

		buffer1 t3;
		row_inst * r2 = spec->get_linked_row( row );
		if(!r2) {
			/*
				a row without a linked row is - should be -- what ??
			*/
			e_print( "# NULL # get_linked_row( row )\n" );
			e_print( "# spec # %s.%s TYPE: %s\n",
				(STR0) spec->tbl->name->str,
				(STR0) spec->name->str,
				(STR0) spec->type_name(t3)
			);
			row->prints();
			break;
		}

		JSObject * js_r2 = r2->get_js_self();
		if( !js_r2 )
		{
	// e_print(" fetching-linked table \n");
			/*
				convert r2 to a JSObject
				requires row_class of jsc_Table of t2
			*/
			table * t2 = spec->get_linked_table();
			JSObject * js_t2 = t2->get_js_self();
			if(!js_t2) break;
	// e_print(" got-js_t2 \n");
			j_cx_obj cx_obj( args->cx, js_t2 );
			j_class_base * arg_class = j_class_base::get_class_base( cx_obj );
			if(!arg_class) break;
	// e_print(" got arg_class \n");
			jsc_Table * jsc_t2 = (jsc_Table *) arg_class;
			jsc_Row * jsc_r2 = jsc_t2->row_class;
			if(!jsc_r2) break;
	// e_print(" got jsc_r2 \n");
			js_r2 = jsc_r2->mk_js_self( r2 );
		}
		if( js_r2 )
		{
	// e_print(" got js_r2 \n");
			args->set_ret( js_r2 );
			return JS_TRUE;
		}
	 }
	 break;

         case T_amnt:
	 {
		amnt_t amnt;
		col_spec_amnt * spec_amnt = (col_spec_amnt *) spec;
		if(! spec_amnt -> get_amnt( row, amnt ))
		{
			/*
				this particularly happened during

					v = tbl.key.col2

				where tbl.key created a NEW row
				and that had NULL as initial default value

				The answer is to LOOK but dont ADD
			*/
			e_print("# FAIL # spec->get_amnt(row, var) # PROB: %s.%s.%s # using ZERO \n",
				(STR0) spec_amnt->tbl->name->str,
				(STR0) row->debug_row_name(),
				(STR0) spec_amnt->name->str
			);
			if(1) row->prints();
			args->set_ret( 0.0 );
			return JS_TRUE;

			gdb_invoke( true );
			if(! spec_amnt -> get_amnt( row, amnt )) {}
			return args->ReportError( "get_amnt failed" );
		}
		// TESTER // args->set_ret( (int) amnt.get_pounds_double() );
		args->set_ret( amnt.get_pounds_double() );
		// e_print(" T_amnt-dbl\n");
		return JS_TRUE;
	 }
	 break;
         case T_date:
         case T_date_rel:
         case T_percent:
         case T_str_key:
         case T_str_desc:

         case T_expr:
	 default:

		str2 * s2 =  spec->get_str2_for_edit( row );
		if(!s2){
			 return args->ReportError( "NULL from get_str2_for_edit" );
		}
		args->set_ret( args->mk_str( (STR0) s2->str ));
		return JS_TRUE;

	 break;

	}

	str2 * s2 =  spec->get_str2_for_edit( row );
	if(!s2) return args->ReportError( "NULL from get_str2_for_edit" );
	args->set_ret( args->mk_str( (STR0) s2->str ));
	return JS_TRUE;
}

/*!
*/
// virtual
JSBool jsc_Row::j_set_property_by_id( j_args_prop_set * args )
{
	if( args->id_int >= tbl->col_specs.N() ) return JS_FALSE;
	row_inst * row = (row_inst *) args->inst;
	col_spec * spec =  tbl->col_specs[ args->jsc_prop->idx ];
	if(!spec)
	{
		return args->ReportError_v( "spec is NULL" );
	}

	// switch( * args->val ) {
	// jsval.h describes stupidity as:
	// /* Use different primitive types so overloading works. */
	jsval jsv = * args->val ;
	if( jsv == JSVAL_NULL) {
		return args->ReportError_v( "val is NULL" );
	}
	if( jsv == JSVAL_VOID) {
		return args->ReportError_v( "val is VOID" );
	}

	// jsapi.h line 54
	JSType tag = args->val_type_tag();
	// e_print( "tag is %d\n", tag );
	// e_print( "FIELD: %s\n", (STR0) args->as_string( args->val ));
	buffer2 pn;
	buffer2 buf;
	switch( tag ) {
	 case JSTYPE_OBJECT:
	{
		// e_print( "JSTYPE_OBJECT \n" );
		j_cx_obj cx_obj( args->cx, JSVAL_TO_OBJECT( * args->val ) ); // move this there
		j_class_base * arg_class = cx_obj.get_class_plus();
		if( !arg_class ) // its not an extension type
		{
			 return args->ReportError( "FAIL OBJECT is not an ACCTS extension or PLAIN" );
		}
		if( arg_class ) // its an extension type - LURK PRESUME ITS A ROW
		{
			/*
				this callback is for JS objects of type jsc_Row
				the field might be a linked row
				but it might be some other class
				but it might be from a different table
				so the assumption is valid, cast is valid
			*/
			jsc_Row * row_class = (jsc_Row *) arg_class;
			table * t2 = row_class->tbl;
			/*
				linked row might be froma  different table
			*/
			table * t3 = spec->get_linked_table();
			if( t2 != t3 )
			 if( t2 && t3 )
			{
			 return args->ReportError( "FAIL t2 != spec->get_linked_table(): linked row type mismatch" );
			}
			j_cx_obj_inst cx_obj2( args->cx, JSVAL_TO_OBJECT(* args->val) );
			row_inst * r2 = (row_inst *) cx_obj2.inst;
			if( r2 )
			{
				spec->set_obj( row, r2 );
			}
			return JS_TRUE;
		}
	}
		break;
#if 0
	 case JSTYPE_VOID:
	 case JSTYPE_FUNCTION:
	 case JSTYPE_OBJECT: // NOT

	 case JSTYPE_INT: // NOT
	 case JSTYPE_DOUBLE: // NOT
	 case JSTYPE_NUMBER:
	 {
		// e_print( "JSTYPE_INT \n" );
		int intval = JSVAL_TO_INT( * args->val );
		switch( spec->get_field_type() ) {
		 case T_link_grp:
			return args->ReportError_v( "expected GRP got INT" );
		 case T_link_row:
			return args->ReportError_v( "expected ROW got INT" );
		 case T_amnt:
		 {
			amnt_t amnt;
			amnt.set_pounds( intval );
			col_spec_amnt * spec2 = (col_spec_amnt *) spec;
			if( spec2->set_amnt( row, amnt ) )
				return JS_TRUE;
			else
				return args->ReportError_v( "set_amnt(row,amnt) failed" );
		 }
		 case T_date:
		 {
			// fall through to string assignment
			break;
		 }
		 case T_date_rel:
		 {
			// fall through to string assignment
			break;
		 }
		 case T_percent:
		 {
			per_cent_t percent;
			percent.set( (double)intval );
			col_spec_percent * spec2 = (col_spec_percent *) spec;
			if( spec2->set_percent( row, percent ) )
				return JS_TRUE;
			else
				return args->ReportError_v( "set_percent(INT) failed" );
		 }
		 case T_str_key:
		 case T_str_desc:
		 case T_expr:
			// fall through to string assignment
			break;
		}
	 }
	break;
#endif
	 case JSTYPE_NUMBER:
	{
		// e_print( "JSTYPE_DOUBLE \n" );

		double dblval;
		if(!args->ok_as_double(* args->val, dblval)) {
			return args->ReportError_v( "expected GBP got non-double" );
		}
		switch( spec->get_field_type() ) {
		 case T_link_grp:
			return args->ReportError_v( "expected GRP got DBL" );
		 case T_link_row:
			return args->ReportError_v( "expected ROW got DBL" );
		 case T_amnt:
		 {
			amnt_t amnt;
			amnt.set_pounds( dblval );
			col_spec_amnt * spec2 = (col_spec_amnt *) spec;
			if( spec2->set_amnt( row, amnt ) )
				return JS_TRUE;
			else
				return args->ReportError_v( "set_amnt(DBL) failed" );
		 }
		 case T_date:
		 {
			return args->ReportError_v( "set_percent(DBL) failed" );
		 }
		 case T_date_rel:
		 {
			return args->ReportError_v( "set_percent(DBL) failed" );
		 }
		 case T_percent:
		 {
			per_cent_t percent;
			percent.set( (double)dblval );
			col_spec_percent * spec2 = (col_spec_percent *) spec;
			if( spec2->set_percent( row, percent ) )
				return JS_TRUE;
			else
				return args->ReportError_v( "set_percent(DBL) failed" );
		 }
		 case T_str_key:
		 case T_str_desc:
		 case T_expr:
			// fall through to string assignment
			break;
		}
	 }
	break;
	 case JSTYPE_STRING:
		// e_print( "JSTYPE_STRING \n" );
		// fall through to set from string
		break;
	 case JSTYPE_BOOLEAN:
		e_print( "j_set_property_by_id(args) JSTYPE_BOOLEAN, %s \n",
			(STR0) args->prop_name(pn) );
		// fall through to set from string
		return JS_TRUE;
		break;
	 default:
		e_print( "FAIL ON %s\n", (STR0) args->as_string( args->val, buf ) );
		return args->ReportError_v( "val is of unknown type # js_accts.cxx" );
		break;
	};


	/*
		if args->val is string, simply set_from_edit
		(the code does row linking itself)

		if args->val is row_inst
			check its of a suitable row-type (linked table matches?)
			and simply assign it

		OR: js access to a row_inst is via a transporter
		that knows the ("row_spec") table of the row
		but that is a bit naff as we want dset.TRANS[t].vat.rate

		SO: js access to row-inst IS JSObject of class TABLE_ROW_xxx
		which could have the data
	*/

	buffer2 buf2;
	const char * s = (STR0) args->as_string( args->val, buf2 );
	if(! spec->set_from_edit( row, s ))
	{
		e_print( "## TBL=%s COL=%s ROW=%s VAL=%s\n",
			spec->tbl->name->uget(),
			spec->name->uget(),
			row->debug_row_name().uget(),
			s
		);
		THROW_dgb_fail(s);
		spec->set_from_edit( row, s );
		return args->ReportError( "FAIL spec->set_from_edit( row, '%s')", s );
	}
	return JS_TRUE;
}
