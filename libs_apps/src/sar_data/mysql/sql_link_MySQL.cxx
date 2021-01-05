
#include "sql_link_MySQL.h"
#include "e_print.h"
#include "buffer2.h"

#define DBG_PRINT 1

/*!
	Constructor - init 0
*/
sql_link_MySQL::sql_link_MySQL() {
	results = NULL;
	mysql_init( & mysql );
}

/*!
	MySQL error string
*/
const char * sql_link_MySQL::get_err_msg() {
	return mysql_error( &mysql );
}

/*!
	Connect to the remote database
*/
bool sql_link_MySQL::sql_connect(
	const char * host,
	unsigned int port,
	const char * user,
	const char * pass,
	const char * db
) {
	const char *unix_socket = NULL;
	unsigned int client_flag = 0;
		
	INFO("Connecting to sql host %s port %d user %s db %s", host, port, user, db );
	fflush(0);
	if(!mysql_real_connect(
		& mysql,
		host,
		user,
		pass,
		db,
		port,
		unix_socket,
		client_flag ) )
	{
		FAIL( "%s", get_err_msg() );
		return FALSE;
	}
	return TRUE;
}

/*!
	Theres no need to disconnect?
*/
bool sql_link_MySQL::disconnect() {
	return FALSE;
}

/*!
	MySQL need this between transactions
*/
void sql_link_MySQL::free_result( ) {
	if( results ) {
		mysql_free_result( results );
		results = NULL;
	}
}

/*!
	Run the MySQL query str, and fetch all rows in one go.
*/
bool sql_link_MySQL::query_store( const char * str ) {
	free_result();

	if(DBG_PRINT) { e_print( "QUERY: %s\n", str ); }
	int t = mysql_query( & mysql, str );
	if( t ) {
		e_print( "QUERY ERROR: %d %s\n", t, get_err_msg() ); 
		return FALSE;
	}
	// CHOICE _use_results or _store_results
	results = mysql_store_result( & mysql );
	// nrows affected ? effected // call after store
	nrows_affected = (long) mysql_affected_rows( & mysql );
	// nrows in the result set
	nrows_result = 0;
	if( results ) nrows_result = (long) mysql_num_rows( results );
	// grab insert_id from previous INSERT with AUTO_INCREMENT (else 0)
	insert_id = (long) mysql_insert_id( & mysql );

	if(DBG_PRINT) e_print( "Q: id=%ld nrows_affected=%ld nrows_result=%ld\n",
		insert_id,
		nrows_affected,
		nrows_result
	);
	// results may be null for non SELECT like queries
	return TRUE;
}

bool sql_link_MySQL::query_use( buffer2 & enq ) {
	enq.trailing_nul();
	return query_use( (str0) enq );
}


/*!
	Run the MySQL query str, and fetch all rows in one go.

	This looks a bit confused, seems that it also uses mysql_store_results.
*/
bool sql_link_MySQL::query_use( const char * str ) {
	free_result();

	if(0) { e_print( "QUERY: %s\n", str ); }
	int t = mysql_query( & mysql, str );
	if( t ) {
		e_print( "QUERY ERROR: %d %s\n", t, get_err_msg() ); 
		return FALSE;
	}
	// CHOICE _use_results or _store_results
	results = mysql_store_result( & mysql );
	// CANNOT get this info ...
	nrows_affected = 0;
	nrows_result = 0;
	// grab insert_id from previous INSERT with AUTO_INCREMENT (else 0)
	insert_id = (long) mysql_insert_id( & mysql );
	return TRUE;
}

/*!
	TODO: str0 is now closer to char * so merge this with above,
	probably rework all to use str0, as it is better at comparing
	unsigned chars (c<=' ') doesnt falsely match 128-255.
*/
bool sql_link_MySQL::query_use( str0 str ) {
	free_result();

	if(0) { e_print( "QUERY: %s\n", str.sget() ); }
	int t = mysql_query( & mysql, str.sget() );
	if( t ) {
		e_print( "QUERY ERROR: %d %s\n", t, get_err_msg() ); 
		return FALSE;
	}
	// CHOICE _use_results or _store_results
	results = mysql_store_result( & mysql );
	// CANNOT get this info ...
	nrows_affected = 0;
	nrows_result = 0;
	// grab insert_id from previous INSERT with AUTO_INCREMENT (else 0)
	insert_id = (long) mysql_insert_id( & mysql );
	return TRUE;
}

