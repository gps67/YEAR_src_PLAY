#include "dgb.h"
// #include "ints.h"
// #include <stdio.h>
#include "file_stat.h"
#include "dir_name_ext.h"
#include "fd_hold.h"
#include "file_rename.h"
#include "copy_restart.h"
#include "dir_list.h"
#include "dir_reader_sorted.h"
#include "fs_mkdir.h"
// typedef unsigned int uns;

bool copy_tree(
	const char * src_tree,
	const char * dst_over
) {
	INFO("src_tree     %s", src_tree );
	INFO("dst_over     %s", dst_over );

	file_stat stat_item;
	if(!stat_item.stat_expect_is_dir( dst_over )) {
		return FAIL_FAILED();
	}

	dir_reader_base dir_list;
//	dir_reader_sorted dir_list;
	if(!dir_list.open( src_tree )) {
		FAIL("%s", src_tree );
		return FAIL_FAILED();
	}

	buffer1 src_sub_dir;
	buffer1 dst_sub_dir;

	while( dir_list.next() ) {
		INFO("DIR ITEM %s %s",
		 dir_list.name(), 
		 dir_list.item.file_type_str()
		);
		str0 name = dir_list.name();
		if( name.ends_with(".cpy") ) {
			INFO("skipping .cpy %s", (STR0) name );
		}
		// TODO FIXUP NOW have growing list of ALL types to redup
		switch( dir_list.item.file_type ) {
		 case is_dir: 
		 	src_sub_dir.clear();
			src_sub_dir.print("%s/%s", src_tree, (STR0) name);
		 	dst_sub_dir.clear();
			dst_sub_dir.print("%s/%s", dst_over, (STR0) name);
			if(!stat_item.stat( (STR0) dst_sub_dir )) { // quiet stat for absent
				// INFO("TODO MKDIR %s", (STR0) dst_sub_dir ); // stat found absent
			}
			switch( stat_item.linked_file_type ) {
			 case is_dir: ; // perfect already exists
			 	INFO("OK already exists %s", (STR0) dst_sub_dir );
			 break;
			 case is_absent: // perfect mkdir
			 	if(!fs_mkdir( dst_sub_dir )) return FAIL_FAILED();
			 break;
			 default: // anything else is in the way 
			 	// not a good thing
				// MAYBE allow symblink to alternative ?
			 	return FAIL("NOT A DIR %s %s", 
				 (STR0) dst_sub_dir,
				 stat_item.linked_file_type_str()
				);
			}
			if(!copy_tree( 
			 (STR0) src_sub_dir,
			 (STR0) dst_sub_dir
			)) {
				return FAIL_FAILED();
			}

		 break;
		 case is_file:
		 case is_link:
		 	if(!copy_src_name_dst( src_tree, dir_list.name(), dst_over ) ) {
				return FAIL_FAILED();
			}
		 break;
		 default:
		 	if(!copy_src_name_dst( src_tree, dir_list.name(), dst_over ) ) {
				return FAIL_FAILED();
			}
			return true;
		 	return FAIL("not a plain dir or file %s %s",
				dir_list.name(),
				dir_list.item.file_type_str() );
		}
	}

	dir_list.close();
	return true;
}
