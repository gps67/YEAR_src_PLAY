#ifndef EV_STUBS_H
#define EV_STUBS_H

namespace EV {

class api_ev;	// an event aerial // signals to and from this radio
// bias is to receive events, send signals KNOW_DONE, accumulate into EDGE
// EDGE is a wiper blade, it passes everywhere, lands and rests when DONE
// it has virtual on_event(); // on_event( ARGS ); // on_event( CTXT, ARGS );
// 
// on_event SIGNAL update _required queued_enq_ack add_ev_to_list QUEUED
// RUN_NOW_ONE_STEP pass over available SCRIPT send_all one_step_on
// return LIST of things needed to get RESULT

}; // namespace

#endif
