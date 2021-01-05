#ifndef PDB_app_H
#define PDB_app_H

#include "post_array.h"
#include "ints.h"
#include "misc.h"
#include "e_print.h"

// class NS_POST::storage;
class pdb_root_type;

#include "pdb_dset.h"

//! mounts a pdb_ file - not a part of it though!

/*!
	PDB_app is a mounted NS_POST DB file

	It resolves the 'root-page' depending on which subarea you want.
	(TODO) It might create any MEM data required (import/export)
*/
class PDB_app : public GRP_sar_data
{
 public:
	bool writable;
	NS_POST::storage * post_storage;
	pdb_root_type * post_root;
	bool all_lines_loaded;

	pdb_dset * dset;

	PDB_app( const char * filename, bool readonly );
	bool disconnect();
	bool connect( const char * filename, bool readonly );

	//! 300 MB hardcoded limit (expect 15 per machine @60 sec)
	const static size_t maxStorageSize = 300 * 1024 * 1024;

	~PDB_app();
	PDB_app();

 // virtual const char * get_err_msg();

//	bool load_all_line_ids(
//		line_store_tbl * specs,
//		line_spec * search_spec1
//	);

// EXTRA
	void flush()
	{
		if( post_storage ) post_storage->flush();
	}

	bool garbage_collection();

	void g_print_error(const char * msg = "" )
	{
		char buf[1024];
		if( post_storage )
			post_storage->get_error_text(buf, sizeof buf);                       
		else
			strcpy( buf, "((NULL_POST_STORAGE))" );
		e_print( "PDB_app %s ERROR: %s\n", msg, buf );
	}

// DONE BY ROOT_PAGE_CONSTRUCTOR?

	void root_page_created();	// so create any new things
	void root_page_loaded();	// so load any table caches

// dset_api wants lots of utility functions

	pdb_line_and_segs * find_line( mem_line_spec * spec )
	{
		return dset->find_line( spec );
	}

};

#endif

