import string

class A_line_id:
  # when a line is loaded it is auto put into a class
  # A_line_id offers the added feature of splitting attr to left.right
  def load_fixup( line ):
	# this should be unused
	l = string.split( line.attr, '.', 2 )
	if len(l) == 1:
	    l = [ '-', l[0] ]
	line.attr_left  = l[0]
	line.attr_right = l[1]
 #def __init__( row ):

class Line_list:
  def __init__( lines, sql ):
	lines.sql = sql
	lines.id_list = None
	lines.row_list = None
	lines.map_dry = None

  def list_rows( lines ):
	if None == lines.row_list:
		query = "SELECT * FROM line_id ORDER by id"
		ctor = A_line_id
		row_list = lines.sql.load_list_of_rows( query, ctor )
		lines.row_list = row_list
	return lines.row_list 

	#	for r in row_list:
	#    r.load_fixup()
	return lines.row_list 

  def list_ids( lines ):
	if lines.id_list == None:
		list = []
		for line in lines.list_rows():
			list.append( line.id )
		lines.id_list == list
	return lines.id_list 

  def map( lines ):
	if lines.map_dry == None:
		map = {}
		for line in lines.list_rows():
			map[ line.id ] = line
		lines.map_dry == map
	return lines.map_dry

