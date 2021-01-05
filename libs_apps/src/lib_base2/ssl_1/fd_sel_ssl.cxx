#include "fd_sel_ssl.h"
#include "dgb.h"

// ------------------------

#if 1 // you MIGHT want these =0 to force actual implementation

// virtual 
void fd_sel_ssl::ssl_says_avail_space()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	STEP("You may write more and maybe unblock upstream");

	// the OTHER side of the application
	// now subscribes to events that generate data that fills the space
	// or unblocks, or ...

	// this base class doesnt have an other side ...
	// so generate no data, and do nothing
	// except complain
}

// virtual 
void fd_sel_ssl::ssl_says_avail_data()
{
	FAIL("FD %d %s - not replaced!", fd_get(), debug_get_obj_name() );
	STEP("You should read as much as possible or else maybe unblock upstream");

	// THIS SIDE of the application has received some data
	// and should handle it
	// this base class throws it away ...
	
	uchar * pos = in_buff.circ_addr();
	int len = in_buff.circ_len();
	fwrite( (const char *) pos, 1, len ,stdout);
	in_buff.circ_used( len );

	// if the buffer gap is smaller than 16K
	// if the buffer gets full
	// remember you wont fet another select_says_read
	// to remind you tht data is still in the SSL buffer
	
}

void fd_sel_ssl::ssl_says_renegociating()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	STEP("TBS");
}

void fd_sel_ssl::ssl_says_renegociated()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	STEP("TBS");
}

void fd_sel_ssl::ssl_says_eof_input()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	STEP("TBS");
	ssl_shutdown();
	ssl_close();
	// close();
}

void fd_sel_ssl::ssl_says_error(err_int_t & err_int )
{
 //	gdb_invoke();
	INFO("BASE FD %d %s %s", fd_get(), debug_get_obj_name(),
		(STR0) err_int.e_str
	);
	err_int.FAIL_REPORT();
//	eof_synthetic();
	call_eof_input(); // move towards shutdown
}

void fd_sel_ssl::ssl_says_negociation_error(err_int_t & err_int )
{
	INFO("BASE FD %d %s %s", fd_get(), debug_get_obj_name(),
		(STR0) err_int.e_str
	);
//	err_int.FAIL_REPORT();
	ssl_says_error( err_int ); // 
	call_eof_input(); // move towards shutdown
}


/*!
*/
// virtual
void fd_sel_ssl:: eof_synthetic()
{
	FAIL("OK but synthetic");
	ssl_says_eof_input();
	WARN("calling base class");
	fd_selectable_base::eof_synthetic();
}

#endif
// ------------------------

#include <openssl/err.h>
#include <openssl/ssl.h>


#include "SSL_global.h"
#include "X509_cert.h"

#include "fd_selectable.h"
#include "buffer2.h"
#include "sock_addr_ip.h"
#include "sock_addr_ip.h"

/*!
	POLL_STATE is an inline macro that calls poll_for_state_change()
	if this->ssl->state has changed from this->ssl_state_prev 
*/
// #define POLL_STATE if( ssl_state_prev != ssl->state ) poll_for_state_change()
#define POLL_STATE 

extern int errno;

void fd_sel_buf2::dgb_show_in()
{
	WARN("%s", (STR0) in_buff );
	// in_buff.dgb_show( debug_get_obj_name(), "in_buff" );
}

void fd_sel_buf2::dgb_show_out()
{
	WARN("%s", (STR0) out_buff );
	// out_buff.dgb_show( debug_get_obj_name(), "out_buff" );
}


/*!
	the start of something interesting?
*/
//! constructor - init NULL
fd_sel_ssl::fd_sel_ssl( bool _I_am_client)
// : fd_selectable_base() // actually an fd_sel_buf2 // keeps changing
{
	ssl = NULL;
//	bio = NULL;
//	more_to_write = false;
	first_connected = false;
	done_negociation = false;
	priv_I_am_client = _I_am_client;
	poll_for_state_change_init();
	write_chunk_size = 1340;
	// HINT: 1024 + 256 + 60 == 1340
	// subscribe_each();
#if 1
	in_buff.get_space(1024*16*2);
	out_buff.get_space(1024*8);
#else
#warning strange debug settings - small SSL buffer sizes to debug a bug
	in_buff.get_space(16*2);
	out_buff.get_space(4*8);
#endif
	// ethernet TCP MSS 1500
	// 1500 - 20 (MAC) - 5 (TLS_HDR) = 1475
	// multiple of 64 = 1472 (512 bit XXX = 64 bytes) 23*64
	// multiple of 32 = 1472 (256 bit SHA = 32 bytes) 46*32
	// multiple of 32 = 1440 (256 bit SHA = 32 bytes) 45*32
	// -1 (padding always used) = 1471 (but padding not 1 , now 2 4 or 8 )
	// -1 (padding always used) = 1449
	// multiple of 2 = 1470 (padding=2)
	// multiple of 4 = 1468 (padding=4)
	// multiple of 8 = 1464 (183) (padding=8)
	set_write_chunk_size_from_mtu( 1500 );
	set_write_chunk_size_from_mtu( 1497 );
	set_write_chunk_size_from_mtu( 1400 );
	out_buff.get_space( write_chunk_size * 8 );
	STEP("CTOR -- GLOBAL OPTIONS MUST BE SET BEFORE HERE");
	// and before this function was called, and the base class CTOR!
}

/*!
*/
fd_sel_ssl::~fd_sel_ssl()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	STEP("HERE");
	STEP("HERE");
	STEP("HERE");
	STEP("HERE");
	STEP("HERE");
//	if( bio ) BIO_free_all(bio);
	if( ssl ) ;
}

/*!
	This is a bit of old code, that detected done_negociation

	Ironically, it needs to be revamped for renegociation,
	particularly at unexpected times, like during read/write (_some)
*/
void fd_sel_ssl::poll_for_state_change_init()
{
	// const int ssl_state_impossible = 0x1000;
	const int ssl_state_impossible = 0x1000;
	ssl_state_prev = ssl_state_impossible;
	if(!ssl) return;
	// print_state();
	// POLL_STATE;
	// INFO("That is the pre-ssl initial state");
}

