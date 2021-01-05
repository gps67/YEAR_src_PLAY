# import MySQLdb
import pg
from pg import INV_READ
# uses PyGreSQL http://druid.net/pygresql/

class SQL_link:
  def __init__( sql ):
	host="localhost"
	user="obj_stat_gather_user"
	user="obj_stat_gather"
	pasw="gather99"
	sql.base="obj_stat_1"
	sql.lnk = pg.connect(host=host,user=user,passwd=pasw,dbname=sql.base)
#	sql.cursor = sql.lnk.cursor()
  def close( sql ):
	sql.lnk.close()

  def lo_fetch( sql, oid ):
	lo = sql.lnk.getlo( oid )
	sql.lnk.query("BEGIN")
	lo.open( INV_READ )
	str1 = lo.read( 1024000 )
	lo.close()
	sql.lnk.query("END")
	return str1

  def lo_size( sql, oid ):
	lo = sql.lnk.getlo( oid )
	sql.lnk.query("BEGIN")
	lo.open( INV_READ )
	sz = lo.size()
	lo.close()
	sql.lnk.query("END")
	return sz

  def lo_delete( sql, oid ):
	lo = sql.lnk.getlo( oid )
	sql.lnk.query("BEGIN")
	lo.unlink( )
	sql.lnk.query("END")

  def begin_cmd_end( sql, cmd ):
	sql.lnk.query("BEGIN")
	qo = sql.lnk.query( cmd )
	if qo == None:
		ret = None
	else:
		ret = qo.getresult()
	sql.lnk.query("END")
	print "ret=", ret
	return ret

  def load_tpls( sql, querystr ):
	qo = sql.lnk.query( querystr )
	rows = qo.getresult()
	return rows

  def load_list_of_rows( sql, querystr, ctor ):
	id_list = []
	row_list = []
	qo = sql.lnk.query( querystr )
	# sql.cursor.execute( querystr )
	rows = qo.getresult()
	infos = qo.listfields()
	for vals in rows:
		aline = ctor()
		# sql field names must be valid as python names
		# and no join (.) in the table.colname
		col = -1
		for info in infos:
			name = info
			col = col + 1
			val = vals[col]
			# aline.$name = val # -ish-
			aline.__dict__[name] = val
		row_list.append( aline )
	return row_list

