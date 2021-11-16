# python component
# spin_eh
# spin_eh - the generic event handler for SPIN events

from spin_eh_base import spin_py, SPIN_EH_BASE

# Where is the spin_py.so lib-binary ?
# Adding this dir to PATH and LD_LIBRARY_PATH seems to not be working
# You still need libsp.so on the ld.so path !

import sys

# Dtd_info is a dictionary of elem_info items
# you probably need two: sgml_in, html_out (plus any number of mid phase)
# it auto grows as new elements are found, but you want to pre-set
# special items, such as <PRE> or <VERB> and distinguish <FLOW> from <BLOCK>
#
# actually Dtd_info is just a convenience wrapper that administers .elem_info
# python references to dictionary objects, remain after changes
# spin uses EH.elem_info, EH.dtd_info.elem_info is to confuse you

from dtd_info import Dtd_info

# standard python

import traceback
import types
import sys
 
# SP_EH is a normal PY class that has told "spin_py" about itself
# EH creates the parser, configures it, and runs it
# then it receives the events from it 
#
# you derive a subclass and override it to suit you
#
# NB the class is central, but not special in a C/API way
# it is simply the Python instance object recorded as starting the Run
# which receives that callbacks - so it must provide them ...

class SP_EH( SPIN_EH_BASE ):

	def __init__(self, elem_pool ):
		SPIN_EH_BASE.__init__( self ) # other parameters TBS
		# self.dgb( "%s.__init__" % self.__class__.__name__ )
		#
		# self.elem_info is special
		# although SPIN would create .elem_info{} (if you didnt)
		# we choose to pre-set it to A COPY OF another dict
		# dtd_info is just an alternative with the same core DRY
		# so you could have some sort of sub-class
		#
		self.dtd_info = Dtd_info( self.top.dtd_tables )	# utility
		self.elem_info = self.dtd_info.elem_info # same core obj
		# LURK - ABOVE is confusing - pick one, delete other
		#
		# there is no required initialisation for spin
		# what follows is for the script only
		#
		# self.avoid_lots_of_sdata = 5
		#
		self.guess_file_type = 'DOC'
		self.curr_file = ''
		self.files_seen = {}

		#
		self.put_error_tags_in_document = 0
		self.show_generalEntity_other = 0
		self.show_generalEntity_sdata = 0
		self.show_generalEntity_sgml = 0
		# show where the dtd file came from
		self.show_DtdStart = 1
		# not finding the DTD is the biggest setup bug
		self.debug3 = 1
		self.debug2 = 0
		#
		self.set_default_kit_options()
		self.set_default_sdata()
		#
		# the actual DTD should set these 
		#
		self.dtd_line = ''
		self.xml_line = ''

	def dump_fields( self, fields ):
		# script local facility, used to dump generalEntity
		if fields == None:
			return
		for (key, item) in fields.items():
			print " %10s:" % key, item

	#
	# calls to SPIN not callbacks from it
	#

	#
	# callbacks from SPIN 
	#

	def ElementInfoInit( self, name, info ):
		if name=='xxxHTML':
			print "init elem_info HTML"
			self.dtd_info.show_elem_info( 'HTML' )
		# spin couldn't find elem_info
		# so it created an inline elem
		# you can now change those guessed defaults
		#
		# info.element_breaks_word = 1
		#
		# debug user forgot to preload the DTD settings
		# (this is called when an elem_info DIDNT exist)
		#
		if name == "VERB":
			# VERB is set when LINUXDOC is seen - so ...
			self.dgb( "WARNING -- VERB WASN'T SET" )
			self.dtd_info.set_elem_is_pre( name )
		elif name == "PRE":
			# PRE is set when HTML is seen - so ...
			self.dgb( "WARNING -- PRE WASN'T SET" )
			self.dtd_info.set_elem_is_pre( name )
		#
		# to show that you can also add any other attribute
		# except "atts" which is where the other atts are stored
		#
		# nb the dict is only created on request
		# so if you don't use the facility delete the following
		# ie it is wasteful to do this for EVERY element-info
		#
		info.other_attr = "any 'other_attr'"
		info.var_pool['also'] = "defaulted"
		#
		# HINT: thats how you map an ELEM node into a Py Class
		# you set an attribute on the elem_info, and then for
		# each 'elem' you convert it into an elam_handler
		# then have one class per elem type
		# (you have to have comprehensive code for the HTML generation
		# so its not hard coded to have one type per elem)
		if 0:
			if name=='HTML':
				print "elem_info HTML"
				self.dtd_info.show_elem_info( 'HTML' )

	def report_error( self, type, value, trace ):
		#
		# an error in your script
		#  
		#	eg if you dont define the SGML error callback fn:
		#	exceptions.AttributeError: ErrorEvent
		#
		# a Python error
		#
		#	eg (dunno) ENOMEM
		#
		# combination of the above
		#
		#	array out of bound
		#
		# a comment from SPIN code
		#
		#	this never happens, what is it ?
		#
		# If you dont define report_error, you will get:
		#
		#	AttributeError: report_error
		#	(but not the error it didnt report)
		#	so define it, and re-run
		# 
		# Use this fn to raise an alarm and insert something into the
		# output document, to tell the reader that something
		# failed for computation reasons, not content or intent
		#
		# After this function, (which is nested within callbacks)
		# the process recovers, and continues (so call halt)
		#
		# Errors are generated by (see the spin_py.c code)
		#
		# C: vret( Py_Call_that_shouldnt return_NULL )
		#
		#	message = a Python message for builtin errors
		#
		# C: raise_error( "a string which may or may not help" )
		#
		#	sets the standard python error message/indicator
		#	for a subsequent function to find
		#	(it reports any that were there before)
		#
		# C: report_error()
		#	
		#	immediately report any previously raised error
		#	generating an anonymous one, if necessary.
		#	It can report your errors or ones that Python found
		#	calls this scripted function! (same name)
		#	if error within this function ... uses PyErr_Print!
		#
		# results from traceback vary with where the error happened
		# also the "type" might be a bit slack, could be "spin_error"
		# but at least an error is reported
		#
		print '<!-- ===report_error=================='
		print "Python ERROR %s: %s:" % (type, value)
		print '-'
		# there are funny Python rules about using an error "once"
		# pdb.post_mortem( trace )
		traceback.print_exception( type, value, trace, 4 ) 
		print '===================report_error== -->',
		sys.stdout.flush()
		sys.stderr.flush()
		# tell the reader about the corrupted document
		self.Data( "(( Python ERROR %s: %s ))" % (type, value) )

	def ErrorEvent( self, locn, type, message ):
		# this is an SGML error in your document, not in python code
		# if bored of them, put "return" here
		# If bored of warnings, correct the following spelling error
		if type == "wArning":
			return
		#
		# the locn was previously send with set_locn - for consistency?
		# maybe put it back here
		# self.set_location( locn ) # done by spin
		# SP seems to report errors where you could have comments
		# and its good to tell the reader (invisibly) that there
		# is a markup error, so if they question a misleading
		# document, they stand a chance of finding a real problem
		# rather than causing one ...
		#
		print '<!-- ===errorEvent========'
		print "SGML Error type:", type
		print "SGML Error message: '%s'" % message
		self.print_location( locn ) # print_location here
		print '======errorEvent==== -->',
		#
		# you may also wish to make the invisible comment
		# a visible tag in the output
		# you will have to handle such a tag "anywhere"
		#
		if self.put_error_tags_in_document:
			t = self.dtd_info.get_elem_info('SGML_ERROR')
			t = spin_py.SPIN_elem_obj( t, [], [message] )
			self.SPIN_tree_add( t )
		#
		# calling halt is perhaps drastic but you could do that
		#
		# spin_py.SPIN_halt( self )
		#
		sys.stdout.flush()
		sys.stderr.flush()
		# NB SP's errout is not flushed 

	def setup_HTML( self ):
		"""
			In your EH, each element might have handler code lines,
			those would be set here.
			
			It really helps the tree_printer to have most tags
			known. Those are set in dtd_info !
		"""
		# print "setup_HTML() - creates PRE B BR"
		# set the input and output DTD as being HTML (or similar)
		self.dtd_info.setup_HTML()
		# set the output DTD line
		# The script will probably override this, but it wasnt yet set
		self.dtd_line='<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">'

	def setup_XML_8859( self ):
		# actually -15 has the euro symbol etc ...
		self.xml_line='<?xml version="1.0" encoding="ISO-8859-1" ?>'

	def setup_WML( self ):
		"""
			In your EH, each element might have handler code lines,
			those would be set here.
			
			It really helps the tree_printer to have most tags
			known. Those are set in dtd_info !
		"""
		self.setup_XML_8859() # '<?xml version="1.0" encoding="ISO-8859-1" ?>'
		self.dtd_line='<!DOCTYPE wml PUBLIC "-//WAPFORUM//DTD WML 1.2.1//EN" "http://www.wapforum.org/DTD/wml13.xml">'
		# maybe the above 2 lines should take from the following?
		self.dtd_info.setup_WML()

	def setup_LINUXDOC( self ):
		self.dtd_info.set_elem_is_pre( "VERB" )

	def setup_TDOC( self ):
		#
		# TDOC is a Temp DTD
		#
		# this looks like a decent way to have external scripts
		# without changing the structure here, and independently
		# distributable
		#
		import DTD_TDOC
		DTD_TDOC.setup_DTD_TDOC( self )

	def DtdStart( self, name, syspub, path, line, externalId ):
		#
		# the current "locn" is DOC
		# the following should be DTD (or included in prolog)
		#
		self.guess_file_type = 'DTD'
		#
		# the DOCTYPE has been seen, in document.sgml
		# you can extract the dtd filename from the externalId
		# or wait for a comment to reveal the actual file name
		# (a minor prob with SP)

		#
		#
		if self.show_DtdStart:
			self.dgb( "--" )
			self.dgb( "DtdStart   name: %s"% name )
			self.dgb( "DtdStart syspub: %s"% syspub )
			self.dgb( "DtdStart   path: %s"% path)
			self.dgb( "DtdStart   line: %s"% line )
			# external id is a tuple (varies)
			# self.dgb( "DtdStart externalId: %s, %s - ignore"% externalId )
		#
		self.dtd_line = line
		if syspub == "SYSTEM":
			self.dtd_line = "<!DocType %s SYSTEM>" % name
		#
		# you cannot call app_opt before RUN - it hooks to SP_App
		# and this is probably the best time to decide that the DTD
		# and process wants wrapping on/off (others = ...)
		# Without this wrapping would stay off for the entire document
		# conversely, if space-migration bothers you, switch it off
		# 
		self.top.app_opt( 'wrap', 0 )
		self.top.app_opt( 'wrap', 1 )
		#
		# auto_load is a new feature, where Python doesn't get called.
		# If it is FALSE (default) all auto_load features are disabled
		# elem_info_H1.on_element_end = EH.ElemEnd_H1 # a function
		# elem_info_P.ON_Element_End = 1 # use default builtin
		# -else- default non-auto-load is used
		# Data is not (usually) called when auto_load is true
		# you get it from the following end-tag, BUT then
		# you might need to look in silently loaded sub-braches.
		# MAIN USE is to load an entire tree, then scan it
		# but that still needs Marked_Sections PI Comments Errors
		# as elem_obj - line items (or Py-Inst from your script)
		#
		self.top.app_opt( 'auto_load', 1 )
		# self.SPIN_tree_add( 'DUMB' )
		print "DtdStart/py - created STYLE SCRIPT CODE"

		#
		# Ensure that the elem_info's and code vectors are loaded 
		#
		if name=="HTML":
			self.setup_HTML()
		elif name=="LINUXDOC":
			self.setup_LINUXDOC()
		elif name=="TDOC":
			self.setup_TDOC() # Temp-Doc
		else:
			self.dgb( "DTD '%s' not known here - expected HTML or LINUXDOC" % name )

	def DtdEnd( self, name ):
		# the prolog is processed "before" the dtd
		# so you are almost at the prolog end
		# print "<!-- DdtEnd", name, "-->"
		self.guess_file_type = 'SUBDOC'
		#print "-----------dtd end--------------"
		# now comes a lot of sdata from any included &general;
		# note that that was PREVIOUSLY visited
		# (as shown by the comment in the file, and it's set_location)
		# but the accumulated entities are dumped after here
	
	def set_location( self, locn ):
		(ent, file, line, col, byte, eoff, void) = locn
		# set_location is called as part of an error
		# and the first comment after a file change (fiddly)
		# its expensive, but available after/during most events
		if self.curr_file == file :
			pass
			# print "set_location - in %s" % file
		else:
			try:
				type = self.files_seen[file]
			except:
				# print "set_location - in %s NEW FILE!" % file
				self.files_seen[file] = self.guess_file_type
				if 'DTD' == self.guess_file_type:
					self.guess_file_type = 'dtd'
			self.curr_file = file 
			# self.print_location(locn)

	
	def print_location( self, locn ):
		(ent, file, line, col, byte, eoff, void) = locn
		print "LOCN: ent:", ent
		print "LOCN: file:", file
		print "LOCN: line:", line
		print "LOCN: col:", col
		print "LOCN: byte:", byte
		print "LOCN: eoff:", eoff
		# print "LOCN: void:", void, "(Ignore)"

	def Comments( self, list ):
		# see p1.a.py for how to print these
		# proves problematical when tree building
		# AND matching against dtd structure
		# because comments can appear anythere - ish
		# then where do you move them to ...
		#
		# 1 # place in text list
		# 2 # drop completely
		# 3 # print (and drop)
		# 4 # add to surrounding frame's comment list
		# 5 # move to prev/next/nearest obj (NL BL count)
		# 6 # process as DHTML
		# 7 # other
		#
		return

	# I'm lazy, I defined entities as dictionary items

	def generalEntity_sdata( self, fields ):
		if self.show_generalEntity_sdata:
			print "GeneralEntity_sdata"
			self.dump_fields( fields )
		return

	def generalEntity_sgml( self, fields ):
		if self.show_generalEntity_sgml:
			print "GeneralEntity_sgml"
			self.dump_fields( fields )
		return

	def generalEntity( self, fields ):
		# above two take most of the traffic
		# what other types are there ? 
		# seems that sdata is coming here
		if self.show_generalEntity_other:
			print "GeneralEntity_other"
			self.dump_fields( fields )

	def PrologEnd( self ):
		# from here on, comments are from doc, not dtd
		pass
		# print "PrologEnd (start of document text)"

	# This is the same as the default builtin auto-load action
	# your callbacks will get the same parameters
	#
	def ElementStart( self, eleminfo, name, atts, ctype ):
		# you could set flags to influence gathering
		# or even count sections for numbering
		# there is no return value, but you have the event
		return

	def SPIN_tree_add( self, tree ): 
		""" add text or element to tree building process """
		# NOT called by SPIN, but by script ...
		# print "SPIN_tree_add ..."
		# you can add stuff at almost anytime ;-)
		# as long as there is a stack/tree to add it to
		self.top.tree_add( tree )

	# see LDOC_to_HTML for calling functions from hash table
	# see spin_eh_num for callbacks (same parameters as this)
	# NOTE: you dont push or pop items from the element stack
	# (TODO: get read access!) Thats always done for you
	# this is the default, same as the builtin auto-load default
	# Your code would take the (free standing element), convert it
	# and add it as a data item (or throw it away).
	#
	def ElementEnd( self, info, name, tree ):
		# everything is vectored through here
		# you could manipulate it now, or later
		# not that slow, supprisingly
		self.SPIN_tree_add( tree )
		return

	# this is called as well as the ElementEnd() function
	# so that you can catch the document as it pops off the stack
	# NB is you have more than 10 errors, this wont get called!
	# NO you cant change that, its fixed in the SP Generic API code
	# (you could change it, but then be careful about whose dll you use)
	#
	def DocumentEnd( self, tree ):
		# to be called for top element ASWELLAS ElementEnd
		# that requires tree_add to work at zero!
		# which it now does
		self.tree = tree # keep for return after null ?
		print "DocumentEnd setting tree =", tree
		# nb do not do other work, until SPIN has closed
		# releasing the stack + memory, and not in callback
		# wait for return from Run
		if self.debug2: self.end_report()
		self.end_report()
		# NB if you have more than 10 errors, this is never called!

	def DataSpace( self ):
		# mostly unused - was part of data_trim and space-migration
		print '((DataSpace))', 

	# This is the same as the default builtin autoload function
	# ie add an item to its parents list of data.
	# it is almost NEVER called when auto_load is true,
	# There is no per-element vector, its either this or not.
	# Scripts expect this to be usable
	#
	def Data( self, data ):
		## NEWS ## this is hardly never called
		## auto_load simply adds it to the tree
		# some often comes from the script
		if self.debug3: print "Data:", data
		print "Data:", data
		# everything is vectored through here (not attributes)
		# self.SPIN_tree_add( "(Data)" )
		self.SPIN_tree_add( data )
		return

	# maybe this class could return a converted object
	# str -> add to data WITH/OUT remapping
	# obj -> add to tree as-is
	# None -> skip
	# NB sdata in attrs is still wrong
	def sdata( self, name, text ):
		if self.debug2: print "sdata:", name
		# this is problematical
		# because in SGML we want to RETAIN the sdata as-is
		# but in output we want to convert it
		# preloading sdata_mapping allows overrule
		# eg nbsp might be declared as a space!
		# however it is in the scripting domain
		# so you can fix it for each case
		try:
			txt = self.sdata_mappings[name]
		except:
			# show operator actual NEW sdata used
			if 1: print "<!- == sdata", name, text, "== ->"
			# learn
			self.sdata_mappings[name] = text
			txt = text
			# txt = '&'+name+';' # to retain as sdata
			# obj = sdata_holder # possible
			# txt = "<!-- %s - %s -->" % (name, text) + txt
		# unfortunately this creates a distinct item in zone text list
		self.SPIN_tree_add( txt )

	def MarkedSectionStart( self, status, parameters ):
		print " MARKED SECTION "
		print " status", status
		print " parameters", parameters
		self.Data( " MARKED->>" )
		# notice how self.Data accepts an 8859 string with <> in it
		# SPIN converts all input to plain 8859 (UNICODE not poss?)
		# you CANNOT have '&lt;' in Data unles you want '&amp;lt;'

	def MarkedSectionEnd( self ):
		print " MARKED SECTION END "
		self.Data( "<<-MARKED_END " )

	# Unfortunately SP cannot handle php's dialect format ?
	# Mostly because it stays as PARSED-char-data
	# PHP: if a < b ... confuses it!
	# suggest using marked sections as alternative WITH entityname
	#
	def PI( self, data, entityname ):
		old = self.top.app_opt( 'wrap', 0 )
		self.Data( "<?PI entityname='%s' data='%s' ?>\n" % (entityname, data) )
		self.top.app_opt( 'wrap', 1 )

# additional functions

	def end_report( self ):
		print "-- files_seen --"
		for (file,type) in self.files_seen.items():
			print " %9s -- %s" % (type, file)
		self.dtd_info.end_report()
