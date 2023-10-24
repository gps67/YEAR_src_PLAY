#!/usr/bin/env tclsh

set NAME NAME

proc rename_IMG_dir {dir} {
	global NAME
	puts "# name $NAME # rename_IMG_dir $dir"
}

proc list_pop {listname itemname } {
	upvar $listname list
	upvar $itemname item
	if { $list == {} } {
		return 0
	}
	set item [lindex $list 0]
	set list [lrange $list 1 end]
	return 1
}

# push might use VAL or by_VAR # I like by_VAR except slower

proc my_main {list} {
	global NAME
	while {[ list_pop list item ]} {
		puts "# item # $item # list '$list' #"
		if { $item == "--NAME" } {
			list_pop list NAME
			continue
		}
		rename_IMG_dir $item
	}
	return
}

my_main $argv

