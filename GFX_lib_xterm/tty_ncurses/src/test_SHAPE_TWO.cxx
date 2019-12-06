#include "test_SHAPE_TWO.h"

	bool SHAPE_TWO:: set_X_Y_W_H(
	 i16 _X, 
	 i16 _Y, 
	 i16 _W, 
	 i16 _H
	) {
		X0 = _X;
		Y0 = _Y;
		dx_of_frame = _W;
		dy_of_frame = _H;

		dx_to_box = 7;
		dx_to_box = 2;

		glyph_width = title.str_len();
		return true;
	}

	bool SHAPE_TWO:: draw()
	{
/*TODO

	draw_
	_goto_XY // NO_DRAW
	_line_XY // AUTO RETRO-FITS JUNCTION BOXES XY_ALIAS XY_NAME DRAW_ON
	_fgbg_EXPR // TREE of ITEM used by FGBG

	CACHE runs scripts over PREBUILT // to build it
	CACHE records things like: 

	 steps taken in DIAG {
	 	A mini machine that gathers_stream_of_ ITEM
		 VAR = STEP {PSG}
		 STEP
		  CSR_t & CSR // = drawing_machine_CSR_Name // == EA_ITEM_in_ROM
		  SKIP STEP
		   SKIP // NO_DRAW
		   STEP // DISTANCE // detect H_line V_line TTY line_draw
		  STEP = fork CSR { XY_POS_Name }
		  fork_CSR(goto_SCRIPT_STO} {
		   STO += new_VAR "POS_Name"
		   STO.local_to_NEARBY
		  }
	 }
	 steps taken in DIAG {
	 	CACHE // of_type_PREBUILT // of_EXPR of_VAR of_TOKEN
		CACHE _that_can build_TREE
	 }

	 well_known_phrases
	 DIALECT spelling pool
	 EG
	  DIAG[diag].XPOS[xpos_name]
	  DIAG[diag].YPOS[ypos_name]
	  DIAG[diag].XY_POS[xy_name] // keep PAIR("XY","POS") matches "Name"
	  // MATCHS PAIR("XY","Name")
	  // MATCHS PAIR("POS","Name")

*/

		i16 x0 = X0 + 0; // AT border line
		i16 x2_title_box_left = (x0+1) + dx_to_box;
		i16 x3_title_text = x2_title_box_left + 1 + 1 ; // SP + STEP
		i16 x4_title_box_right = x3_title_text + glyph_width + 1; // SP==1
		i16 x5_frame_right = X0 + dx_of_frame - 1; // -1 thing // LAST
		#define x2 x2_title_box_left
		#define x3 x3_title_text
		#define x4 x4_title_box_right
		#define x5 x5_frame_right
		i16 x_rule = x5_frame_right + 3;


		i16 y0 = Y0;		// GAP TOP LINE 
		i16 y1 = y0 + 1;	// TEXT line
		i16 y2 = y1 + 1;	// LINE

		i16 y4 = Y0 + dy_of_frame;
		i16 y_rule = y4 + 2;


		CSR.box_h_line( y_rule, x0, x_rule );
		CSR.box_v_line( x_rule, y0, y_rule );

		CSR.move( y_rule, x0 ); CSR.puts("0");
		CSR.move( y_rule, x2 ); CSR.puts("2");
		CSR.move( y_rule, x3 ); CSR.puts("3");
		CSR.move( y_rule, x4 ); CSR.puts("4");
		CSR.move( y_rule, x5 ); CSR.puts("5");

		CSR.move( y0, x_rule ); CSR.puts("0");
		CSR.move( y1, x_rule ); CSR.puts("1");
		CSR.move( y2, x_rule ); CSR.puts("2");
		CSR.move( y4, x_rule ); CSR.puts("4");

		CSR.move( y_rule, x_rule ); 
		CSR.putc_box( UDLR_UDLR ); // Top-Left // 


		// The DIAG is prefaced with its own Tile Heading intro
		// also a way of adding an adjacent item, without splicing
		// 
		// Requires GAP to draw into // auto_layout feature //
		// OPTION might redact the following ...
		// 
		// 
#if 1
		// auto_layout required to create this gap to draw into
		CSR.move( Y0-2, X0 );	// PREFIX -2 adds a layer to grouping
		CSR.puts("| / // CRASHING BORDER EXTERNALS // ");
#endif

		// MARK zero with
		CSR.move( Y0, X0 );
		CSR.puts("*");

		CSR.move( y0, x2_title_box_left ); // y0 is title top
		CSR.putc_box( UDLR__D_R ); // Top-Left // 

		CSR.box_h_line( y0, x2_title_box_left+1, x4_title_box_right-1 ); // top_left U_R_ // Match PAIR
		CSR.putc_box( UDLR__DL_ ); // top_right j

		CSR.move( y2, x2_title_box_left ); // y2 is title bottom
		CSR.putc_box( UDLR_U__R );
		CSR.box_h_line( y2, x2_title_box_left+1, x4_title_box_right-1 );			// below
		CSR.putc_box( UDLR_U_L_ );


		// from top_left y1 to title // y1 is title line
		CSR.move( y1, x0 );// top left
		CSR.putc_box( UDLR__D_R ); // top left
		CSR.box_h_line( y1, x0+1, x2_title_box_left-1 );// line hits t
		CSR.putc_box( UDLR_UDL_ ); // Tee-Left // 
		CSR.puts("{"); // SP before Title
		CSR.puts( title ); // str1 Title
		CSR.puts("}"); // SP before Title
		CSR.putc_box( UDLR_UD_R ); // tee

		CSR.box_h_line( y1, x4_title_box_right+1, x5_frame_right-1 );
		CSR.putc_box( UDLR__DL_ );
	// end title line

		CSR.box_v_line( x0, y1+1, y4-1 ); // left
		CSR.box_v_line( x5_frame_right, y1+1, y4-1 ); // right
		CSR.move( y4, x0 );
		CSR.putc_box( UDLR_U__R );
		CSR.box_h_line( y4, x0+1, x5_frame_right-1 ); // bottom
		CSR.putc_box( UDLR_U_L_ );

		return true;
	}

	// no fg_bg yet

