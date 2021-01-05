#include "cd_music.h"

// #include <stdlib.h>
// #include <sys/time.h>

// #define gid_t gid_t
// #define uid_t uid_t

#include <stdio.h> // perror
// #include <unistd.h> // close
// #include <string.h> // memset
// #include <errno.h>
// #include <fcntl.h>
// #include <sys/ioctl.h> // ioctls use IOC macro to generate number

#include <linux/cdrom.h>

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
#else
#endif

#ifdef WIN32
#warning WIN32 code absent
Track_info::Track_info( const struct cdrom_tocentry & tocentry, Track_info * next_trk )
{
}
#else

/*!
	constructor - copy msf_start and msf_next
*/
Track_info::Track_info( const struct cdrom_tocentry & tocentry, Track_info * next_trk )
{
	data_track = (tocentry.cdte_ctrl & CDROM_DATA_TRACK);
	msf_start.minute = tocentry.cdte_addr.msf.minute;
	msf_start.second = tocentry.cdte_addr.msf.second;
	msf_start.frame  = tocentry.cdte_addr.msf.frame;

	if( next_trk )
	{
		msf_next.minute = next_trk->msf_start.minute;
		msf_next.second = next_trk->msf_start.second;
		msf_next.frame  = next_trk->msf_start.frame;
	} else {
		msf_next.minute = 0;
		msf_next.second = 0;
		msf_next.frame  = 0;
	}
}
#endif

/*!
	number of frames in this track - includes 2 sec gap upto next track
*/
int Track_info::len_frames()
{
	return
	  len_frames_of_msf( msf_next )
	- len_frames_of_msf( msf_start );
}

// 75 frames per second is fixed

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
	switch( sc.cdsc_audiostatus ) {
	 case CDROM_AUDIO_INVALID:	return "CDROM_AUDIO_INVALID";
	 case CDROM_AUDIO_PLAY:		return "CDROM_AUDIO_PLAY";
	 case CDROM_AUDIO_PAUSED:	return "CDROM_AUDIO_PAUSED";
	 case CDROM_AUDIO_COMPLETED:	return "CDROM_AUDIO_COMPLETED";
	 case CDROM_AUDIO_ERROR:	return "CDROM_AUDIO_ERROR";
	 case CDROM_AUDIO_NO_STATUS:	return "CDROM_AUDIO_NO_STATUS";
	}
	return "CDROM_AUDIO_NO_STATUS"; // never happens
}

/*!
	a step too far
*/
str0 cd_subchannel::format_string1()
{
	switch( sc.cdsc_format )
	{
		case CDROM_LBA: return "LBA";
		case CDROM_MSF: return "MSF";
	}
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

	if( is_open() ) close();

	media_changed_ever = false;
	status_drive = CDS_NO_INFO;
	status_disc = CDS_NO_INFO;

	/*
		dev_cdrom is set in the constructor,
		open(device) defaults device to NULL
	*/
	if( device ) dev_cdrom = device;

	if( dev_cdrom.is_empty() ) return false;

	if(!fd_hold_1::open_RO_async( dev_cdrom )) {
		return false;
	}
	set_non_block();

	set_vol_255( 255, 255 );
	get_status_all();
	return true;
}

/*!
	pause playing at the current pos
*/
bool CD_music::pause()
{
	if(!is_open()) return false;
	e_print("# CD # pause()\n" );
	return do_ioctl( "CDROMPAUSE", CDROMPAUSE, NULL );
}

/*!
	resume after pause
*/
bool CD_music::resume()
{
	if(!check_open()) return false;
	e_print("# CD # resume()\n" );
	return do_ioctl( "CDROMRESUME", CDROMRESUME, NULL );
}

/*!
	possible use - start on cue
*/
bool CD_music::start_spinning()
{
	if(!check_open()) return false;
	e_print("# CD # start_spinning()\n" );
	return do_ioctl( "CDROMSTART", CDROMSTART, NULL );
}

/*!
	stop playing 
*/
bool CD_music::stop()
{
	if(!is_open()) return false;
	e_print("# CD # stop()\n" );
	return do_ioctl( "CDROMSTOP", CDROMSTOP, NULL );
}

/*!
	needs root perms
*/
bool CD_music::reset()
{
	if(!check_open()) return false;
	e_print("# CD # reset()\n" );
	return do_ioctl( "CDROMRESET", CDROMRESET, NULL );
}

/*!
	open the tray
*/
bool CD_music::eject()
{
	if(!check_open()) return false;
	e_print("# CD # eject()\n" );
	return do_ioctl( "CDROMEJECT", CDROMEJECT, NULL );
}

