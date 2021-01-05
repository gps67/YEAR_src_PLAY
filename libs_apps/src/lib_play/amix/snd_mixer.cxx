#include "dgb.h"
#include "snd_mixer.h"
#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

snd_mixer_card::
snd_mixer_card()
: handle( NULL )
{
};

snd_mixer_card::
~snd_mixer_card()
{
	close_card();
};

bool snd_mixer_card:: open_card( const char * cardname )
{
	int open_mode = 0; // not a clue
	if((err=snd_mixer_open( &handle, open_mode ))) {
		INFO("cardname '%s'", cardname );
		return FAIL("snd_mixer_open returned err %d", err );
	}
	if((err=snd_mixer_attach(handle, cardname))) {
		return FAIL("snd_mixer_attach returned err %d", err );
	}
	if((err=snd_mixer_selem_register(handle, NULL, NULL))) {
		return FAIL("snd_mixer_register returned err %d", err );
	}
	if((err=snd_mixer_load(handle))) {
		return FAIL("snd_mixer_load returned err %d", err );
	}
	return PASS("OK");
}

bool snd_mixer_card:: close_card()
{
	if(!handle) return PASS("handle already NULL");
	if((err=snd_mixer_close( handle ))) {
		handle = NULL;
		return FAIL("snd_mixer_close returned err %d", err );
	}
	handle = NULL;
	return PASS("OK");
}

bool snd_mixer_card:: amix_cmd( int argc, char *argv[])
{
	const char *cardname = "default";
	const char *ctrl_name = "Master";
	long volume = 60;

	if (argc != 3) {
		printf("Usage: %s snd_card_name control_name vol\n",argv[0]);
		return FAIL("usage");;
	}
	cardname = argv[0];
	ctrl_name = argv[1];
	volume = atol(argv[2]);

	INFO("Cardname = %s", cardname );
	INFO("Control = %s", ctrl_name );
	INFO("Volume = %ld", volume );

	long min, max;
	snd_mixer_selem_id_t *sid;

	if(!open_card( cardname )) return FAIL_FAILED();

	snd_mixer_selem_id_alloca(&sid);
	// no err=
	snd_mixer_selem_id_set_index(sid, 0);
	// no err=
	snd_mixer_selem_id_set_name(sid, ctrl_name );
	// no err=

	snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

	if((err=snd_mixer_selem_get_playback_volume_range(elem, &min, &max))) {
		return FAIL("snd_mixer_selem_get_playback_volume_range returned err %d", err );
	}

	if((err=snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100))) {
		return FAIL("snd_mixer_selem_set_playback_volume_all returned err %d", err );
	}

	close_card();

	return PASS("seems good");
	return true;
}



