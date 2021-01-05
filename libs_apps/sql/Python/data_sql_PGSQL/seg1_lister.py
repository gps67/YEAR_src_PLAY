import time

class Seg1:
  """ A seg1 is a seg with data as a vector
      nb line might not be registered, eg dt resampled but not saved
  """
  def __init__( seg, line, t0, t2, vec ):
    raise "unused"
    seg.line = line
    seg.t0 = t0
    seg.t2 = t2
    seg.vec = vec
  def dt( seg ):
    return seg.line.dt

	# return segs.list

class Seg1_lister:
  def __init__( segs, sql ):
	segs.sql = sql
  def seg1_from_tpl( segs, tpl ):	# NOT ACTUALLY A Seg1 !!
	(t0,t2,idx_zflt) = tpl
	print "LOADED LINE SEG:", segs.line, t0, t2, idx_zflt
	zflt = segs.sql.lo_fetch( idx_zflt )
	item = (t0,t2,zflt)
	return item
  def list_query( segs, query ):
	list_ = segs.sql.load_tpls( query ) # (t0,t2)
	# convert to vectors here
        vec_list = []
        for item in list_:
		item = segs.seg1_from_tpl( item )
                vec_list.append( item )
	return vec_list
  def list_line_t0_t2( segs, line, t0, t2 ):
	segs.line = line
	fields ="t0,t2,idx_zflt"
	conds ="line=%d and t0<%d and t2>%d" % (line,t2,t0) 
	query ="SELECT %s FROM seg WHERE %s" % (fields,conds)
	return segs.list_query( query )
  def prints( segs ):
	raise "segs lister OBTAINS the list not HOLD the list"
	t2_prev = 0
	dels = []
	for seg in segs.list_:
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