/*!
	close the drive tray - probably delays during toc read ?
*/
bool CD_music::close_tray()
{
	if(!check_open()) return false;
	speed(1);
	e_print("# CD # close_tray()\n" );
	return do_ioctl( "CDROMCLOSETRAY", CDROMCLOSETRAY, NULL );
}

/*!
	setting speed doesnt help my drive
*/
bool CD_music::speed( int spd )
{
	// setting speed(1) here doesnt work - anywhere for that matter
	// maybe I'm not root, maybe this drive is simple, or ...
	if(!is_open()) return false;
	e_print("# CD # speed(%d)\n", spd );
	return do_ioctl( "CDROM_SELECT_SPEED", CDROM_SELECT_SPEED, &spd );
}

/*!
	jukebox selector
*/
bool CD_music::select_disc( int idx ) // juke box 
{
	if(!is_open()) return false;
	e_print("# CD # select_disc(%d)\n", idx );
	return do_ioctl( "CDROM_SELECT_DISC", CDROM_SELECT_DISC, &idx );
}

/*!
	returns if the media has changed since last call - not the ioctl status
*/
bool CD_music::media_changed()
{
	if(!is_open()) return false;
	int status = 0;
	do_ioctl_int_ret( "CDROM_MEDIA_CHANGED", CDROM_MEDIA_CHANGED, &status );
	if( status ) media_changed_ever = true;
	if( status ) e_print("# CD # media_changed()\n");
	return status;
}

/*!
	few cds have a proper label - what about isofs header though ? media id ?
*/
bool CD_music::get_mcn( struct cdrom_mcn * mcn )
{
	if(!is_open()) return false;
	return do_ioctl_can_fail( "CDROM_GET_MCN", CDROM_GET_MCN, mcn );
}

/*!
	debugging - get and print MCN if its available
*/
bool CD_music::print_mcn()
{
	struct cdrom_mcn mcn;
	if(!get_mcn( &mcn ) )
	{
		return false;
	}
	e_print("# CD # MCN = '%s'\n", mcn.medium_catalog_number );
	return true;
}


/*!
	convert the drive device status code to a string
*/
str0 CD_music::drive_status_string( int status )
{
	switch( status )
	{
	 case CDS_NO_INFO :		return "CDS_NO_INFO";
	 case CDS_NO_DISC :		return "CDS_NO_DISC";
	 case CDS_TRAY_OPEN :		return "CDS_TRAY_OPEN";
	 case CDS_DRIVE_NOT_READY : 	return "CDS_DRIVE_NOT_READY";
	 case CDS_DISC_OK :		return "CDS_DISC_OK";
	}
	e_print("# BAD STATUS # drive_status_string(%d)'\n", status );
	return "CDS_NO_SUCH_CODE_STRING";
}

/*!
	is the drive door open? disc available ?

	My drive is returning CDS_NO_INFO when the tray is open !
*/
int CD_music::get_status_drive()
{
	if(!is_open()) return 0;
	status_drive = CDS_NO_INFO;
	do_ioctl_int_ret( "CDROM_DRIVE_STATUS", CDROM_DRIVE_STATUS, & status_drive );
	// e_print("# CD # get_status_drive # %s\n", (STR0) drive_status_string( status_drive ) );
	return status_drive;
}

/*!
	read the toc header and return the track count
*/
bool CD_music::read_toc_hdr( int & track_lo, int & track_hi )
{
	if(!is_open()) return false;
	get_status_drive();
	switch( status_drive )
	{
		case CDS_DISC_OK : break;
		default:
			if(0) e_print("# CD # read_toc_hdr # %s\n",
				(STR0) drive_status_string( status_drive ) );
			return false;
	}
	struct cdrom_tochdr tochdr;
	if(! do_ioctl( "CDROMREADTOCHDR", CDROMREADTOCHDR, & tochdr ))
		return false;
	track_lo = tochdr.cdth_trk0;
	track_hi = tochdr.cdth_trk1;
	return true;
}

/*!
	fetch the tocentry for one track
*/
bool CD_music::read_toc_entry_msf( int track, struct cdrom_tocentry & tocentry )
{
	if(!is_open()) return false;
	memset( & tocentry, 0, sizeof(tocentry) );
	tocentry.cdte_track=track;                                                     
	tocentry.cdte_format=CDROM_MSF;                                            
	return do_ioctl( "CDROMREADTOCENTRY", CDROMREADTOCENTRY, & tocentry );
}

/*!
	fetch the tocentry for the LOADOUT track 0xAA -
	you need this to find the len of the last track
*/
bool CD_music::read_toc_entry_msf_LEADOUT( struct cdrom_tocentry & tocentry )
{
	// CDROM_LEADOUT is track 0xAA
	return read_toc_entry_msf( CDROM_LEADOUT, tocentry );
}

