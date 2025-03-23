#!/usr/bin/awk -f
#!/usr/bin/gawk -f

# this is the only script you need # mostly
# fortunately the data starts at day 0 of 1979 (1?)
# fortunately the data has no (-1) missing values
# fortunately the data has 365 normalised days in each year (no leap)

# you might also run plot_range, AFTER running this

# every year you must find and edit the filename
#	F="PIO/PIOMAS.vol.daily.1979.2020.Current.v2.1.dat"


BEGIN {
	D_tmp = "/tmp/tmp_" # not secure not multiuser useful # ./tmp/tmp_
	D_tmp = "./tmp_" 
	D_tmp = "./tmp/tmp_" 
	# mk_tarball will ignore *.tmp
	# want to put min max mid into tarfile # so no .tmp
	# what all the files are named, in variables named ...
	filename_plot_data_day = D_tmp "plot_data_day.data.tmp"
	filename_plot_data_min = D_tmp "plot_data_min.data"
	filename_plot_data_max = D_tmp "plot_data_max.data"
	filename_plot_data_mid = D_tmp "plot_data_mid.data"
	filename_plot_data_avg1 = D_tmp "plot_data_avg1.data.tmp"
	filename_plot_data_avg2 = D_tmp "plot_data_avg2.data.tmp"
	filename_plot_data_avg3 = D_tmp "plot_data_avg3.data.tmp"
	filename_plot_data_pencil = D_tmp "plot_data_pencil.data.tmp"
	filename_plot_name ="sea_ice_vol"

	plot_title="PIOMAS Arctic Sea Ice Volume"

	# A365=365 # one year average # scroll down # LAST ONE WINS #
	# MUST be a multiple of 365 # TODO # first 365 then 1.5 year
	# I want to rename avg2 as avg365
	# but that comes from the A365 parameter (set next)

	A365=365*2
	A365=365*10
	A365=365*4
	A365=500 # days - makes less sense than < 365
	A365=300 # days
	A365=90 # days
	A365=180 # days
	A365=365*3
	A365=365*1
	
	A365=365*5	#  flat for 8 years
	A365=365*1
	A365=365*10
	A365=365*3	# curves
	A365=365*3
	A365=365*4
	A365=365*2
	A365=365*1
	A365=365*4
	A365=365*10
	A365=365*6
	A365=365*4
	A365=365*1 # looks like spraying started 2012
	A365=365*5 # looks like a step down, then exponential decay to V2
	A365=365*4
	A365=365*3
	A365=365*2
	A365=365*6 # looks like a step down, then exponential decay to V2
	A365=365*5 # looks like a step down, then exponential decay to V2

	A365=365*1 # this is PROPER # 365 day average # must be exact multiple
#	A365=365*2
	A365=365*5
	A365=365*1
	A365=365*3
	A365=365*5 # looks like a step down, then exponential decay to V2
	A365=365*1
	A365=365*1
	A365=365*4 # 6 years is smoother but 4 is nearer to 1
	A365=365*6 # looks like a step down, then exponential decay to V2
	A365=365*1
	A365=365*9
	A365=365*4 # OK
	A365=365*3 # sensible
	A365=365*2
	A365=365*1
	A365=365*3 # sensible
	A365=365*1
#	A365=365*2

	# PICK A365 # LAST one WON #

	F365=1.0*A365 # as a floating point # YEARS not days

	plot_to_png = 0
	plot_to_png = 1	# it is all .png

	# plot or not # OPTIONS #
#	P_8 = 1
#	P_9 = 0 # pencil lines
	P_max = 1
	P_min = 1
	P_mid = 1 # mid is within numeric year # close to blue
	P_mid = 0 # plot midyear avg(matches blue at YEAR.50)
	P_avg = 1 # plot rolling day 365 average

	# remove old files to avoid making it look OK when not
	rm_f_filename( filename_plot_data_day )
	rm_f_filename( filename_plot_data_min )
	rm_f_filename( filename_plot_data_max )
	rm_f_filename( filename_plot_data_mid )
	rm_f_filename( filename_plot_data_avg1 ) # +8 against max
	rm_f_filename( filename_plot_data_avg2 ) # == avg for year A365
	rm_f_filename( filename_plot_data_avg3 ) # -8 against min
	rm_f_filename( filename_plot_data_pencil ) # 

	year_curr = -1

	data_day[0] = 0	# ALL the data is put into this awk array
	data_day_pos = 0
	data_day_max = 0

	process_data_file()	# this is not my best code # tangled mess #

	b_zoomed_in = 0		# named flag is better than bit
	b_zoomed_out = 1	# named flag to produce the zoomed in version
	process_gnuplot_file( b_zoomed_out ) # produce the file
	process_gnuplot_file( b_zoomed_in ) # produce the file
	# how to avoid the AWK filter theme ?
	# close( /dev/stdin ) # nope
	# dont act as filter over stdin, so exit
	exit(0)
}

 function process_gnuplot_file( zoomed_out )
 {
	if( zoomed_out ) {
		P_9 = 1 # pencil lines
		name_part = "zoomed_out"
	} else {
		P_9 = 0 # pencil lines
		name_part = "zoomed_in"
	}

	filename_plot_script_zoomed   = D_tmp "plot_script_" name_part ".gnuplot"
	filename_plot_zoomed_png  = filename_plot_name "_" name_part ".png"

	rm_f_filename( filename_plot_script_zoomed )
	rm_f_filename( filename_plot_zoomed_png  )

	plot_script = plot_script_daily(filename_plot_zoomed_png, zoomed_out)
	print plot_script > filename_plot_script_zoomed
	close( filename_plot_script_zoomed )
	RUN4( "nl -ba " filename_plot_script_zoomed ) # line numbers are useful
	RUN4( "sh -c pwd" )
  	# print plot_script

	pipe_gnuplot = "gnuplot " filename_plot_script_zoomed
	pipe_view_png = "gm display " filename_plot_zoomed_png  " &"
	copy_png_to_OUT = "cp -p " filename_plot_zoomed_png  " OUT/ "
	# ampersand optional, stops waiting for it # may need SHELL support

	RUN4( "gnuplot " filename_plot_script_zoomed )
 if(plot_to_png) {
	RUN4( pipe_view_png )
	RUN4( copy_png_to_OUT )
 }
}

