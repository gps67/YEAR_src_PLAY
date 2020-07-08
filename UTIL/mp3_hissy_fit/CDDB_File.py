

# default is overridden by parsing discid provider
# version is supposed to be  strng:1.2.3a9
global_submit_string = 'SLIP_GRIP 2005.09.26'
import re_cache
#############################################################################

class CDDB_Track:

 def __init__( self, Album ):
	# self.Album = weak-link-to Album
	self.TTITLE = None
	self.TARTIST = None
	self.offset = None
	self.EXTT = "" # causes probs when used
	self.EXTT = None # causes probs when used

 def regen_cddb_line( self, out, key, idx ):
	try:
		val = self.__dict__[key]
	except:
		return
	if val == None: return
	# DONE # val = val.strip()
	out.append_unicode_ln( "%s%s=%s" % ( key, idx, val ))

 def txt_cddb_line( self, out, key, idx ):
	# for summary
	try:
		val = self.__dict__[key]
	except:
		return
	if val == None: return
	# DONE # val = val.strip()
	out.append_unicode_ln( "%s %2s %s" % ( key, idx, val ))

 def regen_cddb_trk( self, out, idx ):
	self.regen_cddb_line( out, "TARTIST", idx )
	self.regen_cddb_line( out, "TTITLE", idx )
	self.regen_cddb_line( out, "EXTT", idx )
	# print

 def txt_cddb_trk( self, out, idx ):
	# for summary
	self.regen_cddb_line( out, "TARTIST", idx )
	self.txt_cddb_line( out, "TTITLE", idx )
	# self.regen_cddb_line( out, "EXTT", idx )
	# print

 def set_attr( self, key, val ):
	val = val.strip()
	if val:
		try:
			V = self.__dict__[key] + val
		except:
			V = val
		self.__dict__[key] = V
	if 0:
		print "# track XX  set_attr( %-10s, %s )" % ( key, val )

 def set_attr_list( self, key, val ):
	if 0:
		print "# Track set_attr_list( %-10s, '%s' )" % ( key, val )
	val = val.strip()
	if not val:
		return
	try:
		lst = self.__dict__[key]
		if not lst: lst = []
	except:
		lst = []
		self.__dict__[key] = lst
	lst.append( val )

#############################################################################

class unicode_buffer:

 def __init__( self ):
	self.buff = []
	self.UNI = None

 def append_number( self, numb ):
	uni = str( numb ).encode
	
 def append_unicode( self, ustr ):
	self.buff.append( ustr )

 def append_unicode_ln( self, ustr ):
	self.buff.append( ustr )
	self.buff.append( u"\n" )

 def get_unicode_text( self ):
	if not self.UNI:
		uni = u''
		for addn in self.buff:
			uni = uni + addn
		self.UNI = uni
	return self.UNI
 
 def x_line( self, s ):
	try:
		S8 = s.encode( "Latin-1" )
	except:
		S8 = str( s )
	self.buff = self.buff + S8 + "\n"
 def x_word( self, s ):
	self.buff = self.buff + str(s) + " "
 def x_write_as_file( self, filename ):
	print "# CDDB #" + filename
	fd = open( filename, "w" )
	## fd.encoding = "Latin-1"
	fd.write( self.buff )
	fd.close()
	# raise filename

#############################################################################