/*!
	read the disks TOC and calculate the cddb diskid and keep audio index

	I dont think you can have a data track in the middle, if you can
	it doesnt make sense to play the entire span of the disk.
*/
bool CD_music::read_toc()
{
	track_info.clear();
	if(!is_open()) return false;

	int track_lo;
	int track_hi;
	if( !read_toc_hdr( track_lo, track_hi ) ) return false;
	// e_print("# CD # read_toc_hdr(%d,%d)\n", track_lo, track_hi );

	struct cdrom_tocentry tocentry;
	if(!read_toc_entry_msf_LEADOUT(  tocentry ) ) return false;
	track_info_LEADOUT = new Track_info( tocentry );
	Track_info * next_trk = track_info_LEADOUT;

	track_info.resize(track_hi-track_lo+1);

	for( int i=track_hi; i>=track_lo; i-- )
	{
		if(!read_toc_entry_msf( i, tocentry ) ) return false;
		Track_info * trk = new Track_info( tocentry, next_trk );
		int j = i - track_lo;
		track_info[j] = trk;
		next_trk = trk;
	}

	// this requires the data tracks to exist,
	calc_cddb_disk_id(); // private - can only call from read_toc

	// remove data tracks
	for( int i=track_info.N()-1; i>=0; i-- )
	{
		if(( track_info[i] == NULL )
		|| ( track_info[i]->is_data() ) )
			 track_info.del( i );
	}

	media_changed(); // it probably has - get the flag NOW

	return true;
}

/*!
	Theres no need to set the output volume,
	as it has its own soundcard channel but you can,
	eg if front switches have been nudged
*/
bool CD_music::set_vol_255( int left, int right )
{
	if( left  < 0 ) left  = 0;
	if( right < 0 ) right = 0;
	if( left  > 255 ) left  = 255;
	if( right > 255 ) right = 255;

	if(!is_open()) return false;
	struct cdrom_volctrl vols;
	if(!do_ioctl( "CDROMVOLREAD", CDROMVOLREAD, & vols )) return false;

	e_print("# CD # VOL # %d %d %d %d (was)\n", 
		vols.channel0,
		vols.channel1,
		vols.channel2,
		vols.channel3
	);
	vols.channel0 = left;
	vols.channel1 = right;
//	vols.channel2 =   0; // leave as-was
//	vols.channel3 =   0;
	
	if(!do_ioctl( "CDROMVOLCTRL", CDROMVOLCTRL, & vols )) return false;
	if(!do_ioctl( "CDROMVOLREAD", CDROMVOLREAD, & vols )) return false;
	e_print("# CD # VOL # %d %d %d %d (now)\n", 
		vols.channel0,
		vols.channel1,
		vols.channel2,
		vols.channel3
	);
	return true;
}

/*!
	read the toc if it has changed - returns OK if nothing failed
	and a TOC is available (possibly the old one)
*/
bool CD_music::check_toc()
{
	int n = track_info.N();
	/*
		first call finds n==0
	*/
	if(!n) return read_toc();
	/*
		second call might find media has changed
	*/
	if( media_changed() ) return read_toc();
	return true;
}

/*!
	debugging print
*/
bool CD_music::print_toc()
{
	Track_info * trk;
	for( int i = 0; i< track_info.N(); i++ )
	{
		trk = track_info[i];
		if(trk) trk->prints();
	}
	trk = track_info_LEADOUT;
	if(trk) trk->prints();
	return true;
}

/*!
	convert the status to a string
*/
str0 CD_music::disc_status_string( int status )
{
	switch( status )
	{
	 case CDS_NO_INFO : return "CDS_NO_INFO";
	 case CDS_NO_DISC : return "CDS_NO_DISC";

	 case CDS_AUDIO  : return "CDS_AUDIO";
	 case CDS_DATA_1 : return "CDS_DATA_1";
	 case CDS_DATA_2 : return "CDS_DATA_2";
	 case CDS_XA_2_1 : return "CDS_XA_2_1";
	 case CDS_XA_2_2 : return "CDS_XA_2_2";
	 case CDS_MIXED  : return "CDS_MIXED";
	}
	return "CDS_other_disk_type";
}

/*!
	what kind of disc (if any) is in the drive
*/
int CD_music::get_status_disc()
{
	if(!is_open()) return 0;
	status_disc = CDS_NO_INFO;
	do_ioctl_int_ret( "CDROM_DISC_STATUS", CDROM_DISC_STATUS, &status_disc );
	// e_print("# CD #  get_status_disc == %d %s\n", status, (STR0) disc_status_string(status_disc) );
	return status_disc;
	// THIS IS FAILING EVERY TIME
}

