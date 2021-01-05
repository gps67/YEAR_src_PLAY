// DTOR was needed // #pragma implementation "proto_parse_base.h"
#include "proto_parse_base.h"

#include "ASCII_chars.h"
#include "str_base64.h"

#define SPELLING_SET "SET"
#define SPELLING_GET "GET"

// FLUSH REQUEST PASS FAIL BUSY ...

proto_parse_base:: 
~proto_parse_base()
{
}

proto_parse_base:: 
proto_parse_base( fd_sel_buf & fd_sel )
: is_line_mode( false ) // not yet switched on (but would be a better default)
, io_buffered( fd_sel )
, cmd_spec( NULL )
{
	dump_FAILED_cmd_called = false;
	waiting_init(); // n_queries_I_am_processing_aynchronously = 0; 
	splitter.remove_newlines_added_with_curlies = true;
}

/*
	I dont need this .. this belongs to fd_sel_buf or buf_pair ...
*/
bool
proto_parse_base:: 
nudge_write_some()
{
	io_buffered.nudge_write_some(); // ssl_write_some
	return true;
}

// waiting has been renamed "BUSY", but it isnt clear

void
proto_parse_base:: 
waiting_init()
{
	STEP("YOUR MUDDY CODE HERE");
	n_queries_I_am_processing_aynchronously = 0;
}

void
proto_parse_base:: 
waiting_more( const char * desc )
{
	INFO("%s", desc );
	n_queries_I_am_processing_aynchronously ++; 
}

void
proto_parse_base:: 
waiting_less( const char * desc ) // should match ??
{
	INFO("%s", desc );
	n_queries_I_am_processing_aynchronously --; 
}

bool
proto_parse_base:: 
waiting_flush()
{
	STEP("NOTHING TO FLUSH");
	// simply nudge all pending things
	// in an async world, it happens when it happens
	// so do nothing until then
	// and then decr count of outstanding queries
	return true; // no errors during processing
}

bool
proto_parse_base:: 
is_waiting()
{
	return n_queries_I_am_processing_aynchronously;
}

/*!
	parse and run several lines, leaving the last half-line

	return true if a complete line was parsed

	if a line fails call du
*/
bool
proto_parse_base:: 
process_some_input() // ; // take it from io_buffered
{
	if(!is_line_mode ) {
		return FAIL("NOT LINE MODE");
	}
	bool did_something = false; // not yet
	while( is_line_mode &&
	 splitter.split_by_spaces_and_quotes_multi_line( io_buffered.in_buff ) )
	{
		did_something = true; // did parse and remove a complete line
		if(!splitter.N() ) {
			WARN("EMPTY LINE ARGV");
			continue;
		}
		if(!run_argv_base()) {
			dump_FAILED_cmd("COMMAND FAILED - continuing");
			dump_FAILED_cmd_called = false; // clear prev error
		}
	}
	return did_something;
}

bool
proto_parse_base:: 
dump_FAILED_cmd( const char * msg )
{
	if(0) gdb_invoke();
	if(!msg) msg = "FAILED";
	word_splitter & argv  = splitter; // nicer name
	if(dump_FAILED_cmd_called) {
		INFO("%s", msg );
		return false;
	}
	dump_FAILED_cmd_called = true;
	FAIL("%s - %s", (STR0) argv[0], msg);
	if(cmd_spec)
		INFO("USAGE: %s", cmd_spec->str_usage );
	int nargs = argv.N();
	buffer2 cmd_line;
	cmd_line.print( "{ '%s'", (STR0) argv[0] );
	for(int i=1; i<nargs; i++ ) {
		cmd_line.print( ", '%s'", (STR0) argv[i] );
	}
	cmd_line.print( " } # ARGV " );
	INFO("ARGV = %s", (STR0) cmd_line );
	return false;
}

/*!
	add the CMD to the proto so that it can find it later
*/
bool
proto_parse_base:: 
add_cmd( proto_cmd_spec * cmd_spec )
{
	if(!cmd_spec) {
		return FAIL("NULL cmd_spec");
	}
	ht_cmd.add( cmd_spec->cmd, cmd_spec );
	return true;
}

bool
proto_parse_base:: 
del_cmd( const char * cmd_name )
{
	INFO("remove %s", cmd_name );
	if(!ht_cmd.remove( cmd_name )) {
		return FAIL("no registered command '%s'", cmd_name );
	}
	return true;
}

