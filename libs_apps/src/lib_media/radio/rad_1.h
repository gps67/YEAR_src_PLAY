#ifndef rad_1_H
#define rad_1_H

/*
	I have not tested this with V4L2
	I dont have a tv card, nor radio
	If you do, look for a version before Nov 2011
	before I tried to make it compile, rather than work.
	It used to work well. Way back when.

	TODO: reword as FAIL not test_val (eg mute)
*/

#include "fd_hold.h"
#ifdef WIN32
	// fake structures
	struct video_tuner {
		int rangelow;
		int rangehigh;
		int signal;
	};
	struct video_audio {
	};
#else
// not ported just fudged
#include <linux/videodev2.h>
#endif

/*!
	Radio Tuner (bttv)

	The device has integer tuner "frequencies", which need converting
	to MHz by the factor 12. Some devices are KHz.

	The device has its own valume and MUTE, but use mixer.
*/
class rad_1 : public fd_hold_1
{
 public:
	struct v4l2_tuner tuner;
	struct v4l2_audio audio;
//	struct video_tuner tuner;
//	struct video_audio audio;

	int freq_factor;

	bool tuner_get();
	bool tuner_set();
	bool audio_get();
	bool audio_set();

//	bool do_ioctl( const char * name, int func, void * data );

	//! The range of device frequencies - not as frequencies
	void get_lo_hi( int & lo, int & hi )
	{
		lo = tuner.rangelow;
		hi = tuner.rangehigh;
	}
	rad_1();
	bool open( const char * device );
//	bool close();
	bool close_unmuted();
	bool freq_set( int freq );
	bool freq_get( int & freq );
	int int_from_mhz( double mhz ) { return (int)(mhz * freq_factor + 0.5); }
	double mhz_from_int( int freq ) { return (double)freq / (double)freq_factor; }
	bool set_freq( float freq );
	bool get_freq( float & freq );
	bool mute_toggle();
	bool unmute();
	bool set_mute(bool mute);
	bool get_mute(bool & mute);
	bool mute();
	bool is_muted();
	bool is_stereo();
	bool get_stereo();
	int get_signal();
	//! The signal strength is 0,1,2,3 of 64K
	int signal() { return tuner.signal; }
	// added 2012 - should be private and specific to UNIX
	bool get_v4l2_control_value( int cid, int & value );
	bool set_v4l2_control_value( int cid, int   value );
	bool get_vol_100( int & vol );
	bool set_vol_100( int   vol );
	bool get_vol_100( double & vol );
	bool set_vol_100( double   vol );
};

#endif

