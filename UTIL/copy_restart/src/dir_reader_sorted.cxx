
#include "dir_reader_sorted.h"
#include "file_stat.h"
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>


name_stat::
name_stat( STR0 name_ )
: name(name_)
{
	// to stat need dir //
}


// obj_list<name_stat> name_stat_list;

bool dir_reader_sorted:: open( const char * path )
{
	pos = 0;
	dir_reader_base dir_list;
	if(!dir_list.open(path)) return FAIL_FAILED();
	while( dir_list.next() ) {
		name_stat * item = new name_stat(dir_list.name());
	}

	pos = 0;
	return false;
}

bool dir_reader_sorted:: sort_by_name()
{
	return false;
}

bool dir_reader_sorted:: sort_by_mtime()
{
	return false;
}

bool dir_reader_sorted:: sort_by_size()
{
	return false;
}

bool dir_reader_sorted:: close()
{
	return false;
}

bool dir_reader_sorted:: next()
{
	return false;
}

// const char * name();
struct stat & dir_reader_sorted:: get_stat()
{
static struct stat NONSENSE;
	return NONSENSE;
}

