#ifndef proto_parse_base_H
#define proto_parse_base_H

#include "buffer3.h"
#include "word_splitter.h"
//#include "fd_sel_ssl.h"
#include "fd_sel_buf.h"
#include "str2.h"
#include "ht2.h"

class proto_parse_base;

// could move into class for NAMESPACE reasons?

bool check_varname_for_legal_chars( const str0 & s );
bool check_cmd_for_legal_chars( const str0 & s );

/*
	processing the input (script) happens in phases
	LEX splitting of WORDS LINE(s) CMNT
		this must be generic
	PSG scanning of ARGV of WORDS + CMNT
		this can be chosen to ne specific to cmd_spec["argv[0]"]
		this can upgrade any word to
		 - word is varname
		 - word is var.name in style of EXPR
		 - word is "quoted" or {BLOB64}
		 - word is {CODEBLOCK(lang)}
		 - word is cmdword
		 - word is {PAIRS(name,value,cmnt,linepos)}
		 - word is $VAR or bind(varname,STBL_binding)
		cmd_spec helps in the LEX/PSG for ARGV starting with cmd
		 - common approach for decode word[idx] as var.name
	RUN evaluates an ARGV
		 - get current bindings for variables
		 - get access to RELAY1 top object
		 - get access to return fd_sel_buf to REPLY
		 - give REPLY out of band
		cmd_spec helps by calling correct functions, holding closure

	The proto_parse_base does the generic stuff
	The proto_parse_plus bundles a lot of stuff together
	The cmd_spec (created by request of _plus) does the unique work
	The cmd_spec gets the RELAY object from _plus
	_plus does not do that much (and stays as _base, most of the time)
	(Slight waste of an extra pointer or two, but middle is generic)
		 
*/

/*!
	This is a registered protocol command spec
*/
class proto_cmd_spec : public obj_ref0
{
 public:
	//! The cmd name
	obj_hold<str2> cmd;

	//! some commands only have a fixed number of args
	int nargs_exact; // incl cmd, eg set var val is 3. 0|-1 means ANY
	bool varname_can_be_split; // calling context holds the split value
	bool template_builtin; // FLUSH,  COPY_MODE, REPLY, GET, SET, ...
	bool arg_one_is_varname;

	bool template_GETCMD_objname; // noopts - yet, nargs==2
	bool template_SETCMD_objname_value; // noopts - yet, nargs==3
	bool template_CMD_tail; // no varname usually single CMD1

	bool template_TYPE_obj_CMD_extra; // noopts nargs==3+ // sub cmd lookup
	bool template_TYPE_obj_SET_blob; // noopts nargs==4+
	bool template_GETFIELD_obj_fieldname;
	bool template_SETFIELD_obj_fieldname_value;
	bool value_is_always_base64;
	// bool trim_leading_blank_lines;
	// bool trim_trailing_blank_lines;
	// bool trim_surrounding_blank_space;;
	// bool indent_unindent;;;
	// nonsense - use base64 if it matters, subsequent parse sorts it out
	//
//	bool value_is_sensitive; // SCRUB vars after use
//	bool value_needs_trust_check; // carries an extra flag (2) , do check it

	const char * str_usage;	// must be static str, most common usage

	~proto_cmd_spec();
	proto_cmd_spec(
		const char * _cmd
	);
	bool init0();

	virtual bool run( proto_parse_base & calling );
};

/*
	The vocabulary of this parser is very simple, almost fixed

	If the last char of a line is { it starts a multiline text }

		SETSTR varname "simplestr"
		SETPEM varname {
			indented
			multiline
			PEMtext
		}
		SETBLOB varname {
			indented
			base64
			data
		}

		SET varname "quotedstring" # parser removes quotes single line
		STR varname "quotedstring"
		BLOB varname {
			...	// base64 encoded in this TEXT transit only
		}
		TEXT varname {
			...	// UTF8 CRLF encoded only
		}
		PEM varname {
			...	// UTF8 CRLF encoded only
			same as text but might PEM check it?
		}

		BLOB (and others) receive the TEXT converted from CRLF to LF
		and must then do its own BASE64 decoding (or fail)
		comments within TEXT will cause problems here

		RSAPAIR varname PUB {
		-blob64-
		} PRIV {
		-blob64-
		}
		# PEM and LIBRS take advantage of the fact that the
		# client_CERT hold the public part
		# client_KEY holds the private part

		Transmit RSA as BLOB if that is what it is

		DATE varname "str"

		GET varname

		REPLY simple words only

		login

		connection one vnc 127.0.0.1 5901 
		connect one

		REPLY OK connect one
		REPLY FAIL connect one
		REPLY FAIL connection one 127.0.0.1 5901 - NOT ALLOWED!
		REPLY WARN connection one 127.0.0.1 5901 - curr offline
		connection puppy vnc 127.0.0.1 5910 # VNC connection not SMB!
		connect puppy
		REPLY FAIL "connect puppy" 'no route to host"
		COPY_MODE_READY
		# OOB message BREAK #
		# INL MESSAGE <EMPTY> - not yet, get a proper MUX OK
		

		connection_one
		pick_connection one 127.0.0.1 5901
		set connection.one.host 127.0.0.1
		set connection.one.port.5901
		check connection one 
		connection one {
			host 127.0.0.1
			port 5901
		}
		connection_one 127.0.0.1 5901 # validate by values
		connection_one some_name

		FLUSH
		FLUSH_ACK

		COPY_MODE_READY
		COPY_MODE_READY_ACK
		COPY_MODE
		COPY_MODE_ACK

		Since we have the SSL input buffer, we can peek
		if there is a COPY_MODE.* text we can see it
		if there is a VNC byte sequence we can see it ..

			FLUSH_FOR_COPY_MODE
			FLUSH_ACK
			COPY_MODE
		


-- NOT YET DOING --

	login {
	 airhub myhub
	 user me
	 pass secret
	 chap ...
	 desktop vncdefault
	 desktop 
	}

*/

