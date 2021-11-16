#!/usr/local/bin/python

from Tkinter import *
import sys
from s_text1 import *

class zone_stack:
	def __init__(self, txt):
	    self.txt = txt
	    self.depth = 0
	    self.name = ["ANON",1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]
	    self.pos0 = ["1.1",1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]
	    self.atts = [None,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]

	def start(self, pos0, tag, atts):
	    self.depth = self.depth + 1
	    self.name[ self.depth ] = tag
	    self.atts[ self.depth ] = atts
	    self.pos0[ self.depth ] = pos0

	def end(self):
	    self.name[ self.depth ] = None
	    self.atts[ self.depth ] = None
	    self.pos0[ self.depth ] = None
	    if self.depth > 0:
		self.depth = self.depth - 1
	    else:
		print "\ntag depth zero\n---------ERROR------ERROR------\n\n"

	def no___get_tag( self ):
		return self.name[ self.depth ]

class S_text2(S_text1):
	def __init__(self, w0=None):
		S_text1.__init__(self,w0) # create the text frame etc
		self.zone = zone_stack( self )
		self.is_pre = 0

	def zone_start( self, tag, atts ):
		self.vspace_before( tag )
		pos0 = self.get_pos()
		if tag == "VERB":
		    self.is_pre = 1
		self.zone.start( pos0, tag, atts )
		if tag == "HTMLURL":
			self.zone_htmlurl( atts )

# DTD has a bug, text is an attr
# normal use would need CLICK_ZONE and a new unique tag
# and handler fn
	
	def tag_bind( self, tag, key, fn ):
		apply(self.txt.tk.call, 
			(self.txt._w,
			'tag',
			'bind',
			tag,
			key,
			fn ))
	def zone_htmlurl( self, atts ):
		self.puts( atts.get_cdata_field( "NAME" ))
#		self.tag_bind( tag, key, fn )


	def zone_end( self ):
		P0 = self.zone.pos0[ self.zone.depth ]
		tag= self.zone.name[ self.zone.depth ]
		P1 = self.get_pos()	# probably repeats call :-(
		self.zone.end()		# pop from stack

		self.zone_tag( P0, P1, tag ) # apply effect to zone
		if self.is_pre:
		    self.is_pre = 0 # BUG VERB can contain attrs!
		self.vspace_after( tag )
		if tag == "SECT":
			self.text.update_idletasks() # unwanted

	def is_invis( self, tag):
		return tag in [
			'LINUXDOC', 'ARTICLE', 'SECT',
			'SECT1', 'SECT2', 'ITEMIZE', ]
	def is_inline( self, tag):
		return tag in ['BF', 'TT', 'URL', 'HTMLURL' ]

# really, should output glyphs, spaces, newline, blankline
# but counting every word is a speed problem in python
# also the tk TEXT widget is rather dumb, considering how sophisticated it is
# eg nested indents with leading bullet list requires something
# maybe we require blocked rectangles, as independent widgets ...

	def vspace_before( self, tag ):
		if self.is_inline( tag ): return
		if self.is_invis( tag ): return
		if tag in ['ITEM']:
		    self.puts(" o ")
		#else:
    		 #   self.puts("("+tag)

	def vspace_after( self, tag ):
		if self.is_inline( tag ): return
		if self.is_invis( tag ): return
		#self.puts(" vspace_after "+tag+"\n")
		self.puts("\n")
		if tag in ['P','VERB','TITLEPAG' ]:
			self.puts("\n")

	def esis_newline(self):
		if self.is_pre:
		    self.puts("\n")
		else:
		    self.puts(" ")

	# this test shows more than one attribute
	def	test2(self):
		lng="aaa bbb ccc ddd eee fff \nggg hhh iii jjj kkk lll \nmmm nnn ooo ppp qqq rrr sss ttt uuu vvv www xxx yyy zzz"
#		lng="a long line"
		lng = lng + "\n" + lng
		for i in range(1,4):
			self.zone.start( 'PRE' )
			self.puts("Hello ")	# zone contents
			self.puts(lng)		# zone contents
		#	self.puts("\n")		# has to happen?
			self.zone.end()

			self.zone.start( 'P' )
			self.puts("Hello ")	# zone contents

			self.zone.start( 'BF' )
			self.puts(" nested Bold ")	# zone contents
		#	self.puts("\n")		# has to happen?
			self.zone.end()

			self.puts(lng)		# zone contents
		#	self.puts("\n")		# has to happen?
			self.zone.end()
			self.puts("\n")		# has to happen?

print "loaded s_text2"	;# python + tcl takes a while to load, say hello



