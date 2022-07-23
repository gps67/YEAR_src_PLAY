set term png size 1200, 800
set output 'CET2.png'
set title 'cetml1659on.dat'
set ylabel 'Centigrade'

init_X(x) = (X1 = X2 = X3 = X4 = X5 = X6 = X7 = X8 = X9 = sum = 0 )
init_Y(x) = (Y1 = Y2 = Y3 = Y4 = Y5 = Y6 = Y7 = Y8 = Y9 = sum = 0 )
init_XY(x) = ( init_X(x), init_Y(0) )

# count $0 number of points upto 9 then 9
N9(x) = x > 8 ? 9 : (x+1)
N5(x) = x > 4 ? 5 : (x+1)
N3(x) = x > 2 ? 3 : (x+1)

# store a new value, and shift all 9 stored values
shift_X9(x) = (X9=X8, X8=X7, X7=X6, X6=X5, X5=X4, X4=X3, X3=X2, X2=X1, X1=x)
shift_Y9(y) = (Y9=Y8, Y8=Y7, Y7=Y6, Y6=Y5, Y5=Y4, Y4=Y3, Y3=Y2, Y2=Y1, Y1=y)
# shift_XY(x,y) = ( shift_X9(x), shift_Y9(y) )

# shift value and compute avg # nb $0 
avg_X9(x) = (shift_X9(x), (X1+X2+X3+X4+X5+X6+X7+X8+X9)/N9($0))
avg_Y9(y) = (shift_Y9(y), (Y1+Y2+Y3+Y4+Y5+Y6+Y7+Y8+Y9)/N9($0))

avg_X3(x) = (shift_X9(x), (X1+X2+X3)/N3($0))
avg_Y3(y) = (shift_Y9(y), (Y1+Y2+Y3)/N3($0))

datafile = "CET2.tmp"
sum = 0

# init_XY(0) # done ONCE not every point
# second init_XY(0)  explains all statement run then next
plot 	\
	sum = init_XY(0), \
	\
	datafile using 1:2 \
	with lines \
	title 'data' \
	lw 1 \
	lc rgb 'forest-green', \
	\
	\
	'' using (avg_X9($1)):(avg_Y9($2)) \
	with lines \
	title "avg 9 points" \
	lw 3 \
	lc rgb "blue",  \
	\
##	pt 7 \
##	ps 0.5 \
#	\
#	'' using 1:(sum = sum + $2, sum/($0+1)) \
#	title "cumulative mean" \
#	pt 1 \
#	lw 1 \
#	lc rgb "dark-red"

#	'' using (avg_X3($1)):(avg_Y3($2)) \
#	with lines \
#	title "avg 3" \
#	lw 1 \
#	lc rgb "red",  \
#	\
#	sum = init_XY(0), \

