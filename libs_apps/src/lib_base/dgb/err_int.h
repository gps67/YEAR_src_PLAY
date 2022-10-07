#ifndef err_int_H

#define err_int_H

// STUB declarations
typedef struct _GError GError;
class G_rrr; // base2 g_rrr.h
class fd_hold_1; // base2 g_rrr.h
#define fd_hold fd_hold_1

#include "buffer2.h"

/*!
	not errno - but a surround that can use errno, or more

	TODO
		enum err_from; // _zone as _enum
		int err;
	
	TODO
		_zone += UDEF1 UDEF2 UDEF3 UDEF
		_name += "LIBR_ONE" "LIBR_TWO" "LIBR_THREE"
			WIN32 sockets
			LIBR _ssl_
			LIBR _sqlite_
			LIBR _SM2_
			LIBR _py_ # or exception objects
			LIBR _tcl_ # does it have a list of codes? 
		_list_of_zone += RT_allocated_enum + RT_str1 
		IE module registers itself, .so .dll .scripted


	err == errno	+= EVENT { errno } // open(filename) OPTS

	using libc errno as source // UNIX LIBC // todo port WIN32 //

		single call sets int err; // clears as_if init_done

			buffer2 e_name; // "ENOENT"

				"ENOENT" // no such item //

			buffer2 e_zone; // "OS" "UNIX" "WIN32" "NET" "GLIB" "GTK" "SSL"

				"LIBC"	// eg open(...) on_fail RETVAL==-1 errno=errno

			buffer2 e_str;

				"No such device or address"

			buffer2 e_obj_name;// "DEFAULT-INPUT-STREAM"

				open(filename) // ARGV // DIALECT JUMP_JOIN

				# that permits incorrect prototype
				# open(filename)
				# open(filename, )
	
	WHERE IS

		err(errno)



	err_int_t is errno (on UNIX) GetLastError() on WIN32

	err_int_t is a part of ERROR_DIAGNOSTIC_STACK system

	BUG-ETTE {
		the CTOR for an on-the-stack object
		does not init0 the int err;

		the buffer2 CTOR's are called
	}

	USAGE {
		RETVAL = errno // obtain copy of curr errno GET()

		?*

			Every EXTRA_FIELD mk_%s 

			  "idx" || "fd = open(connection) # ARGV"

			  // need an immediate PRE_PARSED PARSER
			  // optimised when pre_run on_SET() NEWVAL OLDVAL CODE

			  idx_t idx = lookup( "idx" ) ; 

			  bool is_NULL_or_M1() // or other V_CALL

		*?

		int t = open( name, opts, mode ); // via MACRO
		if( t == -1 ) {
			KNOW err = errno;
			KNOW t == fd == "fd = open(filename) ARGV"
			ARGV "var{'OPTS'}"
			ARGV "var{'chmod'}"
			ARGV "var{'all_async'}" // mostly
		} else {
			KNOW t == fd == "fd = open(filename) ARGV"
		}
	}

*/
struct err_int_t
{
	int err;	// DWORD on WIN32

	buffer2 e_name;	// "ENOENT"
	buffer2 e_zone;	// "OS" "UNIX" "WIN32" "NET" "GLIB" "GTK" "SSL"
	buffer2 e_str;	// "No such device or address"
	buffer2 e_obj_name;// "DEFAULT-INPUT-STREAM"

	// function decl_zone_( %s == "LIB" ) // %s == "FT2" // 
	// obj == "SEGMENT" // N_BYTES // "%s" //

	err_int_t();
	~err_int_t();

	void set_to( err_int_t & rhs );

	void clear(); 
	void clear_quietly(); 

	// these return ERROR status, so slightly inverted logic
	// TRUE = error state ERROR
	// FALSE = error state NO-ERRO

	bool get_error_X(); // also checks GLIB SSL ...

	bool get_OS_error(); // OS not GTK not SSL nor ...

	bool zap_OS_error(); // OS not GTK not SSL nor ...
	bool eat_OS_error(); // OS not GTK not SSL nor ...
	bool eat_DNS_error(); // get_host_by_name

//	bool set_to_DNS_error( int h ); // get_host_by_name

