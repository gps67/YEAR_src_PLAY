2020-09-03 Sep Thu 00:59 README_lib_FT2.txt

	README_lib_FT2.txt

USE freetype // freetype2 obvs?

	render glyph to x1x1 x2x4 x3x9 x4x16 monochrome

	 anti-alias-reduce 5x5 to u5_x25
	 anti-alias-reduce 4x4 to u4_x16
	 anti-alias-reduce 3x3 to u4_x9
	 anti-alias-reduce 2x2 to u2_x4
	 anti-alias-reduce 1x1 to u0_x1

	 sweep_2dim over pixmap_mono reading multibits
	  sum of 4x4 or of vari_shape
	  sum u4 grey per pixel, 2 pix per byte, 16 per word_u64
	  SPLIT each byte ; u8 lhs ; u8 rhs ; // or u16 or u32 or u64 or u48_xx
	  
	  u8 per pixel // u16 u32 u64

	  	16 shades of grey
		16 pixels in circle // square 
		12 shades of grey

		..**..
		.*++*.
		.*++*.
		..**..

		...**...
		...*+*..
		..*+++*..
		..*+++*..
		...*+*...
		...**...

	EDIT FILTER

		pre build bit weighting with gamma

		32 x 32 x u8 grey

		32 x 32 x u32 float

		32 x 32 x (f32 + f32)

		POINT = SUM_SQR SUM N

		smudge 32x32 into 5x5

		sweep 5x5 over glyph pixmap or over text pixmap
		optimised per CPU bit counter
		load 32 bit mono into u64 scroller
		load 64 bit mono into u64 scroller
		move 5 bits to bits 43210 // sweep right to left
		multiply each bit by a byte // PAIR b?lo,hi
		add_5 together 255 x 5
		OPTION compress to u8 // or u16 or u32 of f32
		save to next pixmap ? u8 or u32
		
		shift 1 bit for next rolling average
		shift 5 bits for reduction
		shift 2 bits for reduction // reduce count by 4

		add rows
		 compress sum
		  save next pixmap

		2 into 2
		2 into 1
		3 into 1
		4 into 1

		the idea is to resample every 4'th pixel 4'th row
		that is already anti-aliased at that 1/4 pixel
		so PRE-build those values

		COMPRESS
			4 x u4 == 64 == u6
			16 x u4 == 256 = u8
			16 * u8 == u12 // >> 4 into u8 // 
			shape range xy xy xy xy xy

fonts
https://github.com/RedHatOfficial/RedHatFont

https://keithp.com/~keithp/talks/xtc2001/xft.pdf



