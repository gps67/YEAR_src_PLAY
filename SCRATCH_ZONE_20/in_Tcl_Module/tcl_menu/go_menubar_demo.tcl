#!/usr/bin/env tclsh

# \
lappend auto_path \
/home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_menu \
/home/gps/YEAR/src/PLAY/SCRATCH_ZONE_20/in_Tcl_Module/tcl_menu/MENU \
;#

lappend auto_path \
[pwd] \
[pwd]/MENU \
;#

 package require Tk

# lappend auto_path .
 demo_main_menubar
