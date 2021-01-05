
#include "mixa.h"
#include "dgb_fail.h"

/*
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <math.h>
*/


#ifdef WIN32
#else
#endif


// extern int errno;

// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/soundcard.h>
// #include <sys/ioctl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <errno.h>
// #include <string.h>
// #include <math.h>

//! constructor - _fd can override _dev_mixer
mixa::mixa(
	const char * _dev_mixer,
	int _fd 
)
: fd( _fd )
, dev_mixer( _dev_mixer )
{
	cant_open_count = 0;
	check_connected();
}

//! destructor
mixa::~mixa()
{
	close();
}

bool mixa::check_connected()
{
	if( fd < 0 )
		return reconnect();
	return true;
}

bool mixa::reconnect()
{
	return open( NULL );
}

//! open "/dev/mixer" or other
bool mixa::open(const char *device)
{
	if(! device )
	{
		device = (STR0) dev_mixer;
	} else {
		cant_open_count = 0;
		dev_mixer = device;
	}
	if(! device ) return false;
	static const int TRY_MAX = 5;
	if( cant_open_count > TRY_MAX ) {
		e_print( ".");
		return false;
	}
	if( cant_open_count == TRY_MAX ) {
		cant_open_count ++;
		e_print( "# MIXER # XXX %d # open %s # retry count done \n",
			cant_open_count,
			(STR0) dev_mixer
		);
		return false;
	}
	close();
	fd = ::open(device,O_RDWR);
	if( fd == -1 ) {
		cant_open_count ++;
		e_print( "# MIXER # try %d # open %s # %m \n",
			cant_open_count,
			(STR0) dev_mixer
		);
		return false;
	}
	cant_open_count = 0;
	return true;
}

//! close device (its not EXCL)
void mixa::close(void)
{
	if( fd>=0) ::close(fd);
	fd = -1;
}

//! sound device names are actually constant, but as if not
const char * mixa::get_devname(int device)
{
#ifdef WIN32
return "ABSENT";
#else
	const char * names[] = SOUND_DEVICE_NAMES;
	if( device < SOUND_MIXER_NRDEVICES ) 
		return  names[device];
	FAIL("device=%d SOUND_MIXER_NRDEVICES=%d", device, SOUND_MIXER_NRDEVICES );
	/*
	"vol", "bass", "treble", "synth", "pcm", "speaker", "line",
	"mic", "cd", "mix", "pcm2", "rec", "igain", "ogain",
	"line1", "line2", "line3", "dig1", "dig2", "dig3",                                                                           
	"phin", "phout", "video", "radio", "monitor"  
	*/
#endif
}

bool mixa::gdb_list_devnames()
{
#ifdef WIN32
	return false;
#else
	static bool repeats = false;
	if( repeats ) return false;
	repeats = true;
	int mask = 0;
	if(ioctl( fd, SOUND_MIXER_READ_DEVMASK, &mask ) < 0) {
		FAIL("ioctl failed - following may be junk");
	}
	const char * names[] = SOUND_DEVICE_NAMES;
	int max = sizeof( names ) / sizeof( names[0] );
	for( int i=0; i< max; i++ ) {
		int bitval = 1<<i;
		char c1 = '-';
		if( mask & bitval ) c1 = '+';
		INFO("[%2d] 0x%8.8X %c '%s'",
			i,
			bitval,
			c1,
			names[i]
		);
	}
	return true;
# endif
}

//! Convert name to int-pos (NB pos called 'dev', could be 'channel' or 'subdev')
bool mixa::get_dev_pos( const char *devname, int & dev_id )
{
	if( !check_connected() ) return false;
#ifdef WIN32
#warning "MISSING CODE on WIN32"
return false;
#else
	int mask = 0;
	if(ioctl( fd, SOUND_MIXER_READ_DEVMASK, &mask ) < 0) return false;

	int bit1 = 1;

	for( int i=0; i<SOUND_MIXER_NRDEVICES; i++ ) {
		if( 0==strcmp( devname, get_devname(i) ))
		{
			if( mask & bit1 ) {
				dev_id = i;
				return true;
			} else {
				FAIL("name match but bit not set in mask");
				INFO("devname='%s'", devname );
				INFO("posi=%d", i );
				INFO("bit1=0x%6.6X", bit1 );
				INFO("mask=0x%6.6X", mask );
				return false;
			}
		}
		bit1 <<= 1;
	}
	INFO("devname='%s'", devname );
	gdb_list_devnames();
	return false;
#endif
}

//! simple func to get a single 0-100 value for sub-device
bool mixa::get_mix( const char *device, int & vol )
{
	int dev = 0;
	if(! get_dev_pos( device, dev )) return false;
	return get_mix( dev, vol );
}

//! simple func to get a single 0-100 value for sub-device
bool mixa::get_mix(int dev, int & vol )
{
	int lvol;
	int rvol;
	if(! get_mix( dev, lvol, rvol ) ) return false;
	vol = (lvol + rvol) / 2;
	return true;
}

//! get two 0-100 values for sub-device("line")
bool mixa::get_mix(int dev, int & lvol, int &rvol )
{
#ifdef WIN32
#warning "MISSING CODE on WIN32"
return false;
#else
	if( !check_connected() ) return false;
	int v;
	if(ioctl( fd, MIXER_READ( dev ), &v ) < 0) return false;

	lvol = (v >> 8) & 0xFF ;
	rvol = (v     ) & 0xFF ;
	return true;
#endif
}

//! simple func to set a single 0-100 value for sub-device
bool mixa::set_mix( const char *device, int volume)
{
	int dev;
	if(! get_dev_pos( device, dev )) return false;
	return set_mix( dev, volume);
}
//! simple func to set a single 0-100 value for sub-device
bool mixa::set_mix( int dev, int volume)
{
	if( volume < 0 ) volume = 0;
	if( volume > 255 ) volume = 255;
	return set_mix( dev, volume, volume );
}

//! set two 0-100 values for sub-device("line")
bool mixa::set_mix( int dev, int lvol, int rvol )
{
#ifdef WIN32
#warning "MISSING CODE on WIN32"
return false;
#else
	if( !check_connected() ) return false;
	int vol_l_r;
	if( !join( vol_l_r, lvol, rvol ) )
	{
	}
	if( ioctl( fd, MIXER_WRITE( dev ), &vol_l_r ) < 0)
	{
		return false;
	}
	return true;
#endif
}

//! convert single int 2 device bytes into 2 integer values (1-100)
bool mixa::split( int value, int & l, int & r )
{
	r =  value       & 0xFF;
	l = (value >> 8 )& 0xFF;
	return true;
}

//! join 2 integer values into 2 device bytes (single int)
bool mixa::join( int & value, int l, int r )
{
	bool ok = true;
	if( l < 0 ) {
		l = 0;
		ok = false;
	}
	if( r < 0 ) {
		r = 0;
		ok = false;
	}
	if( l > 255 ) {
		l = 255;
		ok = false;
	}
	if( r > 255 ) {
		r = 255;
		ok = false;
	}
	value = (l << 8) + r;
	return ok;
}

//! its very confusing to have the initial zero line vol - fixup
bool mixa::fixup_zero_startup()
{
	/*
		if you dont run aumix on system startup
		(eg its linked to a library not on default path and LD_L_PATH unset)
		the line volume is zero
	*/
	bool ok = true;
	int line_id;
	if( !get_dev_pos( "line", line_id ) ) return false;
	int vol;
	if( !get_mix( line_id, vol ) ) return false;
	if( vol < 3  ) // zero is uninitialised
		if(! set_mix( line_id, 88 ) ) ok = false;;
	return ok;
}

