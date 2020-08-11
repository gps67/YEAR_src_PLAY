#!/usr/bin/env python

import subprocess
def run_cd_diskid():
	exe = 'cd-discid'
	argv = [ exe, '/dev/cdrom' ]
	proc = subprocess.Popen('cd-discid', stdout=subprocess.PIPE)
	output = proc.stdout.readlines()
#	print "PIPE:", output
	return output

	

if 0:
	TXT="cddb_line.txt"
	fd = open(TXT, "r" )
	text = fd.readlines()
	fd.close()
else:
	text = run_cd_diskid()

line1 = text[0]
words = line1.split( )
# print line1
# print words

id_id = words[0]
id_ntracks = words[1]
id_seconds = words[-1]
id_starts = words[2:-1]
## print len( words )
## print line1
## print id_starts

CD_TITLE = "Title"
CD_ARTIST = "Artist"
CD_GENRE = "misc"
CD_YEAR = "2020"

# not a clue
CD_ID3="118"

print "# xmcd CD database file generated by SCRIPT"
print "#"
print "# Track frame offsets:"
for offset in id_starts:
	print "#	%s" % offset
print "#"
print "# Disc length: %s seconds" % id_seconds
print "#"
print "# Revision: 0"
print "# Submitted via: SCRIPT 0.1.2"
print "#"
print "DISCID=%s" % id_id
print "DTITLE=%s / %s" % ( CD_ARTIST, CD_TITLE )
print "DYEAR=%s" % CD_YEAR
print "DGENRE=%s" % CD_GENRE
# XX # print "DID3=%s" % CD_ID3
#print "DID3=%s" % CD_ID3
N0=0-1
N1=1-1
for offset in id_starts:
	N0 += 1
	N1 += 1
	# print "TTITLE%d=_" % (N0)
	print "TTITLE%d=T%2.2d" % (N0, N1)

