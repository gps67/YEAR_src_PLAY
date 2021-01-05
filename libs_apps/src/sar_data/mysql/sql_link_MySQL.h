#ifndef sql_link_mysql_H
#define sql_link_mysql_H

// #include "line_id_spec.h"
#include "misc.h"
#include "blk1.h"
#include <mysql/mysql.h>
#include "str0.h"

struct buffer2;

/*!
	sql_link_MySQL is a generic connection to a MySQL server.

	It auto-gets the insert_id of new rows.

	TODO: distinguish different SQL statement types, and match
	against what PostGreSQL needs.

	called from sar_data_X 
	called from cfg_demo - which should not be in base2

*/
struct sql_link_MySQL : public GRP_sar_data
{
	MYSQL mysql;
	MYSQL_RES * results;
	long nrows_result; // was ulonglong
	long nrows_affected; // was ulonglong
	long insert_id; // was ulonglong

	const char * get_err_msg();
	 sql_link_MySQL();
	~sql_link_MySQL() { }
	// connect too common a word
	bool sql_connect(
		const char * host,
		unsigned int port,
		const char * user,
		const char * pass,
		const char * db
	);
	bool disconnect();
	bool query_store( const char * str );
	bool query_use( const char * str );
	bool query_use( str0 str );
	bool query_use( buffer2 & str );
	void free_result();
};

#endif
