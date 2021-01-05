#ifndef w_alignment_H
#define w_alignment_H

#include "dgb_fail.h"
#include "w_container_.h"

class w_alignment : public w_container_1
{
 public:

	GtkAlignment * Alignment() { return GTK_ALIGNMENT( w ); }


	w_alignment(
		float xalign, // 0 = left 1 = right
		float yalign, // 0 = top 1 = bottom
		float xscale, // 0 = asis 1 = fill
		float yscale
	);

	void set_alignment(
		float xalign, // 0 = left 1 = right
		float yalign, // 0 = top 1 = bottom
		float xscale, // 0 = asis 1 = fill
		float yscale
	);

	~w_alignment();
};


#endif