/*!
	TODO move this to get it from BIO_get_ssl_mode( bio ); (which does not exist)

	OK (ssl_ctx*)->server == 1 or == 0
	BUT when does ssl_ctx exist ? (and keep it from CTOR)
*/
bool fd_sel_ssl::ssl_mode_is_client()
{
	// only one  of  these is needed
	// the rest is debugging belt and braces
	bool cli1 = class_is_client(); // virtual
	bool cli2 = cli1;
	// openssl knows with this flag ... (but after CTOR tells openssl)
	if(!ssl) WARN("NULL ssl");
#warning absent	if(ssl) cli2 = !(ssl->server);
//	if(ssl) cli2 = !(ssl->server);
	// deprecated, check it for now, delete soon
	bool cli3 = priv_I_am_client;
	if( cli1 != cli2 ) FAIL("cli1 != cli2");
	if( cli1 != cli3 ) FAIL("cli1 != cli3");
	return cli1;
}

/*!
*/
bool fd_sel_ssl::expect_I_am_client()
{
	if(!ssl_mode_is_client()) {
		FAIL("FD %d %s", fd_get(), debug_get_obj_name() );
		return FAIL("mode");
	}
	return true;
}

/*!
*/
bool fd_sel_ssl::expect_I_am_server()
{
	if(ssl_mode_is_client()) {
		FAIL("FD %d %s", fd_get(), debug_get_obj_name() );
		return FAIL("mode");
	}
	return true;
}


/*!
	After accept, and after any plain io
*/
bool fd_sel_ssl::build_bio_client( SSL_global_base & SSL, int fd )
{
	// fd must already exist and be open
	fd_set_socket( fd );
	return build_bio_client( SSL );
}

/*!
	After accept, and after any plain io
*/
bool fd_sel_ssl::build_bio_client( SSL_global_base & SSL )
{
	if(!expect_I_am_client()) FAIL("HERE");
	return build_bio_(SSL);
}

/*!
	After accept, and after any plain io
*/
bool fd_sel_ssl::build_bio_server( SSL_global_base & SSL, int fd )
{
	STEP("HERE");
	// fd must already exist and be open
	fd_set_socket( fd );
	return build_bio_server( SSL );
}

/*!
	After accept, and after any plain io
*/
bool fd_sel_ssl::build_bio_server( SSL_global_base & SSL )
{
	STEP("HERE");
	if(!expect_I_am_server()) FAIL("HERE");
	return build_bio_(SSL);
}

/*!
	HMM it is only SSL_sef_fd that needs fd ... survive without it?
*/
bool fd_sel_ssl::build_bio_( SSL_global_base & SSL )
{
	errno_zero();
	STEP("HERE");
	// fd must already exist and be open
	// fd_set_socket( fd );
	if(!expect_open()) {
		return FAIL("fd must be open to set SSL on it");;
	}

	// the _CLASS_ must _know_ that it is a client/server, to tell ssl!

	if(ssl_mode_is_client()) {
		ssl = SSL.SSL_new_client(this);
	} else {
		ssl = SSL.SSL_new_server(this);
	}
	if(!ssl) return FAIL("NULL ssl"); // it has own diag?

	// AUTO creates a BIO_new_socket
	SSL_set_fd( ssl, fd_get() );

 // and we dont need direct access to that bio, OK
 //	bio = SSL_get_wbio( ssl ); //humph

 // and this is NOT how it is set (see SSL_new_server())
 //	set_ssl_mode_client_or_server();

// again - want full VTB:
	WARN("NEED FULL VTBL"); // but we do have it!
	set_verify();

	// STEP("register_info_cb");
	register_info_cb();
	poll_for_state_change_init(); // after ssl set
	return true;
}

/*!
	I think this is maybe the libraries private function
void fd_sel_ssl::set_ssl_mode_client_or_server()
{
	FAIL("HERE");
	return;
	STEP("HERE");
	if( ssl_mode_is_client() ) {
		BIO_set_ssl_mode( bio, 1 );
	} else {
		BIO_set_ssl_mode( bio, 0 );
	}
}
*/

#if 0
#see ssl.h
#define SSL_ERROR_NONE                  0
#define SSL_ERROR_SSL                   1
#define SSL_ERROR_WANT_READ             2
#define SSL_ERROR_WANT_WRITE            3
#define SSL_ERROR_WANT_X509_LOOKUP      4
#define SSL_ERROR_SYSCALL               5 /* look at error stack/return value/errno */
#define SSL_ERROR_ZERO_RETURN           6
#define SSL_ERROR_WANT_CONNECT          7
#define SSL_ERROR_WANT_ACCEPT           8
#endif

const char * str_SSL_get_error(int code)
{
	switch (code)
	{
	 case SSL_ERROR_NONE:		return "SSL_ERROR_NONE";
	 case SSL_ERROR_SSL:		return "SSL_ERROR_SSL";
	 case SSL_ERROR_WANT_READ:	return "SSL_ERROR_WANT_READ";
	 case SSL_ERROR_WANT_WRITE:	return "SSL_ERROR_WANT_WRITE";
	 case SSL_ERROR_WANT_X509_LOOKUP: return "SSL_ERROR_WANT_X509_LOOKUP";
	 case SSL_ERROR_SYSCALL:	return "SSL_ERROR_SYSCALL";
	 case SSL_ERROR_ZERO_RETURN:	return "SSL_ERROR_ZERO_RETURN";
	 case SSL_ERROR_WANT_CONNECT:	return "SSL_ERROR_WANT_CONNECT";
	 case SSL_ERROR_WANT_ACCEPT:	return "SSL_ERROR_WANT_ACCEPT";

	 default:			return "SSL_ERROR_****";
	}
}

int fd_sel_ssl::calls_SSL_get_error( int e )
{
	/*
		ERRM ... surely this disappears with err_int as its replacement
	*/
	if(e>0) {
		dgb_sleep_less(); // I am not so mad
	 	recent_error_code = SSL_ERROR_NONE;
		recent_error_str = "SSL_ERROR_NONE";
		return recent_error_code;
	} else {
		int old_error_code = recent_error_code;
		recent_error_code =  SSL_get_error( ssl, e );
		recent_error_str = str_SSL_get_error( recent_error_code );
		if(recent_error_code != SSL_ERROR_WANT_READ ) // common
		 if(1)	INFO("recent_error_str = '%s'", recent_error_str );
		if(old_error_code == recent_error_code)
			dgb_sleep_more(); // possibility of repeat READ_MORE
		return recent_error_code;
	}
}

/*!
	see http://www.mail-archive.com/ssl-users@lists.cryptsoft.com/msg00514.html
	SSL_do_handshake
*/
void fd_sel_ssl::calls_SSL_connect()
{
	FAIL("DEPRECATED");
}