class CDDB_Album:

 def __init__( self ):
	global global_submit_string
	self.DISCID = None
	self.DTITLE = None
	self.DTITLE_ARTIST = None	# used for dir
	self.DTITLE_ALBUM = None	# used for dir
	self.DYEAR = "1999"		# somtimes omitted from grip ?
	self.DYEAR = None		# best detected as an error ?
	self.DGENRE = "Blues"		# 
	self.DID3 = None
	self.TRACK = []
	self.seconds = 1
	self.cddb_revision = "-1"	# must be +ve integer  or ""
	# client_name client_version optional_comments
	self.cddb_submitted_via = global_submit_string
	self.PLAYORDER = ""		# usually blank - set_attr probably drops?
	self.EXTD = []		# usually blank - set_attr probably drops?

	self.cddb_text = None	# generate once
	self.out = None	# generate once

 def get_track( self, idx ):
	# Album is an array of tracks, extend to idx
	try:
		trk = self.TRACK[ idx ]
	except:
		while len( self.TRACK ) <= idx:
			self.TRACK.append( CDDB_Track( self ))
		trk = self.TRACK[ idx ]
	return trk

 def set_attr( self, key, val ):
	val = val.strip()
	if val:
		self.__dict__[key] = val
	if 0:
		print u"# album set_attr( %-10s, %s )" % ( key, val )

 def set_attr_list( self, key, val ):
	# EXTD #
	if 0:
		print u"# ALBUM set_attr_list( %-10s, %s )" % ( key, val )
	val = val.strip()
	if not val:
		return
	try:
		lst = self.__dict__[key]
	except:
		lst = []
		self.__dict__[key] = lst
	lst.append( val )

 def regen_cddb_attr_list( self, key, once=False ):
	# EXTD # DISC extended data # can be repeated
	lst = self.__dict__[key]
	if lst == None: 
		if once:
			lst = ['']
		else:
			return
	for val in lst:
		val = val.strip()
		self.out.append_unicode_ln( u'%s=%s' % (key, val ))

 def regen_cddb_attr( self, key ):
	# move this to print_out_cddb
	# it knows about Album, Track, but they hardly know about cddb
	# for now its here
	val = self.__dict__[key]
	if val == None: return
	val = val.strip()
	self.out.append_unicode_ln( u'%s=%s' % (key, val ))

 def txt_cddb_attr( self, out, key ):
	# used for summary not db
	val = self.__dict__[key]
	if val == None: return
	val = val.strip()
	out.append_unicode_ln( u'%-10s %s' % (key, val ))

 def regen_cddb_DTITLE( self ):
	DTITLE = "DTITLE"
	if self.__dict__[DTITLE]:
		self.__dict__[DTITLE] = self.DTITLE_ARTIST + u" / " + DTITLE_ALBUM


 # if write perm needed use fd, remember encoding though
 def write_as_file( self, filename ):
	UNI = self.get_cddb_text_UNI()
	# LURK: cant double guess the filesystem charset!
	# OK is was the dir that really didnt exist!
	filename = filename.encode( "utf-8" )
	fd = file( filename, "w" )
	# READONLY!! ## fd.encoding = "Latin-1"
	iso_bytes = UNI.encode( "iso-8859-1" )
	fd.write( iso_bytes )
	fd.close()

 def get_cddb_text_UNI( self ):
	self.gen_cddb_text_out()
	return self.out.get_unicode_text()

 def gen_cddb_text( self ):
	# out is one unicode string buffer
	self.gen_cddb_text_out()

 def gen_cddb_text_out( self ):
	if self.out: return
	self.out = unicode_buffer()
	out = self.out
	out.append_unicode_ln( u'# xmcd CD database file generated by %s' % self.cddb_submitted_via)
	out.append_unicode_ln( u'# ')
	out.append_unicode_ln( u'# Track frame offsets:')
	for trk in self.TRACK:
		out.append_unicode_ln( u"#        %s" % trk.offset)
	out.append_unicode_ln( u'# ')
	out.append_unicode_ln( u'# Disc length: %s seconds' % self.seconds)
	out.append_unicode_ln( u'# ')
	rev = self.cddb_revision
	if rev == "0" : rev = u""
	out.append_unicode_ln( u'# Revision: %s' % rev )
	out.append_unicode_ln( u'# Submitted via: %s' % self.cddb_submitted_via)
	out.append_unicode_ln( u'# ')
	self.regen_cddb_attr( u"DISCID" )
	self.regen_cddb_attr( u"DTITLE" )
	self.regen_cddb_attr( u"DYEAR" )
	self.regen_cddb_attr( u"DGENRE" )
	self.regen_cddb_attr( u"DID3" )
	# should be different format
	# UNUSED self.regen_cddb_attr( "seconds" )
	idx = -1
	for trk in self.TRACK:
		idx += 1
		trk.regen_cddb_trk( self.out, idx )
	self.regen_cddb_attr_list( u"EXTD" )
	self.regen_cddb_attr( u"PLAYORDER" ) # LURN should be submitted empty

