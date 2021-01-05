
#include "tv_panel.h"

#include "mixa.h"	// /dev/mixer sound mixer (OSS or ALSA emulation)
#include "rad_1.h"	// /dev/radio tuner (built to be wrapped)
#include "cd_music.h"	// /dev/cdrom ioctls are not aync!

#include "rem_xawtv.h"	// xawtv remote uses X11 atoms to switch channels

#include "w_btn_plus.h"	// button base class plus a bit more
#include "w_socket.h"	// GTK socket+plug is X11 reparenting external xawtv
// #include "gdk/gdkx.h"	// GDK_DISPLAY() - to get X11 id (should be in sub?)
#include "w_win_scrolled.h"	// scrolled WINDOW 

#include "tv_clock.h"	// custom part of tv_panel

// using namespace TV_REM;

//! TV_REM is the code used to build an OK TV_REMOTE GUI panel
namespace TV_REM {


// norm=pal, capture=over

//! A Button for the cable channel menu - (3 digit code)
class w_btn_cbl_channel : public w_btn_plus
{
 public:
	tv_panel * panel;		// this should be a weak link
	ir_cable_channel * item;	// the cable channel to switch to

	w_btn_cbl_channel(
		tv_panel * _panel,
		ir_cable_channel * _item
	)
	: w_btn_plus( "" )		// calc and set name later
	{
		panel = _panel;
		item = _item;

		if( !item->channel3 )
		{
			set_text( "-" );
			return;
		}

		int len = strlen(item->name);
		char buff[6 + len + 10 ];
		sprintf(buff,"%3.3d - %s", item->channel3, item->name );
		set_text( buff );
		// set_align_left();
	}

	void event_clicked()
	{
		panel->tv_change_channel( item );
	}
};

//! A Button used to call tv_change_channel() (xawtv setup for 7 channels)
class w_btn_xawtv_channel : public w_btn_plus
{
 public:
	tv_panel * panel;
	int chan;

	w_btn_xawtv_channel(
		tv_panel * _panel,
		const char * name,
		int _chan
	)
	: w_btn_plus( name )
	{
		panel = _panel;
		chan = _chan;
	}

	void event_clicked()
	{
		panel->tv_change_channel( chan );
	}
};

//! A Button used to call tv_send_cmd_line() mute PAL FULL
class w_btn_xawtv_cmd : public w_btn_plus
{
 public:
	tv_panel * panel;
	const char * line;

	w_btn_xawtv_cmd(
		tv_panel * _panel,
		const char * _label,
		const char * _line
	)
	: w_btn_plus( _label )
	{
		panel = _panel;
		line = _line;
	}

	void event_clicked()
	{
		panel->tv_send_cmd_line( line );
	}
};

//! A Button used to call ir_change_channel()
class w_btn_ir : public w_btn_plus
{
 public:
	tv_panel * panel;
	ir_command_info * chan; // or any command
	
	w_btn_ir(
		tv_panel * _panel,
		ir_command_info * _chan // or any command
	)
	: w_btn_plus( _chan->name )
	{
		panel = _panel;
		chan = _chan;
	}

	void event_clicked()
	{
		panel->ir_change_channel( chan );
	}
};

//! A Button used to call cd_command()
class w_btn_cd : public w_btn_plus
{
 public:
	tv_panel * panel;
	str1 cmds;
	
	w_btn_cd(
		tv_panel * _panel,
		const char * _cmds
	)
	: w_btn_plus( _cmds )
	, panel( _panel )
	{
		cmds = _cmds;
	}
	void event_clicked()
	{
		panel->cd_command( cmds );
	}
};

//! A Button used to call radio_change_channel()
class w_btn_radio_freq : public w_btn_plus
{
 public:
	tv_panel * panel;
	int channel;