/*!
*/
void fd_sel_ssl::calls_SSL_accept()
{
	FAIL("DEPRECATED");
}


/*!
*/
void fd_sel_ssl::ssl_shutdown()
{
	// RTFM - BOOK and examples part2.pdf
	//
	//	shutdown(fd,1); // trigger remote to do its thing ?
	//
	//	if(is_client()) ... else ...
	//
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	// need to be at a proper disconnection point in the application
	// need to send (and receive) a close_verify, to not be truncated
	// need to see everything normal, or actually abrupted, both sides
	// one sided shutdown - needs knowledge of protocol
	// at least sending our goodbye makes sense
	SSL_shutdown( ssl );
	// do a BIO_flush
	// then do a shutdown on the write side
	// then wait for select_says_error
	// maybe set a flag to monitor how htings go, dont complain
}

/*!
*/
void fd_sel_ssl::ssl_close()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	dgb_show_in();
	dgb_show_out();
	ssl_shutdown();
	// close();
}

/*!
	call BIO_read()

	call PKT_IN()

	note: select_says_read  may or may not be relevent, and more data
	can be in the SSL buffer (16K blocks) without fd signalling
	(if you left some there), and SSL can absorb data (upto 16K blocks)
	without it being available. However from quiescent, they are close.

	RETURN = stay_subscribed to read (can be ignored?)

	OR RETURN = progress was made (subscription is a flag in obj)
	OR RETURN = no error was made (subscription is a flag in obj)

	DEBUGGING SHOWS: multiple SSL frames in buffer, each from SSL_read
	EG multiple 10 byte mouse moves, each framed in SSL frame
	TODO test when buffer is less than frame ...
	ie its actually strange when i==len, because SSL has more data
	than we have space for it (or exact match!)
*/
bool fd_sel_ssl::ssl_read_some()
{
	// The initial guess is that we are _PROBABLY_ subscribed
	// always handle _changes_ to that subscription

	// INFO("FD %d %s", fd_get(), debug_get_obj_name() );

	if(!done_negociation) {
		WARN("FD %d %s NOT done_renegociation ", fd_get(), debug_get_obj_name() );
		do_negociation(); // might also change subscription
		dgb_sleep_more(); // I am mad, maybe
		return is_subscribed_IN();
	}

	// LURK using buffer2 when its declared as buffer2 .. OK
	// TODO rewrite circ_ to avoid confusing same use (but require knowl)
	//
	// lock down 16k (asif)
# warning this will grow - only extend if total space is less than ...
#if 0
#define K32 (1024*32)
#define K16 (1024*16)
#else
#warning buffer sizes effectively limited to external assignment
#define K32 (32)
#define K16 (16)
#endif
	// dont bloat up beyond 16/32 K of data we ALREADY have
	// do grow upto that size though
	int bloat = in_buff.circ_data_avail();
	if( bloat < (K32) ) {
		// expand buffer to max 32 + 16 K (or total - bloat)
		// possible circ ness makes it more, but not infinite
		in_buff.get_space(K16);
	}

	int loop_count = 0;
	bool repeating_over_chunks = true;
	while( repeating_over_chunks ) {
	  loop_count++;
	  // can copy down
	  in_buff.circ_check();

	  int len = in_buff.space_avail();
	  if( len < 1 ) {
		WARN("FD %d %s FULL", fd_get(), debug_get_obj_name() );
		dgb_sleep_more(); // I am on the edge of madnesses consequence
		if(0) {
			ssl_says_avail_data();
			// check space again
		}
		dgb_show_in();
		return subscribe_IN_off();
	  }

	  dgb_show_in();
	  uchar * pos = in_buff.gap_addr();
	  int i = SSL_read( ssl, pos, len );
	  if(i>0) {
		  INFO("FD %d %s loop_count %d SSL_read() returned %d of %d",
			fd_get(),
			debug_get_obj_name(),
			loop_count,
			i,
			len
		  );
		// i is the number of bytes read in
		// the data is already in the buffer, count it
		in_buff.circ_added( i );

		// I am not mad progress has been made
		dgb_sleep_less();
		//
	  	if(i!=len) {
			// last chunk, or last part of a chunk?
			if(0) repeating_over_chunks = false;
		}
		//
		if(i==len) {
		 WARN("FD %d %s SSL_read() returned %d of %d - !! EXACTLY FULL ALERT!!",
			fd_get(),
			debug_get_obj_name(),
			loop_count,
			i,
			len
		 );
		 WARN("Maybe copy down, get_space, try again and then see ?");
		 }
		//
		// unlock, transfer lock to next phase
		//
		// in_buff might have passed TIDE markers (incl 0 100%)
		// maybe make it an object, or give it an obj for events
		// it_tells->event_buffer_has_more_space_check_world()
		//
		// ################# PASS DATA TO PROGRAM ###############
		ssl_says_avail_data();
		// ###### packet boundries sort of preserved ######
		// stay subscribed - if already subscribed
		// boundry is SSL_ frame within IP packet stream
	  	if(repeating_over_chunks) {
		//	STEP("REPEATING_over_chunks"); // sounds worse
		//	is_subscribed_IN(); // prints stats!
			continue;
		} else {
			return is_subscribed_IN();
		}
		return true;
	  } else {
	   repeating_over_chunks = false;
	   err_int_t & err_int = BASE1::get_err_int_for_this_thread();
	   err_int.eat_OS_error();
	   err_int.get_SSL_error();
// gdb_invoke();
	   if(!err_int.peek_if_read_dry()) // dont be noisy here is common case
		  INFO("FD %d %s loop_count %d SSL_read() returned %d of %d err_int says %s",
			fd_get(),
			debug_get_obj_name(),
			loop_count,
			i,
			len,
			(STR0) err_int.e_str
		  );
	   switch( calls_SSL_get_error(i) ) {
	    case SSL_ERROR_NONE:
		FAIL("CODE_ERROR - already handled above");
		dgb_sleep_more(); // but not a lot, maybe _here // done
	    break;
	    case SSL_ERROR_ZERO_RETURN:
		INFO("SSL ERROR %s", recent_error_str);
		dgb_sleep_less(); // eof is not progress but ..
		ssl_says_eof_input();
		return true;
	    case SSL_ERROR_SYSCALL:
		FAIL("SSL_read() says %s", recent_error_str );
		dgb_sleep_more(); // an error ... or put into C_SSL_ERR
		ssl_says_error(err_int);
		ssl_says_eof_input();
		err_int.clear();
		WARN("NEED ssl_says_error_disconnect");
		return subscribe_IN_off();
		return false;
		
	    case SSL_ERROR_WANT_READ:
		if(err_int.clear_if_read_dry()) {
			if(loop_count==1) {
				// NO PROGRESS HAS BEEN MADE
				WARN("This might happen on fresh connection");
				WARN("This might happen on FRAME > PKT");
				dgb_sleep_more();
				dgb_sleep_here();
				if(!SSL_is_init_finished(ssl)) {
					WARN("!SSL_is_init_finished - is_this_a_renegociation?");
					// WE are bonkers - thats tested above
					dgb_sleep_more();
					dgb_sleep_here();
					return subscribe_IN_on(); // want READ
				}
			} else {
				// GOOD progress has been made
				// and its been counted above
				// dont nag about the end of the loop
				// but to debug this edge case
#if 1
				INFO("PROGRESS HAS BEEN MADE");
		//		if(!SSL_is_init_finished(ssl)) FAIL("DEBUG ME");
#endif
			}
			return subscribe_IN_on(); // want READ
		}
		WARN("UNREACHABLE");
		if(!SSL_is_init_finished(ssl)) {
			STEP("!SSL_is_init_finished - is_this_a_renegociation?");
			dgb_sleep_more(); // reneg is not quite an error
			return subscribe_IN_on(); // want READ
		}
		STEP("SSL_ERROR_WANT_READ - an NBIO case - INCOMPLETE SSL FRAME");
		dgb_sleep_here(); // probably not mad, but if we are ...
		// which is what we tried to avoid in _OUR_ server
		return subscribe_IN_on(); // want READ
		break;
	    case SSL_ERROR_WANT_WRITE:
		err_int.eat_OS_error();
		err_int.clear_if_read_dry(); // _write_ too
		WARN("SSL_ERROR_WANT_WRITE - welcome NBIO case - we have maxed the line out");
		subscribe_OUT_on();
		if(!SSL_is_init_finished(ssl)) {
			FAIL("!SSL_is_init_finished - done_negociation == true?");
			STEP("!SSL_is_init_finished - is_this_a_renegociation?");
			done_negociation = false;
			dgb_sleep_more(); // reneg is not yet expected
			dgb_sleep_more(); // reneg is not yet expected
			dgb_sleep_more(); // reneg is not quite an error
			subscribe_OUT_on(); // want WRITE
			subscribe_IN_off(); // want READ
			do_negociation();
			return is_subscribed_IN();
		}
	    break;
	    default:;
		FAIL("SSL ERROR %s", recent_error_str);
		dgb_sleep_more(); // but not a lot, maybe _here // done
	   }
	  }
	}

	FAIL("would that it be unreached");
	return is_subscribed_IN();
	return false;
	return true;
}

