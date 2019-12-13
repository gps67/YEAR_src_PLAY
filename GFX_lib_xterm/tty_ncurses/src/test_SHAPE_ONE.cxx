#include "test_SHAPE_ONE.h"
#include "tty_curses_CSR.h"

#define EXTRA_EXTERNALS 0


	bool SHAPE_ONE:: set_XYWH( XYWH_t & _XYWH ) {
		XYWH = _XYWH;

		if(!SHAPE_BASE:: set_XYWH( _XYWH ))
		{ return FAIL_FAILED(); }

		return true;
	}


#define x2 x2_title_box_left
#define x3 x3_title_text
#define x4 x4_title_box_right
#define x5 x5_frame_right

	bool SHAPE_ONE:: draw( tty_curses_CSR & CSR ) // make CSR a parameter
	{
		i16 dx_to_box;
		dx_to_box = ( frame_W() - title_glyph_width - 4 ) / 2;
		if( dx_to_box < 0 ) dx_to_box = 0;

		// no truncation 

		i16 x0 = XYWH.XY.X + 0; // AT border line
		i16 x2_title_box_left = (x0+1) + dx_to_box;
		i16 x3_title_text = x2_title_box_left + 1 + 1 ; // SP + STEP
		i16 x4_title_box_right = x3_title_text + title_glyph_width + 1; // SP==1
		i16 x5_frame_right = x0 + frame_W() - 1; // -1 thing // LAST
		i16 x_rule = x5_frame_right - 5;


		i16 y0 = XYWH.XY.Y;		// GAP TOP LINE 
		i16 y1 = y0 + 1;	// TEXT line
		i16 y2 = y1 + 1;	// LINE
		i16 y3 = y2 + 2;	// spare

		i16 y4 = y0 + frame_H() -1 ;
		i16 y_rule = y4 - 4;	// OUTSIDE frame !!

		if(0) {
		x_rule = x0-1;
		y_rule = y0-1;
		}

		if(1) {
		x_rule = x0+7;
		y_rule = y0+7;
		}

		JB_t JB_x2_y1_tee; // tee at x2 y1
		JB_t JB_x4_y1_tee; // tee at x4 y1

		// require _width < limit
		// resolve clipping drawn text
		int dx_title_box = 1 + title_glyph_width + 1 ; // SP==1

#if 0
		// test to show that vline prints N chars
		CSR.move( y2, x2 );

		CSR.move( y4+2, x0 );
		wvline( CSR.win, 0, 1 );

		CSR.move( y4+2, x2 );
		wvline( CSR.win, 0, 2 );
#endif

#if 0
		CSR.move( y4, x0 ); //
		// CSR.JB1.XY = XY_t( x0, y2 );
		CSR.box_line_start();
		CSR.box_line_to_X( x4 );
		CSR.box_line_to_Y( y1 );
		//	CSR.putc_box( CSR.JB1 );
#endif
#if 0

		CSR.move( y3, x2 );
		CSR.box_line_start();
		CSR.box_line_up( 1 );
		CSR.box_line_to_X( x3 );
		CSR.box_line_down( 1 );
#endif
#if 0
		CSR.box_line_to_X( x4 );
		CSR.box_line_to_Y( y2 );
		CSR.box_line_to_X( x0 );
		CSR.box_line_to_Y( y0 );
		CSR.box_line_end();
#endif

#if 1
		CSR.move( XY_t( x3-1, y1 ));
		CSR.puts(" "); // SP before Title
#else
		CSR.move( XY_t( x3, y1 ));
#endif
		CSR.puts( title ); // str1 Title
		// crashes when pressed right, sublex confused
		// CSR.puts(" "); // SP after title clears footspace

// AXES // {
		CSR.box_h_line( y_rule, x0, x_rule );
		CSR.box_v_line( x_rule, y0, y_rule );

		CSR.box_h_line( y_rule, x0, x5 );
		CSR.box_v_line( x_rule, y0, y4 );

		CSR.move( y_rule, x0 ); CSR.puts("0"); 
		CSR.move( y_rule, x2 ); CSR.puts("2");
		CSR.move( y_rule, x3 ); CSR.puts("3");
		CSR.move( y_rule, x4 ); CSR.puts("4");
		CSR.move( y_rule, x5 ); CSR.puts("5");

		CSR.move( y4+3, x0 );
		CSR.move_to_X(x0); CSR.printf("%s=%d ", "x0", x0);
		CSR.move_to_X(x2); CSR.printf("%s=%d ", "x2", x2);
		CSR.move_up(1);
		CSR.move_to_X(x3); CSR.printf("%s=%d ", "x3", x3);
		CSR.move_down(1);
		CSR.move_to_X(x4); CSR.printf("%s=%d ", "x4", x4);
		CSR.move_to_X(x5); CSR.printf("%s=%d ", "x5", x5);

		CSR.move( y0, x_rule ); CSR.puts("0");
		CSR.move( y1, x_rule ); CSR.puts("1");
		CSR.move( y2, x_rule ); CSR.puts("2");
	//	CSR.move( y3, x_rule ); CSR.puts("3");
		CSR.move( y4, x_rule ); CSR.puts("4");

		CSR.move( y0, x_rule -5 ); CSR.printf(" %d", y0);
		CSR.move( y1, x_rule -5 ); CSR.printf(" %d", y1);
		CSR.move( y2, x_rule -5 ); CSR.printf(" %d", y2);
	//	CSR.move( y3, x_rule -5 ); CSR.printf(" %d", y3);
		CSR.move( y4, x_rule -5 ); CSR.printf(" %d", y4);

		CSR.move( y_rule, x_rule ); 
		CSR.putc_box( UDLR_UDLR ); // Top-Left // 
// AXES // }

#if 1
		// use the pre-computed shape positions / exprs

		CSR.move( XY_t( x0, y1 )); // box_panel top_left
		CSR.box_line_start(); // box_frame title XYWH UDLR JB
		CSR.box_line_to_X( x2 ); // left_side left_of_title
		CSR.box_line_keep_JB( JB_x2_y1_tee ); // Tee
		CSR.box_line_to_Y( y0 ); 
		CSR.box_line_to_X( x4 ); // DIAG has TABLE of x
	// return true;
		CSR.box_line_to_Y( y1 ); // DOWN 1
		CSR.box_line_keep_JB( JB_x4_y1_tee );
		// tee of box_title right
		CSR.box_line_to_Y( y2 ); // bottom left of box_title bottom
		CSR.box_line_to_X( x2 ); // bottom left of box_title left
		CSR.box_line_to_Y( y1 ); // left of box_title left
		CSR.box_line_amat_JB( JB_x2_y1_tee ); // provide retained tee data

		CSR.box_line_jump_JB( JB_x4_y1_tee ); // jump resume from rhs tee
		CSR.box_line_to_X( x5 );
		CSR.box_line_to_Y( y4 );
		CSR.box_line_to_X( x0 );
		CSR.box_line_to_Y( y1 );

		CSR.box_line_end();
#endif


	// return true;


		// The DIAG is prefaced with its own Tile Heading intro
		// also a way of adding an adjacent item, without splicing
		// 
		// Requires GAP to draw into // auto_layout feature //
		// OPTION might redact the following ...
		// 
		// 
#if EXTRA_EXTERNALS
		// auto_layout required to create this gap to draw into
		CSR.move( XYWH.XY.Y-2, XYWH.XY.X );	// PREFIX -2 adds a layer to grouping
		CSR.puts("| / // CRASHING BORDER EXTERNALS // y0-2 ");
#endif

		return true;
	}

	// no fg_bg yet

