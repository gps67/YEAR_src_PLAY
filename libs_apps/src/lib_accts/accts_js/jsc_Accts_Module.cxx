#include "jsc_Accts_Module.h"
#include "jsc_Dset.h"
#define IFDBG if(0)
#include "fork_progs.h"

#include "fd_hold.h"
#include "buffer3.h"

///////////// jsAccts  ///////////////////////

/*
	This constructor creates one instance that the C++ caller keeps

	Doing so, registers the class "Accts_Module" in JavaScript,
	and when the script does JS{  a1 = new Accts_Module(); }
	new_Thing(args) gets called (hopefully only once)

	That defines all these functions, and the one in new_Thing,
	as class/object specific functions JS{ a1.load_script("filename.js") }

	ie this uses a JSC class as a module (with one instance).
*/

jsc_Accts_Module::
jsc_Accts_Module( JS1 * js1 )
: j_class_plus( "Accts_Module", js1, true )
{

	Def_Function(
		"load_script",
		f_load_script,
		1		// number of args
	);

	Def_Function(
		"add_dset",
		f_add_dset,
		0		// number of args
	);

	Def_Function(
		"gdb_invoke",
		f_gdb_invoke,
		0		// number of args
	);

	Def_Function(
		"gdb_error",
		f_gdb_error,
		1		// number of args
	);

	Def_Function(
		"view_pdf",
		f_view_pdf,
		1		// number of args
	);

	Def_Function(
		"view_pdf_page",
		f_view_pdf_page,
		2		// number of args
	);

	Def_Function(
		"view_html",
		f_view_html,
		1		// number of args
	);

	Def_Function(
		"exit",
		f_exit,
		0		// number of args
	);

	Def_Function(
		"make",
		f_make,
		2		// number of args
	);

	Def_Function(
		"subst_dry_f1_f2",
		f_subst_dry_f1_f2,
		3		// number of args
	);

}


/*!
	There should only be zero/one instance of Accts_Module, (like a module)
	but it is created in the script as new Accts_Module()

	JS has already created the (accts) JSObject,
	so upgrade it and give the JSObject a few functions.
*/
//  VIRTUAL
JSBool jsc_Accts_Module::new_Thing( j_args_newfunc * args )
{
	// args->set_obj_inst( new no_priv() ); // LEAKED
	set_inst(
		new no_priv(), // module has no C++ instance
		args->js_obj
	);
	JS_DefineFunction(
		args->cx,
		args->js_obj,
		"load_dset",
		f_load_dset,
		1,
		0
	);

	return JS_TRUE;
}

#define ARG_i(cx,vp,i) JS_ARGV(cx,vp)[i]
#define ARGV0(cx,vp) ARG_i(cx,vp,0)


/*!
	The Accts_Module can create any number of different DSET CLASSES,
	(by loading them from files).

	A DSET is a class, because each has a different set of tables,
	which appear as top level properties (as dset.VAT not dset.table.VAT)
	IE the benefit is the TOKENISED id's of well known tables.
	A different c++dset creates a new class. no deconstructions tested

	It gives me some practice with class-attributes, but POTENTIALLY
	clashes table names against function names, so they have to have
	'odd' names or __names() Ditto for table._row_keys()

	An object could also have varient fields, but using fixed prop id's
	That means the tables cannot be renumbered here or in the C++ side
*/
//  STATIC
JSBool jsc_Accts_Module:: f_load_dset(
	JSContext *	cx,
	uns           argc,
	jsval *         vp
)
{
	// The MODULE object ("Accts_Module")
	j_cx_obj_peer CX_ACCT( cx, argc, vp );
	if(!CX_ACCT.full_sanity_check()) return JS_FALSE;

	// get filename
	if( argc != 1 )
	{
		return CX_ACCT.ReportError( "f_load_dset(filename) - missing filename" );
	}
	buffer1 buf;
	str0 filename = CX_ACCT.as_string( ARGV0(cx,vp), buf );

	if(1) // say hello
		e_print( "f_load_dset(%s)\n", (STR0) filename );

	// create a new instance in C++ and load the file (or fail)
	data_set * dset = new data_set();
	if( !dset->load_file( filename ))
	{
		// would have been nice to get the error string
		CX_ACCT.ReportError( "load file failed" );
		return JS_FALSE;
	}

	str0 classname; // defaults NULL
	jsc_Dset * jsc = jsc_Dset::mk_jsc_Dset( classname, CX_ACCT.get_js1(), dset ); // NEVER DELETED!

	// wrap C++ dset in a JS_Object, and set that as a return JSVAL
	return jsc-> ret_set( cx, vp, dset );
///	*retvar = OBJECT_TO_JSVAL( jsc->mk_js_self( dset ) );
///	CX.set_ret( jsc, dset );

///	IFDBG e_print("Created dset instance %p\n", dset->get_js_self());
///	return JS_TRUE;
}



