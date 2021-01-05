#ifndef jsc_Accts_Module_H
#define jsc_Accts_Module_H

/*
	This is where accts_gtk includes javascript support,
	but if WITH_JS is not defined, some pointers should be
	reduced or removed.

	For make deps to be used, WITH_JS should really be defined
	IN THIS FILE, and all dependent users would get recompiled.
*/

#include "JS1.h"
#include "j_args.h"
#include "j_class_plus.h"
#include "obj_ref_js.h"

////////////////////////////////////////////////////////////

#include "dset.h"
#include "j_args.h"

using namespace JSSM;
using namespace ACCTS;

////////////////////////////////////////////////////////////

/*!
	jsc_Accts_Module is almost a module
*/
class jsc_Accts_Module : j_class_plus
{
 public:
	jsc_Accts_Module( JS1 * js1 );

	/*
		Initialise a new JS Accts_Module JSObject 
	*/
  virtual
	JSBool new_Thing( j_args_newfunc * args );

	static PROTO_JS_FUNC( f_load_script );
	static PROTO_JS_FUNC( f_load_dset );
	static PROTO_JS_FUNC( f_add_dset );
	static PROTO_JS_FUNC( f_gdb_invoke ); // right now
	static PROTO_JS_FUNC( f_gdb_error ); // ReportError will THROW
	static PROTO_JS_FUNC( f_view_pdf ); // calls xpdf
	static PROTO_JS_FUNC( f_view_pdf_page ); // calls xpdf
	static PROTO_JS_FUNC( f_view_html ); // calls netscape
	static PROTO_JS_FUNC( f_exit ); // module allows exit
	static PROTO_JS_FUNC( f_make ); // dir target
	static PROTO_JS_FUNC( f_subst_dry_f1_f2 ); // dictionary file1 file2
};


#endif

