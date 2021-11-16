# python component
# spin_eh_num
# derived from the generic handler for SPIN events

# the base class
from spin_eh import spin_py, SP_EH


# Dtd_info is a dictionary of elem_info items
## from dtd_info import Dtd_info
# the base class has an overridable setup_HTML
# but we PRESUME HTML and do our setup anyway
# SHOULD catch that vector and customise to DTD

# standard python

# import traceback
# import types
# import sys
import re	# used to munge labels  (conv . to _ )
# import toc_tree	# the filter that does the toc renumbering
# nb  the "re-" in 'renumbering' would require removing the original numbers
# that would mean a RE to match [0-9.]+ or any tags aroung the numbers
# that would also forbid any title from starting with a number
# to be added when the source is not available un-numbered
# (However the DIV.TOC does get re-constructed)

class SP_EH_NUM( SP_EH ):
	def dgb( self, msg ):
		print "# SP_EH:",msg

	def __init__(self, name ):
		# parent constructor
		SP_EH.__init__( self, name )
		self.debug3 = 0
		# elemen_info's used by TOC_TREE
		# pre-registered elem_info's
		self.ul  = self.dtd_info.set_elem_block('UL')
		self.li  = self.dtd_info.set_elem_block('LI')
		self.div = self.dtd_info.set_elem_block('DIV')
		self.body= self.dtd_info.set_elem_block('BODY')
		self.title=self.dtd_info.set_elem_defaults('TITLE')
		self.br  = self.dtd_info.set_elem_empty('BR')
		self.hr  = self.dtd_info.set_elem_empty('HR')
		self.h1  = self.dtd_info.set_elem_block('H1')
		self.h2  = self.dtd_info.set_elem_block('H2')
		self.h3  = self.dtd_info.set_elem_block('H3')
		self.a   = self.dtd_info.set_elem_inline('A')
		# pre-built elem_obj's (only works for empty)
		self.br_el = spin_py.SPIN_elem_obj( self.br, [], [] )
		self.hr_el = spin_py.SPIN_elem_obj( self.hr, [], [] )

		self.lvl = {}
		self.lvl['H1'] = 1
		self.lvl['H2'] = 2
		self.lvl['H3'] = 3
		self.lvl_curr = 0
		self.list = {}
		self.list[0] = []	# holds entire result
		self.list[1] = []
		self.list[2] = []
		self.list[3] = []
		self.count = {}
		self.count[1] = 0
		self.count[2] = 0
		self.count[3] = 0
		self.div_toc = None
		self.kept_title = "NO-TITLE"

		self.request_callbacks_on_elem_infos()

	def request_callbacks_on_elem_infos( self ):
		#
		self.h1.on_element_end = self.ElementEnd_H
		self.h2.on_element_end = self.ElementEnd_H
		self.h3.on_element_end = self.ElementEnd_H

		self.div.on_element_end = self.ElementEnd_DIV
		self.body.on_element_end = self.ElementEnd_BODY
		self.title.on_element_end = self.ElementEnd_TITLE

	# # # useful macro # also in base class
	# # def SPIN_tree_add_str( self, str ): 
	# # 	""" add text or element to tree building process """
	# # 	# maybe one day str is not tree elem
	# # 	self.SPIN_tree_add( self, str )

	# default callback is generally unused
	def ElementEnd( self, info, name, elem ):
		if self.debug3: print "ElementEnd", name, "---------------"
		self.toc.filter( name, elem )
		# z remains a part of elem, so z.insert works by side effect
		self.SPIN_tree_add( elem )
		return

	#
	# CALLBACKS
	#
	# Really toc_filter should be this class
	# I was experimenting with filter 'pipeline' ideas
	#

	def ElementEnd_TITLE( self, info, name, elem ):
		if self.debug3: print "ElementEnd_TITLE", name
		self.kept_title = elem.data
		self.SPIN_tree_add( elem )
		return

	def ElementEnd_DIV( self, info, name, elem ):
		if self.debug3: print "ElementEnd_DIV", name
		for a in elem.atts:
			if (a[0] == "CLASS") and (a[1] == "TOC"):
				del elem.data[:]
				self.div_toc = elem
				break
		# if <DIV class="TOC"> isnt found, it will get created
		# contents are deleted later
		self.SPIN_tree_add( elem )
		return

	def ElementEnd_BODY( self, info, name, elem ):
		# end of HTML's BODY, so finish up here
		# could add other data before end of HTML
		if self.debug3: print "ElementEnd_BODY", name
		# we are editing the elem "in-situ" and plan
		# to add it anyway, so add first !
		self.SPIN_tree_add( elem )
		# get the TOC to include
		el = self.end_view()
		if self.div_toc == None:
			print "div_toc NULL - ie not found in doc"
			# graft into BODY, with elem_obj top
			elem.data.insert( 0, el )
		else:
			# print "div_toc FOUND and USED"
			# graft into marked sub-place, without elem_obj top
			self.div_toc.data = el.data
		# use the HTML/HEAD/TITLE/* as an H1 at the top
		# this where the H1 is presumed to be single!
		el = spin_py.SPIN_elem_obj( self.h1, [], self.kept_title )
		elem.data.insert(0, el )
		return

	def ElementEnd_H( self, info, name, elem ):
		# used by H1 H2 H3 occuring in source document
		if self.debug3: print "ElementEnd_H", name

		try:
			l = self.lvl[name]
		except:
			print "filter_H %s but no lvl" % name
			return
		# complete any sub-level index (H2 closes H3 list)
		self.close_down( l )
		# calc number
		self.count[l] = self.count[l] + 1
		i = 1
		point = ""
		nnstr = ""
		while i <= l:
			nnstr = nnstr + point + "%d" % self.count[i]
			point = "."
			i = i + 1
		lbl = "HDR_" + nnstr
		# CRASH the following crashes mod_apache but not shell
		# also it was OK a few minutes ago !!
		# maybe picking up PATH or LD_LIBRARY_PATH
		# ie pcre installed as a sep module + X-talk ??
		if 0:
			# label can/not contain certain chars
			# but \. is not a problem so defer
			lbl = "HDR_" + re.sub( "\.", "_", nnstr, 0 )
		#
		# remove text from input - and keep place
		# (e,a,z) = elem.trip()
		# COPY the data into two zones and clear the original
		z1 = elem.data[:]
		z2 = elem.data[:]
		del  elem.data[:]
		#
		# index item
		z2.insert( 0, nnstr + " " )
		link = self.A_href( lbl, z2 )
		if l==1:
			list = self.kept_title[:]
			list.append( link )
			list = [link]
		else:
			list = [link]
		el = spin_py.SPIN_elem_obj( self.li, [], list )
		self.list[l].append( el )
		#
		# main text item
		z1.insert( 0, "(" + nnstr + ") " )
		anchor = self.A_name( lbl, z1 )
		if l==1:
			elem.data.insert( 0, self.hr_el ) # after hdr!
		elem.data.insert( 0, anchor )
		if l==1:
			elem.data.insert( 0, self.hr_el ) # before hdr!
		#
		# NB side effects of editing original structure

		self.SPIN_tree_add( elem )
		return

	## Functions used above

	def close_down( self, level ):
		# clear down all sub-closed
		i = 3
		while i>level:
			lst = self.list[i]
			self.list[i] = []
			self.count[i] = 0
			h = i - 1
			i = i - 1
			if len(lst) == 0: continue # buf if double jump
			el = spin_py.SPIN_elem_obj( self.ul, [], lst )
			if h==0:
				self.top = el
				return
			lst = self.list[h]
			last = lst[len(lst)-1]
			# (e,a,z) = last.trip()
			if i == 1:
				last.data.append( self.hr_el )
			else:
				last.data.append( self.br_el )
			last.data.append( el )
			#self.list[h].append( el )

	def end_view( self ):
		self.close_down(0)
		attr1 = ["class", "TOC"]
		el = spin_py.SPIN_elem_obj( 
			self.div,
			[attr1],
			[ self.hr_el, self.top ]
		)
		# (e,a,z) = el.trip()
		# el = z[0]		# actual bug if empty
		return el

	## convenience functions

	def A_name( self, lbl, zone ):
		a = ["NAME", lbl]
		el = spin_py.SPIN_elem_obj( self.a, [a], zone )
		return el

	def A_href( self, lbl, zone ):
		""" LURK - design - need file cross references """
		a = ["HREF", "#"+lbl]
		el = spin_py.SPIN_elem_obj( self.a, [a], zone )
		return el


