
#include "cfg_demo_1.h"
#include "blk_base64.h"
#include "SSL_global.h" // RAND_
#include "util_buf.h" // 
#include "sql_link_MySQL.h"

using namespace VARS;
using namespace CFG_DEMO;


kf_cfg_demo_1::
~kf_cfg_demo_1()
{
	STEP("DTOR");
}


kf_cfg_demo_1::
kf_cfg_demo_1()
{
	STEP("CTOR");
}

void kf_cfg_demo_1:: gen_random_str()
{
	blk1 rnd_blob;
	RAND_get_nn_bytes_of_random( 240, rnd_blob );
	blk1 rnd_str;
	blk_base64 conv64;
	conv64.multi_line = true;	// keyfile fails with this
	conv64.multi_line = false;
	conv64.encode( rnd_blob, rnd_str );

	var_random_str.default_value = rnd_str;
	var_random_str.current_value = rnd_str;
}

void kf_cfg_demo_1:: use_random_str()
{
	RAND_add_str( var_random_str.current_value );
}

bool kf_cfg_demo_1:: late_init()
{
	file_name = "cfg_demo_file.dat"; // not in keys?

	// V_file.ref_static();
	V_file.late_init (
		"CFG_DEMO_ONE",
		"\n"	// KEYFILE will add one leading space!
		"CFG_DEMO_ONE\n"
		"CFG_DEMO_ONE The main data store\n"
		"CFG_DEMO_ONE for the time seriesdata\n"
		"CFG_DEMO_ONE"
	);

	GRP_DEMO_CONNECT.ref_static();
	GRP_DEMO_CONNECT.late_init(
		V_file,
		"DEMO_CONNECT",
		"Connection parameters for clients to contact"
	);

	var_random_str.ref_static();
	var_random_str.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"random_str",			// name_sto
		"random_str",			// name_gui
		"(STR0) rnd_str",		// default init val
						// desc_sto
		"Store some randomness for later",
						// desc_gui
		"random_str\n"
		"is a nutricious source of randomness\n"
		"It is updated and stored for later. PLUS cbc"
	);
	gen_random_str();


	var_db_pref.ref_static();
	var_db_pref.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"db_pref",		// name_sto
		"db_pref",		// name_gui
		"-",				// default init val
						// desc_sto
		"The db_pref for the data",
						// desc_gui
		"db_pref \n"
		"mysql || pgsql || pdb || _\n"
		"Which connection to use"
	);

	var_mysql_host.ref_static();
	var_mysql_host.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"mysql_addr",		// name_sto
		"MYSQL_addr",		// name_gui
		"-",				// default init val
						// desc_sto
		"The connection for the server - IP4 addr or DNS",
						// desc_gui
		"server_addr \n"
		"is the ADDRESS of the server where you want to connect to\n"
		"It can be a DNS name or an IP address "
	);

	var_mysql_port.ref_static();
	var_mysql_port.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"mysql_port",		// name_sto
		"MYSQL PORT number not NAME",			// name_gui
		"0",				// default init val
						// desc_sto
		"The connection to the server - IP4 port",
						// desc_gui
		"mysql server_port \n"
		"is the TCP/IP4 PORT number"
	);

	var_mysql_user.ref_static();
	var_mysql_user.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"mysql_user",		// name_sto
		"MYSQL user NAME",			// name_gui
		"-",				// default init val
						// desc_sto
		"The user name",
						// desc_gui
		"mysql server_user"
		"user for normal data acess"
	);

	var_mysql_pass.ref_static();
	var_mysql_pass.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"mysql_pass",		// name_sto
		"MYSQL user pass",			// name_gui
		"-",				// default init val
						// desc_sto
		"The user pass",
						// desc_gui
		"mysql server_pass"
		"pass for normal data acess"
	);

	var_mysql_db.ref_static();
	var_mysql_db.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"mysql_db",		// name_sto
		"MYSQL db NAME",			// name_gui
		"-",				// default init val
						// desc_sto
		"The database name",
						// desc_gui
		"mysql server_db"
		"table unknown but this variable"
	);

	var_mysql_rootuser.ref_static();
	var_mysql_rootuser.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"mysql_rootuser",		// name_sto
		"MYSQL rootuser NAME",			// name_gui
		"root",				// default init val
						// desc_sto
		"The rootuser name",
						// desc_gui
		"mysql server_rootuser"
		"rootuser for admin acess"
	);

	var_mysql_rootpass.ref_static();
	var_mysql_rootpass.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"mysql_rootpass",		// name_sto
		"MYSQL user rootpass",			// name_gui
		"-",				// default init val
						// desc_sto
		"The user rootpass",
						// desc_gui
		"mysql server_rootpass"
		"pass for normal data acess"
	);


