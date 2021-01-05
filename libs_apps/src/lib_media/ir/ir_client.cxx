
#include <errno.h>
#include "obj_ref.h"
#include "obj_hold.h"

#include "fd_sel_line.h"
#include "ir_client.h"
#include "e_print.h"
// #include "misc.h" // sleep_10
#include "sleep_ms.h" // sleep_10
#include "CR_LF_NUL.h"

/*
	send line like rc (except LIST is simpler)

normal response:

	BEGIN
	SEND_ONCE DI4001N Select
	SUCCESS
	END

at any time between packets get:

	BEGIN
	SIGHUP
	END

response from LIST 

	BEGIN
	LIST
	SUCCESS
	DATA
	4
	DI4001N
	UMS9V
	LS
	SV-651B
	END

response from LIST DI4001N

	BEGIN
	LIST DI4001N
	SUCCESS
	DATA
	38
	0000000000001281 1
	0000000000001282 2
	0000000000001283 3
	...
	00000000000002a1 Interactive
	000000000000128c Standby
	END



at any time between packets get: HEX repeat button remote
A simple command word prefix would have been nice

	00000000a0a0a857 00 stop SV-651B

error report:

	BEGIN
	NAFFCMD DI4001N Select
	ERROR
	DATA
	1
	unknown directive: "NAFFCMD"
	END

error report:

	BEGIN
	SEND_ONCE noremote Select
	ERROR
	DATA
	1
	unknown remote: "noremote"
	END

error report:

	BEGIN
	SEND_ONCE DI4001N BadName
	ERROR
	DATA
	1
	unknown command: "BadName"
	END

Its only worth WAITING for responses to check the button names.
The IR transmitter has no idea if the signal was received or not.
2 or 3-digit signals might as well be sent queued up, and simply
put up an error for any reported error.

However, if it did make sense, fd_selectable would need an on-idle event.

*/

	
	/*!
		constructor - connect to /dev/lircd
	*/
	IR_client::IR_client( str0 DEV_LIRCD )
	: fd_sel_line()
	, dev_lircd( DEV_LIRCD )
	, resp_cmd_line( 30 )
	, resp_data( 30 )
	, buf1( 30 )
	{
		reconnect();
	}

	/*!
		functions call this before proceeding. If the connection
		is OPEN return true, else try reconnect() which returns
		TRUE/FALSE;

		If the connection later fails (eg server goes away),
		the call-back system automatically sets fd=-1

		This is how the client application reconnects to
		a new lircd when the old one is respawned.
	*/
	bool IR_client::check_connected()
	{
		if( is_open() ) return true;
		return reconnect();
	}

	/*!
		Reconnect to the lircd server through /dev/lircd
		(saved from constructor), and reset the state.
	*/
	bool IR_client::reconnect()
	{
		open_AF_UNIX_async( dev_lircd ); // closes any open
		pre_BEGIN();
		if(!is_open()) return false;
		subscribe_each(); // to select
		// set_line_mode(); // already set
		// DEBUGGING // test1();
		return true;
	}

	/*!
		re-init parser vars ready for BEGIN respone
	*/
	void IR_client::pre_BEGIN()
	{
		resp_cmd.clear();
		resp_cmd_line.clear();
		resp_data.clear();
		word_list.clear();
		waiting_for = W_BEGIN;
		msg_type = M_NONE;
		N_DATA_expected = 0;
		N_DATA_received = 0;
		status_ok = true;
	}

	/*!
		send a single button to remote
	*/
	bool IR_client::req_SEND_ONCE( str0 rem, str0 btn )
	{
		if(!check_connected() ) return false;
		buf1.clear();
		buf1.put( "SEND_ONCE ");
		buf1.put( rem );
		buf1.put( " " );
		buf1.put( btn );
		buf1.put( "\n" );
		write( buf1 );
		return true;
	}

	/*!
		start repeating button
	*/
	bool IR_client::req_SEND_START( str0 rem, str0 btn )
	{
		if(!check_connected() ) return false;
		buf1.clear();
		buf1.put( "SEND_START ");
		buf1.put( rem );
		buf1.put( " " );
		buf1.put( btn );
		buf1.put( "\n" );
		write( buf1 );
		return true;
	}

	/*!
		stop repeating button
	*/
	bool IR_client::req_SEND_STOP( str0 rem, str0 btn )
	{
		if(!check_connected() ) return false;
		buf1.clear();
		buf1.put( "SEND_STOP ");
		buf1.put( rem );
		buf1.put( " " );
		buf1.put( btn );
		buf1.put( "\n" );
		write( buf1 );
		return true;
	}

	/*!
		request a list of all remotes aynchronously
	*/
	bool IR_client::req_LIST_remotes() // and tell who ??
	{
		if(!check_connected() ) return false;
		buf1.clear();
		buf1.put( "LIST\n");
		write( buf1 );
		return true;
	}

	/*!
		request a list of buttons on remote
	*/
	bool IR_client::req_LIST_buttons( str0 rem ) // and tell who ??
	{
		if(!check_connected() ) return false;
		buf1.clear();
		buf1.put( "LIST ");
		buf1.put( rem );
		buf1.put( "\n" );
		write( buf1 );
		return true;
	}

	/*!
		error in reponse parser
	*/
	void IR_client::goto_recovery_mode( str0 line, str0 error )
	{
		e_print("# IR # goto_recovery_mode(%s,%s)\n", (STR0) line, (STR0)  error );
		got_recovery();
		pre_BEGIN();
		waiting_for = W_RECOVERY;
		got_line( line, line.str_len() ); // maybe its BEGIN or END or ...
	}

	/*!
		response line from lircd
	*/
	void IR_client::got_line( str0 line, int len )
	{
		e_print( "IR_Client got_line: %s\n", (STR0) line );

		switch( waiting_for) {

		 case W_BEGIN:
		 {
			if( line == "BEGIN" )
			{
				waiting_for = W_COMMAND;
				return;
			}

			/*
				when lircd receives an IR command
				(laptops have IR detectors), it tells
				all lirc clients, by sending a line like this:

				00000000a0a0a857 00 stop SV-651B

				I THINK the protocol says this wont
				be received in any other state, only
				waiting_for::W_BEGIN
			*/
			{
				resp_cmd.split_by_spaces( line );
				if( 4 == resp_cmd.word_list.N() )
				{
					str0 hex0 = resp_cmd.word_list[0];
					str0 cnt1 = resp_cmd.word_list[1];
					str0 btn2 = resp_cmd.word_list[2];
					str0 rem3 = resp_cmd.word_list[3];
					got_input_button( hex0, cnt1, btn2, rem3 );
					return;
				}
		
			}
			/*
				otherwise its an error
			*/
			e_print("IS THIS AN INPUT BROADCAST?\n");
			goto_recovery_mode(line,"BEGIN expected");
			return;
		 }

		 case W_COMMAND:
		 {
			resp_cmd_line.clear();
			resp_cmd_line.append( line );
			waiting_for = W_STATUS;

			resp_cmd.split_by_spaces( line );
			str0 cmd0 = resp_cmd.word_list[0];

			if( cmd0 == "SIGHUP" )
			{
				msg_type = M_SIGHUP;
				waiting_for = W_END;
				return;
			}

			if( cmd0 == "SEND_ONCE" )
			{
				msg_type = M_SEND_ONCE;
				return;
			}

			if( cmd0 == "SEND_START" )
			{
				msg_type = M_SEND_START;
				return;
			}

			if( cmd0 == "SEND_STOP" )
			{
				msg_type = M_SEND_STOP;
				return;
			}

			if( cmd0 == "LIST" )
			{
				if( 1 == resp_cmd.word_list.N() )
				{
					msg_type = M_LIST_REMOTES;
					return;
				}

				str0 rem = resp_cmd.word_list[1];

				if( 2 == resp_cmd.word_list.N() )
				{
					msg_type = M_LIST_BUTTONS;
					return;
				}

				if( 3 == resp_cmd.word_list.N() )
				{
					msg_type = M_LIST_REMOTE_BUTTON;
					return;
				}

				goto_recovery_mode(line,"Bad LIST command format");
				return;
			}
	//		goto_recovery_mode(line,"Bad command name in reponse");
			return;
		 }

		 case W_STATUS:

			if( line == "SUCCESS" )
			{
				waiting_for = W_DATA; // or END
				return;
			}
			if( line == "ERROR" )
			{
				status_ok = false;
				waiting_for = W_DATA; // or END
				return;
			}
			goto_recovery_mode(line,"STATUS expected");
			return;

		 case W_DATA:

			if( line == "END" )
			{
				waiting_for = W_END;
				got_line( line, len );
				// that prints got_line(END) twice - but who cares
				return;
			}
			if( line == "DATA" )
			{
				waiting_for = W_N;
				return;
			}
			goto_recovery_mode(line,"END|DATA expected");
			return;

		 case W_N:

			if( line.as_int( N_DATA_expected ) )
			{
				waiting_for = W_DATA_LINE;
				return;
			}
			goto_recovery_mode(line,"N expected");
			return;

		 case W_DATA_LINE:

			// paranoia - no remote nor button may have these names
			if( ( line == "BEGIN" )
			 || ( line == "SUCCESS" )
			 || ( line == "ERROR" )
			 || ( line == "DATA" )
			 || ( line == "SIGHUP" )
			 || ( line == "END" )
			) {
				goto_recovery_mode(line,"DATA_LINE was keyword");
				return;
			}

			// accumulate results text, sep. using CR instead of space
			// could maybe have a list of words, or split later

			if( resp_data.isnt_empty() )
				resp_data.append( '\r' );
			resp_data.append( line );

			N_DATA_received++;
			if( N_DATA_received == N_DATA_expected )
			{
				waiting_for = W_END;
			}
			// stay in W_DATA_LINE mode
			return;

		 case W_END:
		 {
			if( line != "END" )
			{
				goto_recovery_mode(line,"END expected");
				return;
			}
			if(! status_ok )
			{
				e_print("# FAIL # IR: %s\n", (STR0) resp_data.get() );
				got_bad_something();
				pre_BEGIN();
				return;
			}
			/*
				END received and status_ok
				if data was requested, its now here, 
				depending on what M_msg_type is being echoed back
				eg: call got_remotes() with the list of remotes
				reset machine for next response: pre_BEGIN()
			*/
			switch( msg_type ) {
			 case M_NONE:
				// this should never happen - report what?
			 break;
			 case M_LIST_REMOTES:
				got_remotes();
			 break;
			 case M_LIST_BUTTONS:
				got_buttons();
			 break;
			 case M_LIST_REMOTE_BUTTON:
				got_remote_button();
			 break;
			 case M_SIGHUP:
				got_sighup();
			 break;
			 case M_SEND_ONCE:
				got_success();
			 break;
			 case M_SEND_START:
				got_success();
			 break;
			 case M_SEND_STOP:
				got_success();
			 break;
			 case M_RECEIVED:
				got_received();
			 break;
			 default:
				// is this posible?
				e_print("# IR # Maybe report success/fail\n");
				e_print("# IR # %s\n", resp_data.get() );
			}
			pre_BEGIN();
			return;
		 }

		 case W_RECOVERY:
		 {
			if( line == "END" )
			{
				e_print("RECOVERY: END FOUND \n");
				waiting_for = W_BEGIN;
				return;
			}
			if( line == "BEGIN" )
			{
				e_print("RECOVERY: BEGIN FOUND \n");
				waiting_for = W_COMMAND;
				return;
			}
			e_print("# IR # RECOVERY: STILL WAITING FOR END\n");
			return;
		 }

		} // switch
		fflush(0);
	}

	/*!
		lircd has disconnected - 
	*/
	void IR_client::got_eof()
	{
		e_print("# IR # got_eof() from fd\n");
	}

	/*!
		fd_sel_line is never in data mode, only text (virtual fn)
	*/
	void IR_client::got_data(void*, int)
	{
		e_print("# IR # got_data() from fd -BUT LINE MODE\n");
	}

	/*!
		fd_sel_line virtual function
	*/
	void IR_client::got_error()
	{
		e_print("# IR # got_error() from fd\n");
	}

	/*!
		debugging test
	*/
	void IR_client::test1()
	{
		const char * remote = "DI4001N";
		const char * button = "Select"; // not harmless - send twice
		const char * command_bad = "BadCommand";
		const char * remote_bad = "BadRemote";
		const char * button_bad = "BadButton";

 		req_LIST_remotes();
 		req_LIST_buttons( remote );

 		print( "%s %s %s\n", command_bad, remote, button );
		fflush(0);
 		req_SEND_ONCE( remote,     button_bad );
 		req_SEND_ONCE( remote_bad, button );
 		req_SEND_ONCE( remote_bad, button_bad );
 		req_SEND_ONCE( remote,     "1 0 1" ); // must send each indiv
 		req_SEND_ONCE( remote,     button );
 		req_SEND_ONCE( remote,     button );
	}

