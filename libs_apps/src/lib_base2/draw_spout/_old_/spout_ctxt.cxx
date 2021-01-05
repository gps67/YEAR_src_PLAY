#include "bad_mad.h"
#include "spout_ctxt.h"
#include "spout_base.h" // run
#include <setjmp.h>

/*!
*/
typedef struct {
        jmp_buf jbuf;
} err_info;

using namespace SPOUT_GEN;

void custom_errorhandler(SPOUT_t *p, int type, const char *msg);

/*!
	destructor - SPOUT_delete()
*/
spout_ctxt::~spout_ctxt()
{
	e_print("# SPOUT_delete( %p )\n", spout );
	SPOUT_delete( spout );
	spout = NULL;
}

	/*!
		this isnt thread safe or anything,
		but it saves having setjmp included everywhere

		Each spout_ctxt should have one allocated, as its address
		is taken by the SPOUT_new2 function, but who cares
	*/
	static err_info ei;

/*!
	constructore - SPOUT_new2()
*/
spout_ctxt::spout_ctxt()
{
	if (setjmp(ei.jbuf) == 0) {
		// possible longjmp within new2 ??
		spout = SPOUT_new2( custom_errorhandler, 0, 0, 0, &ei );
		e_print("# SPOUT_new2( ... ) --> %p \n", spout );
	} else {
		throw bad_mad("SPOUT_new2() failed! (eg malloc)\n");
	}
}

/*!
	uses setjmp to wrap for any SPOUT 'exceptions' thrown

	The dis-advantage of this is that there is only one main() function,
	but you can create the class in advance and set any parameters,
	then run a series of SPOUT commands (which can fail)
*/
bool spout_ctxt::run_wrapped_report( spout_base * report )
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
void custom_errorhandler(SPOUT_t *p, int type, const char *msg)
{
	err_info *ep;
	const char * m1 = "";
	if (type == SPOUT_NonfatalError)
		m1 = "NON-";
	fprintf(stderr, "# SPOUT_t: %sFATAL: %s\n", m1, msg); 
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
			THROW_bad_mad("PDF error");
			longjmp(ep->jbuf, 1);
	}
}


