#include "dgb_fail.h"
#include "pdf_ctxt.h"
#include "pdf_base.h" // run
#include <setjmp.h>

/*!
*/
typedef struct {
        jmp_buf jbuf;
} err_info;

using namespace PDF_GEN;

void custom_errorhandler(PDF *p, int type, const char *msg);

/*!
	destructor - PDF_delete()
*/
pdf_ctxt::~pdf_ctxt()
{
	e_print("# PDF_delete( %p )\n", pdf );
	PDF_delete( pdf );
	pdf = NULL;
}

	/*!
		this isnt thread safe or anything,
		but it saves having setjmp included everywhere

		Each pdf_ctxt should have one allocated, as its address
		is taken by the PDF_new2 function, but who cares
	*/
	static err_info ei;

/*!
	constructore - PDF_new2()
*/
pdf_ctxt::pdf_ctxt()
{
	if (setjmp(ei.jbuf) == 0) {
		// possible longjmp within new2 ??
		pdf = PDF_new2( custom_errorhandler, 0, 0, 0, &ei );
		e_print("# PDF_new2( ... ) --> %p \n", pdf );
	} else {
		throw dgb_fail("PDF_new2() failed! (eg malloc)\n");
	}
}

/*!
	uses setjmp to wrap for any PDF 'exceptions' thrown

	The dis-advantage of this is that there is only one main() function,
	but you can create the class in advance and set any parameters,
	then run a series of PDF commands (which can fail)
*/
bool pdf_ctxt::run_wrapped_report( pdf_base * report )
{
	return report->run_wrapped_report();
}

/*!
	this is copied from the documentation
	it does the job, but still doesnt quite fit C++

	You must be careful about not creating any significant objects
	in a scope that longjmp will leave allocated. 

	The alternative is the recommended one, which wraps each call,
	so any error returns to the caller, (C++ standards).

	YOU CAN MOVE THIS TO ANOTHER FILE AND CHANGE IT
*/
void custom_errorhandler(PDF *p, int type, const char *msg)
{
	err_info *ep;
	const char * m1 = "";
	if (type == PDF_NonfatalError)
		m1 = "NON-";
	fprintf(stderr, "# PDF: %sFATAL: %s\n", m1, msg); 
	switch (type) {
		case PDF_NonfatalError:
		/* The error handler may return after a non-fatal exception. */
			return;

		case PDF_MemoryError:
		case PDF_IOError:
		case PDF_RuntimeError:
		case PDF_IndexError:
		case PDF_TypeError:
		case PDF_DivisionByZero:
		case PDF_OverflowError:
		case PDF_SyntaxError:
		case PDF_ValueError:
		case PDF_SystemError:
		case PDF_UnknownError:
		default:
			ep = (err_info *) PDF_get_opaque(p);
			PDF_delete(p);
			THROW_dgb_fail("PDF error");
			longjmp(ep->jbuf, 1);
	}
}


