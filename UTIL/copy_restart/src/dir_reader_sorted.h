// UNUSABE ATM
#include "dir_list.h"
// #include "dir_reader.h"

#include "obj_list.h"

#include "obj_list_sorter.h"
#include "sort_spec_base.h"

class name_stat : public obj_ref
{
 public:
	str1 name;
	struct stat linked_st;

	name_stat( STR0 name );
};

class sort_spec_one : public sort_spec_base
{
 public:
};

class dir_reader_sorted { // : public dir_reader_base {

	obj_list<name_stat> name_stat_list;
	int pos;
 public:
 	bool open( const char * path );
	bool sort_by_name();
	bool sort_by_mtime();
	bool sort_by_size();
 	bool close();
	bool next();
	const char * name();
	struct stat & get_stat();
};