class proto_parse_base : public obj_ref0
{
 public:

	~proto_parse_base();
	proto_parse_base( fd_sel_buf & fd_sel );

	/*
		proto only makes sense in line mode,
		but it does switch into copy mode
		(and possibly back)

		The caller should really notice,
		but proto must also notice,
		when it is not in line_mode.

		copy_mode (not line mode) is for binary copy
		eg HTTP uses a NN-BYTES blob then returns to line_mode
		The HTTP blank line is a part of the protocol
		(errm - add blank-line-detector convenience func here!)
	*/
	bool is_line_mode;

	/*
		io_buffered will usually be an SSL with a buffer

		but that is not a base class (is it server or client).

		You can put back any unused data (circ buffer3 in_buff)

		This API to the parser, has a derived class for actions.
		Or at least prepacking options for the elay to do.

		In future, with some VAR_POOL and attribues,
		the API will automatically be able to answer questions.
		For now it is read-only, mostly
	*/
	fd_sel_buf & io_buffered;

	bool nudge_write_some();

	/*
		When there is a packet boundry break,
		the word_splitter falls back to the start
		if the incomplete (multi-)line.

		Every time we get some more input,
		we can simply ask word_Splitter to have another look,
		BUT this might be a resource drain if we are being
		fed a byte at a time, over all interfaces,
		with long delays (to annoy swap pages).

		Good applications should only jitter every 1340 bytes,
		and only once max per line (we are not transferring
		large blobs, just PEMS and things).

		So count how mant times we have stopped, mid way.
	*/
	int stopped_mid_line;

	word_splitter splitter; // splits line(s) into words, retain buffer

	typedef ht2<str2, proto_cmd_spec> ht_cmd_t;
	ht_cmd_t ht_cmd;
	bool add_cmd( proto_cmd_spec * cmd_spec );
	bool del_cmd( const char * cmd_name );

	/*
		errm ... is this things I am waiting for,
		or things I am doing, that others should wait for ...

		As a generalisation, we do not track each waiting item _HERE_,
		(maybe we should do), but when the remote wants us to be idle,
		we should not have any outstanding things remaining undone.

		The FLUSH that the remote sends through us,
		causes us to process all our queues,
		complete all tasks,
		send ACKS to all ENQ's (where we can - now is the time)

		The general TRUST environment (which is still _SAFE_,
		if a question is not answered, a FAIL will eventually happen,
		we just dont want to FAIL because of lazy evaluation),
		and the general work environment, is that we reply
		to questions ASAP, but leave the output buffered, unsent.

		Ties when we cannot complete the work,
		(but could resume when a reply happens),
		is when we are waiting for a reply from ... ANYONE ...

		eg after open_TCP_async() we are waiting for an external
		thing (the network) to reply with a deferred: no-route-to-svc
		
		eg if the REMOTE holds seomthing that we want,
		we send a GET and wait for N replies.

		If we have received N-GETS - we process each of them
		immediately, excep those we cannot.

		Maybe these should be associated with a variable, as a lock,
		with the string for explaining/debugging, not matching.
		The string is nice, but unused (currently)

	USAGE

		FLUSH
			it tries to wait for a minimal time
		FLUSH_FAIL current state is not going to change soon
		FLUSH_PASS
 ***			can now switch mode, no replies will happen

	OK so that is still _ME_ where the answer can be the remote,
	or the network, or other delayed, but it might cause a reply,
	which would break the line_mode/copy_mode protocol

	SO
		for now simply answer everything immediately
		if you cant, waiting_more("for-what");

	*/
	int n_queries_I_am_processing_aynchronously;
	virtual void waiting_init();
	virtual void waiting_more(const char * cmnt);
	virtual void waiting_less(const char * cmnt);
	virtual bool waiting_flush();
	virtual bool is_waiting();

