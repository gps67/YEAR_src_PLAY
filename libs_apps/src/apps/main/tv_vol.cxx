#ifdef WIN32
int main(int argc, char ** argv )
{
	return 5;
}
#else

#include "rad_1.h"
#include "mixa.h"

/*
	tv_vol

	The kde desktop starts up automatically at boot,
	which unfortunately blasts the TV out at full volume.

	I used to prefer to use the audio mixer, which is
	loaded at reasonable values, but after moving to a 
	new machine, the tv_rem wasnt active, the hot-keys
	(that change volume full screen) werent active,
	and at full screen I reverted to using the card volume.

	It would be better to move this to js_many.

	This sets the volume to (90) on the card.
*/
#define IF_LOUD if(0)

void exit_bad_usage( const char * progname, const char * word )
{
	e_print("\n" );
	e_print("# ERROR # %s bad usage '%s' : Try one of these:\n", progname, word );
	e_print("#  %s 99 \n", progname );
	e_print("#  %s +5 \n", progname );
	e_print("#  %s -5 \n", progname );
	e_print("\n" );
	exit(22);
}

bool a_to_f( str0 word, bool & abs, double & dbl )
{
	const char * str = (STR0) word;
	bool ok = true;
	abs = true;
	float flt = 0.0;
	flt = atof( str );
	if( flt == 0.0 ) {
		ok = false; // poss redemption later
	}
 restart:
	char c = * str;
	switch( c ) {
		case '-':
		case '+':
			abs = false;
			IF_LOUD e_print( "Relative %6.2f\n", flt ); 
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
	// why wasnt this ok? or is it 0x30 - 0x39 == -9
	//	case '0' - '9':
			ok = true;
			IF_LOUD e_print( "Absolute %6.2f\n", flt ); 
			break;
		case ' ':
		case '\t':
			str++;
			IF_LOUD e_print( "SPACE AT START\n" ); 
			goto restart;
		default:
			IF_LOUD e_print( "CHAR AT START\n" ); 
			ok = false;
	}
	if(ok) dbl = flt;
	return ok;
}

//! tv_vol
int main(int argc, char ** argv )
{

	int line_vol_100 = 99;
	int old_vol_i = 0;
	mixa mix;
	mix.get_mix( "line", line_vol_100 );
	e_print("line %d\n", line_vol_100 );
	// if too silent or too distorted - force back to 80
	if( (line_vol_100 < 60) || (90 < line_vol_100) ) {
		line_vol_100 = 80;
		mix.set_mix( "line", line_vol_100 );
	}
	// holder does this // setlocale(LC_ALL,"");
//	char * filename = NULL;
//	if( argc == 2 ) {
//		filename = argv[1];
//	}
	float K64 = 64.0 * 1024.0 - 1.0 ;
	double vol100 = 80.0;
	vol100 = 100; // actually xawtv device vol 100 is not distorted
	float new_vol = 50.0;
	float old_vol = 0.0;
	bool abs = true;
	int vol64k = 80;
	const char * dev_name = "/dev/video0";

	const char * progname = argv[0];
	argv++;
	argc--;
	while( argc > 0 ) {
		str0 word = (char * ) argv[0];
		argv ++;
		argc --;
		IF_LOUD e_print( "word = '%s'\n", (STR0) word );
		if( a_to_f( word, abs, vol100 )) {
			; // OK // 100 0 -5 + 5
			continue;
		}

	/*
		After all that, the device is busy
		seems the only way would be ...
	*/
		if(( word == "-r" ) || ( word == "--radio" )) {
			dev_name = "/dev/radio0";
			continue;
		}
			
		exit_bad_usage( progname, word );
	}

	// static or stack versions of obj_ref cause debug
	// mostly because stack is not valid address
	// and debugging another situation
	//
	obj_hold<rad_1> ptrrad = new rad_1();
	rad_1 & rad = * ptrrad;;
	if(! rad.open( dev_name ) ) goto fail;
//	if(! rad.audio_get() ) goto fail;
	if(!rad.get_vol_100( old_vol_i )) goto fail;
	old_vol = old_vol_i;

	if( abs )
		new_vol = vol100;
	else
		new_vol = old_vol + vol100;

	e_print( "volume = %5d %4.1f (%4.1f) -> %4.1f\n",
		-1,
		old_vol,
		vol100,
		new_vol
	);

	if( new_vol > 100.0 ) new_vol = 100.0;
	if( new_vol < 0.0 ) new_vol = 0.0;

	if(! rad.set_vol_100( new_vol) ) goto fail;
	rad.clear_dont_close();
	
	return 0;
fail:
	perror("FAIL");
	return errno;
}

// DOXYGEN Main Page

/*!

	tv_vol 99 - sets the radio cards volume to 99 percent

*/
#endif
