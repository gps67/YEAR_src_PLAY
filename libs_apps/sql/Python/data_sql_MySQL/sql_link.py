import MySQLdb

class SQL_link:
  def __init__( sql ):
	#
	host     = "localhost"
	user     = "obj_stat_gather"
	pasw     = "gather99"
	sql.base = "obj_stat_1"
	#
	sql.link = MySQLdb.Connection(
		host=host,
		user=user,
		passwd=pasw,
		db=sql.base
	)
	sql.cursor = sql.link.cursor()

  def close( sql ):
	sql.link.close()

  def load_list_of_rows( sql, querystr, ctor ):
	"""
		returns a list of objects
		each object is created = ctor()
		then each obj.attr = val
	"""
	sql.cursor.execute( querystr )
	rows = sql.cursor.fetchall()
	infos = sql.cursor.description
	#
	# re-process into a list of objects
	#
	row_list = []
	for vals in rows:
		aline = ctor()
		# sql field names must be valid as python names
		# and no join (.) in the table.colname
		col = -1
		for info in infos:
			# print info
			name = info[ 0 ]
			col = col + 1
			val = vals[col]
			aline.__dict__[name] = val
		row_list.append( aline )
	return row_list