/*!
	The SECURE core cannot load files, and we dont want to allow
	clients to load netscape preferences or other, but modular scripts
	make a lot of sense, so ACCTS.load_script("create_schema.js")
	WILL search a path, or table lookup, and load the requested file.
*/
//  STATIC
JSBool jsc_Accts_Module:: f_load_script(
	JSContext *	cx,
	uns           argc,
	jsval *         vp
)
{
	// The MODULE object ("Accts_Module")
	j_cx_obj_peer CX_ACCT( cx, argc, vp );
	if(!CX_ACCT.full_sanity_check()) return JS_FALSE;

	// get filename
	if( argc != 1 )
	{
		return CX_ACCT.ReportError( "load_script(filename) - missing filename" );
	}
	buffer1 buf;
	str0 filename = CX_ACCT.as_string( ARGV0(cx,vp), buf );

	/*
		VALIDATE THE filename (or not in this case)

			a bit like valiadate_date_strings
			with occasional interpretation
			with typedef PSG_TOKEN str2_token;
			with hack_detect_and_defensive_programming_module;
			with VFS_local_store_some_mmapped;
			with VFS_access_for js_script_to_standard_libraries;
			with VFS_VFS_instead_of_symb_links_virtual_ones
			with VFS_VFS_instead_of_symb_links_virtual_ones

		SUGGEST

			send back as fail with suggested alternative
			- remove spaces
			- MaybeAddCap1_
			- Maybe_Use_UnderScores_
			- remove backslashes
			- flag lossless = false || {
				match_exact_in_exact_obj_expr
				name_mangle_ident_99
			}
			- limit { "[A-Z0-9_]+" }
			- limit { "[^/]+" }
			- optimise { "[^/]+/.*", dir & lhs, str_p0p2 & rhs ) {
				void set_lhs_from_rhs( lhs, rhs ) {
					lhs.set_from(rhs);
				}
				str0 ptn = "[^/]+/.*";
				re_cache re( ptn );
	//	/ * ALIAS * /	p0p2 & lhs_p0p2 = re.arg_p0p2[0];
		/ * ALIAS * /	p0p2 & rhs_p0p2 = re.arg_p0p2[1];
				p0p2 rhs_tail = rhs.subzone_all(); 
				 // catchers for ...
				 // code fail!
				 // not a subdir try next RE_EXPR
				if(!re.run(ptn, rhs)) return false; // NO MATCH
				lhs.dirname_subsegment = re.arg_p0p2[0] ;
				rhs_tail = re.arg_p0p2[1];
				}
				bool validate() {
					str0 ptn = "^[A-Z0-9_]";
					if(!re.run( ptn, rhs )) {
					 return FAIL( "expected A-Z 0-9 _" );
					}
					int len = rhs.p0p2_nbytes();
					if( len > 300 ) WARN("len %d", len);
					if( len > 4096 )
						return FAIL("len %d",len);
				}
			}

	*/

#warning "RISK - load_script() requires path search, extn check, etc"

	/*!
		Replace this vector with sanitise() {
		 // #include <stdargs.h>
		 // #include "some_thing_local.h"
		 str2_token * var_filename = sanitise( argv[0], ... );
		 //	... is mostly unused ... soon guide STR0 lookups
		 //	filename = sanitise( argv[pos], pos_info, ctxt_info );  
		 // Servers Private Dir List
		 // 	"./RunOnAsIs/bin/"
		 // 	"RunOnAsIs/bin/" // str2_token str_dir; // = "utf8"; 
		 // 	NEED dir on PATH // load_dir( dir )
		 // Servers Preloaded Table of String values used in scripts
		 //	VFS map "RunOnAsIn" as DIR with SUBTREE on varouus PATH
		}
	*/
	str0 filename_resolved = filename;

		//////// ADD CODE HERE /////////

	/*
		load and run the script
	*/

	JS1 * js1 = CX_ACCT.get_js1();
	JSScript * script = js1 -> CompileFile( filename_resolved );
	if(!script) return JS_FALSE;

	JSBool ok = js1->ExecuteScript( script );

	/*
		Not sure if this is a good idea
		are functions nolonger available ?
	*/
	if(0) js1->DestroyScript( script );

	if(!ok) return JS_FALSE;

	return JS_TRUE;
}