bool fd_sel_ssl::set_write_chunk_size_from_mtu( int size )
{
	if( size < 200 ) {
		if( write_chunk_size < 200 )
			// PATIENCE LOST // back to best // IP4 eth0
			write_chunk_size = 1340;
		return FAIL( "mtu size %d too small", size );
	}
	// what daftness is this ?? 
	int factor_64 = 64; // must be >=4
	int n_5_tls = 5;
	int n_20_mac = 20;
	int n_4_pad = 4; // padding >0 and also aligns to word size
	int multiple = (size - n_5_tls - n_20_mac) / factor_64;
	int size2 = (multiple * factor_64) - n_4_pad;

	INFO("Setting write_chunk_size to %d (to fit in %d) (was %d)",
		size2, size, write_chunk_size );
	write_chunk_size = size2;
	return true;
}


bool fd_sel_ssl::nudge_write_some()
{
	STEP("BOUNCING the NUDGE");
	ssl_write_some();
	return true;
}

/*!
	call BIO_write() to take plain data from out_buff through SSL to NET

	SIDE EFFECT - change subscription

	out_buff - stuff to write

	The application appends stuff into out_buff, and whenever it is happy,
	calls ssl_write_some, which writes out chunks of 1472-1 or something.
	Other functions call call this spontaneously (with implied LOCK).

	Getting the write chunk is not clear - 16K is the SSL maximum

	OpenSSL has some wierd concept of keeping the same buffer.
	That may be cancelled by SSL_MODE_ENABLE_PARTIAL_WRITE
	which will also mess up the chiunk boundries ??
	Hopefully it never happens, except for the last semi-chunk.


*/
void fd_sel_ssl::ssl_write_some()
{
	err_int_t & err_int = BASE1::get_err_int_for_this_thread(); // VAR 

	// INFO("FD %d %s", fd_get(), debug_get_obj_name() );

	if(!SSL_is_init_finished(ssl)) {
		INFO("FD %d %s %s", fd_get(), debug_get_obj_name(),
			"- BUT NOT SSL_is_init_finished(ssl)" );
		dgb_sleep_more(); // runaway train madness (maybe)
		if(!is_open()) {
			FAIL("FD %d %s - not is_open()", fd, debug_get_obj_name() );
			return;
		}
		do_negociation();
		return;
	}

	POLL_STATE;
	int len = out_buff.circ_len();
	// limit SSL packet size to fit in ethernet packets (maybe)
	if( len > (1024*16)) len = (1024*16);
	if( len > write_chunk_size) len = write_chunk_size;
	if( !len ) {
		// no data to write, could be first writable
		// that is catered elsewhere (ssl_says_connect)
		INFO("FD %d %s - NO DATA TO WRITE - cancelling subscription", fd_get(), debug_get_obj_name() );
		subscribe_OUT_off();
		if(0) {
			// SSL is really 'hungry' tell app to resume feeding
			INFO("FD %d %s", fd_get(), debug_get_obj_name() );
			WARN("WHY CALL ssl_says_avail_space() from here");
			ssl_says_avail_space();
		}
		dgb_sleep_more();
		return;
	}

	uchar * pos = out_buff.circ_addr();
	dgb_show_out();
	int i = 0;
	i = SSL_write( ssl, pos, len );
	if(i>0) {
		// data has been written
		out_buff.circ_used( i );
		// Progress has been made
		// I am not as mad as I thought I was
		dgb_sleep_less();
		//
		// any more to do ?
		//
		int remain_data = out_buff.circ_len();
		int remain_space = out_buff.circ_space_avail();
		INFO("FD %d '%s' SSL_write() wrote %d ",
			fd_get(),
			debug_get_obj_name(),
			i
		);
		if( i != len ) {
			WARN("**** SSL_write() only wrote %d of %d **** ", i, len );
			WARN("**** SSL_write() only wrote %d of %d **** ", i, len );
			WARN("**** SSL_write() only wrote %d of %d **** ", i, len );
			WARN("**** SSL_write() only wrote %d of %d **** ", i, len );
			WARN("**** SSL_write() only wrote %d of %d **** ", i, len );
			WARN("**** SSL_write() only wrote %d of %d **** ", i, len );
			// stupid rules about OpenSSL ...
			// or thoughts about XOFF
			// SSL thinks available space is ...
			// dont present bigger buffer for input to flood
			// or do ...
			// NB not calling ssl_says_avail_space()
			// because even the 1340 chunk size failed to write
			// I think that SSL will always allow 16K then zero
			WARN("**** OpenSSL says call again with same ?? **** ");
			// yes but later // subscribe_OUT_on();
		} else {
			out_buff.circ_check(); // only if 10K space
			remain_space = out_buff.circ_space_avail();
			if(remain_space > remain_data ) {
				ssl_says_avail_space();
				remain_data = out_buff.circ_len();
			}
			else
			 // lurk - 50% tide buffer sensitivity
			 WARN("FD %d %s remain_space %d < remain_data %d - so NOT calling ssl_says_avail_space()",
				fd_get(),
				debug_get_obj_name(),
				remain_space,
				remain_data );
		}
		if(remain_data) {
			subscribe_OUT_on();
		} else {
			subscribe_OUT_off();
		}
		return;
	} else {
		INFO("SSL_write() %d NEGATIVE or ZERO", i );
		dgb_sleep_more(); // and _here too
		// MUST // resubscribe to write
		subscribe_OUT_on();
	}

	switch( calls_SSL_get_error( i ) ) {
	 case SSL_ERROR_NONE:
		FAIL("CODE_ERROR - already handled above");
	 break;
	 case SSL_ERROR_WANT_READ:
		STEP("SSL_ERROR_WANT_READ - an NBIO case - but unsubs");
		subscribe_OUT_off();
		subscribe_IN_on(); // usually always on, but be thorough
		break;
	 case SSL_ERROR_WANT_WRITE:
		WARN("SSL_ERROR_WANT_WRITE - welcome NBIO case - we have maxed the line out");
		subscribe_OUT_on(); // probably already on, but you know
		// dgb_sleep_more(); // but not a lot, maybe _here // done
	 break;
	 case SSL_ERROR_ZERO_RETURN:
		err_int.get_OS_error();
		err_int.clear_if_read_dry();
		err_int.get_SSL_error();
		STEP("is this possible ? EOF output?? SSL_ERROR_ZERO_RETURN");
		FAIL("CODE ERROR - this was handled by other stanza");
		ssl_says_error(err_int);
		dgb_sleep_more();
		err_int.clear();
	 break;
	 case SSL_ERROR_SYSCALL:
		err_int.get_OS_error();
		err_int.clear_if_read_dry();
		err_int.get_SSL_error();
		FAIL("SSL ERROR SYSCALL %s calling ssl_says_error", recent_error_str);
		err_int.get_SSL_error();
		ssl_says_error(err_int);
		// eof_shutdown();
		err_int.clear();
	 default:;
		err_int.get_SSL_error();
		ssl_says_error(err_int);
		err_int.FAIL_REPORT();
		INFO("SSL ERROR %s", recent_error_str);
		POLL_STATE;
		err_int.clear();
	}

	return;
}

