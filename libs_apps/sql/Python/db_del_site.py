#!/tools/2001/py/bin/python
#!/usr/bin/env python

doit = 0
doit = 1

# delete segs where they overlap the previous seg

# find scripts in sub-dirs ...
import sys
subdirs = [
	'./graphs',
	'./data_sql_MySQL',
	'./misc',
	'.'
]
for dir in subdirs:
	sys.path.append( dir )

# from line_list import Line_list
# from seg0_list import Seg0_lister
from sql_link import SQL_link

# import sys
# import traceback
# sys.stderr = sys.stdout

sql = SQL_link()

### lines = Line_list( sql )
print "## USE", sql.base

site = "Firm_Building"
query ="SELECT id FROM line_id WHERE site='%s'" % site
t2_prev = 0
ids = []
sql.cursor.execute( query )
for row in sql.cursor.fetchall():
	ids.append( row[0] )
print ids

for id in ids:
	query = "UPDATE seg SET t0=0 WHERE line=%d " % id
	query = "DELETE FROM seg WHERE line=%d  AND t0=0" % id
	t2_prev = 0
	ids = []
	sql.cursor.execute( query )
	print sql.cursor.fetchall()

if 0:
	query ="DELETE FROM line_id WHERE site='%s'" % site
	t2_prev = 0
	ids = []
	sql.cursor.execute( query )
	print sql.cursor.fetchall()
else:
	print "delete line_id records not done"

sql.close()