	w_btn_radio_freq(
		tv_panel * _panel,
		const char * name,
		int _chan
	)
	: w_btn_plus( name )
	{
		panel = _panel;
		channel = _chan;
	}
	void event_clicked()
	{
		panel -> radio_change_channel( channel );
	}
};

void C_mute( GtkWidget *, tv_panel * panel )
{
	panel -> mute_toggle();
}

//! destructor
tv_panel::~tv_panel()
{
	radio.mute();
	factor_fader_changed( 1.0 );
	slider_factor_changed( 1.0 );
}

//! clear the subtitle (may need a 0.1 sec pause before)
void tv_panel::xawtv_subtitle_none()
{
	rem.send_cmd_line( "vtx" );
}

//! Send subtitle annotation to xawtv
void tv_panel::xawtv_subtitle_2( const char * str1, const char * str2 )
{
	int len = 4 + strlen( str1 ) + 1 + strlen( str2 ) + 1;
	char buff[len+10];
	sprintf( buff, "vtx %s %s", str1, str2 );
	rem.send_cmd_line( buff );
}

//! select the radio channel from the predefined array
void tv_panel::radio_change_channel( int chan )
{
	if( chan < 0 ) chan = N_radio_channel_table -1;
	if( chan >= N_radio_channel_table ) chan = 0;
	channel_number_radio = chan;
	radio_change_channel( & radio_channel_table[ chan ] );
}

//! select the radio channel from the channel_info
void tv_panel::radio_change_channel( radio_channel_info * chan )
{
	switch_radio();
	curr_channel -> set_text( chan->name );
	xawtv_subtitle_2( "Radio:", chan->name );
	radio.set_freq( chan->freq );
	track_factor( & chan->vol_factor );
}

//! select the radio channel as a frequency, switch audio on
void tv_panel::radio_set_freq( float freq )
{
	// switch TV capture OFF, etc
	switch_radio();
	radio.set_freq( freq );
}

//! send channel switching command to xawtv
void tv_panel::tv_send_cmd_line( const char * line )
{
	/*
		DESIGN BUG:
			rename: send_cmd_to_CARD
		PROBLEM:
			"volume 100%" - should not start xawtv when radio is on
	*/
	switch_tv();
	rem.send_cmd_line( line );
	rem.flush();
}

//! send channel switching command to xawtv from array
void tv_panel::tv_change_channel( int chan )
{
	if( ( chan < 0 ) ||( chan >= N_tv_chan_table ))
	{
		printf("cannot change channel from %d to %d (out of range)\n", channel_number_tv, chan );
		return;
	}
	channel_number_tv = chan;
	tv_change_channel( &tv_chan_table[ chan ] );
}

//! run a CD (music) command
void tv_panel::cd_command( str0 cmd )
{
	e_print("CALLED: %s\n", (STR0) cmd );
	if(cd_music) cd_music->run( cmd );
}

//! central to the fader
gint C_fader_step( tv_panel * panel )
{
	return (gint) panel->fader_step();
}

//! switch tv tuner casrd to VCR or CABLE (to receive CABLE)
bool tv_panel::switch_tv_VCR_CABLE()
{
	bool ok = true;
	if(! switch_tv() ) {
		ok = switch_tv();
	}
	switch( channel_number_tv ) {
	 case 5: ;	// DaeWoo
	 case 6: ;	// VCR
	 case 7: ;	// CABLE
	 break;
	 default:
		tv_change_channel( 7 ); // CABLE (because of VCR usually being off)
		// tv_change_channel( 6 ); // VCR (because of it forgetting INPUT)
	}
	return ok;
}

//! switch the cable channel using IR
void tv_panel::tv_change_channel( ir_cable_channel * item )
{
	int d = item->channel3;
	int d3 = d % 10; d = d / 10;
	int d2 = d % 10; d = d / 10;
	int d1 = d % 10; 
	char buff[7];
	sprintf( buff, "%d %d %d", d1, d2, d3 );
	ir_client->send_commands(
		"CABLE",
		buff
	);
	switch_tv_VCR_CABLE();
	track_factor( & item->vol_factor );
	return;
}

//! switch TV channel from channel info - fade over nastly click
void tv_panel::tv_change_channel( tv_chan_info * chan_info )
{
	/*
		BTTV has an awful click,
		probably by switching the sound on/off/on
		halfway through a peak.

		So here use the INTERNAL mixer, which is
		less clicky to mask out the cards click,
		and stair it up.

		The tuner delay varies with signal.

		BTTV seems to have fixed this with its own fade!
	*/
	track_factor( & chan_info->vol_factor );
	curr_channel -> set_text( chan_info->name );

	if( !mixer ) return;

	char full_line[ strlen( chan_info->cmd ) + 10 ];
	sprintf( full_line, "keypad %s", chan_info->cmd );
	tv_send_cmd_line( full_line );
	// sending vtx immediately disrupts the tuner
	sleep_10( 1 );
	xawtv_subtitle_none();

	fader_t1 = (int) delay_t1 -> get_val();
	fader_t2 = (int) delay_t2 -> get_val();
	fader_t3 = (int) delay_t3 -> get_val();

	fader_t = 0;
	gtk_timeout_add( 100, (GtkFunction) C_fader_step, this );
}

/*!
	toggle fullscreen on/off

	When its full, the 'f' key goes to the xawtv window,
	but IR events come through here
*/
void tv_panel::tv_fullscreen()
{
	tv_send_cmd_line( "fullscreen" );
}

/*!
	Change the IR Cable channel.

	Could maybe require VCR | CABLE and set the line_factor
	for the channel (saving the old).
*/
void tv_panel::ir_change_channel( ir_command_info * chan_info )
{
	ir_client->send_commands(
		chan_info->remote,	// CABLE or VCR
		chan_info->command	// "101" or "POWER"
	);
	return;
}

//! switch device mute on/off (where device is in either tv/radio mode)
void tv_panel::mute_toggle()
{
	if(radio.is_open()) {
		radio.mute_toggle();
	} else {
		tv_send_cmd_line( "volume mute" );
	}
}

//! callback to set new fader value each 1/10 sec
bool tv_panel::fader_step()
{
	static const bool call_again = TRUE;
	static const bool no_more_calls = FALSE;
	// fader_t always increments by 1 - no clock skips
	// but it could miss the absolute time
	fader_t ++;
	if(0) e_print( "fader_t %d\n", fader_t );
	//
	int t1 = fader_t1;
	int t2 = fader_t2; // still absolute // independent adjustments
	int t3 = t2 + fader_t3;
	//
	if( fader_t < t1 )
	{
		return call_again;
	}
	if( fader_t == t1 )
	{
		factor_fader_changed( 0.1 );
		return call_again;
	}
	if( fader_t < t2 )
	{
		return call_again;
	}
	if( fader_t < t3 )
	{
		gdouble factor = fader_t - t2;
		int nstep = fader_t3;
		factor /= nstep;
		factor_fader_changed( factor );
		return call_again;
	}
	if(0) {
		e_print( "fader_t %d t2=%d fader_t3=%d t3=%d\n",
			fader_t, t2, fader_t3, t3 );
	}
	factor_fader_changed( 1.0 );
	return no_more_calls;
}

//!
void tv_panel::track_factor( gdouble * _factor_channel )
{
	slider_factor -> track_factor( _factor_channel );
}

//! switch (from TV) to radio
bool tv_panel::switch_radio()
{
	if(radio.is_open()) return true;
	if( rem.find_win_id() ) {
		rem.send_cmd_line("capture off");
		rem.flush();
		sleep_10( 1 ); // allow xawtv to catch the X-prop
	}
	for(int i = 0; i<14; i++ )
	{
		if( radio.open( name_dev_radio )) break;
		sleep_10( 2 );
	}
	if( !radio.open( name_dev_radio )) {
		return false;
	}
	// unmute required here if radio was busy when (not) opened
	radio.unmute();
	// radio.set_freq( 100.0 );
	return true;
}

//! fork xawtv but dont wait
bool tv_panel::fork_xawtv()
{
	/*
		this has a problem with process group quiting xawtv
		if this process receives CTRL-C -- LATER

		xwininfo says 448,336 is OK

		-xv 	enable Xvideo
		-noxv	
	*/
	// system("xawtv -xv   -geometry 480x360-5+9 &" );
	system("xawtv -nogl   -geometry 480x360-5+9 &" );
	return true;
}

//! switch to TV from radio (initially) means running xawtv
bool tv_panel::fork_xawtv_wait()
{
	if(0)
	{
		// -noxv has blt problems, and massive redraw issues
		// -xv comes up invisible
		// -noxv THEN -xv works fine!!
		// default -xv
		// THIS WENT AWAY WITH THE NEWEST SiS drivers FROM
		// http://www.winischhofer.net/linuxsis630.shtml
		// xvinfo also showed new modes (YUV)
		
		system("xawtv -noxv -geometry 240x180-5+9 &" );
		rem.clear_win_id();
		rem.wait_and_quit();
	}

	fork_xawtv();

	rem.clear_win_id();
	rem.wait_win_id();

	// give it a chance, so that the following send cmd works
	sleep_10(4);
	return rem.wait_win_id();
}

//! switch to TV (from radio)
bool tv_panel::switch_tv()
{
	if(radio.is_open()) {
		radio.close_unmuted();
		if( rem.find_win_id() ) {
			/*
				win_id is valid
				xawtv must be suspended
				recover the cards TV settings
			*/
			factor_fader_changed( 0 ); // big click immed
			rem.recover();
		}
	}
	if( !rem.find_win_id() ) {
		return fork_xawtv_wait();
	}
	return true;
}

w_vbox * tv_panel::pick_menu( M_menu_id id )
{
	int m = 1;
	switch( id )
	{
	 case M01: m =  1; break;
	 case M02: m =  1; break;
	 case M03: m =  1; break;
	 case M04: m =  4; break;
	 case M05: m =  5; break;
	 case M06: m =  6; break;
	 case M07: m =  6; break;
	 case M08: m =  6; break;
	 case M09: m =  9; break;
	 case M10: m =  2; break;
	 case M11: m =  5; break;
	 case M12: m = 12; break;
	 case M13: m = 13; break;
	 case M14: m = 14; break;
	 case M15: m = 15; break;
	 case M16: m = 16; break;
	 case M17: m = 17; break;
	 case M18: m = 18; break;
	 case M19: m = 19; break;
	 case M20: m = 20; break;
	 case M21: m = 21; break;
	 default:
		m = (int) id;
	};
	if( !cbl_menu[m] ) {
		cbl_menu[m] = w_vbox::new_vbox();
		cbl_menu[m] -> set_border_width(0);
	}
	return cbl_menu[m];
}

//! constructor - GUI stuff with initial radio
tv_panel::tv_panel(
	const char * dev_mixer,
	const char * dev_radio,
	const char * dev_lircd,
	const char * dev_cdrom
)
: w_vbox()
// , rem( NULL, GDK_DISPLAY() )
, rem( NULL, NULL ) // x11_dpy must be different than GTK // but shared other?
{
	name_dev_mixer = dev_mixer; // should copy strings
	name_dev_radio = dev_radio;
	factor_fader = 1.0;
	factor_channel = 1.0;

	mixer = new mixa( name_dev_mixer );
	mixer -> fixup_zero_startup();

	slider_vol    =  new mix_slider( mixer, "vol" );
	slider_pcm    =  new mix_slider( mixer, "pcm" );
	slider_line2  =  new mix_slider_factor( mixer, "line" );
	slider_cd_vol =  new mix_slider( mixer, "cd" );

	ir_client = new IR_client( dev_lircd );		// connect to /dev/lircd

	cd_music = new CD_music( dev_cdrom );
	// cd_music = NULL;
 if(0)
	if(cd_music) cd_music->open();

	init_key_snooper();

	// reduce volume for duration of click
	// 10 - 13 is the hot spot
	int t1 = 4;
	int t2 = 15;
	int t3 = 10;
	// new bttv avoids click - so make much faster
	if(1)
	{
		t1 = 0;
		t2 = 0;
		t3 = 0;
	}
	delay_t1 = new g_adjustment( t1,  0, 10, 1, 5, 5 );
	delay_t2 = new g_adjustment( t2,  5, 30, 1, 5, 5 );
	delay_t3 = new g_adjustment( t3,  1, 30, 1, 5, 5 );

	obj_hold<w_hbox> r1 = w_hbox::new_w_hbox(); // everything

	obj_hold<w_vbox> c_card = w_vbox::new_vbox(); // most of it, not vsliders
	obj_hold<w_vbox> c_screen = w_vbox::new_vbox(); // screen
        w_win_scrolled * cd_cmds_scrolled = new w_win_scrolled();

	obj_hold<w_hbox> r_tv = w_hbox::new_w_hbox(); // unused
	obj_hold<w_hbox> r_radio = w_hbox::new_w_hbox(); // radio left + right

	obj_hold<w_vbox> tv_tune = w_vbox::new_vbox(); // VCR CABLE 5 4 3 2 1
	obj_hold<w_vbox> tv_cmds = w_vbox::new_vbox(); // mute PAL ...
	obj_hold<w_vbox> cd_cmds = w_vbox::new_vbox(); // all cd commands
	obj_hold<w_vbox> c_radio_left = w_vbox::new_vbox();
	obj_hold<w_vbox> c_radio_right = w_vbox::new_vbox();

	obj_hold<w_spin_button> r_spin_t1  = new w_spin_button( delay_t1 );
	obj_hold<w_spin_button> r_spin_t2  = new w_spin_button( delay_t2 );
	obj_hold<w_spin_button> r_spin_t3  = new w_spin_button( delay_t3 );
	slider_factor = new line_factor_slider( this );
	curr_channel = new w_label("CHANNEL");

	struct str_pair // : public GRP_lib_media
	{
		const char * name;
		const char * cmd;
	};

	str_pair name_cmd_2[] = {
//	 {	"mute",		"volume mute" },
	 {	"Vol 80",	"volume 80%" },
	 {	"Vol 90",	"volume 90%" },
	 {	"Vol 100",	"volume 100%" },
	 {	"show",		"show" },
	 {	"setnorm PAL",	"setnorm PAL" },
	 {	"fullscreen",	"fullscreen" },
	 {	"clear_win_id",	"clear_win_id" },
	// rem.clear_win_id()"
	};
	int N_cmds_2 = sizeof( name_cmd_2 ) / sizeof( name_cmd_2[0] );

	int N2 = (N_tv_chan_table-1) / 2;
	int i1;

	for( i1 = N_tv_chan_table-1; i1>=N2; i1-- )
	{
		tv_tune->pack_asis( new w_btn_xawtv_channel(
			this,
			tv_chan_table[i1].name, i1
		));
	}

	for( ; i1>=0; i1-- )
	{
		tv_tune->pack_asis( new w_btn_xawtv_channel(
			this,
			tv_chan_table[i1].name, i1
		));
	}

	tv_cmds->pack_asis( new w_btn_base( "mute", this, (SignalFunc) C_mute ) );
	for( int i = 0; i<N_cmds_2; i++ )
	{
		tv_cmds->pack_asis( new w_btn_xawtv_cmd(
			this,
			name_cmd_2[i].name,
			name_cmd_2[i].cmd
		));
	}

	for(int i = 0; i<N_radio_channel_table; i++ )
	{
		w_vbox * col;
		if( (2*i) < N_radio_channel_table )
			col = c_radio_left;
		else
			col = c_radio_right;
		col->pack_asis( new w_btn_radio_freq( this, radio_channel_table[i].name, i ));
	}

	cd_cmds->pack_asis( new w_btn_cd( this, "eject" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "reset" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "pause" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "resume" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "stop" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "scan1" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "close_tray" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "close_dev" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 1" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 2" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 3" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 4" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 5" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 6" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 7" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 8" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 9" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 10" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 11" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 12" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 13" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 14" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 15" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 16" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 17" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 18" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 19" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 20" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 21" ));
	cd_cmds->pack_asis( new w_btn_cd( this, "play 22" ));

// Menu Items which are channel only

	int n_cbl = N_ir_cable_channel_table;
	for( int i=0; i<n_cbl; i++ )
	{
		ir_cable_channel * item = & ir_cable_channel_table[i];
		w_vbox * menu = pick_menu( item->M_group );
		menu -> pack_asis( new w_btn_cbl_channel( this, item ));
	}

// Menu Items which are IR commands

	int looksee = N_ir_command_table;
	for( int i = 0; i<N_ir_command_table; i++ )
	{
//		if( i==20 ) break; e_print( "iter %d\n", i );
		ir_command_info * item = & ir_command_table[i];
		w_vbox * menu = pick_menu( item->M_group );
		menu->pack_asis( new w_btn_ir( this, item));
	}

	// cant ask XAWTV for its current channel :-(
	// could get nearest radio freq
	channel_number_radio = 0; // 6
	channel_number_tv = 0;
	channel_number_tv = 7; // keypad 8 // CABLE

	if( rem.find_win_id() ) {
		tv_change_channel( channel_number_tv );
	} else {
		// this does the open
		radio_change_channel( channel_number_radio );
		// radio.unmute();
	}

	if(0) sock = new w_socket();
	// c_screen->pack_big( sock );

	r_tv->pack_big( tv_tune );
	r_tv->pack_asis( tv_cmds );

	r_radio->pack_asis( c_radio_left );
	r_radio->pack_asis( c_radio_right );

	c_card->pack_asis( r_tv );
	c_card->pack_asis( r_radio );

	// Play Pause in an easy to reach place
	w_widget0 * btn_vcr_play_pause = new w_btn_ir( this, & vcr_play_pause);
	btn_vcr_play_pause->set_bg("yellow");
	c_card->pack_asis( btn_vcr_play_pause);

	// Browse Select
	static ir_command_info cable_select = {
          M11, " Select",       "CABLE",  "Select"
        };
	static ir_command_info cable_browse = {
          M11, " Browse",       "CABLE",  "Browse"
        };
	obj_hold<w_hbox> r_sel_br = w_hbox::new_w_hbox();
	w_widget0 * btn_select = new w_btn_ir( this, & cable_select);
	w_widget0 * btn_browse = new w_btn_ir( this, & cable_browse);
	r_sel_br->pack_big( btn_select );
	r_sel_br->pack_big( btn_browse );
	c_card->pack_asis( r_sel_br );

	c_card->pack_asis( r_spin_t1 );
	c_card->pack_asis( r_spin_t2 );
	c_card->pack_asis( r_spin_t3 );
	c_card->pack_asis( new tv_clock( this ) );
if(1)	c_card->pack_asis( slider_factor );
	c_card->pack_asis( curr_channel );

// PACK ALL THE M99 MENUS

	w_win_scrolled * horiz_panel_outer = new w_win_scrolled();
	w_hbox * horiz_panel = new w_hbox();
	horiz_panel_outer -> pack_unscrollable( horiz_panel );

	for( int m=0; m<M99_count; m++ )
	{
		if( !cbl_menu[m] ) continue;
		w_win_scrolled * cbl_scrolled;
		cbl_scrolled = new w_win_scrolled();
		cbl_scrolled -> pack_unscrollable( cbl_menu[m] );
		cbl_scrolled -> set_border_width(0);
		horiz_panel->pack_asis( cbl_scrolled );
	}

        cd_cmds_scrolled->pack_unscrollable( cd_cmds );

// left to right
// TODO - add balloon text

if(1)	r1->pack_asis( slider_vol );
if(1)	r1->pack_asis( slider_line2 );
if(1)	r1->pack_asis( slider_pcm );
if(1)	r1->pack_asis( slider_cd_vol );
	r1->pack_asis( cd_cmds_scrolled );
	r1->pack_asis( c_card );
if(0)	r1->pack_asis( c_screen );
	r1->pack_big( horiz_panel_outer );

	this->pack_big( r1 );
	// this->pack_asis( r1 );
}

/*!
	move the xawtv window into a sub-panel
*/
void tv_panel::steal()
{
	sock->steal( rem.get_win_id() );
}

/*!
*/
void tv_panel::slider_factor_changed( gdouble v2 )
{
	factor_channel = v2;
	if(slider_line2) slider_line2->set_factor( factor_channel * factor_fader );
}

/*!
*/
void tv_panel::factor_fader_changed( gdouble v2 )
{
	factor_fader = v2;
	if(slider_line2) slider_line2->set_factor( factor_channel * factor_fader );
}

/*!
*/
void tv_panel::poll_for_external_change()
{
	if(slider_line2) slider_line2->poll_for_external_change();
	if(slider_vol) slider_vol->poll_for_external_change();
	if(slider_pcm) slider_pcm->poll_for_external_change();
}

}; // NAMESPACE
