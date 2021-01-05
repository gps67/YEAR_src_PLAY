#include "snd_mixer.h"
#include "dgb.h"

bool amix_cmd(int argc, char * argv[])
{
	INFO("cmd is %s", argv[0] );
	obj_hold<snd_mixer_card> mix = new snd_mixer_card();
	return mix->amix_cmd( argc, argv );
}