/*
	The splitter has done the basic LEX work,
	in future it might retain 'quotation' "info" {etc} plainforvar
	in future each LEX argv[i] might have lots of flags
	In future, LEX will find CMNT on each line tail, and bring that too!

	Now lookup argv[0] to get cmd_spec

	It parses argv left to right, grabbing well known landmark fields,
	(more flags on each word/obj - in future maybe)

	It lex checks each argv-objv, and pre-prcesses them.
	eg: command expects BLOB as BLOB64 (to get val_blob_uudecoded)
	eg: command expects (varname) to not be in quotes, can be v.a.r.field
	eg: command expect { name1 val1 name2 val2 } all on own lines
	eg: command ...
	OK for now this is not quite true

	It parses the argv to find argv[] = { "SET", "login.user", "myname" }
	It calls cmd_spec["set"]->run( this ) which does a lot of the work
	That calls procolhandler->relay->set_login_user( "myname" )

	cmd_spec is supposed to be a template, which produces a runtime instance
*/
bool
proto_parse_base:: 
run_argv_base()
{
	word_splitter & argv  = splitter; // nicer name
	int nargs = argv.N();
	if( nargs == 0 ) {
		WARN("EMPLY LINE");
		return true;
	}
	// str0 cmd = argv[0];
	// cmd_name = NULL; // not stored - see argv[0]
	// var_name = NULL; // not currently stored
	// val and val blob are the same place
	// if it is ALWAYS BLOB64 it is already uudecoded (or soon will be)
	value_blob.clear(); // not immediately released
	var_name = NULL;
	var_name_left_buf.clear();

	if(!check_cmd_for_legal_chars( argv[0] )) {
		return dump_FAILED_cmd("illegal chars in cmd");
	}
	cmd_spec = ht_cmd.lookup( (STR0) argv[0] );
	if(!cmd_spec) {
		return dump_FAILED_cmd("NO SUCH COMMAND");
	}

	/* MAYBE:

		proto_parse_base (_plus) registers builtins
		they do not have cmd_spec, merely cmd "name" lookup

		then do not run
			cmd_spec -> run( this )
		do run
			this -> run_builtin_SET( cmd, varname, value_blob )
			this -> run_builtin_FLUSH_ACK_WHATEVER(...)

		that will then be derived, to know to tell owner of this fd,

	-- BUT:

		this is another contrived buckle

			simply remember that some cmd_spec's 

				are builting
				have special responsibilities
				need updating from time to time
				to stay in sync

	-- SO:
		FLUSH_ACK_MUDDY ...

	-- ALSO

		builtin_command
			should have an enum that matches the builting command
			(int_enum or pointer to X)
			then it could have a different name
			and be surrounded by pre-post-triggers

			Again - manually typed out

	*/

	if( cmd_spec -> template_GETCMD_objname ) {
		//
		// GET var.name		// TEST var_name // CMD varname
		//
		if(!check_nargs_is_exact(2)) return FAIL_FAILED();
		// already done // argv[0] == "get or similar"
		if(!arg_idx_is_varname(1)) return FAIL_FAILED();
        	if(cmd_spec->template_builtin) {
			return dump_FAILED_cmd("template_builtin not yet implemented");
		} else {
			return cmd_spec->run( *this );
		}
	} else if( cmd_spec -> template_SETCMD_objname_value ) {
		//
		// SET var.name avalue
		//
		if(!check_nargs_is_exact(3)) return FAIL_FAILED();
		// already done // argv[0] == "set or similar"
		if(!arg_idx_is_varname(1)) return FAIL_FAILED();
		/*
			arg_idx_is_varname( idx ) is an assertion!
			The return bool is ok,!error whilst handling

			above has split varname (if it can)
			so calling it is more than a check,
			it is required
		*/
		if(!arg_idx_is_value(2)) return FAIL_FAILED();
		// that set value_blob // either to value of uudecoded value
/*
		if( cmd_spec->varname_can_be_split ) {
			const char * left = splitter[1];
		}
*/
        	if(cmd_spec->template_builtin) {
			return dump_FAILED_cmd("template_builtin not yet implemented");
		} else {
			return cmd_spec->run( *this );
		}

	} else {
		// ok redundant check against itself AND spec
		if(!check_nargs_is_exact(cmd_spec -> nargs_exact)) return FAIL_FAILED();
		if( cmd_spec -> arg_one_is_varname ) {
			arg_idx_is_varname(1);
		}

		bool ok =  cmd_spec->run( *this );
		WARN("a bit lame to nudge every command");
		nudge_write_some();
		return ok;
	}

}