	bool process_some_input(); // take it from io_buffered

	bool check_nargs_is_exact( int n );	// assert else fail
	bool check_arg_idx_is_valid( int idx );	// assert else fail
	bool arg_idx_is_varname(int idx);	// declare it so
	bool arg_idx_is_value(int idx);		// declare it so

	proto_cmd_spec * cmd_spec;
//	str0 cmd_name;
//	str0 var_name;
//	str0 value;
	buffer2 value_blob;
	str0 var_name;
	str0 var_name_left;
	buffer2 var_name_left_buf;
	str0 var_name_right;

	/*
		proto calls itself to run what it has parsed

		at the base level, it parses well known words like "set"
	*/
	virtual bool dump_FAILED_cmd( const char * msg = NULL );
	bool dump_FAILED_cmd_called;
	virtual bool run_argv_base();

	/*
		each derived class should handle its own variables
		but the command { set var val } is almost builtin
		(if not, dont set template_SETCMD_objname_value = false;)

		If still in any doubt, call cmd_spec->run()
	*/

		// RUN_SET_VAR_VAL() // or not //

	/*
		send_CMD_varname_blob(...)

			note that the proto_parse_{base,plus,...}
			do not handle set themselves, but do send_SET

			holding cmd/ var_name/ var_value_blob/ is optional

		For convenience, and systematic quoting CRLF etc
		call these to send commands over the line.
		There is less checking (and use of const char *)
		because of the fact that our code is calling these

		Even so, we want CRLF from LF on PEMs?
		Or accept either, ie allow CRLF prefere LF NOT CR
		(sorry MAC)

		IE some more typing to do, to get the full effect
	*/

	bool send_CMD_str( const char * cmd, str0  );

	bool send_CMD_varname( const char * cmd, str0 varname );
	bool send_CMD_varname_str( const char * cmd, str0 varname, blk1 & value );
	bool send_CMD_varname_text( const char * cmd, str0 varname, blk1 & value );
	bool send_CMD_varname_blob( const char * cmd, str0 varname, blk1 & value );

	bool send_CMD_varname_str( const char * cmd, str0 varname, str0 value );
	bool send_CMD_varname_text( const char * cmd, str0 varname, str0 value );
	bool send_CMD_varname_blob( const char * cmd, str0 varname, str0 value );


	bool send_GET_varname( const char * cmd, str0 varname );
	bool send_SET_varname_str( const char * cmd, str0 varname, blk1 & value );
	bool send_SET_varname_text( const char * cmd, str0 varname, blk1 & value );
	bool send_SET_varname_blob( const char * cmd, str0 varname, blk1 & value );

	bool send_SET_varname_str( const char * cmd, str0 varname, str0 value );
	bool send_SET_varname_text( const char * cmd, str0 varname, str0 value );
	bool send_SET_varname_blob( const char * cmd, str0 varname, str0 value );

	bool prep_CMD( const char * cmd ); // leave line open
	bool prep_ARG_varname( str0 varname ); // noquotes (no lookup)
	bool prep_ARG_keyword( str0 keyword ); // noquotes (no lookup)
	bool prep_ARG_Q1_str( str0 valstr ); // 'valstr'
	bool prep_ARG_Q2_str( str0 valstr ); // "valstr"
	bool prep_ARG_TEXT( str0 valtext ); // { MULTI LINE TEXT }
	bool prep_ARG_TEXT( blk1 valtext ); // { MULTI LINE TEXT }
	bool prep_ARG_blob( str0 valstr ); // { BASE64 }
	bool prep_ARG_blob( blk1 & blk ); // { BASE64 }
	bool prep_EOLN(); // CRLF

	bool send_CMD(
		const char * cmd
	);
	bool send_CMD_KEYWORD(
		const char * cmd,
		const char * keyword
	);
	bool send_CMD_KEYWORD_STRING(
		const char * cmd,
		const char * keyword,
		str0 strval
	);

	// these are really for the derived classes to do
	bool send_FLUSH_kwd( const char * kwd); // priv
	bool send_FLUSH_REQUEST();
	bool send_FLUSH_PASS();
	bool send_FLUSH_FAIL();
	bool send_FLUSH_BUSY();

	/*
		USAGE:

			class cmd_spec_SET_MYAPP : public cmd_spec_SET

				store another pointer to something useful
				use that in run(calling)

			class proto_parse_MYAPP : public proto_parse_base ...
				
				add_cmd( new cmd_spec_SET_MYAPP( that, this ));

			Do not add much to this middle zone,
			except setting up the cmd_spec leaves (individuals)
			That saves having to cast, or use <template> with cast
	*/

};


#endif

