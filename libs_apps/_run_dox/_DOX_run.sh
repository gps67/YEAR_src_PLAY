export TOP=/home/gps/YEAR/src/libs_apps
(
	doxygen _dox_apps
	doxygen _dox_lib_OODB
	doxygen _dox_lib_accts
	doxygen _dox_lib_base
	doxygen _dox_lib_base2
	doxygen _dox_lib_inet
	doxygen _dox_lib_media
	doxygen _dox_lib_play
	doxygen _dox_lib_win32
	doxygen _dox_sar_data
	doxygen _dox_sar_parse
	doxygen _dox_sar_plot
	doxygen _dox_sar_tests
) 2>&1 | tee _fa 
