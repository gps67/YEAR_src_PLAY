#!/usr/bin/env tclsh

# must run from . (manually)

lappend auto_path [pwd]
lappend auto_path [pwd]/global

namespace eval dtd_gen {}
source dtd_gen.tcl ;# force procs to exist, to help them get resolved
source dtd_element.tcl ;# force procs to exist, to help them get resolved

namespace eval temp_tdoc {

	namespace import ::dtd_gen::*
	namespace import ::dtd_element::*

proc gen_tdoc {} {
	# namespace import ::dtd_gen::*
	# namespace import ::dtd_element::*
	##	subslist			{ S0 | S1 | S2 }
	gen_elem_from_spec {
		name				tdoc
		subslist			{ (titlepag, S0+) }
		is_PRE				0
		add_common_atts_2 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	1
	}
}

proc gen_title {} {
	set tag [gen_elem_from_spec {
		name				title
		subslist			{ %ptext }
		is_PRE				1
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	1
	}]
	dtd_element::add_map_line $tag "&#RS;B&#RE;" titleend
	dtd_element::add_map_line $tag "&#RS;&#RE;" titleend
	OBJ_field_set $tag map_name titlemap
}

proc gen_Sn {sn sub} {
	if {$sub!={}} {
		set sub ", $sub"
	}
	set subslist "{title, intro, plist?$sub}"
	gen_elem_from_spec {
		name				$sn
		subslist			$subslist
		is_PRE				0
		add_common_atts_1 		1
		dtd_element::infer_start_tag	0
		dtd_element::omit_end_tag	1
	}
}

proc gen_intro {} {
	set tag [gen_elem_from_spec {
		name				intro
		subslist			{ P }
		is_PRE				0
		add_common_atts_1 		1
		dtd_element::infer_start_tag	0
		dtd_element::omit_end_tag	1
	}]
# no text in intro - only P elements
#	OBJ_field_set $tag map_name intropmap
#	dtd_element::add_map_line $tag "&#RS;B" null 
#	dtd_element::add_map_line $tag "&#RS;B&#RE;" psplit
#	dtd_element::add_map_line $tag "&#RS;&#RE;" psplit
	return $tag
}

proc gen_plist_macro {} {
	set macro_name plist
	set macro_text (P|PRE|OL|UL|HR|LIST)*
	put "<!entity % $macro_name \"$macro_text\" >"
}

proc gen_plist {} {
	set tag [gen_elem_from_spec {
		name				plist
		subslist			{ %plist }
		is_PRE				1
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	1
	}]
	return $tag
}

proc gen_p {} {

#	# there is no concept of null str from macro!
#	put {<!entity null '' >}

	# I hate PSPLIT but here it is
	put {<!entity pend '</P>' >}
	put {<!entity titleend '</TITLE>' >}
	put {<!entity psplit '</P><P>' >}

	set tag [gen_elem_from_spec {
		name				P
		subslist			{ %ptext }
		is_PRE				0
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	1
	}]

	# adjustments follow
	# this relies on tag not being immediately printed

	OBJ_field_set $tag map_name pmap
#	dtd_element::add_map_line $tag "&#RS;B" null 
 if 0 {
	dtd_element::add_map_line $tag "&#RS;B&#RE;" psplit
	dtd_element::add_map_line $tag "&#RS;&#RE;" psplit
 } else {
	dtd_element::add_map_line $tag "&#RS;B&#RE;" pend
	dtd_element::add_map_line $tag "&#RS;&#RE;" pend
 }
	return $tag
}

proc gen_ptext {} {
	# general problem with adding more to this:
	set macro_name ptext
	set macro_text {(#PCDATA|UL|OL|AHREF|A|CODE|TT|B|I|U)*}
	put "<!entity % $macro_name \"$macro_text\" >"
}

proc gen_PRE {} {
	gen_elem_from_spec {
		name				PRE
		subslist			{ %ptext }
		is_PRE				1
		add_common_atts_1 		1
		dtd_element::infer_start_tag	0
		dtd_element::omit_end_tag	1
	}
}

proc gen_LIST_ITEM {} {
	gen_elem_from_spec {
		name				LIST
		subslist			{ ITEM+ }
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	0
	}

	gen_elem_from_spec {
		name				ITEM
		subslist			{ %ptext }
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	0
	}
}

proc gen_LI {} {
	gen_elem_from_spec {
		name				LI
		subslist			{ %ptext }
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	1
	}
}

proc gen_UL {} {
	gen_elem_from_spec {
		name				UL
		subslist			{ LI* }
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	1
	}
	gen_elem_from_spec {
		name				OL
		subslist			{ LI* }
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	1
	}
}

proc gen_B {tag} {
	gen_elem_from_spec {
		name 				$tag
		subslist 			{ %ptext }
		add_common_atts_1 		1
		dtd_element::infer_start_tag	1
		dtd_element::omit_end_tag	1
	}
}

proc gen_TITLEPAG {} {
	set seq1 { TPTITLE, TPAUTHOR, TPDATE, TPABSTRACT }
	gen_elem_from_spec {
		name 				TITLEPAG
		subslist 			{$seq1}
	}
	gen_elem_from_spec {
		name 				TPTITLE
		subslist 			%ptext
	}
	gen_elem_from_spec {
		name 				TPAUTHOR
		subslist 			%ptext
	}
	gen_elem_from_spec {
		name 				TPDATE
		subslist 			%ptext
	}
	gen_elem_from_spec {
		name 				TPABSTRACT
		subslist 			%ptext
	}
}

proc gen_tdoc_things {} {
	# define macros before use
	gen_plist_macro
	gen_ptext
	# any order for elements
	gen_title
	gen_Sn S0 S1*
	gen_Sn S1 S2*
	gen_Sn S2 S3*
	gen_Sn S3 ""
	gen_intro
	gen_plist
	gen_p
	gen_PRE
	gen_UL
	gen_LI
	gen_LIST_ITEM ; # should be UL ?
	# fgbg attrs are all much the same
	# currently they dont nest
	gen_B B
	gen_B CODE
	gen_B AHREF
	gen_B A
	gen_B TT
	gen_TITLEPAG
	gen_tdoc
}
}
temp_tdoc::gen_tdoc_things 
dtd_gen::gen_file
