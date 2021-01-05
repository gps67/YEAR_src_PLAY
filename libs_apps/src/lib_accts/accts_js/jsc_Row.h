#ifndef jsc_Row_H
#define jsc_Row_H

#include "j_class_plus.h"
#include "dset.h"
#include "table.h"

using namespace JSSM;
using namespace ACCTS;

namespace ACCTS {

/*!
	jsc_Row is the JavaScript peer class of a row_inst
*/
class jsc_Row : public j_class_plus
{
	obj_hold<table> tbl; // LURK THIS WILL LEAK AND STICK
 public:
  static
	jsc_Row * mk_jsc_Row( JS1 * js1, table * tbl );

	jsc_Row( str0 classname, JS1 * js1, table * tbl );

 virtual
	JSBool j_get_property_by_id( j_args_prop_get * args );
	JSBool j_set_property_by_id( j_args_prop_set * args );

	void reregister_columns( table * tbl );
};

}; // NAMESPACE
#endif



