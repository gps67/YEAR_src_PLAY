#  this is a gnuplot file, invoke with:
#
#     gnuplot -p -c z.plot
#
# (C) Graham Swallow 2022 # LGPL # open source #

# this is the no_awk version, it replaces a script that did:
# sed 1,7d | awk '{ if($14 >0) print $1 ", "$14 }' > datafile1
# it needs to catch absent data marked with -99.9 (awk did that)
# it uses $14 instead of $2
# gnuplot simply ignores where $14 is absent (so no sed 1,7d)

#  TODO  # how to do string3 = string1 + string2

# script variables can have any varname # not "special" to gnuplot
# these can easily be confused with parameters, which use the "set" command
# last one wins # ditto for set command

  datafile = "../cetml1659on.dat"
  url = "https://www.metoffice.gov.uk/hadobs/hadcet/cetml1659on.dat"
  title = url
  png_file = "CET2.png"
  y_size =  800 # height pixels
  x_size = 1200 # width
  y_label = 'Centigrade'
  # x is obviously year # save screen realestate # reduce distractions
  y_hi  =    11.0 # actually y data goes higher
  y_lo  =     7.8
  y_tics =    0.1
  x_tics =   20 

# other global variables
# X1 # we keep 9 X variables
# Y1 # we keep 9 Y variables
# $0 # we use the data-line-number (from 0)

# set gnuplot parameters # lots of side-effects

  set term png size x_size, y_size 
  set output png_file
  set ylabel y_label
  set ytics y_tics
  set xtics x_tics
  set yrange [y_lo:y_hi]
  set title title

# gnuplot functions, taking a parameter (or two)
# we keep 9 variables, to take the running average of them

  init_X(x) = (X1 = X2 = X3 = X4 = X5 = X6 = X7 = X8 = X9 = 0 )
  init_Y(x) = (Y1 = Y2 = Y3 = Y4 = Y5 = Y6 = Y7 = Y8 = Y9 = 0 )
  init_XY(x) = ( init_X(x), init_Y(0) )

# SYNTAX # myfunc(parameter_required) = EXPR_RETVAL
# SYNTAX # (not exactly certain RTFM)
# comma separates sub-statements
# parentheses around all - not needed but looks OK
# one parameter needed to indicate it is a function ? - not sure

# store a new value, and shift all 9 stored values

  shift_X9(x) = (X9=X8, X8=X7, X7=X6, X6=X5, X5=X4, X4=X3, X3=X2, X2=X1, X1=x)
  shift_Y9(y) = (Y9=Y8, Y8=Y7, Y7=Y6, Y6=Y5, Y5=Y4, Y4=Y3, Y3=Y2, Y2=Y1, Y1=y)

# before there are 9 values, there are fewer
# N9($0+1) counts number of points upto 9 then 9
# SYNTAX # BOOL_EXPR ? ACTION_WHEN_TRUE : ACTION_WHEN_FALSE

  N9(n) = n >= 9 ? 9 : n
  N5(n) = n >= 5 ? 5 : n
  N3(n) = n >= 3 ? 3 : n

# shift value and return computed avg # nb $0 
# nb $0 is the data line number # magically not counting skipped lines

  avg_X9(x) = (shift_X9(x), (X1+X2+X3+X4+X5+X6+X7+X8+X9)/N9($0+1))
  avg_Y9(y) = (shift_Y9(y), (Y1+Y2+Y3+Y4+Y5+Y6+Y7+Y8+Y9)/N9($0+1))

# again with 3 (less tested, not suited for this data)

  avg_X3(x) = (shift_X9(x), (X1+X2+X3)/N3($0+1))
  avg_Y3(y) = (shift_Y9(y), (Y1+Y2+Y3)/N3($0+1))

# if data item is missing use previous
# X DATA is never -99.9 # it is always there
# Y DATA # uses -99.9 for absent data # eg this year is future
# CODE uses -99 # because of floating errors near decimals
# LURK hardcoded -99 #
# allow for small -3 numbers (annual average !! expect +9) 
# remember this, for data that goes anywhere near -90
# remember this, for data that uses -1 as a marker for absent cell
# remember this, for data that actually contains missing data in the middle
# remember this, for data that actually contains missing data at the start
# none of the above CURRENTLY appear in this data, other than future months

  Q_val_X(x) = x
  Q_val_Y(y) = y < -99 ? Y1 : (Y1 = y)

# bounce # double step Q and AVG
# Quality control Y (repeat previous value) then shift it in and return average
# Q_avg_X9(x) = avg_X9(x) # since x is never absent, could be without bounce

  Q_avg_X9(x) = avg_X9(Q_val_X(x))
  Q_avg_Y9(y) = avg_Y9(Q_val_Y(y))

  Q_avg_X3(x) = avg_X3(Q_val_X(x))
  Q_avg_Y3(y) = avg_Y3(Q_val_Y(y))

# plot with 4 sub commands ending with comma or eoln
# one init and 3 lines
# two datalines share the same INPUT from datafile # HOW ? # by naming it #
# third dataline uses a second file, with manually edited values
#
# SYNTAX # more experimentation than RTFM
# datafile # is the varname of 'the_data_file.txt"
# using 1:2 # pick fields 1 2 
# using ($1):($2) # pick fields 1 2 # RTFM why the difference # IDK #
# using (expr):(expr) # RTFM for examples
# title 'linetitle' # appears in box # could be variables # for each line
# with lines # otherwise you get dots #
# with linespoints # etc # RTFM
# lw 3 # line width 3 pixels #
# lc rgb 'colorname' # line colour #
# , # comma ends the sub-command #
# init_XY(0) # done ONCE not every point
# it is done once, not 3 times, not 100 times
# because shift is only called by second line
# actually I am unsure about that # maybe runs 3 phases # 
# cant easily be concurrent, so must be 3 times
# but only the middle stanza uses X9..X1
#
# SYNTAX # BACKSLASH EOLN
# you cannot have a SPACE or ANYTHING after the backslash at EOLN
# seems OK with ^M on DOS (CRLF)
# so no comments on the following :-)

  plot 						\
	unused = init_XY(0), 			\
						\
	datafile 				\
	  using (Q_avg_X9($1)):(Q_avg_Y9($14))	\
	  title "avg 9 points" 			\
	  with lines 				\
	  lw 3 					\
	  lc rgb "blue",  			\
						\
	'mylines.dat' 				\
	  using 1:2 				\
	  title "markers" 			\
	  with lines 				\
	  lw 1 					\
	  lc rgb "black",  			\
						\
  # EOLN # of long line

# removed green line # every year
#
#	datafile 				\
#	  using ($1):(Q_val_Y($14)) 		\
#	  title 'data' 				\
#	  with lines 				\
#	  lw 1 					\
#	  lc rgb 'forest-green', 		\

# 'mylines.dat' # some horizontal and vertical hand-drawn lines
# touch mylines.dat so that no error happens or delete the code below
# or paste the following data in, for fun. edit out un-comment, not indented?
# needs better values, but more pressingly the data source has disappeared
#	
#	1650  8.80
#	1725  8.80
#	
#	1650  9.15
#	1940  9.15
#	
#	1650  9.60
#	1990  9.60
#	
#	1650 10.30
#	2050 10.30
#	
#	
#	1740 11.0
#	1740  7.8
#	
#	1900  9.15
#	1900  7.8
#	
#	1930  9.6
#	1930  7.8
#	
#	1990 10.3
#	1990  7.8
#	
#	2020 11.0
#	2020  7.8
#	
#	# some arbitrary horizontal and vertical lines
#	# for grid measurements

