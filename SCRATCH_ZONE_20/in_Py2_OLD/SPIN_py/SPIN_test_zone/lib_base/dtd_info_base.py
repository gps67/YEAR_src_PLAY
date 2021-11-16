# python component
# dtd_info.py
# holds html elem_info
# no other dtd info avail - sorry

import types
import spin_py
import traceback

class Dtd_info_base:
	"""
		DTd_info.get_elem_info("INTRO") -> inst_ELEM_INFO_type
		this data comes from the script (not the dtd)
		each holds { is_PRE, element_breaks+word,  ... }
		used by the SPIN space handler (optimised to C-vars)
		used by the TREE printer, (omit_end_tag ..)
		used by the TEXT gui layout (one day eh)
		ie elem_info holds keys to style_sheet settings
		which includes names of python callbacks, tcl exprs, panels,
	"""

	def dgb( self, msg ):
		msg2 = "# "+ self.__class__.__name__ +": "+ msg
		print msg2
		return msg2

	def __init__( self, pool ):
		self.debug_2 = 0 # messages
		if self.debug_2:
			print "Dtd_info __init__"
		# top.xxx
		self.elem_info = pool
		#
		# copy old dict to new one
		# the items are shared, but the collection is independent!
		#
		# # # CANNOT COPY - must belong to correct pool !
		# # # for (name, elem) in elems.items():
		# # # 	self.elem_info.dict_elem_info[ name ] = elem
		#
		# keep some types - mk_str needs this early
		#
		STR1 = spin_py.SPIN_string( 'STR1' )
		self.type_spin_str = type( STR1 )
		#
		self.init_SGML_ERROR()
	
	def init_SGML_ERROR( self ):
		#
		# define a tag to get SGML errors shown in documents
		# retain existing if it already exists
		# entirely scripted
		# can appear anywhere
		#
		SGML_ERROR = "SGML_ERROR" # spelling now reserved
		try:
			junk = self.elem_info.dict_elem_info[ SGML_ERROR ]
		except:
			# inline is probably more correct
			# block looks better
			self.set_elem_inline( SGML_ERROR )
			#self.set_elem_block( SGML_ERROR )

	# now unused - ifdef USING_PY_STRINGS
	def mk_str( self, name ):
		return name
		#
		# this was an attempt to define an alternative to PyString
		# for non ASCII documents
		# reverting to UTF whilst waiting for inspiration
		#
		if type(name) == self.type_spin_str:
			pass
		elif type(name) == types.StringType:
			print "dtd_info::mk_str Converting name:", name
			name = spin_py.SPIN_string(name)
		else:
			print 'dtd_info.set_elem_info - not a string'
			print 'name =', name
			raise 'not a string'
		return name

	def set_elem_info( self, name, etype, ctype, breaks ):
		""" create and register an elem_info """
		try:
			el = self.elem_info.dict_elem_info[ name ]
			# this is less than ideal
			# should set individual items
			# eg ctype probably now wrong (some script default)
			# print "Overriding settings for:", name
			el.ctype = ctype
			el.element_breaks_word = breaks
		except:
			name = self.mk_str( name )
			el = spin_py.SPIN_elem_info( self.elem_info, name, etype, ctype, breaks )
		if self.debug_2:
			print "set_elem_info", name, etype, ctype, breaks
			self.show_elem_info( name )
		return el

	def get_elem_info( self, name ):
		name = self.mk_str( name )
		try:
			t = self.elem_info.dict_elem_info[ name ]
		except:
			t = None
			msg = "get_elem_info( "+name+" ) - no such name"
			raise( self.dgb( msg ))
		return t

	def new_elem_inst( self, name ):
		""" eg HTML = dtd_info.new_elem_inst( "HTML" ) """
		info = self.get_elem_info( name )
		inst = spin_py.SPIN_elem_obj( info, [], [] ) 
		return inst

	# macros that make elem_info's

	def set_elem_block( self, name ):
		etype="block"
		ctype="pcdata"
		breaks = 1
		return self.set_elem_info( name, etype, ctype, breaks )

	def set_elem_inline( self, name ):
		etype="inline"
		ctype="pcdata"
		breaks = 0
		return self.set_elem_info( name, etype, ctype, breaks )

	def set_elem_is_pre( self, name ):
		etype="pre"
		ctype="pcdata"
		breaks = 1
		return self.set_elem_info( name, etype, ctype, breaks )

	def set_elem_empty( self, name ):
		etype="empty"
		ctype="pcdata"
		breaks = 1 # PROBABLY
		return self.set_elem_info( name, etype, ctype, breaks )

	def set_elem_phrase( self, name ):
		etype="phrase"
		ctype="mixed"
		breaks = 1 # PROBABLY
		return self.set_elem_info( name, etype, ctype, breaks )

	def set_elem_defaults( self, name ):
		etype="block"
		print "set_elem_defaults(", name, ")"
		# spin couldn't find elem_info
		# so created an inline elem
		# you can change the defaults
		# allow script to call this without changing settings
		#
		# CHANGE: I think that it is ALWAYS ALREADY in dict
		# CHANGE: this is due to the elem_info PyNEW construction
		try:
			t = self.elem_info.dict_elem_info[ name ]
		except:
			t = self.set_elem_inline( name )
			# t = self.set_elem_block( name )
		if self.debug_2:
			self.show_elem_info( name )
		return t


	# utilities to print and debug elem_info's

	def show_elem_info( self, name ):
		elem = self.get_elem_info(name)
		return self.show_elem_info_elem( elem )

	def show_elem_info_elem( self, info ):
		# print "elem_info.dict_elem_info[%s]" % name
		self.show_elem_info_line( info, 'name' )
		self.show_elem_info_line( info, 'is_pre' )
		self.show_elem_info_line( info, 'element_breaks_word' )
		dry = info.atts
		if dry != None:
			for att in dry.keys():
				self.show_elem_info_line( info, att )

	def show_elem_info_line( self, info, field ):
		name = getattr( info, "name" )
		val = getattr( info, field )
		print "%11s %20s: %s " %(name, field, val)

	def end_report( self ):
		dry = self.elem_info.dict_elem_info
		blocks = []
		inline = []
		print "-- BLOCK element_breaks_word --"
		for (name, elem) in dry.items():
			if elem.element_breaks_word:
				print "   ", name
		print "-- Inline elements --"
		for (name, elem) in dry.items():
			if not(elem.element_breaks_word):
				print "   ", name
		print "-- is_pre elements --"
		for (name, elem) in dry.items():
			if elem.is_pre:
				print "   ", name
		print "-- is_empty elements --"
		for (name, elem) in dry.items():
			if elem.is_empty:
				print "   ", name
		return
		print "-- debugging elem info --"
		for name in ['VERB', 'P', 'HTML' ]:
			try:
				self.show_elem_info(name)
			except: pass
		# self.show_elem_info('BF')
		# self.show_elem_info('LINUXDOC')
		# self.show_elem_info('DATE')
		# self.show_elem_info('HTMLURL')