/*!
*/
bool fd_sel_ssl::select_says_write()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );

	if(!SSL_is_init_finished(ssl)) {
		// fd opens as writable, do_handshake
		subscribe_OUT_off(); // do_negociation will switch on if needed
		do_negociation();
		dgb_sleep_here(); // runaway train
		if(!SSL_is_init_finished(ssl)) {
			// should really check for SSL_WANTS_WRITE
			// but honestly, that never happens in renegociation
			// except after/during long write
			// and that never happens
			// return false_subscribe_OUT_off();
			return is_subscribed_OUT();
		}
		// continue to write out the buffer
	}

	// loop a few times, to save some of the select loop overhead
	for(int i=0; i< 32; i++ ) {
		WARN("LOOP WRITE ATTEMPT %d", i);
		ssl_write_some();
		if(!is_subscribed_OUT()) break;
	}
	return is_subscribed_OUT();

//	int more = out_buff.circ_data_avail(); // more to write
//	return subscribe_OUT_set(more); // need read
//	return true; // renew subscription
}

/*!
	return value = stay_subscribed
*/
bool fd_sel_ssl::select_says_read()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );

	if(!SSL_is_init_finished(ssl)) {
		do_negociation();
		//
		// dont worry about optimising the first read
		// stick to doing one thing 
		// select_says_read will happen soon enough
		// If worried look at the data flow
		// to see that the last packet does/not contain data
		// if(SSL_is_init_finished(ssl)) {
		// 	ssl_read_some();
		// }
		//
		// it is quite common to need READ during negociation
		// dgb_sleep_here(); // runaway train
		//
		return is_subscribed_IN();
	}

	/*
		try to write out old buffer before adding to it
		ONLY if flush has previously been called
	*/
	// ssl_write_some(); // keep it simple, dont be opportunist

	/*
		DOES the loop over chunks
	*/
	ssl_read_some();

	return is_subscribed_IN();
}

