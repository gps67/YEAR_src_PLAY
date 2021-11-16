#!/usr/local/bin/python

# xset +fp `pwd`

F1="-unknown-cmss10-medium-r-normal--0-180-0-0-p-0-adobe-fontspecific"
F2="-unknown-cmtt10-medium-r-normal--0-180-0-0-m-0-adobe-fontspecific"
F3="-unknown-cmmi10-medium-r-normal--0-360-0-0-p-0-adobe-fontspecific"
F4="-unknown-cmss10-medium-r-normal--0-240-0-0-p-0-adobe-fontspecific"

F1="-unknown-cmss10-medium-r-normal--0-200-0-0-p-0-adobe-fontspecific"
F2="-unknown-cmss10-medium-r-normal--0-200-0-0-p-0-adobe-fontspecific"
F3="-unknown-cmss10-medium-r-normal--0-300-0-0-p-0-adobe-fontspecific"
F4="-unknown-cmss10-medium-r-normal--0-300-0-0-p-0-adobe-fontspecific"

F1='-*-helvetica-medium-r-*-*-*-180-*-*-*-*-*-*'
F2='-*-courier-medium-r-*-*-*-140-*-*-*-*-*-*'
F3='-adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1'
F4='-adobe-helvetica-bold-r-normal--24-240-75-75-p-138-iso8859-1'

# This is Python API to Tcl/Tk
# Work done by Tcl_SESS_shell { SCRIPT } ;# { DIALECT dictionary } ;# lookup
# It has better TTY and PID and EVENT_MAIN_LOOP because Tcl/Tk is tight


from Tkinter import *
import sys

# S_text is an SGML text viewer
# it only works with a subset of the LINUXDOC dtd
# and it doesn't recognise SGML!
# lifes like that

class S_text1:
	def config( self, opt, value ):
		apply(self.text.tk.call, 
		      	(self.text._w, 'configure', '-'+opt, value))
	def __init__(self, w0=None):
		# create the text frame and scroll bars
		# the exit button is elsewhere
		self.frame = Frame( w0 )
		self.text = Text(self.frame)
		self.config( 'background', 'black' )
		self.config( 'foreground', 'white' )
	#	self.config( 'justify','left')
		self.config( 'wrap'   ,'word')
		self.vbar = Scrollbar(self.frame)
	#	self.hbar = Scrollbar(self.frame)
	# STYLE: there is no hbar, use the mouse instead

		self.vbar.pack({'fill': 'y', 'side': 'left'})
		self.text.pack({'expand': 1, 'fill': 'both', 'side': 'left'})

		# Link Text widget and Scrollbar
		self.text['yscrollcommand'] = (self.vbar, 'set')
		self.vbar['command'] = (self.text, 'yview')

	# now intialise the zone stack 

	# this test shows more than one attribute
	def	test1(self):
		lng="aaa bbb ccc ddd eee fff \nggg hhh iii jjj kkk lll \nmmm nnn ooo ppp qqq rrr sss ttt uuu vvv www xxx yyy zzz"
#		lng="a long line"
		lng = lng + "\n" + lng
		for i in range(1,20):
			P0 = self.get_pos()	# start zone
			self.puts("Hello ")	# zone contents
			self.puts(lng)		# zone contents
		#	self.puts("\n")		# has to happen?
			P1 = self.get_pos()	# end zone
			self.zone_tag( P0, P1, 'VERB' ) # apply effect to zone
			# now another zone ...
			self.puts(" Bold ")	# zone contents
			P2 = self.get_pos()	# zone edge
			self.zone_tag( P1, P2, 'BF' )
			# third zone
			self.puts("world ")
			self.puts(lng)
		#	self.puts("\n")
			P3 = self.get_pos()
			self.zone_tag( P2, P3, 'P' )
		# an alternative would be to create a stack
		# nsgmls guarantees proper bracketing of stack!
		# and have it react to zone_start_type

	def	get_pos(self):
		return self.text.index('end - 1 chars') # NB SANITY HERE !!

	def	puts(self,text):
		self.text.insert('end', text)

	def	zone_tag( self, p0, p1, tagname ):
		self.text.tag_add( tagname, p0, p1 )

	def	tag_config( self, tagname, opt, value ):
		      apply(self.text.tk.call, 
		      	(self.text._w, 'tag', 'configure', tagname,
		        '-'+opt, value))

	# this sets up the styles for the tags
	def	tag_setup(self):
		global F1, F2, F3, F4
		font1=F1
		font2=F2
		H1_font=F3
		H2_font=F4
		L1 = 8
		L2 = 16
		H1_OPTS = {
			'font':		H1_font,
			'lmargin1':	L1,
			'lmargin2':	L1,
			'foreground':	'white', 
		}
		H2_OPTS = {
			'font':		H2_font,
			'lmargin1':	L2,
			'lmargin2':	L2,
			'foreground':	'bisque', 
		}
		P_OPTS = {
			'font':		font1,
			'lmargin1':	L2,
			'lmargin2':	L2,
			'foreground':	'green', 
		}
		VERB_OPTS = {
			'wrap':		'none',
			'font':		font2,
			'lmargin1':	L1,
			'lmargin2':	L1,
			'foreground':	'white', 
		}
		BF_OPTS = {
			'foreground':	'red', 
#			'underline':	'true', 
		}
		URL_OPTS = {
			'foreground':	'yellow', 
#			'underline':	'true', 
		}
		VURL_OPTS = {
			'foreground':	'red', 
#			'underline':	'true', 
		}
		self.text.tag_config( 'TITLE',    H1_OPTS )
		self.text.tag_config( 'HEADING',    H2_OPTS )
		self.text.tag_config( 'TITLEPAG',    H2_OPTS )
		self.text.tag_config( 'P',    P_OPTS )
		self.text.tag_config( 'VERB', VERB_OPTS )
		self.text.tag_config( 'URL', URL_OPTS )
		self.text.tag_config( 'HTMLURL', URL_OPTS )
		self.text.tag_config( 'VURL', VURL_OPTS )
		self.text.tag_config( 'BF',   BF_OPTS )
		self.tag_config( 'TITLE', 'background',	'red' )
 
	#	self.tag_config( 'VERB',	'background',	'green' ) 
		self.tag_config( 'VERB',	'borderwidth',	'-4' ) 
		self.tag_config( 'VERB',	'relief','raised' ) 
# spacing works beautifully for P but not at all for VERB
# relief raised fails due to overlap
#		self.tag_config( 'P',	'spacing1',	'1' ) 
#		self.tag_config( 'P',	'spacing2',	'1' ) 
#		self.tag_config( 'P',	'spacing3',	'1' ) 
#		self.tag_config( 'VERB',	'spacing1',	'4' ) 
#		self.tag_config( 'VERB',	'spacing2',	'1' ) 
#		self.tag_config( 'VERB',	'spacing3',	'4' ) 


print "loaded s_text1.py" ;# python + tcl takes a while to load, say hello


