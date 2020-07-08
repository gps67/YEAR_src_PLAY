# 
# You need gnuplot version 5 to plot multi-lines (or something)
# that is now standard, it might not be for you, upgrade
# so download it, compile it (works first time :-) put it on path
# then write this script to suit YOU not ME !!
#
# this also computes the range graph
#
# show_as_dates.tcl is optional
# remember MIN MAX dates get abbreviated to 2 decimal places
# so accurate to 3 days (365*0.01)/2 = 3.65 days / 2
# actual day data use 3 decimal places, so += 8 hours / 2 or something
# also remember that data is fudged before publish, and time is midday
#
# show_as_dates.tcl 
# it gives you the min max points
# it also shows the most recent data point date
# which is reassuring
# 

[ -d tmp ] || mkdir tmp
. fns_var_add
var_pfx PATH /home/gps/prefix/bin ':'
(
	gnuplot --version
	plot_daily_volume.awk "$@" 
	show_as_dates.tcl
	plot_min_max_range "$@" 
) 2>&1 | tee tmp/tmp_$(basename $0).tmp \
# | less
# )| less

