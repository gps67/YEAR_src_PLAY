#ifndef CTXT_stubs_h
#define CTXT_stubs_h

// TODO move to NS BASE1

 struct err_int_t;

namespace BASE1
{
 struct DIAG;	// mystery hidden thing ;-)
 struct CTXT;	// mystery hidden thing ;-)

 extern CTXT * get_CTXT_for_this_thread();
 extern bool set_CTXT_for_this_thread(CTXT * ctxt);

 extern DIAG * get_DIAG_for_CTXT(CTXT * ctxt); // the recent one

 inline
 DIAG * get_DIAG_for_this_thread()
 {
	return get_DIAG_for_CTXT( get_CTXT_for_this_thread() );
 }; // the recent one

 extern err_int_t & get_err_int_for_this_thread();

};



#endif