/*!
	AFTER the fd is connected, SSL needs to to an initial setup

	return value is OK or abort, nothing useful ?
*/
bool fd_sel_ssl::do_negociation()
{
	err_int_t & err_int = BASE1::get_err_int_for_this_thread();
	err_int.get_OS_error();
	err_int.clear_if_read_dry();
	err_int.get_SSL_error();
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	if(!is_open()) {
		FAIL("FD %d %s NOT is_open()", fd, debug_get_obj_name() );
	}
	if(done_negociation) {
		if(SSL_is_init_finished(ssl)) {
			WARN("do_negociation() already_done");
			dgb_sleep_more(); // runaway train madness (maybe)
			return true;
		}
		FAIL("done_negociation but NOT SSL_is_init_finished(ssl)");
		done_negociation = false;
		// maybe some sort of renegociation
		// maybe need some flag, for "renegociation soon" not FAIL
	}


/*
 http://www.mail-archive.com/ssl-users@lists.cryptsoft.com/msg00514.html

	HOWEVER there is a new issue with renegociation exposing a weakness.
	I think that the answer, is to surround our own renegociations
	with our own protocols, and go into a bunker mode between them,
	(maybe flush out data inbetween?)

	//	done_negociation = false; // once restarted it is started

	You dont have to call do_negociation,
	you can call SSL_read() or many others.
	It knows if it is a client or server ...

	if(ssl_mode_is_client()) {
		calls_SSL_connect();
	} else {
		calls_SSL_accept();
	}
 //	int e = SSL_do_handshake(ssl); // works fine
 //	int e = SSL_read(ssl,0,0); // works fine
 //	int e = SSL_write(ssl,0,0); // works fine
*/

	//
	// tell SSL to do as much as it can without blocking
	//
	int e = SSL_do_handshake(ssl);
	err_int.get_OS_error();
	err_int.clear_if_read_dry();
	err_int.get_SSL_error();
	int ec =  calls_SSL_get_error( e );
	INFO("FD %d %s SSL_do_handshake() returned %d %d %s",
		fd,
		debug_get_obj_name(),
		e,
		ec,
		recent_error_str
	);
	switch( ec ) {
	 case SSL_ERROR_NONE:
		// fully negociated! get_peer_cert() ssl_says_connected()
		done_negociation = true; // or just after these stmts
		POLL_STATE; // even if only to annoy X509 refcounting self
		get_peer_cert();
		//
		if(0) {
			// I think we will get away without this
			// particularly since ssl_read_some() is called below
			subscribe_IN_on();
			subscribe_OUT_on();
		}
		//
		INFO("FD %d %s calling ssl_says_connected()",
			fd,
			debug_get_obj_name()
		);
		ssl_says_connected();
		//
		if(1) {
			// all sorts of reasons why negociation messed up our selects
			// but OK if nothing to write 
			ssl_read_some();
			ssl_write_some();
		}
		// caller is no_negociation // it calls this->ssl_says_connected
	 break;
	 case SSL_ERROR_WANT_READ:
		/*
			do_negociation() is in the middle of a sequence 
			SSL needs to read (after some write, after some read)
			REMOTE needs time to receive-process-reply
			_HOPEFULLY_ we did read something before write + read
			(I dont know how to detect that _NOTHING_ was read)

			We can also reset LIBC-ERRNO-11 from the read()
			( Resource temporarily unavailable )
			NB we do this because the SSL code didnt, it could have
			NB elsewhere we cleared SSL_VERSION_ERROR a bit soon

		*/
		err_int.eat_OS_error();
		err_int.clear_if_read_dry();
		// errno_zero_if_read_dry();
		subscribe_IN_on(); // as if we did not have this already
	 if(0)	INFO("FD %d %s SSL_ERROR_WANT_READ - welcome NBIO case", fd, debug_get_obj_name());
		break;

	 case SSL_ERROR_WANT_WRITE:
		err_int.eat_OS_error();
		err_int.clear_if_read_dry();
		subscribe_OUT_on(); // this IS unexpected
		WARN("FD %d %s SSL_ERROR_WANT_WRITE - unwelcome NBIO case - when is write ever full on init", fd, debug_get_obj_name());
		dgb_sleep_more();
	 break;

	 case SSL_ERROR_SYSCALL: // why was this not ZERO
		err_int.get_OS_error();
		err_int.clear_if_read_dry();
		err_int.get_SSL_error();
		ssl_says_negociation_error(err_int);
		// caused by remote sending rubbish, and closing FD and then ...
		FAIL("FD %d %s EOF? BAD CLIENT? EINTR? %s", fd, debug_get_obj_name(), recent_error_str);
		STEP("Delete the following lines, if above closed on fail");
		eof_synthetic();
		close();
		err_int.clear();
		return false;

	 break;
	 case SSL_ERROR_ZERO_RETURN:
		err_int.get_OS_error();
		err_int.clear_if_read_dry();
		err_int.clear_if_read_dry();
		err_int.get_SSL_error();
		FAIL("FD %d %s IS THIS EOF ? %s", fd, debug_get_obj_name(), recent_error_str);
		// this is a strange place for EOF
		// EOF can happen during negociation - other side rejects us!
		// or simply timout, but SSL should report that as ... verify?
		ssl_says_negociation_error(err_int);
		STEP("Delete the following lines, if above closed on fail");
		eof_synthetic();
		close();
		err_int.clear();
		return false;

	 break;
	 default:;
		err_int.get_OS_error();
		err_int.clear_if_read_dry();
		err_int.get_SSL_error();
		FAIL("FD %d %s CODE ERROR %s", fd, debug_get_obj_name(), recent_error_str);
		ssl_says_negociation_error(err_int);
		STEP("Delete the following lines, if above closed on fail");
		eof_synthetic();
		close();
		err_int.clear();
		return false;;
	}

	/*
		resubscribe to read - always stay (except XOFF)
		resubscribe to write - check for SSL_WANTS_xxx
		think about un-written flush() ? - ssl done
	*/

	return true; // did not throw
}

/*!
	first read ...
*/
// VIRTUAL
void fd_sel_ssl::select_says_connected()
{
	/*
		fd is now connected (as signalled by UNIX first read-able)
		so either SSL_accept() or SSL_connect()
	*/
	INFO("FD %d %s calling do_negociation()", fd_get(), debug_get_obj_name() );
	do_negociation();
}

bool fd_sel_ssl:: select_says_error(err_int_t & err)
{
	ssl_shutdown(); // actually eof_shutdown will do this // rework both
	close();
	return false;
}

/*!
	base class example - derived classes do own thing

	This is called when SSL has completed its negociation, or renegociation,
*/
void fd_sel_ssl::ssl_says_connected()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	STEP("# SSL # CALL on_first_connected( WITH CERT )" );
	peer_cert.show_some_info();
	if(0) out_buff.put( "GET /index.php HTTP/1.0\r\nHost: 127.0.0.1\r\n\r\n" );
	ssl_write_some();
	flush();
}

/*!
*/
bool fd_sel_ssl::flush()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	ssl_write_some(); // try anyway
	return true;
}

/*!
	IF the remote used a cert, get it
*/
bool fd_sel_ssl::get_peer_cert()
{
	INFO("FD %d %s", fd_get(), debug_get_obj_name() );
	peer_cert.set( SSL_get_peer_certificate (ssl) );
	if(peer_cert.cert) {
		peer_cert.show_more_info();
		return true;
		// if( is_client()) ... else ...
	} else {
		WARN("NO CERT");
		return false;
	}
}

