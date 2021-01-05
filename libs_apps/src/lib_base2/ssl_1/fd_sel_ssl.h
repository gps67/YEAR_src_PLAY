#ifndef fd_sel_ssl_H
#define fd_sel_ssl_H

#include "SSL_global.h"
#include "X509_cert.h"

#include "fd_sel_buf.h"
#include "sock_addr_ip.h"
#include "sock_addr_ip.h"

/*
	TODO: rework parallel system, like tunela-state-machine

	That will enable encrypted SSL packets to be placed on a MUX channel
	alongside other channels, from other encryption (or not) uses.

	That is of itself a danger, (as it will then be used to avoid double
	encrypting channels, possibly with distributed processing of channels),
	as well as the usual trust-the-MUX issues, but is would provide
	an interesting way ahead.

	As well as that, the packets could be carried by non-tcp connections,
	possibly UDP or PIPEs.

	UNTIL THEN: the SSL head, covers a TCP-SOCKET (which it owns,
	until it if finished, but it doesnt close it, and it probably
	doesnt open it). It probably MUST be non-blocking, (issues with
	repeating identical parameters for buffers), and the retry logic
	will be fixed within the class (but should TRY to allow dual
	direction optimism, without blocking for an unlucky choice).

	WANT:
		ssl_says_write()
		ssl_says_read()
		ssl_needs_read()	
		ssl_needs_write()
		ssl_says_eof_input()
		///
	

*/


extern "C" {
	struct bio_st;
	typedef struct bio_st BIO;
};

/*!
	just add the io buffers, fd_selectable.cxx uses this->more_to_write

	XOFF will also be something to consider,
	as well as overshooting the hight-tide mark

	This is a small issue that, the buffers are *OUTSIDE* the SSL filter
	not adjacent to the fd. So you cant simply read and write to them,
	you have to call SSL functions that read and write *FROM* them.
	I.E. These are on the application side not the system side.

	NB DUE TO SSL COMPLEXITY IT HOLDS THE BUFFERS

	In an SSL to PLAIN relay (or even one with a FILTER),
	the buffers are held by SSL (fd_ssl_buf2), and the SSL code
	is limited to those buffers. The PLAIN fd_selecteable
	_could_ read/write direct to them (although I am not used
	to un-writing-data-to-a-buffer3) to avoid double buffering
	when FILTER is known to be better that way.

	Do not expect plain fd_selectable to have such buffers,
	do not look for a broader semantic for stream BIO things

	BUFFER LOCKS - its easy to peek at data in the input buffer,
	as long as it doesnt move unexpectedly, then remove it.
	It is slightly more strange to put data into the outbuffer,
	but not allow SSL to write it (second thread is future)
	until we are certain the FILTER has done, then release the LOCK.
	(Currently there are no locks, other than a single process action).
*/
class fd_sel_buf2 : public fd_selectable_base
{
 public:
// # I want this moved to P0P4 R1 R2 W1 W2 maybe
// # ie a buffer that printfs into a P0P4 with callbacks get_space()
// # 
	buffer2 out_buff;	// output buffer - circular
	buffer2  in_buff;	//  input buffer - circular

	void dgb_show_in();
	void dgb_show_out();
};

/*!
*/
class fd_sel_ssl : public fd_sel_buf
{
	bool priv_I_am_client;
 public:
	SSL * ssl;	// the SSL connection
//	BIO * bio;	// the IO wrap on the SSL

	int recent_error_code;
	const char * recent_error_str; // not malloc

	/*
		state changes - happen when - mean what - need what
	*/
	int ssl_state_prev;
	void print_state();

	/*
		TODO: the SSL has an enumerate STATE,
		with one main action todo.
		Other actions should be avoided or queued.

		in handshake, write is particulrly useful?
	*/
//	bool retry_write;

	bool first_connected; // at the SSL level
	X509_cert peer_cert;

	int write_chunk_size;
	bool set_write_chunk_size_from_mtu( int size );

	fd_sel_ssl(bool _I_am_client);
	~fd_sel_ssl();
	virtual bool class_is_client() = 0;

	void poll_for_state_change_init();

	bool build_bio_server( SSL_global_base & SSL, int fd );
	bool build_bio_client( SSL_global_base & SSL, int fd );
	bool build_bio_server( SSL_global_base & SSL );
	bool build_bio_client( SSL_global_base & SSL );
	bool build_bio_( SSL_global_base & SSL );

	bool ssl_mode_is_client();
	bool ssl_mode_is_server() { return !ssl_mode_is_client(); }
	bool expect_I_am_client();
	bool expect_I_am_server();

//	void set_ssl_mode_client_or_server(); // CONFUSED
	void ssl_shutdown();
	void ssl_close();

