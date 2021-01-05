#ifndef sql_link_postgres_H
#define sql_link_postgres_H

// // #include "line_id_spec.h"
#include "misc.h"
#include "blk1.h"
#include "zblk.h"
// #include <mysql.h>
#include <libpq-fe.h>
#include <libpq/libpq-fs.h>

// ALSO // struct PG_BEGIN_END_HOLDER; 

/*!
	SQL_link_Postgres holds and uses a DB connection.

	In future it is hoped to figure out how to make it directly
	exchangeable with MySQL for simple (standard) commands.

	Its a bit noisy on the debugging traces, as I'm splicing in
	the obligatory BEGIN/END and CURSOR subcommands, and only just
	getting it working.
*/
struct SQL_link_Postgres : public GRP_sar_data
{
	PGconn * pgsql;
	PGresult * results;

	long nrows_result; // was ulonglong
	long nrows_affected; // was ulonglong
//	long insert_id; // MySql returns key SERIAL
	long insert_oid; // Postgres returns OID

//	long select_idx;	// must be INTEGER
//	long insert_idx;	//
	long return_idx;	// selected or inserted

	const char * get_err_msg();
	 SQL_link_Postgres();
	~SQL_link_Postgres();
	bool connect(
		str0 host,
		unsigned int port,
		str0 user,
		str0 pass,
		str0 db
	);
	bool disconnect();
	void free_result();
	bool PG_exec_core( str0 str );
	bool PG_exec_DECLARE( str0 str );
	bool PG_exec_FLUFF( str0 str );
	void counters_zero(); // counters is the wrong word ... process results
	void counters_update(); // after PGexec // check OK ?
	bool PG_exec( str0 str ); // with processed resutls etc
	bool PG_BEGIN() { return PG_exec_FLUFF( "BEGIN" ); }
	bool PG_END() { return PG_exec_FLUFF( "END" ); }
	bool PG_exec_BEGIN_cmd_END( str0 str );
	bool x_query_store( str0 str );
	bool x_query_use( str0 str );
	bool exec_insert_get_index( str0 cmd, int & idx );
	bool exec_select_one_index( str0 cmd, int & idx );

	bool PG_lo_fetch(  Oid   objid,       blk1 & data );
	bool PG_lo_store(  Oid   objid, const blk1 & data );
	bool PG_lo_create( Oid & objid, const blk1 & data );

	// typecasts to save/load compressed data asis
	bool PG_lo_fetch(  Oid   objid,       zblk & data ){
		return PG_lo_fetch( objid, data.buff );
	}
	bool PG_lo_store(  Oid   objid, const zblk & data ){
		return PG_lo_store( objid, data.buff );
	}
	bool PG_lo_create( Oid & objid, const zblk & data ){
		return PG_lo_create( objid, data.buff );
	}

};

/*!
	A utilty class that ensures every BEGIN has an END
*/
struct PG_BEGIN_END_HOLDER  : public GRP_sar_data
{
	SQL_link_Postgres * link;
	char * desc;	// STATIC NO ALLOC!
	PG_BEGIN_END_HOLDER( SQL_link_Postgres * _link, char * _desc ) {
		link = _link;
		desc = _desc;
		fprintf( stderr, "%s", desc );
		link->PG_BEGIN();
	}
	~PG_BEGIN_END_HOLDER() {
		fprintf( stderr, "%s", desc );
		link->PG_END();
		link = NULL;
	}
};

#endif
