#ifndef proto_parse_one_H
#define proto_parse_one_H

#include "proto_parse_base.h"

/////////////////////////////////////////////////////////////////

/*!
	proto_parse_base = WORDS
	proto_parse_one = GET SET FLUSH // todo PASS FAIL REPLY ... // empty
	proto_parse_two = CONNECT LOGIN LOGIN_FAIL ... (esp send)
	proto_parse_three = proto_parse_on_hs_from_gw (in .two)

	-CURRENTLY-

	all in proto_parse_base, except min in proto_parse_on_hs_from_gw (+X)
	two is in in proto_parse_on_hs_from_gw
	two done as cmd_spec

	-PLUS-

	cmd_spec_* appear at various levels

	-BUT-

	spelling of LOGIN_FAIL is not clearly positioned

	-MAYBE- 

	more responsibility (or less?) in cmd_spec 
*/
class proto_parse_one : public proto_parse_base
{
public:
// 	// introduces waiting_plus()
// 	// but that does sit will in _base, albeit unused
// 	// keeping it there allows use of base& as a VTL name
// 	//
// 	// adds FLUSH family

	proto_parse_one( fd_sel_buf & io);
//	: proto_parse_base( io )
//	{
//	}
//
   //	bool send_CONNECT_srv_name( const char * SRV, str0 SERVNAME );
//	{
//		return send_CMD_KEYWORD_STRING( "CONNECT", SRV, SERVNAME );
//	}

};

	/////////////////////////////////////////////////////////

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
class proto_cmd_spec_FLUSH : public proto_cmd_spec
{
 public:
	proto_cmd_spec_FLUSH( const char * _FLUSH = "FLUSH" );
	bool run( proto_parse_base & proto_ );
	virtual bool got_FLUSH_REQUEST();
	virtual bool got_FLUSH_PASS();
	virtual bool got_FLUSH_BUSY();
	virtual bool got_FLUSH_FAIL();
};

class proto_cmd_spec_GET : public proto_cmd_spec
{
 public:
	proto_cmd_spec_GET( const char * _GET = "GET" );
	bool run( proto_parse_base & parser );

};

class proto_cmd_spec_SET : public proto_cmd_spec
{
 public:
	proto_cmd_spec_SET( const char * _SET = "SET" );
	bool run( proto_parse_base & parser );
};
#endif