function get_filename_data_gz()
{
	# you have to edit out the 2016 ANYWAY
	# so option of linking to here or finding in 2016/FTP_
	F="PIOMAS.vol.daily.1979.2016.Current.v2.1.dat"
	F="PIOMAS.vol.daily.1979.2017.Current.v2.1.dat"
	F="PIOMAS.vol.daily.1979.2018.Current.v2.1.dat"
	F="PIOMAS.vol.daily.1979.2019.Current.v2.1.dat"
	F="PIO/PIOMAS.vol.daily.1979.2020.Current.v2.1.dat"
	F="PIO/PIOMAS.vol.daily.1979.2021.Current.v2.1.dat"
	F="PIO/PIOMAS.vol.daily.1979.2022.Current.v2.1.dat"
	F="PIO/PIOMAS.vol.daily.1979.2023.Current.v2.1.dat"
	F="PIO/PIOMAS.vol.daily.1979.2024.Current.v2.1.dat"
	F="PIO/PIOMAS.vol.daily.1979.2025.Current.v2.1.dat"
	return F
}
function Q1( str ) {
	return "'" str "'"
}
function RUN4( cmd ) {
	print "## " cmd
	print 9 | cmd
	close( cmd ) # otherwise it is still running 
}
function rm_f_filename( filename )
{
	RUN4(" rm -f " filename )
}

function flush_year_end() {

	# year_min_YF is the YEAR.frac when the min was seen
	# days_past_min is number of days AFTER min seen
	# days_past_max is ...

	Y_min2 = sprintf( "%.2f", year_min_YF )
	Y_max2 = sprintf( "%.2f", year_max_YF )
	L_min = Y_min2 " " year_min_vol 
	L_max = Y_max2 " " year_max_vol
	if( seen_peak_min )
		print L_min > filename_plot_data_min
	if( seen_peak_max )
		print L_max > filename_plot_data_max
	if( 320 < year_vol_avg_count ) { # allow a few missing points
		year_vol_avg = year_vol_avg_sum / year_vol_avg_count
		L_avg = year_curr + 0.5 " " year_vol_avg
		print L_avg > filename_plot_data_mid
	}
	# print L | pipe_gnuplot
}