# GPS # CALL #

 def txt_cddb_summary( self ):
	out = unicode_buffer()
	out.append_unicode_ln( u'# ')
	self.txt_cddb_attr( out, u"DTITLE_ARTIST" )
	self.txt_cddb_attr( out, u"DTITLE_ALBUM" )
	out.append_unicode_ln( u'# ')
	self.txt_cddb_attr( out, u"DISCID" )
	self.txt_cddb_attr( out, u"DTITLE" )
	self.txt_cddb_attr( out, u"DYEAR" )
	self.txt_cddb_attr( out, u"DGENRE" )
	# should be different format
	# UNUSED self.regen_cddb_attr( "seconds" )
	idx = -1
	for trk in self.TRACK:
		idx += 1
		trk.txt_cddb_trk( out, idx )
	return out

 def gen_cddb_line( self, out ):
	# generate the cddb id line
	# TODO: as a string or list of string words
	out.append_unicode( self.DISCID )
	out.append_unicode( len( self.TRACK ) )
	for trk in self.TRACK:
		out.append_unicode( trk.offset )
	out.append_unicode_ln( self.seconds)

#############################################################################


def dbg_bytes( bytes ):
	if not dbg_bytes_uses_bit8( bytes ): return 
	print "## DBG_BYTES ##", type( bytes )
	print "##", dbg_bytes_dump_str( bytes )

def dbg_bytes_uses_bit8( bytes ):
	for c1 in bytes:
		vc1 = ord( c1 )
		if vc1 >= 128:
			return True
	return False

def dbg_bytes_dump_str( bytes ):
	S1 = ''
	for c1 in bytes:
		vc1 = ord( c1 )
		if (vc1 < 32) or (vc1 > 127):
			hex = "\\x%2.2x" % vc1
			hex = "{%2.2X}" % vc1
			S1 = S1 + hex 
		else:
			S1 = S1 + c1
	return S1

def dbg_uni( ustr ):
	try:
		s = str( ustr )
		# plain ascii survives
		return
		# else it contains an 8th bit
	except:
		print "## DBG_UNI ##",
		print type( ustr ),
	
		print ""
		print "##",
		e = "iso-8859-1"
		print "%10s" % e,
		s = ustr.encode( e ),
		print s,
	
		print ""
		print "##",
		e = "UTF-8"
		print "%10s" % e,
		s = ustr.encode( e ),
		print s,
	
		print ""

#############################################################################

