set term png size 1200, 800
set output 'CET2.png'
set title 'cetml1659on.dat'
set ylabel 'Centigrade'

# datafile = "CET2.tmp"
datafile = "../cetml1659on.dat"

# this is the no_awk version
# it uses $14 instead of $2
# it needs to catch -99 absent data (awk did that)
# gnuplot simply ignores where $14 is absent
# sed 1,7d | awk '{ if($14 >0) print $1 ", "$14 }' > datafile1

  init_X(x) = (X1 = X2 = X3 = X4 = X5 = X6 = X7 = X8 = X9 = 0 )
  init_Y(x) = (Y1 = Y2 = Y3 = Y4 = Y5 = Y6 = Y7 = Y8 = Y9 = 0 )
  init_XY(x) = ( init_X(x), init_Y(0) )

# count $0+1 number of points upto 9 then 9
  N9(x) = x >= 9 ? 9 : x
  N5(x) = x >= 5 ? 5 : x
  N3(x) = x >= 3 ? 3 : x

# store a new value, and shift all 9 stored values
  shift_X9(x) = (X9=X8, X8=X7, X7=X6, X6=X5, X5=X4, X4=X3, X3=X2, X2=X1, X1=x)
  shift_Y9(y) = (Y9=Y8, Y8=Y7, Y7=Y6, Y6=Y5, Y5=Y4, Y4=Y3, Y3=Y2, Y2=Y1, Y1=y)
# shift_XY(x,y) = ( shift_X9(x), shift_Y9(y) )

# shift value and compute avg # nb $0 
  avg_X9(x) = (shift_X9(x), (X1+X2+X3+X4+X5+X6+X7+X8+X9)/N9($0+1))
  avg_Y9(y) = (shift_Y9(y), (Y1+Y2+Y3+Y4+Y5+Y6+Y7+Y8+Y9)/N9($0+1))

# again with 3 (less tested, not suited for this data)
  avg_X3(x) = (shift_X9(x), (X1+X2+X3)/N3($0))
  avg_Y3(y) = (shift_Y9(y), (Y1+Y2+Y3)/N3($0))

  avg_X3(x) = (shift_X9(x), (X1+X2+X3)/N3($0+1))
  avg_Y3(y) = (shift_Y9(y), (Y1+Y2+Y3)/N3($0+1))

# if data item is missing use previous # not X it is always there
  Q_val_X(x) = x
  Q_val_Y(y) = y < -99 ? Y1 : (Y1 = y)

# bounce
# Q_avg_X9(x) = avg_X9(x)
  Q_avg_X9(x) = avg_X9(Q_val_X(x))
  Q_avg_Y9(y) = avg_Y9(Q_val_Y(y))

  Q_avg_X3(x) = avg_X3(Q_val_X(x))
  Q_avg_Y3(y) = avg_Y3(Q_val_Y(y))

# init_XY(0) # done ONCE not every point
# second init_XY(0)  explains all statement run then next
# unused # else 0 is on Y-axis #

set yrange [8:10.7]
set ytics 0.1
set xtics 25


  plot 	\
	unused = init_XY(0), \
	\
	datafile using ($1):(Q_val_Y($14)) \
	with lines \
	title 'data' \
	lw 1 \
	lc rgb 'forest-green', \
	\
	datafile using (Q_avg_X9($1)):(Q_avg_Y9($14)) \
	with lines \
	title "avg 9 points" \
	lw 3 \
	lc rgb "blue",  \
	\
	'mylines.dat' using 1:2 \
	with lines \
	title "markers" \
	lw 1 \
	lc rgb "black",  \
	\
#

# you can put the following back
# it is a annual green line
# that bounces around a lot
# it shows the 9 year average is good


