#include "file_utils_glib.h"
#include "dgb.h"
#include <stdlib.h> // system

bool file_utils_glib::chdir( const char * dir )
{
	INFO("%s",dir);
	// system("pwd");
	if( g_chdir(dir) ) {
		FAIL("chdir(%s)",dir);
		return false;
	}
// does not work //  __attribute__((unused))
__attribute__((unused)) int t =
	system("pwd");
	return true;
}
bool file_utils_glib::file_exists( const char * name )
{
	if( !g_file_test( name,  G_FILE_TEST_EXISTS ) ) return false;
	if( g_file_test( name,  G_FILE_TEST_IS_DIR ) ) {
		WARN("same name but its a dir: %s", name );
		return false;
	}
	return true;
}
bool file_utils_glib::dir_exists( const char * dir )
{
	return g_file_test( dir,  G_FILE_TEST_IS_DIR );
}
bool file_utils_glib::chdir_if_exists( const char * dir )
{
	if( !dir_exists( dir ) ) return false;
	// INFO( "%s", dir );
	return chdir( dir );
}

#include "fd_hold.h"
extern "C" int close( int );

bool file_utils_glib:: can_open_file_rw( const char * filename )
{
	int fd = open( filename, O_RDWR | O_CREAT, 0640 );
	if( fd < 0 ) {
		return FAIL( "open( '%s',  O_RDWR | O_CREAT, 0640 );", filename );

	}
	close(fd);
	return true;
}
