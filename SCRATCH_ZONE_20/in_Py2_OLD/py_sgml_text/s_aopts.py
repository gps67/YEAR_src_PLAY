#!/usr/local/bin/python
# s_aopts.py

import regex
import string
import regsub

class aopt1:
	"""	A named option
	"""
	def	__init__( self, name, cset, data ):
		self.attr = name
		self.cset = cset
		self.data = data

	def	prints(self):
		name = self.attr
		cset = self.cset
		data = self.data
		if cset == 'CDATA':
			print '%(name)s="%(data)s"' % vars() ,
		else:
			print '%(name)s="%(data)s[%(cset)s]"' % vars()
#		print "A_opt attr:", self.attr, "cset:", self.cset, "data:",self.data,

expr1 = "^\([^ ]*\) \([^ ]*\) \(.*\)$"
a_opts_prog = regex.compile( expr1 )

def	A_opts(str):
	global a_opts_prog
	n = a_opts_prog.match( str )
	if n > 0:
		name, cset, data = a_opts_prog.group(1,2,3)
		return aopt1( name, cset, data)
	else:
		print "ERROR n=%s" % n
		print "A_opt%s" % str
		return None

class	A_opts_list:
	def __init__(self):
	    self.set_null()
	def set_null(self):
	    self.dry = {}
	def get_cdata_field( self, name ):
	    str = self.dry[ name ]	# BUG NO EXIST
	    return str
	def add_from_esis_line( self, str ):
    	    global a_opts_prog
	    n = a_opts_prog.match( str )
	    if n > 0:
		name, cset, data = a_opts_prog.group(1,2,3)
		if cset != "CDATA":
		    print "WARNING: CDATA expected got", cset
	   # 	print name, "=", data
		self.dry[ name ] = data
	    else:
		print "ERROR n=%s" % n
		print "A_opt%s" % str
		return None
	    