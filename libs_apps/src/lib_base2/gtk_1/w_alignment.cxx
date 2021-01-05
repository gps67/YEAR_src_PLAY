#include "w_alignment.h"


	w_alignment:: 
	w_alignment(
		float xalign, // 0 = left 1 = right
		float yalign, // 0 = top 1 = bottom
		float xscale, // 0 = asis 1 = fill
		float yscale
	)
	{
		w_set( gtk_alignment_new( xalign, yalign, xscale, yscale ));
	}

	void w_alignment:: set_alignment(
		float xalign, // 0 = left 1 = right
		float yalign, // 0 = top 1 = bottom
		float xscale, // 0 = asis 1 = fill
		float yscale
	)
	{
		gtk_alignment_set( Alignment(), xalign, yalign, xscale, yscale );
	}

	w_alignment:: ~w_alignment()
	{
	}
