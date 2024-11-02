#ifndef bench_panel_H
#define bench_panel_H
#if 0

#ifdef WIN32
#warning "WIN32 portable code double rint(double x);"
#include <math.h>
#endif

#include "w_vbox.h"
#include "rem_xawtv.h"
// #include "w_socket.h"

#include "w_scale.h"
#include "rad_1.h"
#include "w_spin_button.h"

#include "mixa.h"
#include "w_label.h"
class mixa;
class w_socket;

#include "ir_client.h"
#include "tv_radio_data.h"
#include "cd_music.h"

namespace TV_REM {

// obj_hold<_> allows incomplete types
class line_factor_slider;
class mix_slider;
class mix_slider_line;
class mix_slider_factor;

/*!
	bench_panel is a load of buttons for TV and RADIO,
	along with a clock and some sliders.

	Its also the objects that do the TV, Radio and Mixer actions
*/
class bench_panel : public w_vbox
{
	const char * name_dev_mixer;
	const char * name_dev_radio;

	rad_1	radio;
	rem_xawtv rem;
	mixa * mixer;
	w_socket * sock;

	obj_hold<mix_slider> slider_cd_vol;
	obj_hold<mix_slider> slider_vol;
	obj_hold<mix_slider_factor> slider_line2;
	obj_hold<mix_slider> slider_pcm;
	obj_hold<g_adjustment> delay_t1;	// before click
	obj_hold<g_adjustment> delay_t2;	// after click (excl t1)
	obj_hold<g_adjustment> delay_t3;	// after click (excl t2)
	obj_hold<line_factor_slider> slider_factor;
	obj_hold<w_label> curr_channel;

	obj_hold<w_vbox> cbl_menu[(int)M99_count];

	friend struct line_factor_slider;
 public:

	w_vbox * pick_menu( M_menu_id id );

	obj_hold<IR_client> ir_client;
	obj_hold<CD_music> cd_music;

	bench_panel(
		const char * dev_mixer = "/dev/mixer",
		const char * dev_radio = "/dev/radio",
		const char * dev_lircd = "/dev/lircd",
		const char * dev_cdrom = "/dev/cdrom"
	);
	~bench_panel();
	void steal(); // after GTK_WIDGET_ANCHORED (packed/show)

	void radio_set_freq( float freq );
	void radio_change_channel( radio_channel_info * channel );
	void radio_change_channel( int chan );

	void tv_send_cmd_line( const char * line );
	void tv_change_channel( tv_chan_info * chan_info );
	void tv_change_channel(int chan );
	void tv_change_channel( ir_cable_channel * item );
	void tv_fullscreen();

	void ir_change_channel( ir_command_info * chan_info );

	void cd_command( str0 cmd );

	void mute_toggle();
	bool switch_radio();
	bool switch_tv();
	bool switch_tv_VCR_CABLE();
	bool fork_xawtv();
	bool fork_xawtv_wait();
	void xawtv_subtitle_2( const char * str1, const char * str2 );
	void xawtv_subtitle_none();

	int channel_number_radio;
	int channel_number_tv; // ?? used for channel +

	void init_key_snooper();
	bool key_snooper(
		GtkWidget *grab_widget,
		GdkEventKey *event
	);
	bool grab_key( int keycode, int modifier );

	bool fader_step();
	int fader_t1;
	int fader_t2;
	int fader_t3;
	int fader_t; // t_now
	float factor_fader;
	float factor_channel;

	void track_factor( gdouble * _factor_channel );
	void slider_factor_changed( gdouble v2 );
	void factor_fader_changed( gdouble v2 );
	mixa * get_mixer() { return mixer; }
	void poll_for_external_change();
};


///////////////////////////////////////

//! See bench_panel.h - volume slider fixup thing
class w_scale_fix : public w_scale
{
	obj_hold<g_adjustment> adj;
 public:
	w_scale_fix( bool horiz, g_adjustment * adj_ )
	: w_scale( horiz, adj_ )
	{
		adj = adj_;
	}

	GtkAdjustment * get_adjustment()
	{
		return adj->Adjustment();
	}

	~w_scale_fix() {}
};

///////////////////////////////////////

#define slide_horiz_1 true
//! See bench_panel.h - volume slider fixup thing
class line_factor_slider : public w_scale_fix
{
	gdouble old_val;
	bench_panel * panel;
	gdouble * factor_channel;
 public:
	/*!
		constructor - track a dummy variable
	*/
	line_factor_slider( bench_panel * _panel )
	: w_scale_fix( slide_horiz_1, new g_adjustment_silent( 1.0, 0.3, 2.6, .1, .2, 0.05 ))
	{
		panel = _panel;

		static gdouble dummy_factor = 1.0;
		old_val = 1.0;
		set_inverted( FALSE );
		track_factor( & dummy_factor );
	}

