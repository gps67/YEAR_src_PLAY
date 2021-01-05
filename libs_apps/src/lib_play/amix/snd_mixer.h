#ifndef snd_mixer_H
#define snd_mixer_H

#include "obj_ref.h"

/* STUBS instead of
	<alsa/asoundlib.h>
	<alsa/mixer.h>
 hmm didnt need "C"
*/
	struct _snd_mixer;
	typedef struct _snd_mixer snd_mixer_t;

/*
	alsa/... provides snd_mixer_t * ;
	alsa/... uses prefix snd_mixer_* 

	we reuse the same name ... ??

	each elem is a slider (?)
	each elem has an uplink which keep the card alive

*/

class snd_mixer_card : public obj_ref0 {
 public:
	snd_mixer_t *handle;
	int err;

	~snd_mixer_card(); // virtual
	snd_mixer_card();

	bool open_card( const char * cardname );
	bool close_card();

	bool amix_cmd( int argc, char *argv[]);
};


class snd_mixer_elem : public obj_ref0 {
 public:
	obj_hold<snd_mixer_card> card;
};
#endif

