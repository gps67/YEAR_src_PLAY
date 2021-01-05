//      static const char const * db_name  = "obj_stat_1";

#define DBG_PRE_PRINT 1

#include "sql_link_postgres.h"
#include "buffer2.h"

/*!
	Constructor - init 0
*/
SQL_link_Postgres::SQL_link_Postgres() {
        results = NULL;
//      mysql_init( & mysql );
        pgsql = NULL;
}

/*!
	Destructor - disconnect
*/
SQL_link_Postgres::~SQL_link_Postgres() {
        free_result();
        disconnect();
}

/*!
	Get PG error message
*/
const char * SQL_link_Postgres::get_err_msg() {
        return PQerrorMessage( pgsql );
}

/*!
	Connect to the remote database
*/
bool SQL_link_Postgres::connect(
        str0 host,
        unsigned int port,
        str0 user,
        str0 pass,
        str0 db
) {
        if(!port) port = 5432;
        char portstr[20];
        char * pgtty = NULL;
        char * pgoptions = NULL;
        sprintf( portstr, "%d", port );
       // pgsql = PQsetdb( host, portstr, pgoptions, pgtty, db );
        pgsql = PQsetdbLogin( host, portstr, pgoptions, pgtty, db, user, pass );

        if (PQstatus(pgsql) == CONNECTION_BAD)
        {
                fprintf(stderr, "connect failed %s\n", get_err_msg() );
                return FALSE;
        }
        return TRUE;
}

/*!
	Disconnect
*/
bool SQL_link_Postgres::disconnect() {
        free_result();
        PQfinish( pgsql );
        pgsql = NULL;
        return TRUE;
}

/*!
	This is called between transactions
*/
void SQL_link_Postgres::free_result( ) {
        if( results ) {
                PQclear( results );
                results = NULL;
        }
}

/*!
	PG is non-trivial to re-wrap, because its BEGIN/END dont
	quite work as expected. Some more reading might make it
	crystal, but right now its mud.
*/
bool SQL_link_Postgres::PG_exec_core( str0 cmd ) {
	bool ok = TRUE; // so far
	free_result();
        if(DBG_PRE_PRINT) { fprintf(stderr, "pg_exec: %s\n", (STR0) cmd ); }
	results = PQexec( pgsql, cmd );
	if( !results ) {
		fprintf(stderr, "PG_exec_FLUFF NULL on '%s'\n", (STR0) cmd );
		return FALSE;
	}
        switch( PQresultStatus(results) ) {
         case PGRES_COMMAND_OK:
                break ; // OK
         case PGRES_TUPLES_OK:
                break ; // OK
         case PGRES_COPY_OUT:
         case PGRES_COPY_IN:
                break ;
         default:
                if(!DBG_PRE_PRINT) {
                        fprintf(stderr, "FAILED SQL: %s\n%s\n", (STR0) cmd, get_err_msg() );
                } else {
                        fprintf(stderr, "# %s\n", get_err_msg() );
                }
                ok = FALSE;
	}
	return ok;
}

/*!
	Run a DECLARE statement
*/
bool SQL_link_Postgres::PG_exec_DECLARE( str0 cmd )
{
	return PG_exec_core( cmd );
}

/*!
	Run a (FLUFF) statement without debugging prints.
	Used mostly for BEGIN/END.

	Overall the PG sub-class generates far too much debugging
	noise.
*/
bool SQL_link_Postgres::PG_exec_FLUFF( str0 cmd ) {
/*
	dont keep results
	dont count rows
	used for BEGIN END ...
	error message will not be available after return
*/
	bool ok = FALSE;
	PGresult * res_kept = results;
	results = NULL;
	ok = PG_exec_core( cmd );
	free_result();
	results = res_kept;
	return ok;
}

/*!
	Call PG_exec_core and update counters
*/
bool SQL_link_Postgres::PG_exec( str0 cmd ) {
	counters_zero();
	// pre_exec_print();
	bool ok = PG_exec_core( cmd );
	counters_update();
	return ok;
}

/*!
	init 0 between transactions
*/
void SQL_link_Postgres::counters_zero() {
        nrows_affected = 0;
        nrows_result = 0;
        insert_oid = 0;
}