	/*!
		Switch to tracking a new variable
	*/
	void track_factor( gdouble * _factor_channel )
	{
		factor_channel = _factor_channel;
		set_value( *factor_channel );
	}

	/*!
		write changed value into the tracked variable,
		and tell panel, so that it can tell the mixer
	*/
	void event_value_changed( double _vol )
	{
		* factor_channel = _vol;
		panel->slider_factor_changed( _vol );
	}

};

///////////////////////////////////////

/*!
	There is a small confusion between the GUI value and the device value,
	there is also an issue with fading factors (not on cd channel)
*/
class mix_slider : public w_scale_fix
{
	static const bool slide_horiz = true;
	static const bool slide_vert  = false;
	//! hold own pointer to ready opened mixer
	mixa * mixer;
	str1 linename;
	int vol_dev;
 public:
	/*!
		constructor, does a slider callback
	*/
	mix_slider(
		mixa * _mixer,
		str0 _linename
	)
	: w_scale_fix( slide_vert, new g_adjustment_silent( 50, 0, 100, 0.0001, 5, 7 ))
	, mixer( _mixer )
	, linename( _linename )
	, vol_dev( -1 )
	{
		set_digits( 0 ); // decimal places
		get_set_initial_value();
	}

	/*!
		When device opens, get its initial value.

		Since this line is of interest give at least a mumbl
	*/
	void get_set_initial_value()
	{
		// get initval and set it into the slider
		// this is coming up with zero on line
		int vol = 15;
		if(!get_mix( vol )) return;
		if( vol < 5 ) vol = 15;
		set_value( vol );
	}

	/*!
		get the mixer device
	*/
	mixa * get_mixer_pointer()
	{
		return mixer;
	}

	/*!
		get the mixer device, and lookup the channel, else NULL
	*/
	mixa * get_mixer()
	{
		mixa * mixer = get_mixer_pointer();
		if(!mixer) return NULL;
		if(!mixer->check_connected() ) return NULL;
		if( vol_dev == -1 )
		{
			if( !mixer->get_dev_pos( linename, vol_dev ) ) 
			{
				e_print("bad mixer channel %s\n", (STR0) linename );
				static bool repeating = false;
				if( !repeating ) {
					repeating = true;
					mixer->gdb_list_devnames();
				}
				return NULL;
			}
		}
		return mixer;
	}

	/*!
		GTK slider/value has changed, so make the app do similarly
		(but avoid calling set_value recursively?)
	*/
	void event_value_changed( double val )
	{
		int vol = (int)rint(val);
		if( vol < 0 ) vol = 0;
		if( vol > 100 ) vol = 100;
		set_mix( vol );
	}

	/*!
		get the mix level without touching the GUI
	*/
        bool get_mix( int & vol )
	{
		mixa * mixer = get_mixer();
		if(!mixer) return false;
		if(! mixer->get_mix( vol_dev, vol ) ) return false;
		return true;
	}

	/*!
		set the mix level without touching the GUI
	*/
        bool set_mix( int vol )
	{
		mixa * mixer = get_mixer();
		if(!mixer) return false;
		if(0) e_print("mix_set_vol %s, %d\n", (STR0) linename, vol );
		if(! mixer->set_mix( vol_dev, vol ) ) return false;
		// panel->slider_cd_vol_changed( vol );
		return true;
	}

	/*!
		external mixer might have changed device

		Treat as if user had clicked this slider directly.

		NB Dont set_value(1.0) after finding 1.4 - fader fails
	*/
	void poll_for_external_change()
	{
		int old_val = (int)rint(get_value());
		int vol;
		if(!get_mix( vol )) return;
		if( old_val != vol )
		{
			 set_value( vol );
		}
	}

};

/*!
	bench_panel calls set_factor( fade_factor * channel_factor )
*/
class mix_slider_factor : public mix_slider
{
	double factor;
 public:
	mix_slider_factor( mixa * _mixer, str0 line )
	: mix_slider( _mixer, line )
	{
		factor = 1.0;
	}
	void set_factor( double factor2 )
	{
		/*
			It would have been better to have a ceiling value,
			and always calculate the setting, even after external
			change but instead risk divide by zero.
		*/
		double vlo = 0.1;
		if( factor2 < vlo ) factor2 = vlo;

		double vol1 = get_value();
		double vol2 = vol1 * factor2 / factor;
		if(0) e_print( "set_factor %5.2f %5.2f vol %5.2f %5.2f\n",
			(float) factor,
			(float) factor2,
			(float) vol1,
			(float) vol2
		);
		factor = factor2;

		set_value( vol2 );
	}
};


};
#endif
#endif // if 0
