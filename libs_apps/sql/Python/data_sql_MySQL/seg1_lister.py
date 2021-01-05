import time

class Seg1:
  """ A seg1 is a seg with data as a vector
      nb line might not be registered, eg dt resampled but not saved
  """
  def __init__( seg, line, t0, t2, vec ):
    seg.line = line
    seg.t0 = t0
    seg.t2 = t2
    seg.vec = vec

  def dt( seg ):
    return seg.line.dt

class Seg1_lister:
  def __init__( segs, sql ):
	segs.sql = sql
  def list_query( segs, query ):
	segs.sql.cursor.execute( query )
	segs.list = segs.sql.cursor.fetchall() # (t0,t2)
	# convert to vectors here
	if 0:
            vec_list = []
            for item in list:
              (t0,t2,zflt) = item
              v = mk_vec_from_zflts( zflt )
              item = (t0,t2,v)
              vec_list.append( item )
  def list_line_t0_t2( segs, line, t0, t2 ):
	segs.line = line
	fields ="t0,t2,zflt"
	conds ="line=%d and t0<%d and t2>%d" % (line,t2,t0) 
	query ="SELECT %s FROM seg WHERE %s" % (fields,conds)
	segs.list_query( query )
	return segs.list
  def prints( segs ):
	t2_prev = 0
	dels = []
	for seg in segs.list:
		(t0, t2, zflts) = seg
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
		str = time.strftime( "%c", tpl )
		print segs.line, t0, str, "+", dt_day, "days", dt_hr, dt_min, dt_sec
