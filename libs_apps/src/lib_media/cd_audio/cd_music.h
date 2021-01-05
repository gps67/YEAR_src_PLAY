#ifndef cd_music_H
#define cd_music_H

#include "fd_hold.h"
#ifdef WIN32
#warning "WIN32 missing code"
struct cdrom_msf0{
	int minute;
	int second;
	int frame;
};
struct cdrom_subchnl
{
	int junk;
};
struct cdrom_msf
{
	int x;
};
#else
#include <linux/cdrom.h>
#endif

#include "str1.h"
#include "obj_list.h"


/*!
	The Track_info is the start and end times of the track, and is_audio()

	This might somehow add a link to the title ...
*/
class Track_info : public obj_ref
{
 public:
	bool data_track;
	struct cdrom_msf0 msf_start;
	struct cdrom_msf0 msf_next;

	Track_info(
		const struct cdrom_tocentry & _tocentry,
		Track_info * next_trk = NULL // LEADOUT
	 );

	bool is_data()
	{
		return data_track;
	}

	bool is_audio()
	{
		return !is_data();
	}

static	void len_frames_to_msf( int f, struct cdrom_msf0 & msf );
static	int len_frames_of_msf( struct cdrom_msf0 msf );
	int len_frames();

	void prints()
	{
		e_print( "Track_info toc_entry - %2d min %2d sec %2d frames %s\n",
			msf_start.minute,
			msf_start.second,
			msf_start.frame,
			is_audio() ? "AUDIO" : "DATA"
		);
	}

	int starting_second() // for cddb-diskid
	{
		return msf_start.minute * 60 + msf_start.second;
	}

	int starting_frame()
	{
		return len_frames_of_msf( msf_start );
	}
};

/*!
	The sunchannel information is the current position and status
*/
struct cd_subchannel : public GRP_lib_media
{
	struct cdrom_subchnl sc;
	cd_subchannel()
	{
		memset( &sc, 0, sizeof(sc) ); // C++ does this anyway
	}
	str0 audio_status_string1();
	str0 format_string1();
};

/*!

	cd_music controls the CD to play music
*/
class CD_music : public fd_hold_1
{
	str1 dev_cdrom;

	obj_list<Track_info> track_info;
	obj_hold<Track_info> track_info_LEADOUT;

	cd_subchannel sc;

	/*!
		app must clear this as soon as it notices
	*/
	bool media_changed_ever;

	bool calc_cddb_disk_id(); // private - can only call from read_toc

 public:

	int cddb_disk_id;
	str1 cddb_disk_id_string; // includes offsets and totals

	CD_music( str0 DEV_CDROM );
	bool open( const char * device = NULL );
	bool check_open();
	int status_drive;
	int status_disc;
	int get_status_drive();
	int get_status_disc();
	void get_status_all();

	bool read_toc();
	bool print_toc();
	bool check_toc();

	bool close_tray();
	bool start_spinning(); // for fast - or use pause anyway
	bool stop();
	bool eject();
	bool reset();
	bool freq_set( int freq );
	bool pause();
	bool speed( int spd );
	bool media_changed();
	bool select_disc( int idx ); // juke box 
	bool get_mcn( struct cdrom_mcn * mcn );
	bool print_mcn();
	bool play( cdrom_msf msf );
	bool play( int track1, int track2 );
	bool read_toc_entry_msf(  int track, struct cdrom_tocentry & tocentry );
	bool read_toc_entry_msf_LEADOUT( struct cdrom_tocentry & tocentry );
	bool read_toc_hdr( int & track_lo, int & track_hi );
	bool resume();
	bool run( str0 cmd_line );
	bool scan1();

	bool diskid( int & id );

	bool set_vol_255(int left=255, int right=255 );

//	virtual void event_track_changed()

	bool do_subchannel();
	str0 drive_status_string( int status );
	str0 disc_status_string( int status );
	str0 pos_str();

	int track_count();

};

#endif

