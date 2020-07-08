
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
	def run( self, str, ptn ):
		try:
			code = self.cache[ ptn ]
		except:
			code = re.compile( ptn )
			self.cache[ ptn ] = code
		self.result = code.match( str )
		return self.result
	def group( self, v ):
		return self.result.group( v )
