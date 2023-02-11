
# frame_scrolled #

# you need to provide an outer frame
# you need to pack that outer frame
# you call 
#	set scrolled_frame [ frame_scrolled $pair ]
# it returns the name of a packed frame # a simple frame #
# you then add stuff to that frame
#	set button99 $scrolled_frame.btn_99
#	button $button99
#	v_pack $button99
# or
#	btns_b_text_cmd $scrolled_frame [mk_id] $btn_text { cmd_here }
#
# currently you have no choice about scroll bars (soz)
# currently you have no default size (soz)
# maybe add OPTS to argv

 proc frame_scrolled { outer } {

	set v_scroll $outer.vscroll
	set canvas $outer.c

	set h_scroll $outer.hscroll

	scrollbar $h_scroll -orient horiz -command "$canvas xview"
	scrollbar $v_scroll -command "$canvas yview"
	canvas $canvas \
	-relief sunken \
	-borderwidth 2 \
	-xscrollcommand "$h_scroll set" \
	-yscrollcommand "$v_scroll set" \
	-bg yellow \
	; #

	set scrolled_frame $canvas.scrolled_frame
	frame $scrolled_frame

	# v_pack $h_scroll
	pack $h_scroll -side bottom -fill x 
	h_pack_big $canvas
	h_pack $v_scroll

	# no need for tag nor id, use "$canval bbox all"

	# h_scroll is at the bottom
	$canvas create window 0 0 -window $scrolled_frame -anchor nw

	# when the SCROLLED frame changes size, tell its holding canvas
	bind $scrolled_frame "<Configure>" "frame_scrolled_on_canvas_configure $scrolled_frame $canvas"

 	return $scrolled_frame
 }

proc frame_scrolled_on_canvas_configure { scrolled_frame canvas } {
	# this is called when the canvas is resized or scrolled
	set bbox  [ $canvas bbox all ]
	# puts "bbox all $bbox"
	foreach {X Y W H } $bbox {}

	# setting H only works once (request then fills the extra)
	set H_max 300
	if { $H > $H_max } { set H $H_max } 

	$canvas configure \
	-scrollregion $bbox \
	-width $W \
	-height $H 


	global recon_count
	incr recon_count
	puts reconfigured_$recon_count
	
}

