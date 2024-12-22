
# cat 5 hurricane top 10 list
# someone says it show something
# it shows a lack of data
# but getting there ...
# ... needs a graph

# example of single script (name needs to be 1 word)
# that calls gnuplot
# result is a second file .png
# but thats usually needed

cat_file() 
{
	# cat plot_cat_5.dat 
	# data is limited to landfall in Florida
	# data omits anything milder than 955/1000 low pressure
	# count means it is the SLOPE that matters
	# shows flurry around 1945
	# shows nothing much
	# shows lack of data
	cat <<-EOT
		1 1935  892 5 LaborDay
		2 2018  919 5 Michael
		3 1992  922 5
		4 1919  927 4
		5 1928  929 4
		6 1926  930 4
		6 1960  930 4
		7 2017  931 4
		8 1948  940 4
		8 2022  940 4 Ian
		9 2004  941 4
		10 1947 943 4
		11 1945 949 4
		12 1949 964 4
		13 1950 955 4
	EOT
}

export PNG=/tmp/one_file_gnuplot.png
rm -f $PNG


plot_one()
{
	local n=0
	# ((n++)) # is how #!/bin/bash does { incr n }
	local title_graph='counting cat 4 cat 5 florida'
	local title_line='sum' # be brief
	local title_y='count'
	local title_x='year'
	cat_file \
	| awk '{print $2}'| sort | grep ..  | \
	while read year
	do
		((n++))
		echo "$year $n"
	done \
	| gnuplot \
	-e "set term png size 1200, 800" \
	-e "set output '$PNG'" \
	-e "set title '$title_graph'" \
	-e "set ylabel '$title_y'" \
	-e "set xlabel '$title_x'" \
	-e "plot '-' using 1:2 with linespoints title '$title_line'" \
	#
}

plot_two()
{
	cat_file \
	| awk '{print $2, $3}'| sort | grep ..  | \
	while read year press
	do
		echo "$year $press"
	done \
	| gnuplot \
	-e "set term png size 1200, 800" \
	-e "set output '$PNG'" \
	-e "set title 'not enough data'" \
	-e "set ylabel 'pressure'" \
	-e "plot '-' using 1:2 with linespoints title 'ranking'" \
	#
}

  plot_one
# plot_two
m $PNG