/*!
	Counters tell you how many nrows_result, but much more
	importantly inert_oid.
*/
void SQL_link_Postgres::counters_update() {

        switch( PQresultStatus(results) ) {
         case PGRES_COMMAND_OK:
                insert_oid = (long) PQoidValue( results );
                {
                char * s = PQcmdTuples(results);
                if( s )
                        nrows_affected = atoi( s ) ;
                }
                break;
         case PGRES_TUPLES_OK:
                insert_oid = PQoidValue( results );
                nrows_result = PQntuples( results );
		break;
	 case PGRES_EMPTY_QUERY:
	 case PGRES_COPY_OUT:
	 case PGRES_COPY_IN:
                insert_oid = 0;
                nrows_result = 0;
		fprintf(stderr,"# WRONG SQL ACTION # EMPTY Query, COPY \n" );
		break;

	 case PGRES_BAD_RESPONSE:
	 case PGRES_NONFATAL_ERROR:
	 case PGRES_FATAL_ERROR:
	 default:
                insert_oid = 0;
                nrows_result = 0;
		fprintf(stderr,"# SQL OR ENGINE ERROR # Return value from PG\n" );

        }

        if(DBG_PRE_PRINT) fprintf(stderr,
                "Q: id=%ld nrows_affected=%ld nrows_result=%ld\n",
                insert_oid,
                nrows_affected,
                nrows_result
        );
        // VOID // return true;
}

/*!
	This calls BEGIN and END for you, some work TODO
*/
bool SQL_link_Postgres::PG_exec_BEGIN_cmd_END( str0 cmd ) {

        bool ok = true;
        if( !PG_exec_FLUFF("BEGIN") ) return FALSE;
        ok = PG_exec( cmd );
        if( !PG_exec_FLUFF("END") ) return FALSE;
        return ok;
}

/*!
	run an INSERT cmd and get the idx of the new row
*/
bool SQL_link_Postgres::exec_insert_get_index( str0 cmd, int & idx) {

        bool ok = true;
        if( !PG_exec_FLUFF("BEGIN") ) return FALSE;
        if( PG_exec( cmd ) ) {	// with processed results
                idx = insert_oid;
                ok = true;
        } else {
                idx = 0;
                ok = false;
        }
        if( !PG_exec_FLUFF("END") ) return FALSE;
        return ok;
}

/*!
	Run a SQL cmd that selects ONE row (or zero)
*/
bool SQL_link_Postgres::exec_select_one_index( str0 cmd, int & idx) {

	idx = 0;		// -1 might be better
        insert_oid = 0;
        bool ok = TRUE;

        if( !PG_BEGIN() ) return FALSE;

        buffer2 cmd2( 400 );
        cmd2.put( "DECLARE csr99 CURSOR FOR ");
        cmd2.put( cmd );
        cmd2.put( '\0' );

        if( !PG_exec_DECLARE( (STR0) cmd2.buff ) ) {
                ok = false;
        }
        if( !PG_exec("FETCH ALL IN csr99") ) {
                ok = false;
        }
// in above //	counters_update()
        if( !PG_END() ) {	// supposed to retain results
		ok = false;
	}

	if( !ok ) return FALSE;

	switch( nrows_result ) {
	case 1:
		break;		// GOOD
	case 0:			// MISSING
		fprintf( stderr, 
			"exec_select_one_index() didnt fnd a row (NB) \n" );
		return FALSE;
	default:		// BAD SQL logic
		fprintf( stderr, 
			"exec_select_one_index() expected 1 row found %ld\n",
				nrows_result );
		return FALSE;
	}

        char * s = PQgetvalue( results, 0, 0 );
        if( s ) {
                idx = atoi( s );
		insert_oid = idx;	// LURK its the KEY not the OID!
                if( !idx ) {
                        ok = FALSE;	// PG serials start with 1 !! (NAFF)
                }
        } else {
		fprintf( stderr, "PQgetvalue() returned NULL\n" );
                ok = FALSE;
        }
	fprintf( stderr, "RESULT == %d ok = %d IDX = %d\n",
		(int) insert_oid, (int) ok, idx );
        return ok;
}

/*!
	deleted
*/
bool SQL_link_Postgres::x_query_use( str0 cmd ) {
	return PG_exec( cmd );
}

/*!
	deleted
*/
bool SQL_link_Postgres::x_query_store( str0 cmd ) {

	bool ok =  PG_exec( cmd );
	if( !ok ) return FALSE;

        if(DBG_PRE_PRINT) fprintf(stderr, "Q: oid=%ld nrows_affected=%ld nrows_result=%ld\n",
                insert_oid,
                nrows_affected,
                nrows_result
        );
        // results may be null for non SELECT like queries
        return TRUE;
}