	bool set_to_OS_error( int e ); // ie the strerror function
// private but complex so left separate
	bool set_to_UNIX_errno( int e ); // ie the strerror function
	bool set_to_WIN32_error( int e ); // ie the strerror function
	bool set_to_NET_error( int e ); // ie the e_zone


	bool get_NET_error_from_socket( fd_hold & fd );
	bool get_GTK_error( GError * rrr );
	bool get_G_rrr( G_rrr & g_rrr );
	bool get_SSL_error(); // openSSL // also gives lines + more info ?

	bool is_error() { return err; } // ie non zero is error, zero is OK
	bool no_error() { return !is_error(); } 
	bool is_minor(); // EWOULDBLOCK EINTR is_not_really_an_error()
	bool is_key_not_found(); // groped for absent item, not an error

	bool peek_if_read_dry(); // save debugging messages
	bool clear_if_read_dry();
	bool clear_if_EINTR();
	bool clear_if_EINPROGRESS(); // true if cleared
	bool clear_if_EAGAIN(); // true if cleared

	bool FAIL_REPORT(const char * FN = NULL);
	bool INFO_REPORT(const char * FN = NULL);
	void PRINT1( buffer2 & buff );
	const char * PRINT1_str();

	const char * e_str_tail();

	void set_obj_one(obj_ref * obj );

};

extern "C" const char * get_name_of_errno( int e, buffer2 & buf );

// TODO - generate URL to resolve error @ microsoft or google or ...

/*
	ERR_DIAG

	errno comes from a system call, and / or LIBC

	It is the callers responsibility, to check the return code,
	check err_int_t =from= errno, and create one of the DIAG_nostic_phrases.

	EG FAILED to open() "filename" (*with unknown flags but readonly*)
	(*for the purpose of reading a PEM key*)
	(*with 32K limit - but that didnt fail*)
	(*
	  YES - may mean XXX-authenticated-by-knowing secret-of-PKEY
	   NO - may mean XXX-ditto-denied,
		poss typo
		poss hack_attack
	 BOTH-EITHER may need the subsequent CONTENTS of the file too
	 BUT - ENOENT may mean that the PKEY is plain, or encrypted elsewhere
	       so that error would be absorbed
	*)

	That should then create the meaningful DIAG struct (varient event,
	but theme is something failed), and create it as a new event.
	The errno should be reset, and the propagation comes from FALSE ret
	and err* obj.

	The callers caller, detects FALSE_ret, and returns FAIL_FAILED(),
	which adds the stack to the STACK, for diagnostics.

	The callers caller can also add an enlightening detail.
	ie open() failed ENOENT becomes susbsytem_not_previously_configured
	or a consequence to the DIAG_nostic list of phrases

	Somehow, _ALL_ of the library must be rewritten.
	However, a global_per_thread system is available,
	for those who have not yet caught up (which is me, right now).

*/
/*
	TODO - use a PSG parser, as a list of POSSIBLE errors,
	cleared by a subsequent sucess, but then re-awoken by its failure.
	attempt to parse STMT
	it is not "val = val"
	it is not "EXPR" // such a fn(args)
	it is not "{ a_block }"
	it is not ...
	-surrounding-ctxt-
	it is not "}" ";" "#EOF#" "else" or "#OUTDENT#"

*/

/*
	get_CTXT() - returns the thread global CTXT

	CTXT is a per-thread catch-all parameter, passed to EVERY SINGLE FUNC!
	CTXT might be held in session object, which is only ever used from
	one aggregate thread (sub-tasks might somehow also be threads,
	that that somehow does not effect session being one thing/task)

	in ASM set a register
	in C use an otherwise unused 16-bit register as the global_CTXT[idx]
	in C use a parameter
	in SCRIPT use RETVAL_or_WHYNOT to return multiple vals from SFUNC
*/

/*
	FAIL

	whoever first calls FAIL, has the responsibility of creating a DIAG
	FAIL_FAILED is a bit simpler
	FAIL("soft error, this text is the diag PTR=%p",PTR) is ...
	is a way of creating a DIAG from a plain (fmt) string

	TO match existing code two changes are to me made
	(+) leave FAIL as it is, 
	(+) create DIAG_FAIL(...) which calls FAIL and creates a new DIAG
	(+) pass( CTXT* ) to FAIL, so that it can report additionals!!
	(+) 
*/

#endif

