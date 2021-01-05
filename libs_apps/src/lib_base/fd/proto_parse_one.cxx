
#include "proto_parse_one.h"

/*
	cmd_spec is supposed to help with parsing the cmd
	not particularly with running it.
	That should then be bounced back to the
	parser_ handler (which uses these fragments)
	or the relay that owns the fd

	This is going to result in a lot of bouncing around,
	but might give some consistency.

	An alternative, would be to use template_X_Y_Z
	(and a function with the same name)
	then have a crresponding callback on the ABC class

	The parser_ class could easily be a BASECLASS for the active one,
	and use virtual functions.

	Because a lot of the work has been done before calling the VFN,
	the derived function need only do the action.
	Even expanding var_name into v.a.r step[s] can be a variable
	
*/

/*!
	FLUSH is a builtin thing really ...
	(GET and SET are semi-builtin, others are UDEF)

	--> calling_send_FLUSH_sends_all_pending_things_from_us ->
	--> FLUSH -->
	<-- calling_got_FLUSH_sends_all_pending_things_from_them <--
	<-- FLUSH_ACK_CLEAR <--

	--> calling_send_FLUSH_sends_all_pending_things_from_us ->
	--> FLUSH -->
	<-- calling_got_FLUSH_sends_all_pending_things_from_them <--
	// BUT there is something that remains unanswered
	<-- FLUSH_ACK_MUDDY <--
	// SO here cancels FLUSH (or retries after progres)

	... after 5 cycles,
	<-- FLUSH_ACK_BROKEN <--

	// FLUSH
	// FLUSH_ACK_CLEAR
	// FLUSH_ACK_FAIL	timeout, cycle in a loop, protocol mismatch, etc
	// FLUSH_ACK_MUDDY

	--> FLUSH
	<-- FLUSH_ACK_CLEAR
	--> COPY_MODE
	<-- COPY_MODE_NOW

	Somehow I should put these string into a function somewhere,
	and even negociate setting them to what the remote uses,
	or at least when the VERSION number of the remote uses
*/


proto_cmd_spec_FLUSH:: proto_cmd_spec_FLUSH( const char * _FLUSH )
: proto_cmd_spec(_FLUSH)
{
	nargs_exact = 2;
	str_usage = "FLUSH REQUEST|PASS|FAIL|BUSY";
}

bool proto_cmd_spec_FLUSH:: got_FLUSH_REQUEST()
{
	INFO("FLUSH REQUEST is being handled - builtin");
	return true;
}

bool proto_cmd_spec_FLUSH:: got_FLUSH_BUSY()
{
	WARN("BUSY is not fantastic");
	return false;
}

bool proto_cmd_spec_FLUSH:: got_FLUSH_PASS()
{
	INFO("PASS is good");
	return true;
}

bool proto_cmd_spec_FLUSH:: got_FLUSH_FAIL()
{
	return FAIL("FAIL is not good");
}

bool proto_cmd_spec_FLUSH:: run( proto_parse_base & parser_ ) {
	// clear flag 
	// parser_->tell_owner_to_flush_all_pending_replies();
	//
	// it adjusts parser_->n_queries_I_am_processing_aynchronously;
	//	waiting_more( "login hasnt happened yes" );
	//	waiting_less( "login jus failed" );
	// it adjusts parser_->count_queries_unanswered_by_me;
	// it adjusts parser_->count_queries_asked_by_me;
	// it can queue answers (out_buf) and count as done
	//
	// I answer all queries immediately, except ... 
	// ... login ... connect ... 
	//
	// either
	//	CLEAR - all done
	//	MUDDY - more to do, possibly async event pending
	//	FAIL - we are in a protocol mismatch, loop, ..

	int nargs = parser_.splitter.N();
	if( nargs != 2 ) {
		return FAIL("Wrong argc -and- should not be deteted here");
	}
	str0 arg = parser_.splitter[1];
	if( arg == "PASS" ) {
		return got_FLUSH_PASS();
	} else
	if( arg == "BUSY" ) {
		return got_FLUSH_BUSY();
	} else
	if( arg == "FAIL" ) {
		return got_FLUSH_FAIL();
	} else
	if( arg == "REQUEST" ) {
		// stay in this function
		// do still call got_FLUSH_REQUEST()
		// but at the right time
		// as a notification? 
		// or maybe add the receommended response ...
	} else {
		return parser_.dump_FAILED_cmd("expected REQUEST|PASS|FAIL|BUSY");
	}

	bool am_waiting = false;
	bool am_fail = false;

	if(!parser_.waiting_flush()) {
		am_fail = true;
	}

	if(parser_.is_waiting()) {
		am_waiting = true;
	}

	/*
		if any other serious FAIL condition set am_fail
		am_fail = true;
	*/

#warning PROTO command strings fixed to "FLUSH_ACK_MUDDY"

	// LURK - return values mean what ?
	if( am_fail ) {
		parser_.send_FLUSH_FAIL(); // argv "FLUSH" "FAIL"
	} else if( am_waiting ) {
		parser_.send_FLUSH_BUSY();
	} else {
		parser_.send_FLUSH_PASS();
	}
	parser_.nudge_write_some();
	return true;
}

proto_cmd_spec_GET:: proto_cmd_spec_GET( const char * _GET )
: proto_cmd_spec(_GET)
{
	template_GETCMD_objname = true; // unused ?
	arg_one_is_varname = true;
	nargs_exact = 2;
	str_usage = "GET varname";
}

bool proto_cmd_spec_GET:: run( proto_parse_base & parser )
{
//	word_splitter & argv = parser.splitter;
	//
	// personally, I call these client_says... server_says...
	// so that on a late night I can remember where I am
	// there is also an added var called remote
	//
	// return remote.client_says_get_var(argv[1]);
	return parser.dump_FAILED_cmd("WRITEME");
}



proto_cmd_spec_SET:: proto_cmd_spec_SET( const char * _SET )
: proto_cmd_spec(_SET)
{
	template_SETCMD_objname_value = true;
	arg_one_is_varname = true;
	nargs_exact = 3;
	str_usage = "SET varname value";
}

bool proto_cmd_spec_SET:: run( proto_parse_base & parser )
{
//	word_splitter & argv = parser.splitter;
	//
	// personally, I call these client_says... server_says...
	// so that on a late night I can remember where I am
	// there is also an added var called remote
	//
	// return remote.client_says_set_var_value(var_name, var_value );
	return parser.dump_FAILED_cmd("WRITEME");
}

///////////////////////////////////////////

/*
	the control of what is in the protocol,
	is upto the derived class to decide.
	This proto_one layer _CURRENTLY_ does no extra

	In future, a BINARY MUX PROTOCOL will have its own builtins

	Plus (TODO) exchange info about the protocol (abbreviated strings)
*/

proto_parse_one:: 
proto_parse_one( fd_sel_buf & io)
: proto_parse_base( io )
{
}

