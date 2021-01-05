#include "dset_api.h"
#include "pdb_root_type.h"
#include "PDB_app.h"
#include "dset_api_pdb.h"

dset_api_pdb::dset_api_pdb(
	const char * filename
)
{
	pdb = new PDB_app( filename, TRUE ); // throws dgb_fail
	// pdb = new PDB_app( filename, FALSE ); // force readonly
}

dset_api_pdb::~dset_api_pdb()
{
	delete pdb;
}

// virtual
bool dset_api_pdb::merge_floats(
	mem_line_spec * spec,
	time_t t0,
	int n,
	float * flts
){
	return pdb->post_root->dset->merge_floats( spec, t0, n, flts );
}

// virtual
bool dset_api_pdb::select_ALL_specs_to_dset (
	dset_api * dest
){
	bool ok = TRUE;
	pdb_line_list   * line_list   = pdb->post_root->dset->line_list;
        pdb_string_pool * string_pool = pdb->post_root->dset->string_pool;
	mem_line_spec spec1;

	int N = line_list->N();
	for( int i=0; i<N; i++ )
	{
		pdb_line_and_segs * line = line_list->get_at( i );
		// assert(line);
		if(!line) continue;

		line->get_line_spec( string_pool, &spec1 );
		if( ! dest -> merge_spec( & spec1 )) ok= FALSE;
	}
	return ok;
}

bool dset_api_pdb::select_segs_to_dset(
	dset_api * dset,
	mem_line_spec * spec,
	time_t t0,
	time_t t2
) {
	bool ok = TRUE;
	pdb_line_and_segs * line;
	line = pdb->find_line( spec ); 
	int n = line->N_seg();
	for(int i = 0; i<n; i++)
	{
		time_t seg_t2 = line->ary_t2[i];
		if( seg_t2 <= t0 ) continue;
		time_t seg_t0 = line->ary_t0[i];
		if( seg_t0 >= t2 ) break;
		int n_flts = ( seg_t2 - seg_t0 ) / spec->dt;
		float flts[ n_flts ];
		if( ! line->ary_blob[i]->un_compress( n_flts, flts )) {
			ok = FALSE;
			continue;
		}
		if(!dset->merge_floats( spec, seg_t0, n_flts, flts ))
			ok = FALSE;
	}
	return ok;
}

bool dset_api_pdb::list_segs_to_dset(
	dset_api * dset,
	mem_line_spec * spec,
	time_t t0,
	time_t t2
) {
	bool ok = TRUE;
	pdb_line_and_segs * line;
	line = pdb->find_line( spec ); 
	int n = line->N_seg();
	for(int i = 0; i<n; i++)
	{
		time_t seg_t0 = line->ary_t0[i];
		time_t seg_t2 = line->ary_t2[i];
		dset->listed_seg( spec, seg_t0, seg_t2 );
	}
	return ok;
}

bool dset_api_pdb::flush()
{
	pdb->flush();
	return true;
}


bool dset_api_pdb::disconnect()
{
	pdb->flush();
	pdb->disconnect();
	return true;
}