// VIRTUAL
	/*!
		The IR receiver (laptops have them) and lirc daemon
		received an IR-BTN press, recognised by a config mapping file.

		It is reported, via got_line(), which calls this virtual.
		Data is something like:

		00000000a0a0a857 00 stop SV-651B

		Your derived sub-class should put its own action here
	*/
	void IR_client::got_input_button(
		str0 hex,
		str0 count,
		str0 button,
		str0 remote
	)
	{
		e_print("# IR # input # %s %s %s %s\n",
			(STR0) hex,
			(STR0) count,
			(STR0) button,
			(STR0) remote
		);
	}

	/*!
		split "1 0 1" into three SEND_ONCE commands
		allow aliases for remotes
		fixup strange behavior on VCR record button needing repeat

		NB req_SEND_START(rem,btn) doesnt do any fixups at all
	*/
	void IR_client::send_commands( str0 remote, str0 buttons )
	{
		str0 remote2;
		if     ( remote == "FV"       ) remote2 = "DS608P";
		else if( remote == "CABLE"    ) remote2 = "DI4001N";
		else if( remote == "VCR"      ) remote2 = "SV-651B";
		else if( remote == "RADIO"    ) remote2 = "UMS9V";
		else                            remote2 =  remote;

		if( remote2 == "SV-651B" )
		{
			if( buttons == "RECORD_PAUSE" )
			{
			/*
				SEND_ONCE RECORD doesnt work - this does
				Should really trap 'RECORD' but thats not on the menu!
				I Only switch RECORD on into PAUSE mode
			*/
				req_SEND_START( remote2, "RECORD" );
				sleep_10( 3 );
				req_SEND_STOP( remote2, "RECORD" );
				sleep_10( 3 );
				req_SEND_ONCE( remote2, "play_pause" );
				return;
			}
		}

		uchar buf[ buttons.str_len() + 10 ];
		const uchar * p1 = buttons;
//		const uchar NUL = 0;

		while( *p1 )
		{
			uchar * p2 = buf;
			while( *p1 > ' ' )
			{
				*p2++ = *p1++;
			}
			while( *p1 && ( *p1 <= ' ' )) p1 ++;
			*p2 = NUL;
			req_SEND_ONCE( remote2, (str0) (char *)buf );
		}

	}

// VIRTUAL - defaults for standard vectors

	void IR_client::got_sighup()
	{
		/*!
			The server has disappeared, but this function
			is never reached, not sure why, but it doesnt
			seem to matter. fd==-1 does happen
		*/
		e_print("got_sighup();\n");
	}

	void IR_client::got_success()
	{
		e_print("got_success(%s);\n", resp_cmd_line.get() );
	}

	void IR_client::got_bad_something()
	{
		e_print("got_bad_something();\n");
	}

	void IR_client::got_bad_directive()
	{
		got_bad_something();
	}

	void IR_client::got_bad_remote()
	{
		got_bad_something();
	}

	void IR_client::got_bad_button()
	{
		got_bad_something();
	}

	void IR_client::got_bad_other()
	{
		got_bad_something();
	}

	// -- VIRTUALS --

	void IR_client::got_remotes()
	{
		e_print("got_remotes();\n");
	}

	void IR_client::got_buttons()
	{
		e_print("got_buttons();\n");
	}

	void IR_client::got_remote_button()
	{
		e_print("got_remote_buttons();\n");
	}

	void IR_client::got_received()
	{
		e_print("got_received();\n");
	}

	void IR_client::got_recovery()
	{
		e_print("got_recovery();\n");
	}