class CDDB_Parser:

 RE_KEY_IDX_LINE = '^([A-Z]+)([0-9]*)=(.*)$'
 RE_DID3_val    = '^DID3=([0-9]*)$' # eg 7
 RE_BLANK_LINE = '^[	 ]*$'
 RE_HASH_TEXT = '^#[	 ]*(.*)$'
 RE_HASH_TEXT = '^#[	 ]*(.*)$'
 RE_xmcd_database = '^# xmcd CD database file generated by (.*)$'
 RE_HDR_Track_Frame_offsets='^# Track frame offsets:$'
 RE_Track_Frame_offsets  = '^#\s*([0-9]+)$'
 RE_Disk_length = '^# Disc length: ([0-9]+) seconds'
 RE_Revision = '^# Revision: (.*)$'
 RE_Submitted_via = '^# Submitted via: (.+)$'

 def __init__( self ):
	self.reread = None
	self.line = None
	self.lines = []
	self.lines_pos = 0
	self.lines_len = 0
	self.RE = re_cache.re_cache()
	self.trip = None # RE match type 
	self.fail_list = []
	self.Album = CDDB_Album()
	self.seconds = 0
	# the record returned from freedb isnt split yet
	self.to_split_title( "", True )
	self.TARTIST_found = False

 def to_split_title( self, sep, artist_title ):
	# derived class migt change this
	if sep == "": sep = None
	self.sep_title_artist = sep
	self.artist_before_title=artist_title

 def conv_lines( shelf, lines, e1, e2 ,e3 ):
	# convert lines to unicode from EXPECTED encoding
	lines_out = []

	for line_in in lines:
		try:
			if 0:
				if dbg_bytes_uses_bit8( line_in ):
					print "# encoding expected:", e1
					dbg_bytes( line_in )
				line_unic = unicode( line_in, e1 )
				if 1: dbg_uni( line_unic )
			# else:
			line_unic = unicode( line_in, e1 )
		except:
			print "# ERROR # creating unicode from enc,line_in:", e1, line_in
			try:
				# If encoding was UTF-8, but contained 8859 an error is generated
				# so get unicode from 8859
				line_unic = unicode( line_in, e2 )
			except:
				# I dont see how this could happen
				line_unic = unicode( line_in, e3 )
		lines_out.append( line_unic )
	return lines_out

 def conv_lines_from_8859( shelf, lines ):
	e1 = "Latin-1"
	e2 = "UTF-8"
	e3 = "iso-8859-1"
	return shelf.conv_lines( lines, e1, e2, e3 )

 def conv_lines_from_UTF8( shelf, lines ):
	e1 = "UTF-8"
	e2 = "Latin-1"
	e3 = "iso-8859-1"
	return shelf.conv_lines( lines, e1, e2, e3 )

 def load_lines_from_file( self, filename, utf8 = False ):
	print "Loading", filename, "utf8:", utf8
	self.Album.cddb_input_filename = filename
	fd = open( filename )
	# ideally set fd.encoding, and call readlines_unicode(), but not avail
	self.lines = fd.readlines()
	self.lines_len = len( self.lines )
	fd.close()

	# most text is in 8859, but some are utf8
	# id3 tags go out in utf8
	# xmms will ignore strings with illegal utf8 bytes (such as 8859)
	# internally work with bytes (8859) - or move over to unicode ?

	if utf8:
		self.lines = self.conv_lines_from_UTF8( self.lines )
	else:
		self.lines = self.conv_lines_from_8859( self.lines )


 def load_lines_from_8859_file( self, filename ):
	 utf8 = False
	 self.load_lines_from_file( filename, utf8 )

 def load_lines_from_utf8_file( self, filename ):
	 utf8 = True
	 self.load_lines_from_file( filename, utf8 )

 def line_unused( self ):
	self.line_reread = self.line
	self.reread = True

 def line_used( self ):
	self.reread = None
	self.line = None
	self.fail_list = []

 def got_line( self ):
	if self.line:
		return self.line
	return self.get_line()

 def get_line( self ):
	# throw away existing line
	# but reuse reread buffer
	if self.reread:
		self.reread = None
		self.line = self.line_reread
		return self.line
	if( self.lines_pos >= self.lines_len ):
		self.line = None
		return None
	self.line = self.lines[ self.lines_pos ]
	self.line = self.line[:-1]
	self.lines_pos += 1
	return self.line

 def expect_blank_line( self ):
	if not self.got_line():
		# MAYBE ALLOW blank line at eof
		raise "blank_line_expected"
	if self.line == '':
		self.line_used()
	else:
		print '# FAIL # expect_blank_line() got "%s"' % self.line
		raise "blank_line_expected"

 def allow_blank_comment( self ):
	# allow initial blank comment
	if self.got_line():
		if self.line == '#':
			self.line_used()
			self.got_line() # optional


 def parse_offsets_section( self ):
	self.allow_blank_comment( )
	idx = -1
	while self.got_line():
		idx += 1
		if self.RE_uses_line( self.RE_Track_Frame_offsets ):
			offs = int( self.RE.group(1) )
			trk = self.Album.get_track( idx )
			trk.offset = offs
			# print( "Track", idx, "offset", offs )
		else:
			self.line_unused()
			break

 def RE_error_parse_fail_uses_line(self):
	print "# RE_error_parse_fail_uses_line(): '%s'" % self.line
	for re in self.fail_list:
		print "TRIED: %s" % re
	self.fail_list = []
	raise "RE_error_parse_fail_uses_line()"

 def RE_uses_line( self, REXP ):
	self.got_line()
	try:
		self.trip = self.RE.run( self.line, REXP )
	except:
		print "LINE: '%s'" % self.line
		print "REXP: '%s'" % REXP
	if self.trip:
		if 0:
			dbg = '# PASS # RE_uses_line("%s") - got %s' % (REXP, self.line)
			print dbg
		self.line_used();
		return self.trip
	else:
		self.fail_list . append ( REXP )
		return None

 def parse_head_comment( self ):
	while self.got_line():

		if self.line[0] != '#':
			self.line_unused()
			return

		if self.RE_uses_line( self.RE_xmcd_database ):
			self.Album.cddb_submitted_via = self.RE.group(1)
			continue

		if self.RE_uses_line( "^#\s*$" ):
			continue

		if self.RE_uses_line( self.RE_HDR_Track_Frame_offsets ):
			self.parse_offsets_section()
			continue

		if self.RE_uses_line( self.RE_Disk_length ):
			self.Album.seconds = self.RE.group(1)
			continue

		if self.RE_uses_line( self.RE_Revision ):
			self.Album.cddb_revision = self.RE.group(1)
			continue

		if self.RE_uses_line( self.RE_Submitted_via ):
			self.Album.cddb_submitted_via = self.RE.group(1)
			continue

		if self.RE_uses_line( '^#(.*)$' ):
			print '## COMMENT:: %s' % self.RE.group(1)
			continue

		else:
			print "### COMMENT #", self.line
			self.RE_error_parse_fail_uses_line()


 def parse_lines( self ):

	self.parse_head_comment()

	while self.got_line():

		if self.RE_uses_line( "^#\s*$" ):
			continue

		if self.RE_uses_line( "^#(.*)$" ):
			print "##- COMMENT #", self.trip.group(1)
			continue

		if self.RE_uses_line( "^\s*$" ):
			continue

		if self.RE_uses_line( self.RE_DID3_val ):
			self.Album.set_attr( "DID3", self.trip.group(1) )

		if self.RE_uses_line( self.RE_KEY_IDX_LINE ):
			key, idx, val = self.trip.groups()
			if idx == "":

				# check key of one of:
				# DISCID
				# DTITLE
				# DYEAR
				# DGENRE
				# PLAYORDER
				# NOT DID3 - it has idx == 3! # ABOVE

				# NOT # TTITLE
				# NOT # TARTIST # not in V1 ?
				# NOT # EXTT
					
				if key == "DTITLE":
				 # NOW CHECK FOR MULTI-LINE DTITLE!
				 # DTITLE is special because of its '/' processing
				 # so simply look for previous _ARTIST value
				 if self.Album.DTITLE_ARTIST:
					print "--MULTI-LINE--"
					print "self.Album.DTITLE_ARTIST:", self.Album.DTITLE_ARTIST
					# LURK # direct access to attribute
					val = val.strip();
					print "val:", val
					DTITLE_ALBUM = self.Album.DTITLE_ALBUM + u" " + val
					self.Album.DTITLE_ALBUM = self.Album.DTITLE_ALBUM + u" " + val
					print "DTITLE_ALBUM:", DTITLE_ALBUM
				 else:
					try:
						DTITLE_ARTIST, DTITLE_ALBUM = val.split(' / ', 1 )
					except:
						DTITLE_ALBUM = val
						DTITLE_ARTIST = val
					self.Album.set_attr( "DTITLE_ARTIST", DTITLE_ARTIST )
				 self.Album.set_attr( "DTITLE_ALBUM", DTITLE_ALBUM )
				 regen = DTITLE_ARTIST.strip() + " / " + DTITLE_ALBUM.strip()
				 ### retain old attribute ?
				 self.Album.set_attr( key, regen )
				elif key == "EXTD":
					# EXTD #
					self.Album.set_attr_list( key, val )
				else:
					# odd
					self.Album.set_attr( key, val )
			else:
				idx = int( idx )
				if 0:
					print "#3# %-10s %2s %s" % ( key, idx, val )
				trk = self.Album.get_track( idx )
				if key == "TTITLE":
					if self.sep_title_artist:
						left, right = val.split(self.sep_title_artist, 1 )
						if self.artist_before_title:
							trk.set_attr( "TARTIST", left )
							trk.set_attr( "TTITLE", right )
						else:
							trk.set_attr( "TTITLE", left )
							trk.set_attr( "TARTIST", right )
					else:
						trk.set_attr( key, val )
				elif key == "TARTIST":
					self.TARTIST_found = True
					trk.set_attr( key, val )
				elif key == "EXTT":
					# EXTT #
					trk.set_attr_list( key, val )
				else:
					# odd
					trk.set_attr( key, val )
			continue
		else:
			self.RE_error_parse_fail_uses_line()

	
# def test1( self ):
#	out = print_out()
#	self.Album.gen_cddb_text( out )
#	out.write_as_file( "test_cddb" )
#	print '# EOF #'
#	print
#	print '# ENQ #',
#	self.Album.gen_cddb_line( out )


#############################################################################
