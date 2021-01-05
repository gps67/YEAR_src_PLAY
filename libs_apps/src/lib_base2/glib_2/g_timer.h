#ifndef g_timer_H
#define g_timer_H

// #include "w_widget0.h" // a bit much 
#include "obj_ref.h" // a bit much 
#include <glib.h>


/*!
	A hi-res timer, in (double) seconds.

	Particularly useful for t1 = T.elapsed_step();
*/
class g_timer : public obj_ref
{
	GTimer * timer;
public:
	GTimer * TIMER() { return timer; }
//	GObject* OBJECT) { return (g_object *) timer; }

	g_timer();
	void stop();
	void start();
	gdouble elapsed_secs();
	gdouble elapsed_step();
	~g_timer();
	void step_print( str0 msg );
};
#endif