function line_in( year, year_frac, day, vol ) {

	# IE( year_frac, vol ) for each days text line
	#
	# we use year to detect new year (jan-1st) and flush line
	# fortunately MIN and MAX are months away from 0101

	# process the line of data, into ALL hungry filters
	# this function is a does-it-all monster
	# it finds the PEAK and LOW values
	# it scrolls the averaging windows
	# that should be split out and rewritten # TODO #
	# slightly specific to PIOMASS #

	fmt_3 = "%.4f"
	fmt_3 = "%.3f"
	if( year_curr != year ) {
	 # start of a new year
	 # flush MIN MAX
	 if( year_curr != -1 ) flush_year_end()
	 year_curr = year
	 year_min_vol = vol
	 year_max_vol = vol
	 recent_min_vol = vol
	 recent_max_vol = vol

	 seen_upslope = 0 # defunct peak from two slope detectors
	 seen_downslope = 0
	 
	 seen_peak_max = 0
	 seen_peak_min = 0
	 days_past_peak = 0
	 peak_was_hi = 1
	 year_vol_avg_sum = 0
	 year_vol_avg_count = 0
	 year_min_YF = year_frac
	 year_max_YF = year_frac
	}
	year_frac3 = sprintf( fmt_3, year_frac )
	Y_now = year_frac
	# looking for PEAK - cant be within 10 days of year start or data end
	# this only works because data is already quite smooth
	# a zig zag noise would accumulate days of BOTH
	# ANY noise spoils monatonic tests here, should go for 10 less than peak

	D10 = 10
	D10 = 1
	D10 = 9
	D10 = 7 # 2016-04 only 8 days before end of month after max !!
	D10 = 3
	D20 = 20 
	C1 = "~"
	C2 = " "
	C3 = " "

	days_past_max ++
	days_past_min ++
	days_past_peak ++

	if( vol < recent_min_vol ) {
		# new min evolving
		C1 = "<"
		recent_min_vol = vol
		days_past_min = 0 
		days_past_peak = 0
		peak_was_hi = 0
		if( vol < year_min_vol ) {
			year_min_vol = vol
			year_min_YF = year_frac;
		}
	}
	if( recent_max_vol < vol ) {
		# new max
		C1 = ">"
		recent_max_vol = vol
		days_past_max = 0
		days_past_peak = 0
		peak_was_hi = 1
		if( year_max_vol < vol ) {
			year_max_vol = vol
			year_max_YF = year_frac;
		}
	}
	{
		if( days_past_peak > D10 ) {
		 if( peak_was_hi ) {
			seen_peak_max = 1
			days_past_max = 0
		 } else {
			seen_peak_min = 1
			days_past_min = 0
		 }
				C2 = "P"
		}
	}

 if(0) {
		S12 = C1 C2 C3
	printf( "day %3d vol %.3f %s days_past_min_max %3d,%3d seen up,down %d,%d seen min_max %d,%d\n",
		day, vol, S12,
		days_past_min,
		days_past_max,
		seen_upslope,
		seen_downslope,
		seen_peak_min,
		seen_peak_max )
 }

	year_vol_avg_sum += vol
	year_vol_avg_count += 1

	data_day_avg = 1
	data_day_pos ++
	if( data_day_pos == A365 ) data_day_pos = 1
	if( data_day_max < A365 ) {
		data_day_max ++
		# print "data_day_max " data_day_max
		data_day_avg = 0
		vol_old = 0
	} else {
		# print "data_day_max " data_day_max
		vol_old = data_day[ data_day_pos ] 
	}

	data_day[ data_day_pos ] = vol
	year_vol_avg1_sum += vol - vol_old
	if( data_day_avg ) {
		A365_half = F365/(365.0*2.0) 
# // A365_half // WHAT IS THIS NONSENSE // why not F365 / 2
# // A units are units of 1 year, x-axis is in units of years not days
# // measurments are daily tho, so multiply by 365.0
# // data_day_pos is where the mid-ends are
		year_frac3 = sprintf( fmt_3, (year_frac - A365_half ))
		L_day_1 = year_frac3 " " (year_vol_avg1_sum/F365 + 8.0)
		L_day_2 = year_frac3 " " (year_vol_avg1_sum/F365)
		L_day_3 = year_frac3 " " (year_vol_avg1_sum/F365 - 8.0)

		# print L_line_data > filename_as_var #
		# awk is not shell # it notices repeat calls with same filename
		# appends to that one file
		# the first opens the file, the same filename string appends
		# you may need to close to flush # before calling gnuplot

		print L_day_2 > filename_plot_data_avg2
	 if( P_8 ) {
		print L_day_1 > filename_plot_data_avg1
		print L_day_3 > filename_plot_data_avg3
	 }
	}

	year_frac3 = sprintf( fmt_3, year_frac )
	L_day = year_frac3 " " vol
	print L_day > filename_plot_data_day
}

