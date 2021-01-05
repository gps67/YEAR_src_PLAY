#ifndef jsc_Table_H
#define jsc_Table_H

#include "j_class_plus.h"
#include "dset.h"
#include "table.h"

using namespace JSSM;
using namespace ACCTS;

namespace ACCTS {
	class jsc_Row;

/*!
	jsc_Table is the JavaScript peer class of (accts::) table.

	Each table instance has a class of its own, and the attributes
	are tokenised for each.
*/
class jsc_Table : public j_class_plus
{
 public:
	jsc_Row * row_class;
	enum FieldTags {
		FT_rowlist,
		FT_colnames,
		FT_name,
		FT_dset,
		F_unused
	};
  static
	jsc_Table * mk_jsc_Table( JS1 * js1, table * tbl );

	jsc_Table( str0 classname, JS1 * js1, table * tbl );

 virtual
	JSBool j_get_property_by_id( j_args_prop_get * args );
	JSBool j_get_property_by_str( j_args_prop_get * args );

	static PROTO_JS_FUNC( f_add_col );
	static PROTO_JS_FUNC( f_add_type_col );
	static PROTO_JS_FUNC( f_add_type_col_after );
	static PROTO_JS_FUNC( f_add_row );
	static PROTO_JS_FUNC( f_hide_col ); // ie dont save
	static PROTO_JS_FUNC( f_has_row ); 
	static PROTO_JS_FUNC( f_del_row ); 
	static PROTO_JS_FUNC( f_pdf_doc ); 
};

}; // NAMESPACE
#endif


