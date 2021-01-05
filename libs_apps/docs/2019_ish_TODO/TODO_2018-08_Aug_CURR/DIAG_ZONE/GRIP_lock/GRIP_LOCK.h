#ifndef GRIP_LOCK_H
#define GRIP_LOCK_H

// MATCH // struct GRIP_LOCK : LIBR_LOCK [ GRIP ]

#import "LIBR_LOCK.H"	// STUBS // PLUS //

namespace GRIP {
struct GRIP_LOCK : LIBR_LOCK :: GRIP {

 /*
 	sole owner want full lock on FILE_OF_PAGES - gets it
 	sole owner uses full lock on FILE_OF_PAGES --> SCRIPTS_VILLE ==>
	sole owner finishs last task
	cahe tells sole owner to give up locks
	sole owner keeps OPTIMISTIC fast-re-open 
	sole owner loosens GRIP

	remote appears
		MAYBE server tells (all) subsriber
			LOOSEN GRIP // gets even looser
			GRIP gets even looser
			FAST RESP if anything was waiting, elae LAZY 2s ACK
	
		GRIP now owned by REMOTE

			local cache subscibes to ERA and DEFERRED DATA
			 DATA_DEFERRED
			  SCHEDULE DECAY DELAY
			   Q_NOW
			  	Q_NOW for SOON shipment
			  	Q_NOW for HUNGRY connection

			    Q_EDGE ongoing changes upto end of transaction

			    Q_SOON broadcast changes to change lists
			    Q_NEED ACK from data server, that can DROP some data

			    Q_10_seconds

			    	Q-EDGE on the 10 seconds after full XMIT

				early update

		GRIP remote send batched patch (pre-sent present data)


		GRIP round robin team

			SPOKE HUB does the relaying
			round-robin-broadcast-mix
			EDGE all synced, can open with { PERMIT ON KEY }


		LEXER MATCH
		 MATCH( "EDGE" )

			{ PERMIT on KEY }
			{ "PERMIT on KEY" }
			{"PERMIT on KEY"}
			{" PERMIT on KEY "}

				LEADING SP MIDDLE SP TRAILING

					or any sensible GAP
					possibly ABRUPT EDGE NEXT
 */
	

};
}; // namesapace

#endif