bool
proto_parse_base:: 
check_nargs_is_exact( int n )
{
	bool ok = true;
	int nargs = splitter.N();

	if(!cmd_spec) return FAIL("NULL cmd_spec"); // never happens
	int nx = cmd_spec -> nargs_exact;
	if( n <= 0 ) {
		// if( n == nx ) return true;
		if( nx <= 0 ) {
		 WARN("TODO: parser cmd_spec[cmd]->run must check nargs");
		 return true;
		}
		goto warns;
	}
	if( nx != nargs ) goto fails;
	return true;

 fails:
	ok = false;
 warns:
	if(ok) {
		WARN("n == %d nx == %d nargs == %d USAGE %s",
			n, nx, nargs, cmd_spec->str_usage );
		dump_FAILED_cmd("check_nargs_is_exact");
	} else {
		FAIL("n == %d nx == %d nargs == %d USAGE %s",
			n, nx, nargs, cmd_spec->str_usage );
		dump_FAILED_cmd("check_nargs_is_exact");
	}
	return ok;

}

bool
proto_parse_base:: 
check_arg_idx_is_valid( int idx )
{
	if((idx < 0) || ( splitter.N() <= idx )) {
		INFO("OUT OF BOUNDS idx = %d", idx );
		return dump_FAILED_cmd("idx out of bounds");
	}
	return true;
}

bool
proto_parse_base:: 
arg_idx_is_varname(int idx )
{
	if(!check_arg_idx_is_valid(idx)) return FAIL_FAILED(); // double check
	var_name = splitter[idx];
	var_name_left = NULL;
	var_name_right = var_name;
	if(cmd_spec->varname_can_be_split) {
		const char * left = (STR0) var_name;
		char * dot = (char *) strchr( left, '.' );
		if( dot ) {
			*dot = ASCII_NUL;
			var_name_left_buf = left;
			var_name_left = (STR0) var_name_left_buf;
			* dot = '.';
			const char * right = dot+1;;
			var_name_right = right;
			if(0) INFO("SPLIT( '%s', '%s' )", left, right );
		}
	}
	if(!check_varname_for_legal_chars( var_name )) {
		return dump_FAILED_cmd("illegal chars in get varname");
	}
	return true;
}

