#ifndef jsc_Dset_H
#define jsc_Dset_H

#include "j_class_plus.h"
#include "dset.h"
#include "table.h"
#include "run_report.h"

using namespace JSSM;
using namespace ACCTS;

namespace ACCTS {

/*!
	jsc_Dset is the JavaScript class associated with data_set

	Each data_set introduces a new JS class, even though there
	is only one instance. 

	There is no real instance of dset, its all in the class ?
	but jsob.priv cannot be null
*/
class jsc_Dset : public j_class_plus
{
 public:
	~jsc_Dset(){}
  static
	jsc_Dset * mk_jsc_Dset( str0 classname,  JS1 * js1, data_set * dset );

	jsc_Dset( str0 _classname, JS1 * js1 )
	: j_class_plus( _classname, js1, false )
	{
	}

// virtual
//	JSBool new_Thing ( j_args_newfunc * args );
// virtual
	JSBool j_get_property_by_id( j_args_prop_get * args );
// virtual
	JSBool j_get_property_by_str( j_args_prop_get * args );
// virtual
//	JSBool j_set_property_by_id( j_args_prop_get * args );
// virtual
//	JSBool j_set_property_by_str( j_args_prop_get * args );
// virtual
//	JSBool j_finalise( j_args_delfunc * args );

	void reregister_tables( data_set * dset );

	static PROTO_JS_FUNC( f_load_file );
	static PROTO_JS_FUNC( f_save_file );
	static PROTO_JS_FUNC( f_add_table );
	static PROTO_JS_FUNC( f_hide_table );
	static PROTO_JS_FUNC( f_run_reports ); // prep tables _1 _2 _3 _4
	static PROTO_JS_FUNC( f_rep_pdf ); // prep tables _1 _2 _3 _4
	// del_table
	// rename_table

};

}; // NAMESPACE
#endif

