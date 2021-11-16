class printer:

	def __init__( self, fd ):
		self.fd = fd
	def write( self, str ):
		self.fd.write( str )


global_done_list = {}
def done_check( item ):
	global global_done_list
	try:
		t = global_done_list[ item ]
		return 1
	except:
		global_done_list[ item ] = item
		return 0

global_used_dict = {}
global_used_list = []
def used_check( item ):
	if used_check_noadd( item ):
		return 1
	used_list_add( item )
	return 0

def used_list_add( item ):
	global global_used_list
	global_used_list.append( item )

def used_check_noadd( item ):
	global global_used_dict
	try:
		t = global_used_dict[ item ]
		return 1
	except:
		# filter out DONE items (possible self inclusion)
		try:
			t = global_done_list[ item ]
			return 1
		except:
			pass
		global_used_dict[ item ] = item
		return 0

def used_list():
	# return the list of items used IN ORDER of first use
	# reset the list
	global global_used_dict
	global global_used_list
	used_list = global_used_list
	global_used_dict = {}
	global_used_list = []
	return used_list

def uses( item ):
	try:
		item.used()
	except:
		pass

class dtd_loop:
	def __init__( self, dtd ):
		self.dtd = dtd

#
# dtd_rule_XXX
#

class dtd_rule_anon:

	def used( self ):
		if used_check_noadd( self ): return
		try:
			if self.rule:
				self.rule.used()
		except:
			pass
		try:
			for item in self.list:
				item.used()
		except:
			pass
		# anon rules have no declaration ... # 
		# NO NEED # used_list_add( self )

	def print2_decl( self, out ):
		pass

# (self-)building rules also have a ref to dtd ...

class dtd_rule_bracketed( dtd_rule_anon ):
	def __init__( self, contents=None ):
		self.rule = contents
	def print2( self, out ):
		out.write( "(" )
		self.rule.print2(out)
		out.write( ")" )

class dtd_rule_seq( dtd_rule_anon ):
	def __init__( self ):
		self.list = []
	def print2( self, out ):
		sep = ""
		for item in self.list:
			out.write( sep )
			sep = ","
			item.print2(out)
	def add( self, item ):
		self.list.append( item )

class dtd_rule_any( dtd_rule_anon ):
	def __init__( self ):
		self.list = []
	def print2( self, out ):
		out.write( " " )
		sep = ""
		for item in self.list:
			out.write( sep )
			sep = " | "
			item.print2(out)
		out.write( " " )
	def add( self, item ):
		self.list.append( item )

class dtd_rule_opt( dtd_rule_anon ):
	def __init__( self, rule ):
		self.rule = rule
	def print2( self, out ):
		self.rule.print2(out)
		out.write( "?" )

class dtd_rule_star( dtd_rule_anon ):
	def __init__( self, rule ):
		self.rule = rule
	def print2( self, out ):
		self.rule.print2(out)
		out.write( "*" )

class dtd_rule_plus( dtd_rule_anon ):
	def __init__( self, rule ):
		self.rule = rule
	def print2( self, out ):
		self.rule.print2(out)
		out.write( "+" )

# function (not a class)

def dtd_rule_bracketed_star( rule ):
	return dtd_rule_star( dtd_rule_bracketed( rule ))

def dtd_rule_bracketed_plus( rule ):
	return dtd_rule_plus( dtd_rule_bracketed( rule ))

## named rule

class dtd_named_item:

	def __init__( self, dtd, itemtype, name ):
		dtd.map_name[ name ] = self
		dtd.list_decl.append( self )
		self.itemtype = itemtype
		self.name = name

	def used( self ):
		return used_check( self )
		# and all dependencies ...

	def print2( self, out ):
		# named rules print as name, some as %name
		self.print2_name( out )
		self.used()

	def print2_name( self, out ):
		# really REFERENCE not NAME. Opposite of DECL
		out.write( self.name )

	def print2_decl( self, out ):
		print "ERROR: print2_decl():", self
		raise "attribute error - need function print2_decl()"
		# if self.rule:
		# 	self.rule.print2( out )

class dtd_entity_rule( dtd_named_item ):
	"""
		a named rule holder (strongly typed macro)
	"""
	def __init__( self, dtd, name, rule, comment=None ):
		dtd_named_item.__init__( self, dtd, "entity", name )
		self.rule = rule
		self.comment = comment
	def used_late( self ):
		if used_check_noadd( self ): return
		if self.rule:
			self.rule.used()
		used_list_add( self )
	def used( self ):
		if used_check( self ): return
		if self.rule:
			self.rule.used()
	def print2_name( self, out ):
		out.write( '%' )
		out.write( self.name )
	def print2_decl( self, out ):
		if done_check( self ): return
		if self.comment:
			out.write( '<!--\n\t%s\n-->\n' % self.comment )
		out.write( '<!entity % ' + self.name + ' "' )
		self.rule.print2(out)
		out.write( "\" >\n" )

class dtd_entity_sdata_char( dtd_named_item ):

	def __init__( self, dtd, name, charstr, comment ):
		dtd_named_item.__init__( self, dtd, "entity", name )
		self.charstr = charstr
		self.comment = comment
	def print2_name( self, out ):
		out.write( '%' )
		out.write( self.name )
	def print2_decl( self, out ):
		if done_check( self ): return
		out.write( '<!entity %-8s CDATA "%s" -- %s -- >\n' % (
			self.name,
			self.charstr,
			self.comment
		))