// static
PROTO_JS_FUNC( jsc_Accts_Module::f_add_dset )
{
	J_ARGS_FUNC( CX );
	if(!CX.full_sanity_check()) return JS_FALSE;
	// jsc_Accts_Module * jsc_self = (jsc_Accts_Module*) CX.cls;
	if( _argc != 0 )
		return CX.ReportError("add_dset expects no argument");

	data_set * dset = new data_set();
	if( !dset ) 
	{
		return CX.ReportError_v( "new data_set() failed" );
	}

	str0 noname; // ie auto
	jsc_Dset * jsc_dset = jsc_Dset::mk_jsc_Dset( noname, CX.get_js1(), dset);
	if( !jsc_dset )
	{
		delete dset;
		return JS_FALSE; // already reported
	}

	CX.set_ret( jsc_dset, dset );
///	jsc_dset->mk_js_self( dset );
///	CX.set_ret( dset->get_js_self() );
	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Accts_Module::f_gdb_invoke )
{
	gdb_invoke(true);
	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Accts_Module::f_gdb_error )
{
	J_ARGS_FUNC( CX );
	if(!CX.full_sanity_check()) return JS_FALSE;
	if( _argc != 1 )
		return CX.ReportError("gdb_error expects one argument");

	CX.set_JS_ReportError_gdb( true ); // TODO parameter

	return JS_TRUE;
}



// static
PROTO_JS_FUNC( jsc_Accts_Module::f_view_pdf )
{
	J_ARGS_FUNC3( CX, "view_pdf", 1 );
	if(!CX.full_sanity_check()) return JS_FALSE;
	buffer1 buf;
	str0 filenamepdf = CX.as_string( ARGV0(_cx,_vp), buf );
	bool ok = true;

	/*
		all sort sof things should change
		The output filename
		The C++/names
		The ...
	*/

	ok = fork_xpdf( filenamepdf );
	CX.set_ret_bool( ok );

	if(!ok) {
		return CX.ReportError_v("view_pdf(%s)", (STR0)filenamepdf );
	}
	return JS_TRUE;
}

// static
PROTO_JS_FUNC( jsc_Accts_Module::f_view_pdf_page )
{
	J_ARGS_FUNC3( CX, "view_pdf_page", 2 );
	if(!CX.full_sanity_check()) return JS_FALSE;
	buffer1 buf;
	str0 filenamepdf = CX.as_string( ARGV0(_cx,_vp), buf );
	int pageno = 0;
	bool ok = true;
	if( !CX.ok_as_int( ARG_i(_cx,_vp,1), pageno ))
	{
		return JS_FALSE;
		return CX.ReportError_v("view_pdf_page(%s,%d)", (STR0)filenamepdf, pageno );
	}

	ok = fork_xpdf_page( filenamepdf, pageno );
	CX.set_ret_bool( ok );

	if(!ok) {
		return CX.ReportError_v("view_pdf_page(%s,%d)", (STR0)filenamepdf, pageno );
	}
	return JS_TRUE;
}



// static
PROTO_JS_FUNC( jsc_Accts_Module::f_view_html )
{
	J_ARGS_FUNC3( CX, "view_html", 2 );
	if(!CX.full_sanity_check()) return JS_FALSE;
	buffer1 buf;
	str0 filename = CX.as_string( ARGV0(_cx,_vp), buf );
	bool ok = true;

	/*
		all sort sof things should change
		The output filename
		The C++/names
		The ...
	*/

	ok = fork_netscape( filename );
	CX.set_ret_bool( ok );

	if(!ok) {
		return CX.ReportError_v("view_pdf(%s)", (STR0)filename );
	}
	return JS_TRUE;
}



// static
PROTO_JS_FUNC( jsc_Accts_Module::f_exit )
{
	J_ARGS_FUNC3( CX, "exit", 1 );
	if(!CX.full_sanity_check()) return JS_FALSE;
	int ecode = 1;
	bool ok = CX.ok_as_int( ARGV0(_cx,_vp), ecode );
	buffer1 buf1;
	if( !ok ) {
		return CX.ReportError_v("expected INT got '%s' for exit code", (STR0) CX.as_string( ARGV0(_cx,_vp), buf1 ) );
	}
	// int ecode = 1; // if you want 0 - fix this code
	exit( ecode );
	return JS_TRUE;
}


// static
PROTO_JS_FUNC( jsc_Accts_Module::f_make )
{
// this works OK but 'mk' loses exit coe // #error "TEST FAIL"
// #error "TEST FAIL"
	J_ARGS_FUNC3( CX, "make", 2 );
	if(!CX.full_sanity_check()) return JS_FALSE;
	buffer1 buf1;
	buffer1 buf2;
	str0 dir = CX.as_string( ARGV0(_cx,_vp), buf1 );
	str0 target = CX.as_string( ARG_i(_cx,_vp,1), buf2 );
	bool ok = true;
#warning "RISK - fork_make() uses system()"

	ok = fork_make( dir, target );
	CX.set_ret_bool( ok );

// hmmm; return JS_TRUE;

	// if(throw_on_fail) ... // can catch anyway
	if(!ok) {
		return CX.ReportError_v("make(%s,%s)", (STR0)dir, (STR0)target );
	}
	return JS_TRUE;
}