/*!
	request play of the prepared time range
*/
bool CD_music::play( cdrom_msf msf )
{
	if(!check_open()) return false;
	e_print("# CD play # msf01.cdmsf_min0+   %2d %2d %2d\n",
		msf.cdmsf_min0,
		msf.cdmsf_sec0,
		msf.cdmsf_frame0
	);
	e_print("# CD play # msf01.cdmsf_min1+   %2d %2d %2d\n",
		msf.cdmsf_min1,
		msf.cdmsf_sec1,
		msf.cdmsf_frame1
	);
	return do_ioctl( "CDROMPLAYMSF", CDROMPLAYMSF, &msf );
}

/*!
	every 1 second or so ... update stats
*/
void CD_music::get_status_all()
{
	if(!is_open()) return;
	get_status_drive();
	get_status_disc();
	if( check_toc() )
		do_subchannel();
}

/*!
	return a str usable in a label showing current pos

	This should be re-written to call get_status_all

	This should cause a mild re-design to check a disk is in drive,
	maybe add a few state bool or bits
*/
str0 CD_music::pos_str()
{
	if(!is_open()) return "CDROM device is not open";
	get_status_all();

	static buffer2 buff(30);
	buff.clear();
	buff.print("drive %s\n", (STR0) drive_status_string( status_drive ) );

 if( track_count() )
 {
	// if not OK change output format
	buff.print("media %s\n", (STR0) disc_status_string( status_disc ) );

	if(0)
	{
		buff.print("%2.2X = ", sc.sc.cdsc_format );		// 02
		buff.put( sc.format_string1() );
		buff.put( "\n" );
	}

	if(0)
	{
	buff.print("adr: %1.1X ", sc.sc.cdsc_adr );			// 0
	buff.print("ctrl: %1.1X ", sc.sc.cdsc_ctrl );			//  0
	buff.print("ind: %2.2X\n", sc.sc.cdsc_ind );			// 01
	if( sc.sc.cdsc_ctrl & CDROM_DATA_TRACK ) buff.put("DATA_TRACK ");
	}


	buff.print("Track %d ", sc.sc.cdsc_trk );		// TRACK
	buff.print("of %d ", track_count() );
	buff.put( "\n" );

	buff.print("%2.2d:", sc.sc.cdsc_absaddr.msf.minute );
	buff.print("%2.2d ", sc.sc.cdsc_absaddr.msf.second );
//	buff.print("%2.2d ", sc.sc.cdsc_absaddr.msf.frame );
	if( track_info_LEADOUT )
	{
		buff.put( "of " );
		buff.print("%2.2d:", track_info_LEADOUT->msf_start.minute );
		buff.print("%2.2d ", track_info_LEADOUT->msf_start.second );
	}

	int trk = sc.sc.cdsc_trk - 1; // Track 1 is [0]
	Track_info * trk_info = NULL;
	if( 0 < trk && trk < track_count() )
		trk_info = track_info[trk];
	if( trk_info )
	{
		buff.put( "\n" );
		struct cdrom_msf0 msf;

		int F1 = Track_info::len_frames_of_msf( trk_info->msf_start );
		int F2 = Track_info::len_frames_of_msf( sc.sc.cdsc_absaddr.msf );
		int F3 = Track_info::len_frames_of_msf( trk_info->msf_next );

	//	buff.print("(%4.4d frames)", (F2-F1) );
		Track_info::len_frames_to_msf( (F2-F1), msf );
		buff.print("%2.2d:", msf.minute );
		buff.print("%2.2d ", msf.second );

		buff.put( "of " );

		Track_info::len_frames_to_msf( (F3-F1), msf );
		buff.print("%2.2d:", msf.minute );
		buff.print("%2.2d ", msf.second );
	}

	buff.put( "\n" );
	if(0)
	buff.print("%2.2X = ", sc.sc.cdsc_audiostatus );	// 11 15=stopped
	buff.put( sc.audio_status_string1() );
 }
	return (str0) buff;
}

