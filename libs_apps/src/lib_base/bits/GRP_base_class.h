#ifndef GRP_base_class_H
#define GRP_base_class_H // for doxygen, tree, group by lib_origin_etc



/*
	Most things include this file, so this is where I define
	the empty structs that different libraries use as base classes
	to indicate which group they belong to (DOXYGEN)

	ie these bases only available here - know above ints are from here
*/

struct GRP_apps {};
struct GRP_lib_accts {};

struct GRP_lib_base {};
struct GRP_lib_base_fd   : public GRP_lib_base {};
struct GRP_lib_base1_glib : public GRP_lib_base {};

struct GRP_lib_base2 {};
// struct GRP_lib_base2_%s {}; public GRP_lib_base2 {}; // LIBR(%s) // # OBJ #
// struct GRP_lib_base2_%s {}; # OBJ # public GRP_lib_base2_obj {}; // LIBR(%s) // # OBJ #
struct GRP_lib_base2_x11 : public GRP_lib_base2 {};
struct GRP_lib_base2_obj : public GRP_lib_base2 {};
struct GRP_lib_base2_pdf : public GRP_lib_base2 {};
// maybe move this to SPOUT_GEN namespace
struct GRP_lib_base2_spout : public GRP_lib_base2 {};
struct GRP_lib_base2_js  : public GRP_lib_base2 {};
struct GRP_lib_base2_gtk : public GRP_lib_base2 {};

struct GRP_lib_inet {};
struct GRP_lib_media {};
struct GRP_lib_play {};
struct GRP_sar_data {};
struct GRP_sar_parse {};
struct GRP_sar_plot {};

#endif

