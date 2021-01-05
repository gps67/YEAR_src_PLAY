#include "cd_music.h"

// #include <stdlib.h>
// #include <sys/time.h>

// #define gid_t gid_t
// #define uid_t uid_t

// #include <linux/cdrom.h>

#include "word_splitter.h"
#include "buffer2.h"

/*
	This works, but needs a better design. TODO: ...
	Events such as CD-NOW-AVAILABLE.
	Event: CD_MEDIA_CHANGED
	Event: CD_TRACK_CHANGED
	Bool: check_media() - so that readtoc doesnt fail
	Maybe have two layers, a wrap on the ioctls, and a sensible mix,
	eg open() sets the speed, closes the drive, reads the toc, etc
	
	is there a non-blocking ioctl ?
	Do what when not playing?
	After end of play, subchannel is reporting strange values for msf,
	does the device need to be stopped? (ATAPI x10)
*/

#ifdef WIN32
#warning WIN32 code absent
// all of this file is WIN32
#else
#endif

/*!
	constructor - copy msf_start and msf_next
*/
Track_info::Track_info( const struct cdrom_tocentry & tocentry, Track_info * next_trk )
{
}

/*!
	number of frames in this track - includes 2 sec gap upto next track
*/
int Track_info::len_frames()
{
	return 0;
}

/*!
	convert frames to min/sec (static - not specific to any track)
*/
void Track_info::len_frames_to_msf( int f, struct cdrom_msf0 & msf )
{
	/*
		f can be negative offset between tracks,
		fixup her rather than at calling point
	*/
	if( f < 0 ) f = 0;

	msf.frame = f % 75;
	f = f / 75;
	msf.second = f % 60;
	f = f / 60;
	msf.minute = f;
}

/*!
	convert min/sec to frames (static - not specific to any track)
*/
int Track_info::len_frames_of_msf( struct cdrom_msf0 msf )
{
	int f = 0;
	f += msf.minute;
	f *= 60;
	f += msf.second;
	f *= 75;
	f += msf.frame;
	return f;
}

////////////////////////////////////////////

/*!
	return string of status code
*/
str0 cd_subchannel::audio_status_string1()
{
	return "CDROM_AUDIO_NO_STATUS"; // never happens
}

/*!
	a step too far
*/
str0 cd_subchannel::format_string1()
{
	return "odd";
}

////////////////////////////////////////////

/*!
	if the device isnt open, call open
*/
bool CD_music::check_open()
{
	if( is_open() ) return true;
	return open();
}

/*!
	open the device
*/
bool CD_music::open( const char * device )
{
	return false;
}

/*!
	pause playing at the current pos
*/
bool CD_music::pause()
{
	return false;
}

/*!
	resume after pause
*/
bool CD_music::resume()
{
	return false;
}

/*!
	possible use - start on cue
*/
bool CD_music::start_spinning()
{
	return false;
}

/*!
	stop playing 
*/
bool CD_music::stop()
{
	return false;
}

/*!
	needs root perms
*/
bool CD_music::reset()
{
	return false;
}

/*!
	open the tray
*/
bool CD_music::eject()
{
	return false;
}

/*!
	close the drive tray - probably delays during toc read ?
*/
bool CD_music::close_tray()
{
	return false;
}

/*!
	setting speed doesnt help my drive
*/
bool CD_music::speed( int spd )
{
	return false;
}

/*!
	jukebox selector
*/
bool CD_music::select_disc( int idx ) // juke box 
{
	return false;
}

/*!
	returns if the media has changed since last call - not the ioctl status
*/
bool CD_music::media_changed()
{
	return false;
}

/*!
	few cds have a proper label - what about isofs header though ? media id ?
*/
bool CD_music::get_mcn( struct cdrom_mcn * mcn )
{
	return false;
}

/*!
	debugging - get and print MCN if its available
*/
bool CD_music::print_mcn()
{
	return false;
}


/*!
	convert the drive device status code to a string
*/
str0 CD_music::drive_status_string( int status )
{
	return "CDS_NO_SUCH_CODE_STRING";
}

/*!
	is the drive door open? disc available ?

	My drive is returning CDS_NO_INFO when the tray is open !
*/
int CD_music::get_status_drive()
{
	return 0;
}

/*!
	read the toc header and return the track count
*/
bool CD_music::read_toc_hdr( int & track_lo, int & track_hi )
{
	return false;
}

/*!
	fetch the tocentry for one track
*/
bool CD_music::read_toc_entry_msf( int track, struct cdrom_tocentry & tocentry )
{
	return false;
}

/*!
	fetch the tocentry for the LOADOUT track 0xAA -
	you need this to find the len of the last track
*/
bool CD_music::read_toc_entry_msf_LEADOUT( struct cdrom_tocentry & tocentry )
{
	return false;
}

/*!
	read the disks TOC and calculate the cddb diskid and keep audio index

	I dont think you can have a data track in the middle, if you can
	it doesnt make sense to play the entire span of the disk.
*/
bool CD_music::read_toc()
{
	return false;
}

/*!
	Theres no need to set the output volume,
	as it has its own soundcard channel but you can,
	eg if front switches have been nudged
*/
bool CD_music::set_vol_255( int left, int right )
{
	return false;
}

/*!
	read the toc if it has changed - returns OK if nothing failed
	and a TOC is available (possibly the old one)
*/
bool CD_music::check_toc()
{
	return false;
}

/*!
	debugging print
*/
bool CD_music::print_toc()
{
	return false;
}

/*!
	convert the status to a string
*/
str0 CD_music::disc_status_string( int status )
{
	return "CDS_other_disk_type";
}

/*!
	what kind of disc (if any) is in the drive
*/
int CD_music::get_status_disc()
{
	return 0;
}

/*!
	request play of the prepared time range
*/
bool CD_music::play( cdrom_msf msf )
{
	return false;
}

/*!
	every 1 second or so ... update stats
*/
void CD_music::get_status_all()
{
}

/*!
	return a str usable in a label showing current pos

	This should be re-written to call get_status_all

	This should cause a mild re-design to check a disk is in drive,
	maybe add a few state bool or bits
*/
str0 CD_music::pos_str()
{
	return "0";
}

/*!
	play [ track1 .. track2 ]
*/
bool CD_music::play( int track1, int track2 )
{
	return false;
}

/*!
	request the current pos on the disk, and status
*/
bool CD_music::do_subchannel()
{
	return false;
}

/*!
	constructor - init 0
*/
CD_music::CD_music( str0 DEV_CDROM )
: fd_hold_1()
, dev_cdrom( DEV_CDROM )
, cddb_disk_id_string()
{
	cddb_disk_id = 0;
}

/*!
	scan the toc and a few others
*/
bool CD_music::scan1()
{
	return false;
}


/*!
	run a command string
*/
bool CD_music::run( str0 cmd_line )
{
	return false;
}

/*!
	how many music tracks are on the disc (poss 0)
*/
int CD_music::track_count()
{
	return 0;
}

/*!
	cddb uses a sum of decimal digits, for each track start time
*/
static inline	int cddb_sum( int n )
{
	return 0;
}

/*!
	cddb/freedb disk id
*/
bool CD_music::calc_cddb_disk_id()
{
	return false;
}