#define N4K 4096
/*!
	open f1 and f2 (RISK)
	filter text though dictionary
*/
// static
PROTO_JS_FUNC( jsc_Accts_Module::f_subst_dry_f1_f2 )
{
	J_ARGS_FUNC3( CX, "subst_dry_f1_f2", 3 );
	if(!CX.full_sanity_check()) return JS_FALSE;
	JSObject * dry = JSVAL_TO_OBJECT( ARGV0(_cx,_vp) );
	buffer1 buf1;
	buffer1 buf2;
	str0 f1 = CX.as_string( ARG_i(_cx,_vp,1), buf1 );
	str0 f2 = CX.as_string( ARG_i(_cx,_vp,2), buf2 );
	bool ok = true;
#warning "RISK - opening files R, W isnt safe"
	fd_hold_1 fd1;
	fd_hold_1 fd2;
	if( !fd1.open_RO( f1 ) ) {
		ok = false;
		// DONE // e_print("# FAIL # open(%s) - %m\n", (STR0) f1 );
	}
	buffer3 buf_in;
	buffer2 buf_out;
	buffer2 buf_name;
	buf_in.get_space( N4K * 4 );
	if(ok) // file open failed
	while(1) {
		buf_in.circ_copy_down();
		int n = buf_in.space_avail(); // tail space - not front
		// e_print("# INFO # SPACE %d\n", n ); 
		n = fd1.read( buf_in.gap_addr(), n );
		buf_in.nbytes_used += n;
		// e_print("# INFO # USED %d\n", n ); 
		buf_in.trailing_nul(); // just in case (rewrite with scan()
		buf_out.get_space( buf_in.circ_len() + 1000 );
		// e_print("# INFO # CIRC %d\n", buf_in.circ_len()) ;
		if(!buf_in.circ_len()) break; // EOF and all processed
		uchar * P0 = buf_in.circ_addr();
		uchar * P2 = buf_in.gap_addr();
		uchar * P2_lo = P2 - N4K;
		uchar * P = P0;
#warning "CODE - This parser is so bad I dont understand how still needed to add it"
		while(P<P2) {
			uchar c = *P++;
			// e_print("# INFO # CHAR '%c'\n", c ); 
			if( c != '$' ) {	// never appears in UTF extn
				buf_out.put( c ); // no UTF8 convertion
				if( P >= P2_lo ) {
					buf_in.offs_front = P - buf_in.buff;
					break; // loads more
				}
				continue;
			}
			// clearly buffer3 isnt what it should be
			buf_in.offs_front = P - buf_in.buff;
			// e_print("# INFO # DOLLAR SIGN\n" ); 
			if( *P != '{' ) continue;
			P++;
			buf_name.clear();
			while( 1 ) {
				c = * P++;
				if( !c ) break;
				if( c == '}') break;
				buf_name.put( c );
			}
			buf_name.trailing_nul(); // 
			// e_print("# INFO # substitution %s\n", buf_name.buff ); 
			// comes up 'undefined' anyway
			jsval val;
			if( !JS_GetProperty( CX.cx, dry, (STR0) buf_name.buff, &val ))
			{
				e_print("# ERROR # ${%s:-ERROR}", buf_name.buff ); 
				buf_out.print("${%s:-ERROR}", buf_name.buff ); 
				ok = false;
				break;
				continue;
			}
			if(JSVAL_IS_VOID(val)) {
				e_print("# WARN # ${%s:-ABSENT}\n", buf_name.buff ); 
				buf_out.print("${%s}#ABSENT#", buf_name.buff ); 
				continue;
			}
			buffer2 buf2;
			buf_out.print("%s", (STR0) CX.as_string( val, buf2 ));
		}
	}
	if(ok)
	if(!fd2.open_RW_CREATE( f2 )) {
		ok = false;
		// DONE // e_print("# FAIL # open(%s) - %m\n", (STR0) f2 );
	}
	int n = buf_out.nbytes_used;
	if( n != fd2.write( buf_out )) {
			ok = false;
	}

	CX.set_ret_bool( ok );

	fd1.close();
	fd2.close();
	// if(throw_on_fail) ... // can catch anyway
	if(!ok) {
		return CX.ReportError_v("f_subst_dry_f1_f2(DRY,%s,%s)", (STR0)f1, (STR0)f2 );
	}
	return JS_TRUE;
}

