// #!/usr/bin/env -S mutool run
// shebang fail // # is not OK
// #!/usr/bin/env -S mutool run

// TODO: use better font

	// font_name is unused, see font_file
	var font_name = "Courier-Bold"
	var font_name = "Courier"

	// last one wins
	var font_file = "/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf"
	var font_file = "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf"
	var font_file = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"

	var font_file = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
	var font_file = "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf"


	// PROBLEM LOADING TTF
	// The builting names work, the load from file fails
	// it doesn't fail, it jsut doesn't change the font
	// so times gets used (or something)

	// font_name is used, here it is the font_file
	var font_name = "NO_font_file" // commented out 2024 - as prop spaced
	var font_name = font_file // commented out 2024 - as prop spaced
	var font_name = "Courier-Bold" // worked but builtin
	var font_name = "Courier" // works
	var font_name = "Helvetica-Bold" // not proportional
	var font_name = font_file // fails to load font from file
	// last one wins
	var font_name = "Courier-Bold"
	var font_name = "Courier" // works

	// point size
	var pt_sz = 8.0
	var pt_sz = 36.0
	var pt_sz = 12.0
	var pt_sz = 14.0
	// 
	var pt_sz = 66.0 // this does zoom the text
	var pt_sz = 36.0
	var pt_sz = 66.0 // this does zoom the text
	var pt_sz = 22.0

	function dbg_print_2( key, val ) {
		// if(false) print( "# INFO #", key, val )
		print( "# INFO #", key, val )
	}

	// expand tabs in str
	// TTY tabs are every 8 positions
	// str_line_in ==> str_line_out // RETVAL //
	function expand_tabs( str_line_in ) {
	 var str_line_out = ""
	 var pos_out_x = 0 // output pos_out_x
	 var len = str_line_in.length
	 // iterate over str_line_in
	 for( var str_pos_in = 0; str_pos_in < len; str_pos_in++ ) { 
	   // look at each char byte // OK for ASCII 8859, wrong pos_out utf8
	   var ch = str_line_in[str_pos_in]
	   if( ch == "\n" ) {
	   	// dont support multi-line text, but this might actually work
		 pos_out_x =-1 
	   }
	   if( ch == "\t" ) {
		// expand TAB to multiple SPACES
	        // dbg_print_2( "TAB", str_line_in )
		do {
		 // ALWAYS the first SPACE, and pos_out_x
		 str_line_out = str_line_out + " " // tabby space
		 pos_out_x ++
		 // dbg_print_2( "str_line_out", str_line_out ) 
		 // stop when x advances to position multiple of 8
		} while( pos_out_x % 8 > 0 ) 
	   } else {
	   	// all other byte chars carried through, and count as 1 X
		 str_line_out = str_line_out + ch
	   	 pos_out_x++
	   }
	  }
	 return str_line_out
	}

	var dir_name_txt = scriptArgs[0];

	// strip .txt // or fail
	var pos2 = dir_name_txt.length // first pos after end
	var pos2_4 = pos2 - 4
	if( ".txt" == dir_name_txt.substring( pos2_4, pos2 )) {
		pos2 = pos2_4 // at dot after end of name
	} else {
		print("# WARN # filename doesnt end in .txt")
		// quit()
	}
	var dir_filename = dir_name_txt.substring( 0, pos2 )

	// strip leading dir, to save here, not there
	var pos1 = dir_name_txt.lastIndexOf( "/" )
	// when no / pos1 is -1 then 0 is start of str
	var dir = dir_name_txt.substring( 0, pos1 )
	var name = dir_name_txt.substring( pos1+1, pos2 )

	// add .png
	var name_png = name + ".png"

	// dbg_print_2("dir_name_tx", dir_name_txt);
	// dbg_print_2("dir", dir);
	// dbg_print_2("name", name);
	// dbg_print_2("name_png", name_png);

	var text_in = read( dir_name_txt ) // mutool run specific
	text_in = expand_tabs( text_in )
	var text_lines = text_in.split("\n")
	var n_lines = text_lines.length
	n_lines -- // no line after final \n // hopefully
	dbg_print_2( "n_lines", n_lines);

	// count line lengths (simple mono-font)
	var n_cols = 0
	var line
	for( var i = 0; i < n_lines; i++ ) { // es5 not es6 ?
		line = text_lines[i]
		var l = line.length;		// bytes not tab not utf8
		if( n_cols < l ) n_cols = l;
	}
	dbg_print_2( "n_cols", n_cols);

	if(false) {
		var dpi = 150
		var i_per_pt = 1.0/72.0
		var d_per_pt = dpi * i_per_pt // 2 ish
		dbg_print_2( "d_per_pt" , d_per_pt )
	}


	// get font and metrics
	var font = new Font(font_name) // WAS OK upto Jan
	var font = new mupdf.Font(font_name) // NEW from Jan
	dbg_print_2( "font_name", font_name);
	var font_name_two = font.getName()
	dbg_print_2( "font.getName()", font_name_two);