class dtd_entity_macro( dtd_named_item ):

	def __init__( self, dtd, name, decl, comment ):
		dtd_named_item.__init__( self, dtd, "entity", name )
		self.decl = decl
		self.comment = comment
	def print2_name( self, out ):
		# NOT # out.write( '%' )
		out.write( self.name )
	def print2_decl( self, out ):
		if done_check( self ): return
		out.write( '<!entity %-8s \'%s\' -- %s -- >\n' % (
			self.name,
			self.decl,
			self.comment
		))

## usemap

class dtd_shortref_mapping:
	def __init__( self, left, right ):
		self.left = left
		self.right = right

	def print2_out( mapping, out ):
		out.write( "\n\t" )
		try:
			mapping.left.print2( out )
		except:
			out.write( mapping.left )
		out.write( " " )
		try:
			mapping.right.print2( out )
		except:
			out.write( mapping.right)

	def used( self ):
		if used_check( self ): return
		uses( self.left )
		uses( self.right )

	def print2_decl( self, out ): pass

class dtd_shortref_map( dtd_named_item ):
	def __init__( self, dtd, name ):
		dtd_named_item.__init__( self, dtd, "map", name )
		self.map = []

	def add( self, left, right ):
		mapping = dtd_shortref_mapping( left, right )
		self.map.append( mapping )

	def used( self ):
		if used_check_noadd( self ): return
		for mapping in self.map:
			mapping.used()
		used_list_add( self )

	def print2_decl( self, out ):
		if done_check( self ): return
		s1 = "<!shortref " + self.name
		out.write( s1 )
		for mapping in self.map:
			mapping.print2_out( out )
		out.write( ">\n" )

##  -------

class Attr_Base:

	def print2( self, out ):
		s1 = "  %-9s %-9s %-14s -- %s --\n" % (
			self.name,
			self.cdata,
			self.implied,
			self.comment
		)
		out.write( s1 )

class Attr( Attr_Base ):

	def __init__( self, name, cdata="CDATA", implied="#IMPLIED", comment="" ):
		self.name = name
		self.cdata = cdata
		self.implied = implied
		self.comment = comment

class Attr_enum( Attr_Base ):

	def __init__( self, name, list, default, comment ):
		self.name = name
		sep = ""
		s = "("
		for item in list:
			s = s + sep + item
			sep = "|"
		s = s + ")"
		self.cdata = s
		self.implied = default
		self.comment = comment

class dtd_element( dtd_named_item ):

	def __init__( self, dtd, name, rule=None ):
		dtd_named_item.__init__( self, dtd, "element", name )
		self.shortref = None
		self.opentag = "-"
		self.closetag = "o"
		self.rule = rule
		self.attlist = None

	def usemap( self, map ):
		self.shortref = map

	def used( self ):
		if used_check_noadd( self ): return
		uses( self.shortref )
		uses( self.rule )
		used_list_add( self )

	def print2_decl( self, out ):
		if done_check( self ): return
		if self.shortref:
			self.shortref.print2_decl(out)
			out.write( '<!usemap %s %s>\n' % (
				self.shortref.name,
				self.name
			))
		out.write( "<!element %s %s %s " % (
			self.name,
			self.opentag,
			self.closetag
		))
		if self.rule:
			self.rule.print2(out)
		else:
			# out.write( "<!--NONE-->" )
			out.write( "EMPTY" )
			raise self.name
		out.write( " >\n" )
		if self.attlist:
			out.write( "<!attlist " )
			out.write( self.name )
			out.write( "\n" )
			for att in self.attlist:
				att.print2( out )
			out.write( ">\n" )

	def add_attr( self, attr ):
		if not self.attlist: self.attlist = []
		self.attlist.append( attr )

class dtd_rule_predeclared( dtd_named_item ):
	def __init__( self, dtd, itemtype, name ):
		dtd_named_item.__init__( self, dtd, itemtype, name )

	def print2_name( self, out ):
		out.write( self.name )

	def print2_decl( self, out ):
		# there is no declaration
		pass

class dtd_rule_pcdata( dtd_rule_predeclared ):
	def __init__( self, dtd ):
		dtd_rule_predeclared.__init__( self, dtd, "#", "#PCDATA" )

class dtd_rule_empty( dtd_rule_predeclared ):
	def __init__( self, dtd ):
		dtd_rule_predeclared.__init__( self, dtd, "#", "EMPTY" )

class dtd_builder:
	def __init__( self, out ):
		self.out = out
		self.map_name = {}
		self.list_decl = []
		self.rule_pcdata = dtd_rule_pcdata( self )
		self.rule_empty = dtd_rule_empty( self )

	def print_out( self ):
		for decl in self.list_decl:
			decl.print2_decl( self.out )

	def print_tree( self, decl=None ):
		uses( decl )
		self.print_used()

	def print_used( self ):
		# this is good when limiting to reachable items from top
		l = used_list()
		while len(l):
			for decl in l:
				decl.print2_decl( self.out )
			l = used_list()

	def element( self, name, rule=None ):
		try:
			e = self.map_name[ name ]
			if rule:
				e.rule = rule
		except:
			e = dtd_element( self, name, rule )
		return e

	def entity_rule( self, name, rule, comment="" ):
		e = dtd_entity_rule( self, name, rule, comment )
		return e

	def shortref_map( self, name ):
		m = dtd_shortref_map( self, name )
		return m

	def entity_macro( self, name, decl, comment=None ):
		return dtd_entity_macro( self, name, decl, comment )

	def entity_sdata_char_numb( self, name, charnumber, comment ):
		charstr = '&#%d;' % charnumber
		C = dtd_entity_sdata_char( self, name, charstr, comment )
		C.used()
		return C