////////////////////////////

	var_pgsql_host.ref_static();
	var_pgsql_host.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"pgsql_addr",		// name_sto
		"PGSQL_addr",		// name_gui
		"-",				// default init val
						// desc_sto
		"The connection for the server - IP4 addr or DNS",
						// desc_gui
		"server_addr \n"
		"is the ADDRESS of the server where you want to connect to\n"
		"It can be a DNS name or an IP address "
	);

	var_pgsql_port.ref_static();
	var_pgsql_port.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"pgsql_port",		// name_sto
		"PGSQL PORT NAME",			// name_gui
		"-",				// default init val
						// desc_sto
		"The connection to the server - IP4 port",
						// desc_gui
		"pgsql server_port \n"
		"is the TCP/IP4 PORT number"
	);

	var_pgsql_user.ref_static();
	var_pgsql_user.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"pgsql_user",		// name_sto
		"PGSQL user NAME",			// name_gui
		"-",				// default init val
						// desc_sto
		"The user name",
						// desc_gui
		"pgsql server_user"
		"user for normal data acess"
	);

	var_pgsql_pass.ref_static();
	var_pgsql_pass.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"pgsql_pass",		// name_sto
		"PGSQL user pass",			// name_gui
		"-",				// default init val
						// desc_sto
		"The user pass",
						// desc_gui
		"pgsql server_pass"
		"pass for normal data acess"
	);

	var_pgsql_db.ref_static();
	var_pgsql_db.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"pgsql_db",		// name_sto
		"PGSQL db NAME",			// name_gui
		"-",				// default init val
						// desc_sto
		"The database name",
						// desc_gui
		"pgsql server_db"
		"table unknown but this variable"
	);

	var_pgsql_rootuser.ref_static();
	var_pgsql_rootuser.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"pgsql_rootuser",		// name_sto
		"PGSQL rootuser NAME",			// name_gui
		"root",				// default init val
						// desc_sto
		"The rootuser name",
						// desc_gui
		"pgsql server_rootuser"
		"rootuser for admin acess"
	);

	var_pgsql_rootpass.ref_static();
	var_pgsql_rootpass.late_init(
		GRP_DEMO_CONNECT,		// section
		"STRING",			// vartype
		"pgsql_rootpass",		// name_sto
		"PGSQL user rootpass",			// name_gui
		"-",				// default init val
						// desc_sto
		"The user rootpass",
						// desc_gui
		"pgsql server_rootpass"
		"pass for normal data acess"
	);

////////////////////////////


	GRP_DEMO_DEMO.ref_static();
	GRP_DEMO_DEMO.late_init_no_sto(
		V_file,
		"DEMO_DEMO_no_sto",
		"The DEMO DEMO uses some temp values"
	);

	return true;
}

bool kf_cfg_demo_1:: set_DEMO_values()
{
	// gdb_invoke(true);
	var_db_pref.set_default_value( "mysql" );

	var_mysql_host.set_default_value( "127.0.0.1" );
	var_mysql_host.set_default_value( "x3" );
	var_mysql_port.set_default_value( "mysql" ); // 3306
	var_mysql_port.set_default_value( "3306" ); // 3306
	var_mysql_user.set_default_value("obj_stat_gather");
	var_mysql_pass.set_default_value("gather99");
	var_mysql_db.set_default_value("obj_stat_1");
	var_mysql_rootuser.set_default_value("root");
	var_mysql_rootpass.set_default_value("probablyunusedhere");

	var_pgsql_host.set_default_value( "x3" );
	var_pgsql_port.set_default_value( "1212" );
	var_pgsql_user.set_default_value("obj_stat_gather");
	var_pgsql_pass.set_default_value("gather99");
	var_pgsql_db.set_default_value("obj_stat_1");
	var_pgsql_rootuser.set_default_value("postgres");
	var_pgsql_rootpass.set_default_value("probablyunusedhere");

	return true;
}

bool kf_cfg_demo_1:: connect_mysql( sql_link_MySQL & link )
{
	int port = 99;
	if( !((str0)var_mysql_port).as_int( port )) return FAIL_FAILED();
	if( link.sql_connect(
	 (STR0)	var_mysql_host,
		port,
	 (STR0)	var_mysql_user,
	 (STR0)	var_mysql_pass,
	 (STR0)	var_mysql_db
	// (STR0)	var_mysql_rootuser,
	// (STR0)	var_mysql_rootpass,
	)) {
		STEP("connected");
		return true;
	} else {
		return FAIL_FAILED();
	};
	
}

// BASE CLASS
// nb feeds RAND entropy collector RAND
kf_cfg_demo_0::
kf_cfg_demo_0()
{
}

kf_cfg_demo_0::
~kf_cfg_demo_0()
{
}

bool kf_cfg_demo_1:: save_file( const char * filename )
{
//	if(!filename) filename = get_filename_default();
	if(!filename) return FAIL("NULL filename");
	if(!filename) return FAIL_FAILED();

	buffer2 blk_in;
	if(!V_file.save_into_buffer( blk_in )) return FAIL_FAILED();
	if(!blk_write_to_file( blk_in, filename )) return FAIL_FAILED();

	RAND_add_blk(blk_in); // add some randomness to early pool
	// TODO write seed_file_write_soon()
	// seed_file_write_soon(); // have some new (albeit old) random

	dgb_sleep_less();
	return true;
}

bool kf_cfg_demo_1:: load_file( const char * filename )
{
//	if(!filename) filename = get_filename_default();
	if(!filename) return FAIL("NULL filename");
	if(!filename) return FAIL_FAILED();
	buffer2 blk_in;
	u32 max_K_size = 200;
	if(!blk_read_entire_file( blk_in, filename, max_K_size ))
		return FAIL_FAILED();

	if(!V_file.load_from_buffer( blk_in )) return FAIL_FAILED();

	RAND_add_blk(blk_in); // add some randomness to early pool
//	seed_file_write_soon(); // have some new (albeit old) random

	dgb_sleep_less();
	return PASS("loaded %s", filename );
	return true;
}



// static const char * get_filename_hardcoded(); // cfg_demo_1.kf