// WARN // FAKE *= 4
	pt_sz *= 4
	pt_sz /= 4
// FAIL there is a LIMIT somewhere near 36 *1.5 pt
	dbg_print_2( "pt_sz", pt_sz);
//  FAIL # still not MONO spaced # still not PT_SZ
	// EDIT was no different both get [objext fz_font]
	if(font) {
		dbg_print_2( "font", font ) 
	} else {
		dbg_print_2( "font-NULL", font ) 
	}
	var gid = 32
	var glyph_dy = 1.0
	var glyph_dx = font.advanceGlyph(gid, 0)

	// from em size to point size // of a monofont char
	glyph_dy *= pt_sz // em
	glyph_dx *= pt_sz // em

	dbg_print_2( "glyph_dx", glyph_dx);
	dbg_print_2( "glyph_dy", glyph_dy);

	// cant find font info
	// NO // var Y_top_to_baseline = font.ascent * pt_sz // no
	var Y_top_to_baseline = glyph_dy * 0.77 // fudge part of em
// NaN	var Y_top_to_baseline = font.ascent * pt_sz // 
	dbg_print_2( "Y_top_to_baseline", Y_top_to_baseline );

	// surrounding margins
	if( false ) {
		var Y_margin = 0
		var X_margin = 0
	} else {
		var Y_margin = 0.3 * glyph_dy
		var X_margin = 0.9 * glyph_dx
	}

	var page_W = glyph_dx * n_cols  + X_margin + X_margin
	var page_H = glyph_dy * n_lines + Y_margin + Y_margin

	var pixmap = new Pixmap( ColorSpace.DeviceRGB, [0,0,page_W,page_H], false )
	pixmap.clear(255)
	var device = new DrawDevice(Matrix.identity, pixmap);
	var transform = [1,0,0,-1,0,page_H]
	var text = new Text();

	var X = X_margin
	var Y = page_H - Y_margin // top down
	var Y = 0 + Y_margin + Y_top_to_baseline // zero is at top
	var Y = page_H - Y_margin - Y_top_to_baseline // zero is at bottom

	for( var i = 0; i < n_lines; i++ ) { // es5 not es6 ?
		// stupid upside down font raster-Y
		// daft font scale AND post-scale X,Y
		var mat = [pt_sz,0,0,pt_sz,X,Y]
		line = text_lines[i]
		// fails with TAB
		text.showString(font, mat, line );
		// dbg_print_2( "mat", mat )
		Y -= glyph_dy // moving down decreases Y
	}

	var rgb_one = [0,1,0] // green // 0.0 < 1.0
	var rgb_one = [0,0,0] // black
	device.fillText(text, transform, ColorSpace.DeviceRGB, rgb_one, 1);

// other stuff to draw
//	device.strokePath(path, {
//		dashes:[5,10],
//		lineWidth:3,
//		lineCap:'Round'}, transform, ColorSpace.DeviceRGB, [0,0,0], 1);
	
	device.close();
	pixmap.saveAsPNG( name_png );







