#ifndef pdf_base0_H
#define pdf_base0_H

#include "obj_ref.h"
#include "obj_ref_js.h"
#include "obj_hold.h"

/*
	avoiding the runtime dynamic load - no point, still have DLL linkage
*/

#include "pdflib.h"

/*!
	app_lib defines this refcounted class
*/

namespace PDF_GEN {

/*!
	A base class so that doxygen looks nicer
*/
class pdf_obj_ref : public obj_ref_js
{
 public:
	// external
};

/*!
	A base class so that doxygen looks nicer
*/
struct pdf_root : public GRP_lib_base2_pdf
{
 public:
};

/*!
	lots of classes need the PDF pointer but not the extra functions
*/
class pdf_base0 : public pdf_obj_ref
{
 public:
	PDF * pdf;

	/*!
		destructor
	*/
	~pdf_base0()
	{
		// NO NO NO // PDF_delete( pdf );
		// see pdf_ctxt
		pdf = NULL;
	}

	/*!
	*/
	pdf_base0( PDF * _pdf )
	{
		pdf = _pdf;
	}
};


}; // NAMESPACE

#endif