/*!
	play [ track1 .. track2 ]
*/
bool CD_music::play( int track1, int track2 )
{
	if(!is_open()) return false;
	cdrom_msf msf01;
	int n = track_count();
	// check_toc(); // called by track_count
	if( track1 < 0 ) track1 = 0;
	if( track2 < 0 ) track2 = n-1; // -1 becomes END
	if( track2 >= n ) track2 = n-1;
	if( track1 >= n )
	{
		e_print("# CD # NO SUCH TRACK %d of %d\n", track1+1, n );
		return false;
	}

	Track_info * trk = track_info[ track1 ];
	msf01.cdmsf_min0   = trk->msf_start.minute;
	msf01.cdmsf_sec0   = trk->msf_start.second;
	msf01.cdmsf_frame0 = trk->msf_start.frame;

	trk = track_info[ track2 ];
	msf01.cdmsf_min1   = trk->msf_next.minute;
	msf01.cdmsf_sec1   = trk->msf_next.second;
	msf01.cdmsf_frame1 = trk->msf_next.frame;

	e_print("# CD # playing tracks %d - %d of %d\n", track1+1, track2+1, n );
	return play( msf01 );
}

/*!
	request the current pos on the disk, and status
*/
bool CD_music::do_subchannel()
{
	if(!is_open()) return false;
	// struct cdrom_subchnl    sc.sc;
	sc.sc.cdsc_format = CDROM_MSF;
	return do_ioctl( "CDROMSUBCHNL", CDROMSUBCHNL, &sc.sc );
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
	check_open();
	speed(1);
	close_tray();
	get_status_drive();
	if(!read_toc() ) return false;
	print_toc();
	get_status_disc();
	print_mcn();
	return true;
}


/*!
	run a command string
*/
bool CD_music::run( str0 cmd_line )
{
	word_splitter args;
	args.split_by_spaces( cmd_line );
	int argc = args.N();
	if(argc < 1 ) return false; // NULL COMMAND IS  OK  ?
	str0 arg0 = args[0];
	if( arg0 == "eject" )
	{
		return eject();
	}
	 else if ( arg0 == "open_dev" )	{ check_open(); }
	 else if ( arg0 == "pause" )	{ pause(); }
	 else if ( arg0 == "resume" )	{ resume(); }
	 else if ( arg0 == "stop" )	{ stop(); }
	 else if ( arg0 == "reset" )	{ reset(); }
	 else if ( arg0 == "start_spinning" )	{ start_spinning(); }
	 else if ( arg0 == "close_dev" ) { close(); } // check_open will reopen
	 else if ( arg0 == "close_tray" ) { return close_tray(); }
	 else if ( arg0 == "scan1" ) { return scan1(); }
	 else if ( arg0 == "play" )
	{
		int t1 = -1;
		int t2 = -1;
		if( argc >= 2 )
		{
			if( !args[1].as_int( t1 ) ) return false; // DIAG
			t1 --;
			if( argc >= 3 )
			{
				if( !args[2].as_int( t2 ) ) return false; // DIAG
				t2 --;
			}
		}
		play( t1, t2 );
	}
	return false;
}

/*!
	how many music tracks are on the disc (poss 0)
*/
int CD_music::track_count()
{
	check_toc();
	return track_info.N();
}

/*!
	cddb uses a sum of decimal digits, for each track start time
*/
static inline	int cddb_sum( int n )
{
	unsigned int ret = 0;
	while( n > 0 )
	{
		ret += (n % 10 );
		n /= 10;
	}
	return ret;
}

/*!
	cddb/freedb disk id
*/
bool CD_music::calc_cddb_disk_id()
{
	cddb_disk_id = 0;
	int ntrks = track_count(); // INCLUDING DATA TRACKS
	if(!ntrks) return false;

	Track_info * trk = NULL;
	int n = 0;
	for( int i=0; i<ntrks; i++ )
	{
		trk = track_info[i];
		if(!trk) return false;
		n += cddb_sum( trk->starting_second() );
	}

	trk = track_info_LEADOUT;
	if(!trk) return false;
	int leadout_start_second =  trk->starting_second();

	trk = track_info[0];
	if(!trk) return false;
	int track_1_start_second =  trk->starting_second();

	int disk_second = leadout_start_second - track_1_start_second;

	cddb_disk_id = ((n%0xFF) << 24 | disk_second << 8 | ntrks );

	// OK // e_print( "cddb_disk_id %08x\n", cddb_disk_id );

	buffer2 buff(280);

	buff.print( "%08x", cddb_disk_id );
	buff.print( " %d", ntrks );
	for( int i=0; i<ntrks; i++ )
	{
		trk = track_info[i];
		buff.print( " %d", trk->starting_frame() );
	}
	buff.print( " %d", track_info_LEADOUT->starting_frame()/75 );
	buff.put( 0 );
	// OK // e_print( "# CD # %s\n", (STR0)(str0)buff );
	
	cddb_disk_id_string = buff; // includes offsets and totals
	e_print( "# CD # cddb_disk_id_string %s\n", (STR0)(str0)cddb_disk_id_string );


	return true;
}
