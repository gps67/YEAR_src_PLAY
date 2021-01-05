#ifndef pdf_ctxt_H
#define pdf_ctxt_H

#include "pdf_base0.h"
// #include <stdlib.h>

namespace PDF_GEN {

	class pdf_base;	// virtual thing that produces a document



/*!
	The PDF library has one contaext that could produce a series of document,
	but only one doc can be active at a time
*/
class pdf_ctxt : public pdf_obj_ref
{
 public:
	PDF * pdf;
	// err_info ei;

	~pdf_ctxt();
	pdf_ctxt();

	bool run_wrapped_report( pdf_base * report );
};


}; // NAMESPACE

#endif
