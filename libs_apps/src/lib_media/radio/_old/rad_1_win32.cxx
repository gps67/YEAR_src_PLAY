#include "rad_1.h"

// #include <stdlib.h>
// #include <sys/time.h>

/*
	these snippets compared with various sources
*/

#define gid_t gid_t
#define uid_t uid_t

// #include <sys/ioctl.h> // ioctls use IOC macro to generate number

// DONE // #include <linux/videodev.h>
// #include <linux/videodev.h>

//! get the tuner parameters
bool rad_1::tuner_get()
{
	return false;
}

//! set the tuner parameters
bool rad_1::tuner_set()
{
	return false;
}

//! get the device volume
bool rad_1::audio_get()
{
	return false;
}

//! set the device volume
bool rad_1::audio_set()
{
	return false;
}

//! Set the tuner frequency (as device int val not MHZ)
bool rad_1::freq_set( int freq )
{
	return false;
}

//! Get the tuner frequency (as device int val not MHZ)
bool rad_1::freq_get( int & freq )
{
	return false;
}

//! constructor - init 0
rad_1::rad_1()
: fd_hold_1()
{
	memset( &tuner, 0, sizeof(tuner) );
	memset( &audio, 0, sizeof(audio) );
}

//! open "/dev/radio0", dont tune, keep muted
bool rad_1::open( const char * device )
{
#warning WIN32 TODO
	return false;
}

//! CTRL-C shows that the device can be closed but still running (GOOD)
bool rad_1::close_unmuted()
{
	return false;
}

//! Tune into MHz
bool rad_1::set_freq( float mhz )
{
	return false;
}

//! Tuned into WHAT MHz
bool rad_1::get_freq( float & mhz )
{
	return false;
}

//! Device MUTE is good for start, exit, phone-calls
void rad_1::mute_toggle()
{
	if( is_muted() )
		unmute();
	else
		mute();
}

//! Is the device muted? You have to reread if another prog changed things
bool rad_1::is_muted()
{
	return false;
}

//! Toggle is good, on is good, off is good
void rad_1::unmute()
{
}

//! Toggle is good, on is good, off is good
void rad_1::mute()
{
}

//! Did the device report this tuning as strong and stereo?
bool rad_1::is_stereo()
{
	return false;
}

//! Does the device report stereo? GET NEW VALUE NOT CACHE
bool rad_1::get_stereo()
{
	return false;
}

//! Signal strength is 0,1,2,3 fractions of 64K , GET NEW VALUE NOT CACHE
int rad_1::get_signal()
{
	return 0;
}

