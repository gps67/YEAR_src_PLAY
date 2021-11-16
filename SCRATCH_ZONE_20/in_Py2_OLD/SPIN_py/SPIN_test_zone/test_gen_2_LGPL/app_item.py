
app_list = []
app_dict = {}

import os
import stat
import re_cache
from vfs import os_listdir_cached

# concurrent access is limited to 1
RE = re_cache.re_cache()                                                         

def odd_even():
	global odd_even_var
	try:
		b = odd_even_var
	except:
		print "INIT"
		b = 0
	b = not( b )
	odd_even_var = b
	return b
def odd_even_str():
	b = odd_even()
	if b:
		return "ODD"
	else:
		return "EVEN"


def cmp_app_item_date( a, b ):
	return cmp( a.str_date, b.str_date)

def app_list_sorted_by_date():
	global app_list
	l2 = app_list[:]
	l2.reverse()
	for item in l2:
		item.find_file_dir0()
	l2.sort( cmp_app_item_date )
	l2.reverse()
	return l2
#
# app_item is a downloadable project 
# its fields are set in a script (could be XML file)
# POM = Python Object Model ...
# the downloadable files MUST be tagged with a date, in dir0
#
class app_item:

	def __init__( self, name, left="" ):
		key= left + "::" + name
		global app_list
		global app_dict
		app_list.append( self )
		app_dict[ name ] = self

		self.group = None
		self.alias = None
		self.name = name
		self.name_left = left
		self.label = name
		self.desc_line = "TBS"
		self.desc_text = "TBS"
		self.lang = "TBS"
		self.status_1 = "demotest"
		self.status_2 = "prototype"
		self.str_date = "1999-12-31"
		# snippet simple-script prototype fun-run beta stable solid

	def find_file_dir0( self ):
		dir0 = "/home/gps/2000/SITE/www.information-cascade.co.uk/htdocs"
		dir0="/home/gps/2000/SITE/www.gps.uklinux.net/cascade"
		dir0="/home/gps/2000/SITE/www.gps.talktalk.net"
		dir1= dir0 + "/LGPL"
		return self.find_file( dir1 )

	def find_file( self, dir ):
		# this app_item knows its own FILE name
		if self.alias:
			name = self.alias
		else:
			name = self.name
		#
		# sweep through lots of similarly named files
		#	name-2004-12-31.tgz
		# keep the most recent NAME (or none)
		# warn: doesnt check for DIR, for .tgz -vs- .odd
		#
		R = "%s-([0-9]{4}-[0-9]{2}-[0-9]{2})(.*)" % name
		str_date_recent = None
		str_tail_recent = None
		f_recent = None
		for f in os_listdir_cached( dir ):
			if RE.run( f, R ):
				# print "++ Match", f, R
				str_date = RE.group( 1 )
				str_tail = RE.group( 2 )
				if str_date_recent:
					if str_date < str_date_recent:
						continue
				str_date_recent = str_date
				str_tail_recent = str_tail
				f_recent = f
			else:
				# print "No Match", f, "!!"
				pass
		if f_recent:
			pathname = '%s/%s' % (dir, f_recent)
			# should maybe be some sort of VFS stat ;-) dir_obj?
			size = os.stat(pathname)[stat.ST_SIZE]
			self.size_k = size
			self.size_k = size / 1024
			self.str_size_k = "%.1f_K" % (size / 1024.0)
			self.str_date = str_date_recent
			self.pathname = pathname
			self.filename = f_recent
			return 1
		else:
			print "ABSENT:", name
			self.size_k = 0
			self.str_size_k = "0"
			self.str_date = "1970-01-01"
			self.str_date = "0 ABSENT FILE"
			self.pathname = "/tmp/ABSENT"
			self.filename = "ABSENT: " + name
			return 0

	def tree_print( self, printer ):
		# print "tree_print:", self.name
		s1 = "<H1><B>%s</B> - %s</H1>\n<P>%s</P>\n" % (
			self.name,
			self.desc_line,
			self.desc_text
		)
		printer.write( s1 )

	# is this used - broken?
	def UNUSED_fmt1( self, parent ):
		self.find_file_dir0()
		H = parent.add_elem( "H2" )
		if self.alias:
			# H.add_text( "("+self.alias+") " )
			H.add_text( self.alias+"::" )
			B = H.add_elem( "B" )
			B.add_text( self.name )
		else:
			B = H.add_elem( "B" )
			B.add_text( self.name )
		H.add_text(" - ")
		H.add_text( self.desc_line )

		P = parent.add_elem( "P" )
		P.add_text( "Date: " )
		B = P.add_elem("B")
		B.add_text( self.str_date )

		P.add_elem( "BR" )
		P.add_text( "Size: " )
		B = P.add_elem("B")
		B.add_text( self.str_size_k )

		P.add_elem( "BR" )
		P.add_text( "Language: " )
		B = P.add_elem("B")
		B.add_text( self.lang )

		P.add_elem( "BR" )
		P = parent.add_elem( "P" )
		P.add_text( self.desc_text )

	# FMT2 is used
	# output row of table that leads to fmt3 'item-screen'
	def fmt2_table_row( self, parent ):
		self.find_file_dir0()

		cls = odd_even_str()
		TR = parent.add_elem( "TR" )
		TR.add_attr( "class", cls )
		if 0: # this suggested - and worked
		 TR.add_attr( "onMouseOver", "this.className='CURR'")
		 TR.add_attr( "onMouseOut", "this.className='"+cls+"'")
		if 0: # yes but hardcodes colours
		 C2 = '#C0C0C0'
		 C1 = '#C0C090'
		 C0 = '#FFFFFF'
		 C3 = C1
		 if cls == "EVEN": C3 = C2
		 TR.add_attr( "onMouseOver", "this.style.backgroundColor='"+C0+"'")
		 TR.add_attr( "onMouseOut", "this.style.backgroundColor='"+C3+"'")
		Z = TR

		TD = Z.add_elem( "TD" )
		if self.alias:
			TD.add_text( self.alias+"::" )
		A = TD.add_elem( "A" )
		A.add_text( self.name )
		A.add_attr( "href", "#" + self.label )

		TD = Z.add_elem( "TD" )
		TD.add_text( self.str_date )

		TD = Z.add_elem( "TD" )
		TD.add_text( self.str_size_k )
		TD.add_attr( "align", "right" )

		TD = Z.add_elem( "TD" )
		TD.add_text( self.lang )

		TD = Z.add_elem( "TD" )
		# if type is libs_app_item.derived add this feature ..
		try:
			TD.add_text( self.categ )
			TD.add_text( ": " )
		except: pass
		TD.add_text( self.desc_line )

	# each item gats a mini-page (in same page) (that table points to)
	def fmt3_item_screen( self, parent ):
		# used where
		# self.find_file_dir0()

		H2 = parent.add_elem( "H2" )
		if self.alias:
			H2.add_text( self.alias+"::" )
		A=H2.add_elem( "A" )
		A.add_attr( "name", self.label )
		A.add_text( self.name )

		P = parent.add_elem( "P" )
		B = P.add_elem( "B" )
		B.add_text( self.desc_line )

		P = parent.add_elem( "P" )
		P.add_text( self.desc_text )

		P = parent.add_elem( "P" )

