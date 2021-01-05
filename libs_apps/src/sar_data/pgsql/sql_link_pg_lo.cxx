#include "sql_link_postgres.h"

/*
	PostgreSQL Large Objects are a pain in the neck,
	they do mean that SELECT returns a manageable size,
	but you then have to fetch the object (this handles any BLOB)

	You will need to hoover/delete any lo (oid) which you
	no-longer refer to. The calling code will also need to
	distinguish create/replace (store).

	The makes more sense if you create your own OID in other SQL's
	ie if you now create table of key->blob for other DBMS
	your structure will be cross portable, but polarised like that
*/

/* INLINED
bool SQL_link_Postgres::PG_lo_fetch( Oid objid, zblk & data ) {
	return PG_lo_fetch( objid, data.buff );
}
*/

/*!
	Fetch the data of a LargeObject into a blk1 of data
*/
bool SQL_link_Postgres::PG_lo_fetch( Oid objid, blk1 & data ) {

	PG_BEGIN_END_HOLDER wrap( this, (char *) "PG_lo_fetch" );
	/*
		PostgreSQL large object actions needs BEGIN ... END context
		The HOLDER's dctr is run when return FALSE is run

		could use a similar technique for lo_open()/lo_close()
		possibly a class to hold the lo 'fd' holder.close()
	*/

#ifdef TEST_BY_EXPORTING	// dump all lo's to files
	char filename[80];
	sprintf(filename, "/tmp/segs/LO_%d", objid );
	int r = ::lo_export( pgsql, objid, filename );
	fprintf(stderr,"lo_export(%d) returned %d\n", objid, r );
#endif

	data.nbytes_used = 0;		// truncate the buffer, even on fail

	int l = lo_open( pgsql, objid, INV_READ );
	if( l < 0 ) {
		fprintf(stderr,"lo_open() returned %d\n", l );
		return FALSE;
	}

  /*
	the caller will probably reuse a single buffer for a while
	if this is the first request, jump to what we expect + 1 K
	if you get the size wrong, it will auto grow (and might stay)
  */

	data.get_space( 1024*(3+1) );	// usually more than 1 week compressed

	bool ok = TRUE;
	while(1) {
		// some progress must always be made (while(1)/break)

		if( !data.get_space( 1024 ) ) {
			fprintf(stderr,"lo_fetch %d get_space(1024) failed\n", objid );
			ok = FALSE;
			break;
		}

		// request as much data as we have space for
		// direct into the blk1 buffer (which could be redesigned)

		long space = data.nbytes_alloc - data.nbytes_used;
		char * buf = (char *) &( data.buff[ data.nbytes_used ] );
		int nread = lo_read( pgsql, l, buf, space );
		if( nread < 0 ) {
			fprintf(stderr,"lo_read() failed with %d \n", nread );
			ok = FALSE;
			break;
		}
		if( nread == 0 ) {	// EOF 
			break;
		}
		data.nbytes_used += nread;
		// else loop again (with 1K space), probably to read 0==EOF
	}

	int t=lo_close( pgsql, l );
	if( t ) {
		fprintf(stderr,"lo_close(%d) failed with %d \n", l, t );
		ok = FALSE;
	}

	/* allow caller to reuse the same buffer for object, else call:
		data.resize( data.nbytes_used );
	*/
	return ok;
}

/*!
	Send the blk1-data into the (already allocated) objid
*/
bool SQL_link_Postgres::PG_lo_store( Oid objid, const blk1 & data ) {

	if( objid < 1 ) { 
		fprintf(stderr,"lo_store(objid==%d)\n", objid );
		return FALSE;
	}

	PG_BEGIN_END_HOLDER wrap( this, (char *) "lo_store" );

	int l = lo_open( pgsql, objid, (INV_READ|INV_WRITE) );
	if( l < 0 ) {
		fprintf(stderr,"lo_open(%d) returned %d\n", objid, l );
		return FALSE;
	}

	bool ok = TRUE;		// need to stay to call lo_close();

	// call lo_write with the entire data (specific to blk1)

	long len = data.nbytes_used;
	char * buf = (char *) data.buff;
	int t = lo_write( pgsql, l, buf, len);
	if( t != len ) {
		fprintf(stderr,"lo_write() id=%d returned %d of %ld\n", objid, t, len );
		ok = FALSE;
	}

	if( lo_close( pgsql, l ) != 0) {
		fprintf(stderr,"lo_close() failed\n" );
		ok = FALSE;
	}
	return TRUE;
}

/*!
	Create a new objid and store data there.
*/
bool SQL_link_Postgres::PG_lo_create( Oid & objid, const blk1 & data ) {

	PG_BEGIN();
	objid = lo_creat( pgsql,  (INV_READ|INV_WRITE) );
	PG_END();

	if( objid <= 0 ) {
		fprintf(stderr,"lo_create() failed %d\n", objid );
		return FALSE;
	}
	return PG_lo_store( objid, data );
}