/*
	TODO - better diagnostic

	error:14090086:SSL routines:SSL3_GET_SERVER_CERTIFICATE:certificate verify failed ## 

	... ok 0, depth 1, err 20 unable to get local issuer certificate

	issuer=... subject=... @ each depth


*/

// static
int C_verify_callback(int preverify_ok, X509_STORE_CTX *ctx)
{
	fd_sel_ssl * self = SSL_global_base::get_fd_self_from_X509_STORE_CTX( ctx );
	if( !self ) {
		FAIL("get_fd_self_from_X509_STORE_CTX() returned NULL");
		return 0; // false = reject connection
	}
	STEP("Calling callback");
	if( self -> verify_step( preverify_ok, ctx ) ) {
		return 1; // OK acceptable ath this level
	} else {
		FAIL("CERTIFICATE VALIDATION - TODO=BETTER");
		return 0; // NOT OK - reject
	}
}

/*!
	You might replace this one,
	you MUST replace the one tht it calls!
*/
// virtual
bool fd_sel_ssl::set_verify(int MODE1_eg_SSL_VERIFY_PEER )
{
	if(MODE1_eg_SSL_VERIFY_PEER < 0 ) {
		MODE1_eg_SSL_VERIFY_PEER = SSL_VERIFY_PEER;
	}
	INFO("Setting callback on %s", debug_get_obj_name());
	SSL_set_verify( ssl, MODE1_eg_SSL_VERIFY_PEER, C_verify_callback );
	return true;
}

/*
	SSL finds (3) errors all at depth (0) (but only if you return TRUE)

		unable to get local issuer certificate
		certificate not trusted
		unable to verify the first certificate

*/

// virtual 
bool fd_sel_ssl::verify_step(int preverify_ok, X509_STORE_CTX *ctx)
{
	bool my_ok = preverify_ok; // so if SSL thinks its bad ...

	int     depth;
	X509   *err_cert;
	int     err;

	// http://www.openssl.org/docs/crypto/X509_STORE_CTX_get_error.html

	depth = X509_STORE_CTX_get_error_depth(ctx);
	err_cert = X509_STORE_CTX_get_current_cert(ctx);
	err = X509_STORE_CTX_get_error(ctx);

	int nn = 60;
	buffer2 subj_buff; subj_buff.get_space(nn);
	buffer2 issr_buff; issr_buff.get_space(nn);
#warning LEAK free after use
	// following malloc and leak OK, upgrade to buf, nbytesmax

	const char * estr = X509_verify_cert_error_string(err);
	const char * subj = X509_NAME_oneline (X509_get_subject_name(err_cert),0,0);
	const char * issr = X509_NAME_oneline (X509_get_issuer_name (err_cert),0,0);

	buffer2 msg;
	msg.print( "ok %d, depth %d, err %d %s",
		preverify_ok,
		depth,
		err,
		estr
	);
	INFO( "issuer  = %s", (STR0) issr );
	INFO( "subject = %s", (STR0) subj );
	INFO( "%s", (STR0) msg );

	if(!my_ok) FAIL("%s", (STR0) estr );
	if( err == 20 ) INFO("err20unable-to-get-local-cert-means ...");

	return my_ok;
	return true;
}

/*
	client and server connect,
	client sends GET / HTTP/1.0
	client WAITS for a response from server THEN poll_for_state_change
	(which is clearly wrong! - its a POLL problem)
*/

/*!
	TODO - do_handshake should repeatedly exit until SSL_ST_OK.
	Currently the calls to BIO_write() do that, but then
	write something, before event_ssl_init_finished()

	Actually the initial ssl_says_connected does the expected work,
	it is when either side decides to switch X509 that the surprise happens.
	The underlying protocol, behaves like renegociate (SSL_WANTS_READ...)
	and then a new certificate appears. We want to detect that ...
	but then again, we should really get some callback from SSL,
	rather than some shadow past some window.

	Also _changing_ the CERT is something that the app might know about?

	PLUS this code has fallen into the distraction of coding for first connect,
	so it is useless for that anyway!
	
*/
void fd_sel_ssl::poll_for_state_change()
{
	// if( ssl_state_prev == ssl->state )
	if( ssl_state_prev == SSL_get_state(ssl) )
	{
		// since this is called by the MACRO it would be a CODE_ERROR
		FAIL("poll_for_state_change() - but no change");
		return;
	}
	print_state();
	dgb_sleep_less(); // progress is sanity, maybe
	e_print( "poll_for_state_change()  ssl_state_prev %d ==> ssl->state %d\n",
		ssl_state_prev, SSL_get_state(ssl) );
	ssl_state_prev = SSL_get_state(ssl);
	// if( ssl->state == SSL_ST_OK ) // as a macro:
	if( !SSL_is_init_finished( ssl ) )
	{
		e_print("# SSL # poll_for_state_change() - NOT is_init_finished()\n");
		return;
	}
	e_print("# SSL # poll_for_state_change() - CONNECTED\n");
	if( first_connected ) return;
	if( peer_cert.cert )
	{
		FAIL("This code is unreachable");
		FAIL("because get_peer_cert is called AFTER first_connected=true");
		// DONE // if( first_connected ) return;
		first_connected = true;
		return;
	}
	if( get_peer_cert() )
	{
		first_connected = true;
		done_negociation = true;
		STEP("NOT calling ssl_says_connected - with peers cert");
		STEP("NOT calling ssl_says_connected - maybe need cert changed");
		// ssl_says_connected();
	}
}

#undef POLL_STATE


class SSL_ST_decoder {
 public:
	int st;
	int st0;
	SSL_ST_decoder( int _st )
	: st(_st)
	, st0(_st)
	{
	}

	void st_bit( const char * name, int bitval )
	{
		if(( st0 & bitval ) != bitval ) { return; }
		st &= ~bitval;
		e_print( "# STATE %4.4X %4.4X %4.4X # SSL_ST_ %s\n",
			st0,
			bitval,
			st,
			name
		);
	}

