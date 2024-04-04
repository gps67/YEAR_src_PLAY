
proc wm_always_in_front {{wtop .} {onoff 1}} {
	set wtop [at_least_dot $wtop]
	wm attributes $wtop -topmost $onoff ;# -fullscreen 1
#	wm attributes $wtop -topmost 1    -fullscreen 1
}

proc wm_full_screen {{wtop .} {onoff 1}} {
	set wtop [at_least_dot $wtop]
	# TODO test
	wm attributes $wtop -fullscreen $onoff
}

proc wm_raise {{wtop .}} { ;# not a WM things but feels like it might be
	# I use { set w1 {} } so that $w1.zone is not { ..zone }
	set wtop [at_least_dot $wtop]
	raise $wtop
}

proc wm_full_screen_get {{wtop .}} {
	# 1 if already fullscreen
	set wtop [at_least_dot $wtop]
	wm attributes $wtop -fullscreen 
}

proc wm_full_screen_toggle {{wtop .}} {
	set wtop [at_least_dot $wtop]
	wm attributes $wtop -fullscreen \
	 ![wm attributes $wtop -fullscreen]
}

proc wm_minsize_WH { wtop W H } {
	set wtop [at_least_dot $wtop]
	wm minsize $wtop $W $H
}

proc wm_geometry_XYWH { wtop X Y W H } {
	set wtop [at_least_dot $wtop]
	# pixels # not chars #
	wm geometry $wtop "=${W}x$H+$X+$Y"
}