#		P.add_elem( "BR" )
		B = P.add_elem( "B" )
		B.add_text( "Date: " )
		P.add_text( self.str_date )

		P.add_elem( "BR" )
		B = P.add_elem( "B" )
		B.add_text( "Lang: " )
		P.add_text( self.lang )

		P.add_elem( "BR" )
		B = P.add_elem( "B" )
		B.add_text( "Size: " )
		P.add_text( self.str_size_k )

		P.add_elem( "BR" )
		B = P.add_elem( "B" )
		B.add_text( "Download: " )
		A = P.add_elem( "A" )
		uri = self.filename
		if 0: uri = "../LGPL/" + uri
		A.add_attr( "href", uri )
		A.add_text( uri )

		# putting HR into P causes errors
		# so put it outside
		parent.add_elem( "HR" )


class libs_apps_item( app_item ):

	def __init__( self, name, categ ):
		pfx = "libs_apps"
		app_item.__init__( self, name, pfx )
		self.alias = pfx
		self.group = pfx
		self.categ = categ


class libs_apps_app_item( libs_apps_item ):

	def __init__( self, name ):
		libs_apps_item.__init__( self, name, "APP" )


class libs_apps_lib_item( libs_apps_item ):

	def __init__( self, name ):
		libs_apps_item.__init__( self, name, "LIB" )

