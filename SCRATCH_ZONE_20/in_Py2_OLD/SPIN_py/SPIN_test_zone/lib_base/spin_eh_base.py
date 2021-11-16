# python component
# spin_eh_base
# spin_eh_base - the generic event handler for SPIN events

# rename top_loader 
# from not_installed import spin_py

import sys
sys.path.append( '/home/gps/2000/src/SPIN_py/SP_lib' )
import spin_py

class SPIN_EH_BASE:
	"""
		EH The Event Handler receives all the events
		and is MOSTLY independent of SPIN_* but the few
		dependencies make it a good place to initialise SPIN

		You must derive a sub-class that handles each event
		(ElementStart, etc) and/or sets code handlers in-place-of
		each event. See the examples (renumber) for samples.
	"""
	def __init__(self, progname = "SPIN", top = None ):
		if not top:
			top = spin_py.SPIN_top( progname );
		self.top = top

	def dgb( self, msg ):
		""" Debugging message """
		print "# SPIN_EH_x:",msg

	def set_default_sdata( self ):
		#
		# converting sdata to text (scripted)
		# these are the HTML base set - move ?
		#
		self.sdata_mappings = {}
		self.sdata_mappings['lt'] = '&lt;'
		self.sdata_mappings['gt'] = '&gt;'
		self.sdata_mappings['amp'] = '&amp;'
		self.sdata_mappings['nbsp'] = '&nbsp;'
		self.sdata_mappings['quot'] = '&quot;'

	def set_default_kit_options( self ):
		top_kit = self.top.kit
		# DONE # top_kit.setProgramName( progname )
		top_kit.showOpenEntities()
		top_kit.outputCommentDecls()
		top_kit.outputMarkedSections()
		top_kit.outputGeneralEntities()
		#
		# where SP searches for cataloges and DTD files
		# BUG - why do these get searched twice?
		top_kit.addSearchDir( "/tools/1999/serv2/htdocs/py/etc_sgml" )
		top_kit.addSearchDir( "/tools/1999/serv2/etc_sgml" )
		top_kit.addSearchDir( "../../test_number/dtd/etc_sgml" )
		top_kit.addSearchDir( "etc_sgml" )
		top_kit.addSearchDir( "dtd/etc_sgml" )
		top_kit.addSearchDir( "../dtd/etc_sgml" )
		top_kit.addSearchDir( "../etc_sgml" )
	#	top_kit.addSearchDir( "/etc/sgml" )
		top_kit.addSearchDir( "." )
		top_kit.addSearchDir( "/usr/src/sp/pubtext/" )
		top_kit.addCatalog( "catalog")
		# top_kit.addCatalog( "html.soc")
		#
		# this doesnt work as expected
		# this causes some entity to not be found, requires dtd to be OK?
		# top_kit.mapCatalogDocument()
		#
		# <!ENTITY %SPIN "INCLUDE">
		#
		# includeParam is for marked sections
		top_kit.includeParam( "SPIN" )
		# where is top_kit.exclude ... ?
		#
		# -wwarning - see nsgmls docs
		# nb without warning you might not get the DTD filename!
		# top_kit.enableWarning( "all" )
		# return "naff" # check

	#
	# opt is a SPIN thing, set flags such as wrap, during Run
	# only callable whilst the parse exists, as it holds its own opts
	# usually called as soon as you see DtdStart()
	#

	def SPIN_opt( self, name, value ):
		spin_py.SPIN_opt( self, name, value )

	def SPIN_opt_off( self, name ):
		self.SPIN_opt( name, 0 )

	def SPIN_opt_on( self, name ):
		self.SPIN_opt( name, 1 )

	#
	# cfg_opt is an SP thing - only before Run
	#

	def kit_cfg_opt( self,opt ): # wierd call back system 
		spin_py.SPIN_parser_opt_val(self, opt)

	def kit_cfg_opt_val( self,opt,val ): # wierd call back system 
		spin_py.SPIN_parser_opt_val(self, opt, val)

	def XX_configure_parser( self ): # wierd call back system instead of argv
		self.configure_parser_default( ) # sub-class normally does this

	#

	def halt( self ):
		self.dgb( "halt" )
		spin_py.SPIN_halt( self )
		# spin_py (module) finds "self" and halts

	def Run( self, files ):
		# guess what this is nolonger used! Its now top.run
		# you can have multiple filenames, which are considered
		# one document, so put doctype + prolog in one, then text,
		# You are supposed to override this Run with yours
		# -D now set by callback
		# files = [ file ]
		self.top.EH = self;
		errs = self.top.run( files )
		# this is done anyway after Run - to curb ref loops
		# self.top.EH = None;
		self.dgb( "SPIN_run returned %d errors" % errs )
		try:
			t = self.tree
		except:
			msg = "Number of errors %d exceeded 9 - no tree" % errs
			self.dgb( msg )
			self.tree = msg
		return errs

