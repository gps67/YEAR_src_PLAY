#include "rad_1.h"

// #include <stdlib.h>
// #include <sys/time.h>

/*
	these snippets compared with various sources
*/

#define gid_t gid_t
#define uid_t uid_t

#include <stdio.h> // perror
#include <unistd.h> // close
#include <string.h> // memset
// #include <errno.h>
// #include <fcntl.h>
#include <sys/ioctl.h> // ioctls use IOC macro to generate number

// DONE // #include <linux/videodev.h>
// #include <linux/videodev.h>

//! get the tuner parameters
bool rad_1::tuner_get()
{
	return do_ioctl( "VIDIOC_G_TUNER", VIDIOC_G_TUNER, &tuner );
//	return do_ioctl( "VIDIOCGTUNER", VIDIOCGTUNER, &tuner );
}

//! set the tuner parameters
bool rad_1::tuner_set()
{
	return do_ioctl( "VIDIOC_S_TUNER", VIDIOC_S_TUNER, &tuner );
//	return do_ioctl( "VIDIOCSTUNER", VIDIOCSTUNER, &tuner );
}

//! get the device volume
bool rad_1::audio_get()
{
	return do_ioctl( "VIDIO_G_AUDIO", VIDIOC_G_AUDIO, &audio );
//	return do_ioctl( "VIDIOGAUDIO", VIDIOCGAUDIO, &audio );
}

//! set the device volume
bool rad_1::audio_set()
{
	return do_ioctl( "VIDIO_S_AUDIO", VIDIOC_S_AUDIO, &audio );
//	return do_ioctl( "VIDIOSAUDIO", VIDIOCSAUDIO, &audio );
}

//! Set the tuner frequency (as device int val not MHZ)
bool rad_1::freq_set( int freq )
{
	return do_ioctl( "VIDIOC_S_FREQUENCY", VIDIOC_S_FREQUENCY, &freq );
//	return do_ioctl( "VIDIOCSFREQ", VIDIOCSFREQ, &freq );
}

//! Get the tuner frequency (as device int val not MHZ)
bool rad_1::freq_get( int & freq )
{
	return do_ioctl( "VIDIOC_G_FREQUENCY", VIDIOC_G_FREQUENCY, &freq );
//	return do_ioctl( "VIDIOCGFREQ", VIDIOCGFREQ, &freq );
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
	/*
		/dev/radio might be busy (xawtv closing),
		and the caller sits in a loop trying 10 times
		so if its already open, keep it. To reuse an fd,
		call close()  then open()
	*/
	if(is_open()) return true;
	if(!fd_hold_1::open_RO( device )) {
		return false;
	}
	int LURK_wait_for_async_to_open;
#warning tuner = 0
//	tuner.tuner = 0;
	tuner_get();
	audio_get();

	freq_factor = 16;
	if ((tuner.flags & VIDEO_TUNER_LOW))
		freq_factor *= 1000;	// KHz not MHz

	return true;
/*
 gdb says:
 tuner = {
	tuner = 0,
	name = "Radio",
	rangelow = 1040,
	rangehigh = 1728,
	flags = 0,
	mode = 0,
	signal = 32768
 }, 
 audio = {
	audio = 0,
	volume = 0,
	bass = 0,
	treble = 0,
	flags = 0, 
	name = '\000',
	mode = 0,
	balance = 0,
	step = 0
 }
*/

}

//! CTRL-C shows that the device can be closed but still running (GOOD)
bool rad_1::close_unmuted()
{
	int fd1 = fd_steal();
	::close( fd1 );
	return true;
}

//! Tune into MHz
bool rad_1::set_freq( float mhz )
{
	if(!expect_open()) return false;
	return freq_set( int_from_mhz( mhz ) );
	return freq_set( (int)( mhz * freq_factor + 0.5 ));
}

//! Tuned into WHAT MHz
bool rad_1::get_freq( float & mhz )
{
	if(!expect_open()) return false;
	int freq = 0;
	if(! freq_get( freq ) ) return false;
	mhz = (float) mhz_from_int( freq );
	return true;
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
//	if(!expect_open()) return TRUE;
//	audio_get();
	return (audio.flags & VIDEO_AUDIO_MUTE );
}

//! Toggle is good, on is good, off is good
void rad_1::unmute()
{
	if(!expect_open()) return;
//	audio_get();
	audio.flags &= ~VIDEO_AUDIO_MUTE;
	audio_set();
}

//! Toggle is good, on is good, off is good
void rad_1::mute()
{
//	if(!expect_open()) return;
//	audio_get();
	audio.flags |= VIDEO_AUDIO_MUTE;
	audio_set();
}

//! Did the device report this tuning as strong and stereo?
bool rad_1::is_stereo()
{
	return audio.mode == VIDEO_SOUND_STEREO ;
}

//! Does the device report stereo? GET NEW VALUE NOT CACHE
bool rad_1::get_stereo()
{
	if(!audio_get()) return false;
	return is_stereo();
}

//! Signal strength is 0,1,2,3 fractions of 64K , GET NEW VALUE NOT CACHE
int rad_1::get_signal()
{
	if(!expect_open()) return 0;
	// memset(&vt,0,sizeof(vt));
	tuner_get();
	return signal();
}

