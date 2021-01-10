README.txt
/home/gps/YEAR/src/PLAY/UTIL/geo_sea_ice_volume/

	plot the piomass data (sea ice volume)

EDIT the DATE YEAR

	the filename contains the year
	the script contains the filename
	every year you must edit it, into the script(s)
	Decembers data arrives 7'th Jan, so cant easily use date +%Y
	PIOMAS.vol.daily.1979.2020.Current.v2.1.dat.gz
	PIOMAS.vol.daily.1979.2021.Current.v2.1.dat.gz

	vi PIO/PIOMAS_fetch
	vi plot_daily_volume.awk

DOWNLOAD the data

	cd PIO
	vi ./PIOMAS_fetch # edit the YEAR
	./PIOMAS_fetch

mkdir OUT dir

 	OUT/ = dir where .png files are copied to after a good run
 maybe
	mkdir OUT
	ln -s $HOME/FTP_/ OUT

check you have gnuplot awk tclsh and graphics magick installed

	check_env_debian

run the main script = create and display 3 graphs

	plot_all_using_gnuplot5.sh

edit the parameters, to run it again

	vi \
	plot_daily_volume.awk	# it is all in here # mostly

display the MAX and MIN points previously found

	./show_as_dates.tcl
	Shows the MAX and MIN for each cycle
	This shows how you can access the data from tcl as dates
	It also shows the most recent data point's date
	Which is reassuring
	MAX/MIN uses 1dp so +- 2 days

display a graph

	gm display sea_ice_vol_zoomed_out.png

	This is done within the scripts
	NB the range is an extra graph in a second script

	CTRL-Q quits as does closing the window
