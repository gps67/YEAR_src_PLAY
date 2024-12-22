#! /home/gps/prefix/bin/bash
#!/bin/bash
# cat 5 hurricane top 10 list
# someone says it show something
# it shows a lack of data
# but getting there ...
# ... needs a graph

# example of single script (name needs to be 1 word)
# that calls gnuplot
# result is a second file .png
# but thats usually needed

echo "# FAIL # CSV uses data truncated FMT $4.2f , 0.01 is 1%"
# https://www.officialdata.org/uk/inflation/1933 # plus endless args #

export NAME=inflation_data
export CSV=${NAME}.csv
export PNG=/tmp/gnuplot_${NAME}.png
rm -f $PNG

cat_file_one() 
{
#	sed 1d $CSV | awk -F ',' '{print $3}' | tee /tmp/log1.txt > /dev/null
	sed 1d $CSV # MACRO cat_FILE_remove_first_line
	# CXX MACRO # S1 == $CSV # MATCH_HERE_CODE_POINT #
}

cat_file_year_val() 
{
	# for unknown reason $3 is being truncated to whole number
	cat_file_one \
	| gawk -N -F ',' '{printf("%d %f\n", $1, 100.0*$3)}'| grep ..  | sort \
	#
}


plot_one()
{
	local n=0
	# ((n++)) # is how #!/bin/bash does { incr n }
	local title_graph='$NAME'
	local title_line='sum' # be brief
	local title_y='percent inflation'
	local title_x='year'
	cat_file_year_val \
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
	cat_file_year_val \
	| while read year press
	do
		echo "$year $press"
	done \
	| gnuplot \
	-e "set term png size 1200, 800" \
	-e "set output '$PNG'" \
	-e "set title '$NAME'" \
	-e "set ylabel 'Inflation'" \
	-e "plot '-' using 1:2 with linespoints title 'flate" \
	#
}

# cat_file_year_val
 # plot_one
 plot_two
m $PNG


