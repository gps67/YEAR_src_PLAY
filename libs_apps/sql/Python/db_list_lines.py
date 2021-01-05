#!/usr/bin/env python
# list segs

# import sys
# import traceback
# sys.stderr = sys.stdout

# find scripts in sub-dirs ...
# or use PTH ??
import sys
subdirs = [
	'./graphs',
#	'./data_sql_PGSQL',
	'./data_sql',
	'./misc',
	'.'
]
for dir in subdirs:
	sys.path.append( dir )

print 'PATH:', sys.path

# import time
from line_list import Line_list
from seg0_list import Seg0_lister
from sql_link import SQL_link

sql = SQL_link()
line_mgr = Line_list( sql )
lines = line_mgr.list_rows()
# map = line_mgr.map()

atts = []
atts.append('id')
atts.append('site')
atts.append('host')
atts.append('cmd')
atts.append('opt')
atts.append('dt')
atts.append('avg')
atts.append('dev')
if 0:
	for line in lines:
		print 'line {'
		for at in atts:
			val = line.__dict__[ at ]
			print ' %11s  %s' % (at,val)
		print '}'
else:
	for line in lines:
		for at in atts:
			val = line.__dict__[ at ]
			if at=='host': val = '%9s' % val
			print '%s' % (val),
			#print '%s %s' % (at,val),
		print 