bool
proto_parse_base:: 
arg_idx_is_value(int idx )
{
	if(!check_arg_idx_is_valid(idx)) return FAIL_FAILED(); // double check
	word_splitter & argv  = splitter; // nicer name
	str_base64 conv;
	if( cmd_spec-> value_is_always_base64 ) {
		if(!conv.decode( argv[2], value_blob )) {
			dump_FAILED_cmd("arg_idx_is_value");
			return FAIL("%s %s ... uudecode failed arg[%d]",
				(STR0) argv[0], (STR0) argv[1], idx );
		}
	} else {
		if(!value_blob.set( argv[2] )) return FAIL_FAILED();
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////
//		simple senders (dont nudge yet)
////////////////////////////////////////////////////////////////////////////

bool proto_parse_base:: prep_CMD( const char * cmd )
{
	io_buffered.out_buff.print("%s", cmd );
	return true;
}

// EACH ARGS MUST ADD ITS LEFT SPACE
// TODO INDENT, conv CRLF, check no multi-line when not allowed, etc

bool proto_parse_base:: prep_ARG_varname( str0 varname )
{
	io_buffered.out_buff.print(" %s", (STR0) varname );
	return true;
}

bool proto_parse_base:: prep_ARG_keyword( str0 keyword )
{
	io_buffered.out_buff.print(" %s", (STR0) keyword );
	return true;
}

bool proto_parse_base:: prep_ARG_Q1_str( str0 valstr )
{
	io_buffered.out_buff.print(" '%s'", (STR0) valstr );
	return true;
}

bool proto_parse_base:: prep_ARG_Q2_str( str0 valstr )
{
	io_buffered.out_buff.print(" \"%s\"", (STR0) valstr );
	return true;
}

bool proto_parse_base:: prep_ARG_TEXT( str0 valtext )
{
	io_buffered.out_buff.print(" {\r\n%s\r\n}", (STR0) valtext );
	return true;
}

bool proto_parse_base:: prep_ARG_TEXT( blk1 valtext )
{
	return prep_ARG_TEXT( (str0) valtext );
	return true;
}

bool proto_parse_base:: prep_ARG_blob( blk1 & value )
{
	blk1 value_blob;
	str_base64 conv;
	conv.multi_line = true;
	conv.encode( value, value_blob );

	// LURK ...  useing same function for easy life
	return prep_ARG_TEXT( value_blob );
}

bool proto_parse_base:: prep_ARG_blob( str0 value )
{
	blk1 val;
	val.set( value );
	return prep_ARG_blob( val );
}

bool proto_parse_base:: prep_EOLN()
{
	io_buffered.out_buff.print("\r\n");
	return true;
}

bool
proto_parse_base:: 
send_CMD_str( const char * cmd, str0  s )
{
	if(!prep_CMD( cmd ) ) return FAIL_FAILED();
	if(!prep_ARG_Q1_str( s ) ) return FAIL_FAILED();
	if(!prep_EOLN() ) return FAIL_FAILED();
	return true;
}


bool
proto_parse_base:: 
send_CMD_varname( const char * cmd, str0 varname ){
	io_buffered.out_buff.print("%s %s\r\n", cmd, (STR0) varname );
	return true;
}

#warning no safe filtering of texts yet - avoid all dodgy chars!

bool
proto_parse_base:: 
send_CMD_varname_str( const char * cmd, str0 varname, blk1 & value ){
	buffer3 & b = io_buffered.out_buff;
	b.print("%s %s '%s'\r\n", cmd, (STR0) varname, (STR0) value );
	return true;
}

bool
proto_parse_base:: 
send_CMD_varname_text( const char * cmd, str0 varname, blk1 & value ){
	buffer3 & b = io_buffered.out_buff;
	b.print("%s %s {\r\n%s\r\n}\r\n", cmd, (STR0) varname, (STR0) value );
	return true;
}

bool
proto_parse_base:: 
send_CMD_varname_blob( const char * cmd, str0 varname, blk1 & value ){
	prep_CMD( cmd );
	prep_ARG_varname( varname );
	prep_ARG_blob( value );
	prep_EOLN();
	return true;
}


// str0

bool
proto_parse_base:: 
send_CMD_varname_str( const char * cmd, str0 varname, str0 value ){
	buffer3 & b = io_buffered.out_buff;
	b.print("%s %s '%s'\r\n", cmd, (STR0) varname, (STR0) value );
	return true;
}

bool
proto_parse_base:: 
send_CMD_varname_text( const char * cmd, str0 varname, str0 value ){
	buffer3 & b = io_buffered.out_buff;
	b.print("%s %s {\r\n%s\r\n}\r\n", cmd, (STR0) varname, (STR0) value );
	return true;
}

bool
proto_parse_base:: 
send_CMD_varname_blob( const char * cmd, str0 varname, str0 value ){
	prep_CMD( cmd );
	prep_ARG_varname( varname );
	prep_ARG_blob( value );
	prep_EOLN();
	return true;
}


bool
proto_parse_base:: 
send_GET_varname( const char * cmd, str0 varname ){
	// LURK hardcoded here
	if(!cmd) cmd = SPELLING_GET;
	return send_CMD_varname( cmd, varname );
}

bool
proto_parse_base:: 
send_SET_varname_str( const char * cmd, str0 varname, blk1 & value ){
	// LURK hardcoded here
	if(!cmd) cmd = SPELLING_SET;
	return send_CMD_varname_str( cmd, varname, value );
}

bool
proto_parse_base:: 
send_SET_varname_text( const char * cmd, str0 varname, blk1 & value ){
	if(!cmd) cmd = SPELLING_SET;
	return send_CMD_varname_text( cmd, varname, value );
}

bool
proto_parse_base:: 
send_SET_varname_blob( const char * cmd, str0 varname, blk1 & value ){
	if(!cmd) cmd = SPELLING_SET;
	return send_CMD_varname_blob( cmd, varname, value );
}

// allow str0 for easy calls for testing

bool
proto_parse_base:: 
send_SET_varname_str( const char * cmd, str0 varname, str0 value ){
	// LURK hardcoded here
	if(!cmd) cmd = SPELLING_SET;
	return send_CMD_varname_str( cmd, varname, value );
}

bool
proto_parse_base:: 
send_SET_varname_text( const char * cmd, str0 varname, str0 value ){
	if(!cmd) cmd = SPELLING_SET;
	return send_CMD_varname_text( cmd, varname, value );
}

bool
proto_parse_base:: 
send_SET_varname_blob( const char * cmd, str0 varname, str0 value ){
	if(!cmd) cmd = SPELLING_SET;
	return send_CMD_varname_blob( cmd, varname, value );
}


bool proto_parse_base:: send_CMD(
	const char * cmd
)
{
	prep_CMD( cmd );
	prep_EOLN();
	return true;
}

bool proto_parse_base:: send_CMD_KEYWORD_STRING(
	const char * cmd,
	const char * keyword,
	str0 strval
)
{
	prep_CMD( cmd );
	prep_ARG_keyword( keyword );
	prep_ARG_Q1_str( strval );
	prep_EOLN();
	return true;
}

bool proto_parse_base:: send_CMD_KEYWORD(
	const char * cmd,
	const char * keyword
)
{
	prep_CMD( cmd );
	prep_ARG_keyword( keyword );
	prep_EOLN();
	return true;
}

bool proto_parse_base:: send_FLUSH_kwd( const char * kwd ){
	return send_CMD_KEYWORD("FLUSH", kwd );
}
bool proto_parse_base:: send_FLUSH_REQUEST() {return send_FLUSH_kwd("REQUEST");}
bool proto_parse_base:: send_FLUSH_PASS() {return send_FLUSH_kwd("PASS");}
bool proto_parse_base:: send_FLUSH_FAIL() {return send_FLUSH_kwd("FAIL");}
bool proto_parse_base:: send_FLUSH_BUSY() {return send_FLUSH_kwd("BUSY");}

////////////////////////////////////////////////////////////////////////////
//		proto 
////////////////////////////////////////////////////////////////////////////
// proto_cmd_spec
proto_cmd_spec::
~proto_cmd_spec()
{
}

proto_cmd_spec::
proto_cmd_spec( const char * _cmd )
: cmd( new str2( _cmd ) )
, str_usage( NULL ) // must be static str, most common usage
{
	init0();
        varname_can_be_split = true; // but dont know which is varname!
	nargs_exact = 1; // incl cmd, eg set var val is 3. 0|-1 means ANY

//      bool value_is_sensitive; // SCRUB vars after use
//      bool value_needs_trust_check; // carries an extra flag (2) , do check it
}

bool 
proto_cmd_spec::
init0()
{
	nargs_exact = 0; // incl cmd, eg set var val is 3. 0|-1 means ANY
        template_builtin = false;
	arg_one_is_varname = false; // must all but they must set it
	template_GETCMD_objname = false;
        template_SETCMD_objname_value = false;
	template_CMD_tail = false; // a CMD without a varname ...

        template_TYPE_obj_CMD_extra = false;
        template_TYPE_obj_SET_blob = false;
        template_GETFIELD_obj_fieldname = false;
        template_SETFIELD_obj_fieldname_value = false;
        value_is_always_base64 = false;
        varname_can_be_split = false; // but dont know which is varname!
	return true;
}

// virtual
bool
proto_cmd_spec::
run( proto_parse_base & calling )
{
	return calling.dump_FAILED_cmd("BASE CLASS CALLED");
}
////////////////////////////////////////////////////////////////////////////
//	BUILTINS - hard to rename (easy but manjana) -- FLUSH
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//	C
////////////////////////////////////////////////////////////////////////////

#include "cset_bit_map.h"


bool check_varname_for_legal_chars( const str0 & s ){
	static bool done_init = false;
	static cset_bit_map valid_chars;
	if(!done_init) {
		done_init = true;
		valid_chars.set_null();
		valid_chars.set_range('A','Z');
		valid_chars.set_range('a','z');
		valid_chars.set_range('0','9');
		valid_chars.set_bit('_');
		valid_chars.set_bit('.');	// allow nested var.field
		// should really have CSET1 CSET2 ...
		// also ${VAR:-expr} parsing
	}
	bool ok = true;
	int bads = 0;
	u8 * P = s;
	while( u8 c1 = *P++ ) {
		if(valid_chars.get_bit( c1)) {
			continue; // its good
		}
		if(c1 <= ' ') 
			ok = FAIL("illegal ctrl char 0x%2.2d", c1);
		else if( c1 < 127 )
			ok = FAIL("illegal punct/glyph char 0x%2.2d", c1);
		else if( c1 == 127 )
			ok = FAIL("illegal DEL char 0x%2.2d", c1);
		else if(c1 <= 128 + ' ') 
			ok = FAIL("illegal HIGH ctrl char 0x%2.2d", c1);
		else 
			ok = FAIL("illegal HIGH char 0x%2.2d", c1);
		if( ++bads < 4 ) continue; else break;
	}
	return ok;
}

bool check_cmd_for_legal_chars( const str0 & s ){
	return check_varname_for_legal_chars( s ); // close enough
}
////////////////////////////////////////////////////////////////////////////

