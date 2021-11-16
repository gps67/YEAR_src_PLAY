from string import split # word
from regsub import gsub

class Line_buffer:

	def __init__( self, fd_, indent_str, wrap_ ):
		# print "Line_Buffer: indent_str = '%s'" % indent_str 
		# indent_str = "("+indent_str+")"
		self.fd = fd_
		self.indent_str = indent_str
		self.do_wrap = 1
		self.is_wrap = 1
		self.wrap = wrap_
		self.boln = 1
		self.lpos = 0 # position 0 is boln anyway
		self.indent = 0

	def indent_more( self ):
		# self.fd.write("(+)")
		self.indent = self.indent + 1
	def indent_less( self ):
		# self.fd.write("(-)")
		self.indent = self.indent - 1
	def write1( self, string ):
		if self.boln:
			self.boln = 0
			for i in range(self.indent):
				self.fd.write( self.indent_str )
				# LURK count LPOS
			self.lpos = self.indent * len( self.indent_str )
		self.fd.write( string )
		self.lpos = self.lpos + len( string )
	def write_text( self, string ):
		string = gsub( '&', '&amp;', string ) # FIRST!
		string = gsub( '<', '&lt;', string )
		string = gsub( '>', '&gt;', string )
		# others would be a good idea
		# but thats enough to be unambiguous
		if self.boln:
			self.boln = 0
			for i in range(self.indent):
				self.fd.write( self.indent_str )
				# LURK count LPOS
			self.lpos = self.indent * len( self.indent_str )
		if self.is_wrap & self.do_wrap:
			sp1 = ""
			if string[0]==' ': # first char
				if 0!=self.boln:
					self.write1( ' ')
				# happens between paragraphs!
			words = split( string )
			for word in words:
				w = len( word )
				x = self.lpos + w + 1
				if x > self.wrap :
					self.own_line()
					sp1 = ""
					x = self.lpos + w 
					x = w 
					# self.fd.write( "(%d)" % self.lpos )
				self.write1( sp1 )
				self.write1( word )
				sp1 = " "
				self.lpos = x 
			if string[-1:]==' ':	# last char in s
				self.write1( ' ')
		else:
			self.fd.write( string )
	def writeln( self ):
		self.fd.write( '\n' )
		self.boln = 1
		self.lpos = 0 # position 0 is boln anyway
	def own_line( self ):
		if not self.boln:
			self.writeln()
			# self.fd.write( "own_line(%d)" % self.lpos )
	def blank_line( self ):
		# I don't have any blank lines in my output!
		pass
		# ... errm ... whats that ...
        def mid_line_space( self ):
		if not self.boln: self.write1( ' ' )