function gen_pencil_data() {
	gen_pencil_data_2()
}


function gen_pencil_data_1() {

	print "1970 21.0" > filename_plot_data_pencil
	print "2035 0" > filename_plot_data_pencil
	print "" > filename_plot_data_pencil

	print "1970 28.0" > filename_plot_data_pencil
	print "2060 0" > filename_plot_data_pencil
	print "" > filename_plot_data_pencil

	print "1970 36.0" > filename_plot_data_pencil
	print "2090 0" > filename_plot_data_pencil
	print "" > filename_plot_data_pencil

}

function gen_pencil_data_2() {

	VL = 21.0
	VM = 29.0
	VH = 36.0
	V0 = 0

	# Y is Year which is X axis #
	Y0 = 1970
	YL = 2030
	YM = 2060
	YH = 2090

	YH1 = 2000
	YM1 = YH1
	YL1 = YH1

	YH2 = 2230.47
	YM2 = 2144.80
	YL2 = 2057.45

	# V is Volume which is Y axis
	VH1 = 25.22
	VM1 = 15.85
	VL1 =  6.29

	VH2 = 0
	VM2 = 0
	VL2 = 0

if (1) {
	# DV delta value is peak to peak over 2
	# the middle to top middle to bottom
	DV = 17.5 / 2
	DV = 18.5 / 2
	DV = 18.0 / 2

	# same start and end years
	YH1 = YM1
	YL1 = YM1

	YH2 = YM2
	YL2 = YM2

	# (YM1 VM1) # to # (YM2 VM2) # manually placed line

	# (YM2 VM2)
	# YM1 is YEAR - middle line - POS 1
	# VM1 is VOLUME - MIDDLE - POS1 # is ZERO

	VH1 = VM1 + DV
	VH2 = VM2 + DV

	VL1 = VM1 - DV
	VL2 = VM2 - DV

	# (YM2 VM2)
	# YM2 is YEAR - middle line - POS 2
	# VM2 is VOLUME - MIDDLE - POS2 # is ZERO

	# (YM1 VM1)

}


if (0) {
	print Y0 " " VL > filename_plot_data_pencil
	print YL " " V0 > filename_plot_data_pencil
	print "" > filename_plot_data_pencil

	print Y0 " " VM > filename_plot_data_pencil
	print YM " " V0 > filename_plot_data_pencil
	print "" > filename_plot_data_pencil

	print Y0 " " VH > filename_plot_data_pencil
	print YH " " V0 > filename_plot_data_pencil
	print "" > filename_plot_data_pencil
}

	print YH1 " " VH1 > filename_plot_data_pencil
	print YH2 " " VH2 > filename_plot_data_pencil
	print "" > filename_plot_data_pencil

	print YM1 " " VM1 > filename_plot_data_pencil
	print YM2 " " VM2 > filename_plot_data_pencil
	print "" > filename_plot_data_pencil

	print YL1 " " VL1 > filename_plot_data_pencil
	print YL2 " " VL2 > filename_plot_data_pencil
	print "" > filename_plot_data_pencil

}

