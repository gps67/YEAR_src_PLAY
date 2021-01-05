import time
import sql_link

class Seg0_lister:
  def __init__( segs, sql ):
	segs.sql = sql
	segs.lista = [] # doc
	# raise "unused in main - used in list segs code"
  def N( segs ):
	return len( segs.lista )
  def list_query( segs, query ):
	segs.lista = segs.sql.load_tpls( query ) 
  def list_line( segs, line ):
	segs.line = line
	query = "SELECT t0,t2 FROM seg WHERE line=%d" % line
	segs.list_query( query )
  def list_line_t0_t2( segs, line, t0, t2 ):
	segs.line = line
	query ="SELECT t0,t2 FROM seg WHERE line=%d and t0<%d and t2>%d" % (line,t2,t0) 
	segs.list_query( query )
  def list_all_t0_t2( segs, t0, t2 ):
	segs.line = line
	query ="SELECT t0,t2 FROM seg WHERE t0<%d and t2>%d" % (line,t2,t0) 
	segs.list_query( query )
  def overlaps( segs ):
	t2_prev = 0
	dels = []
	for seg in segs.lista:
		(t0, t2) = seg
		if t2_prev <= t0:
			t2_prev = t2
			continue
		else:
			dels.append( seg )
	return dels

  def prints( segs ):
	t2_prev = 0
	dels = []
	for seg in segs.lista:
		(t0, t2) = seg
		dt1 = t2 - t0
		dt_sec = dt1 % 60
		dt1 = dt1 / 60
		dt_min = dt1 % 60
		dt1 = dt1 / 60
		dt_hr = dt1 % 24
		dt1 = dt1 / 24
		dt_day = dt1
		# tpl = time.localtime( t0 )
		tpl = time.gmtime( t0 )
		tpl2 = time.gmtime( t2 )
		fmt = "%c"
		fmt = "%Y-%m-%d %H:%M:%S"
		str = time.strftime( fmt, tpl )
		str2 = time.strftime( fmt, tpl2 )
		print segs.line, t0, str, "+", dt_day, "days", dt_hr, dt_min, dt_sec, "=", str2
