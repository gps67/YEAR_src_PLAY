#include "PDB_app.h"
#include "pdb_root_type.h"
#include "dgb_fail.h"

/*!
	destructor - disconnect
*/
PDB_app::~PDB_app() {
	disconnect();
}

/*!
	constructor - connect with POST file
*/
PDB_app::PDB_app( const char * filename, bool _writable )
{
	writable = _writable;
	post_storage = NULL;
	post_root = NULL;
	all_lines_loaded = FALSE;

	if(!connect( filename, writable ))
	{
		// OK const char * // throw "PDB_app: connect failed";
		// OK but not POINTER // throw dgb_fail( "PDB_app: connect failed" );
		throw new dgb_fail( "PDB_app: connect failed" );
	}
}

/*!
	connect to filename
*/
bool PDB_app::connect( const char * filename, bool writable )
{
	disconnect();
	if(1) e_print("PDB_app::connect(%s,writable=%d)\n", filename, writable );
	int max_locked_pages = 30;
	if(1)
		max_locked_pages = 0;	// locking mmap writes requires root
	else
		max_locked_pages = 30;	// async backup writes on modify

	// create an in memory controller object
	post_storage = new NS_POST::storage(
		filename,
		maxStorageSize,
		NULL, // default_base_address
		max_locked_pages
	);
	if(!post_storage) {
		g_print_error("new NS_POST::storage"); // lame
		return FALSE;
	}

	int flags = 0;

	// if file is readonly, RELOCATION will create new pages
	if( !writable )
		flags |= NS_POST::storage::read_only;

	// keeps old pages in db.log until commit() or rollback()
	// else in-core until flush() when db.tmp copies entire file
	// WIN32 dances old/del through db.sav
	flags |= NS_POST::storage::use_transaction_log;

	// pointers in VTBL will need updating (or shortcut)
	flags |= NS_POST::storage::support_virtual_functions;

	// does GC on load, or manually call garbage_collection()
 //	flags |= NS_POST::storage::do_garbage_collection;

	// Load entire file into mem
 //	flags |= NS_POST::storage::no_file_mapping;

	// file is not incrementally changed
 //	flags |= NS_POST::storage::fault_tolerant;

	// fixed mmap address
 //	flags |= NS_POST::storage::fixed;

	if(!post_storage->open( flags ) ) {
		// NS_POST::storage::read_only
		g_print_error("post_storage->open");
		g_print_error( filename );
		if(!writable) {
			e_print("Cannot create a readonly file!\n");
			return FALSE;
		}
		return FALSE;
	}

	post_root = (pdb_root_type *) post_storage->get_root_object();
	if(post_root) {
		root_page_loaded();
	} else {
		post_root = pdb_root_type::mk_new( post_storage );
		post_storage->set_root_object( post_root );
		root_page_created();
	}
	dset = post_root->dset;
	if(!post_root) {
		g_print_error("post_root_is_null");
		return FALSE;
	}

	// debugging - run GC every other connect

	if(writable) {
		post_root->mount_count ++;
		e_print( "mount_count = %d\n", post_root->mount_count );
		if(!(post_root->mount_count% 10 )) {
			garbage_collection();
		}
	}
	return TRUE;
}

/*!
	Run NS_POST garbage collection
*/
bool PDB_app::garbage_collection()
{
	if(!writable) return TRUE; 
	e_print( "PDB_app::garbage_collection() ... ");
	if(!post_storage ) {
		e_print( "NULL post_storage!\n" );
		return FALSE;
	}
	int n = post_storage->garbage_collection();
	if( n ) {
		e_print( "GC returned %d objects deleted\n", n );
	} else {
		e_print( "GC OK\n" );
	}
	return TRUE;
}

/*!
	Derived class can now do any special early-load
*/
// so load any tables
void PDB_app::root_page_loaded()
{
	e_print( "The ROOT database opened\n");
}

/*!
	Derived can do any special -one-off- setup
*/
// so create any new things
void PDB_app::root_page_created()
{
	e_print( "New ROOT database created\n");
	// actually the NEW in pdb_root_type is this function
	//
	// create a fake line_spec
	/*
		line_id_spec fake_spec_zero;
	//	fake_spec_zero.db_id = -1; // should be in CTR
		fake_spec_zero.spec.site.set((const char*)"FAKE");
		fake_spec_zero.spec.host.set((const char*)"FAKE");
		fake_spec_zero.spec.cmd.set((const char*)"FAKE");
		fake_spec_zero.spec.opt.set((const char*)"FAKE");
		fake_spec_zero.spec.attr.set((const char*)"FAKE");
		fake_spec_zero.spec.dev.set((const char*)"FAKE");
		fake_spec_zero.spec.avg.set((const char*)"FAKE");
		fake_spec_zero.spec.dt = 1;
		lookup_add( & fake_spec_zero );
	*/
}

/*!
	flush and close the file
*/
bool PDB_app::disconnect()
{
	if( !post_storage ) return FALSE;
	e_print("Closing NS_POST database\n");
	// garbage_collection();
	post_storage->flush();
	post_storage->close();
	post_storage = NULL;
	post_root = NULL;
	dset = NULL;
	return TRUE;
}


namespace NS_POST {
	/*!
		POST like to know when to rebuild the clas index
	*/
	extern char* program_compilation_time;

	/*!
		AUTO set compilation time - see Makefile to ensure used
	*/
	class set_compilation_time {
	  public:
	    set_compilation_time() {
		program_compilation_time = (char *) __DATE__ " " __TIME__;
	    }
	};

	/*!
		The stored timestamp in the executable
	*/
	static set_compilation_time timestamp;
}