function process_data_file() {
	# nb you manualy fetch the file, with PIO/fetch to help you
	# what is the file called this year (and where is it)
	F = get_filename_data_gz()

	# the command line that expands the compressed file
	CMD = "zcat " F 
	LNO = 0
	# awk runs CMD once, the while loops over every getline from it
	while(( CMD | getline ) > 0 ) {
		# getline reads a single line and splits it into fields
		# default field separation works fine #
		# $1 $2 $3 # TODO check NF == 3 #
		LNO ++
		if(LNO == 1 ) continue	# skip non data header

		year = $1
		day  = $2
		vol  = $3

		# convert day of year to float year 
		YY = year + (day/365.0) # different to A365
		# process the line of data, into ALL hungry filters
		line_in( year, YY, day, vol )
	}

# SHOULD CALL but not doing so because min max not passed
	if( year_curr != -1 ) flush_year_end()

	gen_pencil_data();

	close(CMD) # it exited ? code ?
	close( filename_plot_data_day )
	close( filename_plot_data_min )
	close( filename_plot_data_max )
	# was BUG not flushing avg before graph plotted!! errm all 3
	close( filename_plot_data_avg1 )
	close( filename_plot_data_avg2 )
	close( filename_plot_data_avg3 )
	close( filename_plot_data_pencil )
	# old way # close( pipe_gnuplot )
	# belt and braces
	fflush()
}

function plot_script_daily(filename_plot_png, zoomed_out)
{
	# pick the axis limits #
	if(zoomed_out) {
		X_lo = "1950"
		X_hi = "2100"
		Y_lo = -10
		Y_lo = 0
		Y_hi = 40
		X_lo = "1970"
		X_hi = "2070"
		X_hi = "2100"
	} else {
		X_lo = "1980"	# data starts at 1980
		X_lo = "1975"	# a gap
		X_hi = "2035"	#
		Y_lo = 0
		Y_hi = 35
	}

	plot_title_line_max = "max" # 
	plot_title_line_day = "day" # better than "<CAT"
	plot_title_line_min = "min" # 
	plot_title_line_mid = "mid" # is annual average
	plot_title_line_avg1 = "avg2+8" # 
	plot_title_line_avg2 = "avg" # rolling average
	plot_title_line_avg3 = "avg2-8" # 
	plot_title_line_pencil = "pencil" # 
	plot_Y_label = "1000 km3"
	plot_filename = "<cat"
	plot_filename = filename_plot_data_day

	CRLF = "\n"
	T = ""
 if(plot_to_png) {
 	# things specific to PNG
	T = T CRLF "set terminal png"
	T = T CRLF "set output " Q1(filename_plot_png)
	T = T CRLF "set term png size 1200, 800"	# pixel size of png
#	T = T CRLF "set term png size 800, 600"
#	T = T CRLF "set term png size 400, 300"
#	T = T CRLF "set term png size 500, 450"
	
 } else {
 	# things specific to SCREEN
	# probably unused as now it is all PNG # ?
 }

	# both SCREEN and PNG
	T = T CRLF "set xrange [" X_lo ":" X_hi "]"
	T = T CRLF "set yrange [" Y_lo ":" Y_hi "]"
	T = T CRLF "set title " Q1(plot_title)
	T = T CRLF "set ylabel " Q1(plot_Y_label)
	T = T CRLF "set multiplot"
	T = T CRLF "plot \\" 
P_max&&	T = T CRLF Q1(filename_plot_data_max) " with lines title " Q1(plot_title_line_max) ", \\"
	T = T CRLF Q1(filename_plot_data_day) " with lines title " Q1(plot_title_line_day) ", \\"
P_mid&&	T = T CRLF Q1(filename_plot_data_mid) " with linespoints title " Q1(plot_title_line_mid) ", \\"
P_8&&	T = T CRLF Q1(filename_plot_data_avg1) " with lines title " Q1(plot_title_line_avg1) ", \\"
P_avg&&	T = T CRLF Q1(filename_plot_data_avg2) " with lines title " Q1(plot_title_line_avg2) ", \\"
P_8&&	T = T CRLF Q1(filename_plot_data_avg3) " with lines title " Q1(plot_title_line_avg3) ", \\"
P_9&&	T = T CRLF Q1(filename_plot_data_pencil) " with lines title " Q1(plot_title_line_pencil) ", \\"
P_min&&	T = T CRLF Q1(filename_plot_data_min) " with lines title " Q1(plot_title_line_min)
	T = T CRLF 
 if(!plot_to_png) {
	T = T CRLF "set nomultiplot"
	T = T CRLF "pause 111" # I dont know, and its in bg
	T = T CRLF "pause 11"
	T = T CRLF "pause -1"
 }
	T = T CRLF 
#	print "RETURNING >>>>>" T "<<<<<<"
	return T
}

# UNUSED FILTER for every line of stdin #
{
	print "## EVERY LINE"
	exit(22)
}
