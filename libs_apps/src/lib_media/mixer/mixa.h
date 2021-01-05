#ifndef MIXA_H
#define MIXA_H

#ifdef WIN32
#else

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
#endif

#include "ints.h"

#include "str1.h"

/*!
	mixa provides access to /dev/mixer (OSS style not ALSA).

	The simpler functions presume evenly balanced, to get
	off-balance stereo-mix, use the advanced functions.

	You usually convert a sub-device name "line" or "vol"
	into an integer, then set or get its values.

	TODO: rebase on fd_hold_0 (without FILE *)
*/
class mixa : public GRP_lib_media
{
	int fd;
	str1 dev_mixer;
	int cant_open_count;

	mixa( const mixa & mixb ); // linkage error or protected error
 public:
	mixa(
		const char * _dev_mixer = NULL,
		int _fd = -1
	);
	~mixa();

	bool check_connected();
	bool reconnect();
	bool open(const char *device = NULL);
	void close(void);
	const char * get_devname(int device);
	bool get_dev_pos( const char *devname, int & dev_id );
	bool get_mix( const char *device, int & vol );
	bool get_mix(int dev, int & vol );
	bool get_mix(int dev, int & lvol, int &rvol );
	bool set_mix( const char *device, int volume );
	bool set_mix( int dev, int volume );
	bool set_mix( int dev, int lvol, int rvol );
	bool split( int value, int & l, int & r );
	bool join( int & value, int l, int r );
	bool fixup_zero_startup();
	bool gdb_list_devnames();
};

#endif