	// call_X is a simple wrapper on X, checks retval etc
	// calls_X is a bit more, based on calling X
	// these do the state updates, possibly nested activity?
	//
	bool do_negociation();
	bool done_negociation;
	void calls_SSL_connect();
	void calls_SSL_accept();
	int  calls_SSL_get_error( int e );

	// these mess with the subscription (also do_negociation)
	bool ssl_read_some(); // can call ssl_says_avail_data
	void ssl_write_some(); // can call ssl_says_avail_space

	// fd_sel_buf - does not know it is an SSL thing
	// this is where it is connected
	
	bool nudge_write_some();
//	bool nudge_read_some();

	bool select_says_read();
	bool select_says_write();
	void select_says_connected();
	bool select_says_error(err_int_t & err);

	virtual void ssl_says_connected(); // and first negociated
	virtual void ssl_says_renegociating(); // stop trusting until ...
	virtual void ssl_says_renegociated(); // get_cert() a good idea
	virtual void ssl_says_eof_input(); // ZERO from SSL_read
	virtual void ssl_says_error(err_int_t & err_int ); // 
	virtual void ssl_says_negociation_error(err_int_t & err_int ); // 
	virtual void ssl_says_avail_space();
	virtual void ssl_says_avail_data();
//	virtual void ssl_says_write(); // to its buffer only
//	virtual void ssl_says_read(); // from its buffer only

	virtual void eof_synthetic(); // from base class, poss from non fd

	virtual bool set_verify(int MODE1 = -1 );
	virtual bool verify_step(int preverify_ok, X509_STORE_CTX *ctx);

/*
	TIDE = third quarter buffer full
	really needs better approach, such as cdrecord's buffer analysis
	One easy way would be to have 4 gears for the 4 quarters
	but there is still the handling of XOFF_upstream XOFF_relax
	bool XOFF_is_full;
	There is even more fun, when other code uses SSL to do its XOFF for it
	(as not doing so would be wrong!!)

	When we have a MUX, it will have to do its own window ACKS,
	and divide the buffer space available between all the sub-links,
	up to the point where a sub-channel can hold itself back
	even when the line is idle, becuase the downstream window lacks space.
	And if the sub-channel restarts, it has just enough to not go idle.

	For now, the read channel will probably throttle on the write side
	because ACKS are never send, because SSL wants (_something_).
	Watch out for unresponsive KEYS when the SCN is intensive.

	virtual void ssl_says_TIDE_output_full()=0; // event
	virtual void ssl_says_TIDE_output_space()=0; // event
	virtual void ssl_says_TIDE_output_idle()=0; // event
//	virtual void avail_space()=0;
//	virtual void avail_data()=0;
*/

/*
	LOCKS

		for now - application is single tasking

		SSL forces control around ssl_says_{read,write}
		and copy_down can happen on the relevent buffer

	COPY DOWN

		it would be nice for the library to adhere to 4K pages
		and only copydown with alignment. Then use MMAP
		to roll pages around (left4K + 16K + right4K).
		This would help with ASCII lines less than 1K too!

		It would NOT be nice to copydown each individual byte,
		_BUT_ the other half of the process should make progress
		in 1K chunks or so, (not if text processing lines,
		but see CSV line parser which repeats ove multiline)

	NUDGE

		when the output buffer is empty the subscription is cancelled
		so after writing a transaction, to out_buff, call write_some()

		when data is read into the in_buff, ssl_says_read() 
		when the in_buff is full, the subscription is cancelled.
		application MUST notice this and know to call read_some()



	NUDGE = tell SSL to write_some() if it can
		ssl_says_output_full(); //
		ssl_says_output_empty(); //

	SSL will de-subscribe read, when the buffer is full
	SSL will de-subscribe write, when the buffer is full

	SHARED COPY THROUGH BUFFER

		eg like stunnel reads plain TCP converts to SSL
		the read() can go _DIRECTLY_ to the out_buf, _BUT_ LOCKS!

		In particular, the app might scan the data looking for embedded
		controls, or parsing the frames for some FILTER, with the
		optimistic theory that most packets are best placed there.
		Then copy out, manipulate, copy back on occasional need.

		For this to happen there must be a LOCK on the out_buff

			app_lock_out_buff_start() # by app
			app_lock_out_buff_stop()

		For reading, the lock only needs to lock the start address
		so that the data doesnt move. It can be parsed from in_buff.
		
*/

	bool flush();

	bool get_peer_cert();

	void poll_for_state_change();

	void register_info_cb();
};

class fd_sel_ssl_client : public fd_sel_ssl
{
 public:
	fd_sel_ssl_client()
	: fd_sel_ssl(true)
	{
	}
	virtual bool class_is_client() { return true; }
};


class fd_sel_ssl_server : public fd_sel_ssl
{
 public:
	fd_sel_ssl_server()
	: fd_sel_ssl(false)
	{
	}
	virtual bool class_is_client() { return false; }
};


#endif
