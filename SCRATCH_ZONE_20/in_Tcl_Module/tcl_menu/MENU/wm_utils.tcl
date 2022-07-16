
proc wm_always_in_front {{wtop .} {onoff 1}} {
	wm attributes $wtop -topmost $onoff ;# -fullscreen 1
#	wm attributes $wtop -topmost 1    -fullscreen 1
}

proc wm_full_screen {{wtop .} {onoff 1}} {
	# TODO test
	wm attributes $wtop -fullscreen $onoff
}

proc wm_raise {{wtop .}} { ;# not a WM things but feels like it might be
	raise $wtop
}

proc wm_full_screen_get {{wtop .}} {
	# 1 if already fullscreen
	wm attributes $wtop -fullscreen 
}

proc wm_full_screen_toggle {{wtop .}} {
	wm attributes $wtop -fullscreen \
	 ![wm attributes $wtop -fullscreen]
}

proc wm_minsize_WH { wtop W H } {
	wm minsize $wtop $W $H
}

proc wm_geometry_XYWH { wtop X Y W H } {
	# pixels # not chars #
	wm geometry $wtop "=${W}x$H+$X+$Y"
}
