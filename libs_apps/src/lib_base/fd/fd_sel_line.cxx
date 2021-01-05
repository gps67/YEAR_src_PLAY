
#include "fd_sel_line.h"
#include "fd_selectable.h"
#include "buffer1.h"
#include "dgb.h"
// #include <stdio.h> // perror

//	virtual void got_line( str0 line, int len );
//	virtual void got_data( void * data, int len );

	fd_sel_line::fd_sel_line( int _fd )
	: fd_selectable_base( _fd )
	, part_line()
	{
		sensible_read_size = 1600; // 1548 4096 1340
	//	sensible_read_size = 1; // testing
		line_mode = true;
		/*
			protocols other than HTTP would set line1seen=true;
		*/
	}

// VIRTUAL
	/*!
		destructor - the underlying class closes the fd
	*/
	fd_sel_line::~fd_sel_line()
	{
	}

	/*!
		Switch (back) into line mode. You will have to figure out how
		to push unused data back. Line mode means each incoming line
		will be individually delivered (without CRLF) via the
		got_line( str0 line, int len ) callback - which you must provide.
	*/
	void fd_sel_line::set_line_mode()
	{
		line_mode = true;
	}

	/*!
		You can switch to binary mode, and get all the available data,
		probably on a per-packet basis, but not necessarily. That can then
		switch back to line_mode but remember to push back data.
	*/
	void fd_sel_line::set_data_mode()
	{
		line_mode = false;
	}

// VIRTUAL
#define READ_TIMES_4  4 //
	/*!
		You CAN override this if you need to, but you SHOULDN'T want to.
		It splits the data into lines and calls got_line(...), except
		when not in line mode. It strips CRLF, and accepts LF.
	*/
	bool fd_sel_line::select_says_read()
	{
		if(0) STEP("fd_sel_line::select_says_read()");
		bool read_something = false;
		/*
			keep reading until no more data,
			but only a few times, dont let one
			fd hog the whole machine
		*/
		int read_times = READ_TIMES_4;
		if( blocks_badly ) 
		{
			STEP("# FD # blocks badly - reducing read count to 1");
			read_times = 1;
		}
	//	else if(0) INFO("# FD # blocks properly - read count is %d", read_times );
		for( int retry=0; retry<read_times; retry++ )
		{
			if(0) INFO("fd_sel_line::select_says_read() retry=%d", retry);
			if(!part_line.get_space( sensible_read_size + 1 )) return false;
			uchar * p2 = part_line.gap_addr();
			int max = part_line.space_avail() - 1;
			if( max <= 0 ) break; // never happens?

			int len = 0;
			if( read_something )
				len = read_quiet( p2, max );
			else
				len = read( p2, max );
			if( len <= 0 ) break;
			read_something = true;
			part_line.nbytes_used += len;

			uchar * p1 = part_line.buff;
			uchar * p6 = part_line.gap_addr();
			// Always NUL terminated - even data
			*p6 = 0;

			/*
				With the amount of data currently in buffer,
				split it into lines and call got_line()
				any number of times. Only call copy_down
				at the end, when theres a part_line left

				P1	- start of buffered line
					- no newline 
				P2	- oldgap, newdata
					- newdata here
				P3	- temp P4
				P4	- NUL where CR or LF was
					NUL, CR, LF
				P5	- start of next line
				P6	- end of new data, gap
			*/
			while( p1 < p6 )
			{
				if(!line_mode) {
					/*
						if mode changed in loop
						now STUCK in binary mode, send all
					*/
					got_data( p1, p6-p1 );
					part_line.clear();
					p1 = p6 = part_line.buff;
					break;
				}

				/*
					start search for newline from P2 (not P1)
					because [P1[P2 doesnt contain NL
				*/
				uchar * p4 = (uchar *)strchr( (char*)p2, '\n' );
				if( !p4 ) {
					/*
						no newline found
						part of a bigger line
						reclaim left space (copy_down)
						continue with read (x4)
					*/
					part_line.del_copy_down( p1 );
					break;
				}

				// p4 points to '\n' just past end of str
				uchar * p5 = p4 + 1; // start of next
				// possible CRLF
				uchar * p3 = p4 - 1;
				if(( p3 >= p1 ) && (*p3 == '\r'))
				{
					p4 = p3;
				}
				// OVERWRITE CR or LF with a NUL
				*p4 = 0;	// NUL TERMINATED
				int line_len = p4 - p1;

				got_line( (str0)(STR0) p1, line_len );
				// might change line_mode
				p1 = p5;
				p2 = p1;
			}
			part_line.del_copy_down( p1 );
			// continue with next read() (x4)
		}

		/*
			overall, how did you rate the service ... ?
			(and would you like to resubscribe)
		*/
		if(read_something) return subscribe_IN_on(); // true

#if 1
		errno_zero_if_read_dry();
#else
		switch( errno ) {
		 case EINTR: return true;
		 case EAGAIN: return true;
//		 case ERESTARTSYS: return true; // commented out OK doesnt exist
		}
#endif

		WARN("I used to call eof_input() here - now it is auto?");
		// call_eof_input();
		// cancel_subscription_at_select
		subscribe_IN_off();
		return false;
	}


