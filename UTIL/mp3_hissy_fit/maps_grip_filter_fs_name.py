# -*- coding: utf-8 -*-
# NOT #  -*- coding: iso-8859-15 -*-

# maps_grip_filter_fs_name.py
##########################.py

# converts a string to a filename
# it used to follow 'grip'
# it now does what I want
# TODO: a range of flavours (each carefully exact)
# TODO: rename functions
# TODO: export less, a class, its api, some factories("grip")


import string
import maps_plausible_list

#############################################################################

# this is very contentious - should build complete charset mapping, and utf8
# for now, using grip to call cdparanoia is a comparison of its names !
# there is also the LOCALE of the filesystem (8859, utf8, XXXX)
# http://127.0.0.1/Docs/Python-Docs-2.4/lib/module-codecs.html

# worse - I think UTF is seen as each byte sep!
# worse - I think vi is writing the source file as utf8 (so adopted)

# VI # :set
# VI # fileencoding=utf-8
# VI # fileencodings=ucs-bom,utf-8,latin1


def grip_filter_fs_name_1( str ):
	# _1 is unused see: grip_filter_fs_name( ustr ):
	# grip has odd rules
	# we copy it to get its wav files
	# and general debugging
	# but maybe move to my_filter_fs_name()
	#
	# NON ASCII # str = str.replace( "´", '_' )
	str = str.replace( "´", '_' )
	str = str.strip()
	str = str.lower()
	str = str.replace( "'", '_' )
	str = str.replace( "'", '_' )
	str = str.replace( ",", '_' )
	str = str.replace( ".", '_' )
	str = str.replace( '-', '_' ) # hyphenated name bodged by grip (hate)
	str = str.replace( ".", '_' ) # PREFER  as _ not '' ABOVE __ dr_name
	str = str.replace( ":", '_' ) # PREFER  as _ not '' ABOVE __ dr_name
	str = str.replace( ' ', '_' )
	str = str.replace( '?', '_' )
	str = str.replace( '(', '_' )
	str = str.replace( ')', '_' )
	# where this happens is false, before &
	str = str.replace( '___', '_' )
	str = str.replace( '__', '_' )
	str = str.replace( "&", '' )
	# test grip against these
	str = str.replace( '/', '' ) # grip simply removes
	# chop trailing _
	str = str.strip(' 	_')
	return str

grip_filter_fs_name_map = None

#def pair( map, hex, letter):
#	c1 = chr( hex )
#	map[c1] = letter

def map_lower_to_upper( map ):
	ord_c1 = ord('A')
	ord_c2 = ord('a')
	ord_z = ord( 'Z' )
	while ord_c1 <= ord_z:
		c1 = chr( ord_c1 )
		c2 = chr( ord_c2 )
		map[ c2 ] = c1
		ord_c1 += 1
		ord_c2 += 1

def map_upper_to_lower( map ):
	ord_c1 = ord('A')
	ord_c2 = ord('a')
	ord_z = ord( 'Z' )
	while ord_c1 <= ord_z:
		c1 = chr( ord_c1 )
		c2 = chr( ord_c2 )
		map[ c1 ] = c2
		ord_c1 += 1
		ord_c2 += 1

def map_plausible_ascii_alternatives( map ):
	""" what you call plausible will vary ...
	"""
	lst = maps_plausible_list.plausible_list()
	for ( c1_hex, c1_vis, c2_ascii ) in lst:
		c2 = map[ c2_ascii ]
		# print "mapping '%s' --> '%s'" % (c1_hex, c2)
		map[ c1_hex ] = c2

def map_print_item( map, c1 ):
	c2 = map[ c1 ]
	print "'%s' --> '%s'" % (c1, c2 )

def map_print( map ):
	print map
	map_print_item( map, ' ' )
	map_print_item( map, '\xEF' )

def get_grip_filter_fs_name_map():
	global grip_filter_fs_name_map
	if grip_filter_fs_name_map:
		return grip_filter_fs_name_map

	grip_filter_fs_name_map = {}
	map = grip_filter_fs_name_map
	# default '_' everywhere
	for vc1 in range( 0, 255 + 1 ):
		c1 = chr( vc1 )
		map[ c1 ] = "_"
	# map straight through a-z, 0-9, A-Z
	groups = (string.ascii_letters, string.digits )
	for strs in groups:
	 for c1 in strs:
		# c2 = c1.lower()
		map[ c1 ] = c1
	# I'd pref it if caps were kept, but ...
	map_upper_to_lower( map )
	# this must come after the basic A - Z
	map_plausible_ascii_alternatives( map )
	#
	# map_print( map )
	return grip_filter_fs_name_map

def grip_filter_fs_name( ustr ):
	# this is the exported function
	map = get_grip_filter_fs_name_map()
	s1 = ustr.encode( "iso-8859-1" )
	s1 = s1.replace( "'", '' ) # apostraphe
	s1 = s1.replace( "'", '' ) # apostraphe
	s2 = '_'
	for c1 in s1:
		c2 = map[c1]
		s2 = s2 + c2
	s2 = s2 + "_"
	# spelling fixups (some based on apostraphe)
	s2 = s2.replace( '_it_s_', '_its_' )
	s2 = s2.replace( '_you_ve_', '_youve_' )
	s2 = s2.replace( '_can_t_', '_cant_' )
	s2 = s2.replace( '_didn_t_', '_didnt_' )
	s2 = s2.replace( '_sex_o_matic_', '_sexomatic_' )
	s2 = s2.replace( '_i_ve_', '_ive_' )
	s2 = s2.replace( '_i_m_', '_im_' )
	s2 = s2.replace( '_re_mix_', '_remix_' )
	s2 = s2.replace( '_s_', 's_' )
	s2 = s2.replace( '___', '_' )
	s2 = s2.replace( '__', '_' )
	s2 = s2.strip(' 	_')
	return s2
	return grip_filter_fs_name_1(s1)

# Is there a place
# for the hopeless sinner,
# Who has hurt all mankind
# just to save his own beliefs?