	void st_val( const char * name, int bitval )
	{
		if(!( st == bitval )) { return; }
		st = 0;
		e_print( "# STATE %4.4X # SSL_ST_ %s\n", st0, name );
	}

void prints() {

WARN("SSL_ST_INIT - removed from code, replace with X");

st_bit("SSL_ST_CONNECT",                  0x1000 );
st_bit("SSL_ST_ACCEPT",                   0x2000 );
// st_bit("SSL_ST_MASK",                     0x0FFF );
st_bit("SSL_ST_INIT",                     (SSL_ST_CONNECT|SSL_ST_ACCEPT) );
st_bit("SSL_ST_BEFORE",                   0x4000 );
st_bit("SSL_ST_OK",                       0x03 );
//st_bit("SSL_ST_RENEGOTIATE",              (0x04|SSL_ST_INIT) );

st_bit("SSL_CB_LOOP",                     0x01 );
st_bit("SSL_CB_EXIT",                     0x02 );
st_bit("SSL_CB_READ",                     0x04 );
st_bit("SSL_CB_WRITE",                    0x08 );
st_bit("SSL_CB_ALERT",                    0x4000 /* used in callback */ );
st_bit("SSL_CB_READ_ALERT",               (SSL_CB_ALERT|SSL_CB_READ) );
st_bit("SSL_CB_WRITE_ALERT",              (SSL_CB_ALERT|SSL_CB_WRITE) );
st_bit("SSL_CB_ACCEPT_LOOP",              (SSL_ST_ACCEPT|SSL_CB_LOOP) );
st_bit("SSL_CB_ACCEPT_EXIT",              (SSL_ST_ACCEPT|SSL_CB_EXIT) );
st_bit("SSL_CB_CONNECT_LOOP",             (SSL_ST_CONNECT|SSL_CB_LOOP) );
st_bit("SSL_CB_CONNECT_EXIT",             (SSL_ST_CONNECT|SSL_CB_EXIT) );
st_bit("SSL_CB_HANDSHAKE_START",          0x10 );
st_bit("SSL_CB_HANDSHAKE_DONE",           0x20 );

/* Is the SSL_connection established? */
// TODO: look at an active renegociation
// define SSL_get_state(a)                SSL_state(a)
// define SSL_is_init_finished(a)         (SSL_state(a) == SSL_ST_OK)
// define SSL_in_init(a)                  (SSL_state(a)&SSL_ST_INIT)
// define SSL_in_before(a)                (SSL_state(a)&SSL_ST_BEFORE)
// define SSL_in_connect_init(a)          (SSL_state(a)&SSL_ST_CONNECT)
// define SSL_in_accept_init(a)           (SSL_state(a)&SSL_ST_ACCEPT)

/* The following 2 states are kept in ssl->rstate when reads fail,
 * you should not need these */
st_val("SSL_ST_READ_HEADER",                      0xF0 );
st_val("SSL_ST_READ_BODY",                        0xF1 );
st_val("SSL_ST_READ_DONE",                        0xF2 );
}
};

/*!
*/
void fd_sel_ssl::print_state()
{
	if(!ssl) return;
// DEBUG_print_stack_trace();
	e_print( "# STATE # %4.4X %s -- %s\n",
	 SSL_get_state(ssl),
	 SSL_state_string(ssl),
	 SSL_state_string_long(ssl)
	);
	return;
	SSL_ST_decoder decoder( SSL_get_state(ssl) );
	decoder.prints();
}

STR0 decode_where( int where ) 
{
        if (where & SSL_CB_ALERT) return "SSL_CB_ALERT"; else
        if (where & SSL_CB_LOOP)  return "SSL_CB_LOOP"; else
        if (where & SSL_CB_EXIT)  return "SSL_CB_EXIT"; else
        if (where & SSL_CB_READ)  return "SSL_CB_READ"; else
        if (where & SSL_CB_WRITE) return "SSL_CB_WRITE"; else
        if (where & SSL_CB_ALERT) return "(SSL_CB_ALERT)";
        return "(SSL_CB_xxxx)";
}

 /*!
	This is called everytime ??? XXX changes state ???

STATE = 5000 = UNKWN  = before/connect initialization
STATE = 5000 = UNKWN  = before/connect initialization
STATE = 1110 = 3WCH_A = SSLv3 write client hello A
STATE = 1120 = 3RSH_A = SSLv3 read server hello A
STATE = 1130 = 3RSC_A = SSLv3 read server certificate A
STATE = 1140 = 3RSKEA = SSLv3 read server key exchange A
STATE = 1160 = 3RSD_A = SSLv3 read server done A
STATE = 1180 = 3WCKEA = SSLv3 write client key exchange A
STATE = 11A0 = 3WCCSA = SSLv3 write change cipher spec A
STATE = 11B0 = 3WFINA = SSLv3 write finished A
STATE = 1100 = 3FLUSH = SSLv3 flush data
STATE = 11D0 = 3RFINA = SSLv3 read finished A
STATE = 0003 = SSLOK  = SSL negotiation finished successfully
-you-may-now-write-
STATE = 0003 = SSLOK  = SSL negotiation finished successfully

# PROG # TRACE # select_says_connected() # # select_says_connected( have socket get CERT 
 */
 void SNIPPET_apps_ssl_info_callback(const SSL *ssl, int where, int ret)
        {
        const char *str;
        int w;


        w=where& ~SSL_ST_MASK;


        if (w & SSL_ST_CONNECT) str="SSL_connect";
        else if (w & SSL_ST_ACCEPT) str="SSL_accept";
        else str="undefined";

	buffer2 buf;
	buf.print( 
		"@%-13s "
		"STATE %4.4X "
		" %s = %s", 
		decode_where(where),
		SSL_get_state(ssl),
		SSL_state_string(ssl),
		SSL_state_string_long(ssl)
	);
	INFO( "%s", (STR0) buf );
return;

	buf.clear();


        if (where & SSL_CB_LOOP)
                {
 // done               buf.print("%s:%s\n",str,SSL_state_string_long(ssl));
                }
        else if (where & SSL_CB_ALERT)
                {
                str=(where & SSL_CB_READ)?"read":"write";
                buf.print("SSL3 alert %s:%s:%s\n",
                        str,
                        SSL_alert_type_string_long(ret),
                        SSL_alert_desc_string_long(ret));
                }
        else if (where & SSL_CB_EXIT)
                {
                if (ret == 0)
                        buf.print("%s:failed in %s\n",
                                str,SSL_state_string_long(ssl));
                else if (ret < 0)
                        {
                        buf.print("%s:error in %s\n",
                                str,SSL_state_string_long(ssl));
                        }
                }
	else buf.print("!!");
	if(buf) INFO("%s",(STR0) buf);
        }

 void fd_sel_ssl::register_info_cb() {
	 SSL_set_info_callback(ssl, SNIPPET_apps_ssl_info_callback );
 }

