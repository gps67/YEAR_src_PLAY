
# (C) Graham Swallow
# LGPL - Library GNU Public Licence - http://www.gnu.org/licenses/lgpl.html

import re

class re_cache:
	"""
		retains the compiled regular expressions

		if R.run( line, "pattern" ):
			print R.group(1)

	"""
	def __init__( self ):
		self.cache = {}

	def run_match( self, str, ptn ):
		# match anchors to the start of str
		try:
			code = self.cache[ ptn ]
		except:
			code = re.compile( ptn )
			self.cache[ ptn ] = code
		self.result = code.match( str )
		if 0:
			print( "str", str )
			print( "ptn", ptn )
			print( "result", self.result )
		return self.result

	def run_search( self, str, ptn ):
	# search can skip the start of string
		try:
			code = self.cache[ ptn ]
		except:
			code = re.compile( ptn )
			self.cache[ ptn ] = code
		self.result = code.search( str )
		if 0:
			print( "str", str )
			print( "ptn", ptn )
			print( "result", self.result )
		return self.result

	def run( self, str, ptn ):
		return self.run_search( str, ptn )
	#	return self.run_match( str, ptn )

	def group( self, v ):
		return self.result.group( v )
	
	def wrap_group_re( self, ptn ):
		# ([0-9]+) # python dialect
		# \([0-9]+\) # other dialects
		return "(" + ptn + ")"
