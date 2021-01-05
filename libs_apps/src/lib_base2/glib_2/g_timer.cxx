#include "g_timer.h"

// #include "w_widget0.h" // a bit much 

/*!
	constructor - with timer started NOW
*/
g_timer::g_timer()
{
	timer = g_timer_new(); // started already
}

/*!
	stop timer
*/
void g_timer::stop()
{
	g_timer_stop( TIMER() );
}

/*!
	start timer - from ZERO
*/
void g_timer::start()
{
	g_timer_start( TIMER() );
}

/*!
	return timer as (double) seconds not fraction, it keeps running
*/
gdouble g_timer::elapsed_secs()
{
	gulong microsecs;
	static gdouble factor = 1.0/1000000;
	gdouble secs = g_timer_elapsed( TIMER(), & microsecs );
	secs += (gdouble)microsecs * factor;
	return secs;
}

/*!
	return the time and restart from zero
*/
gdouble g_timer::elapsed_step()
{
	stop();
	gdouble step = elapsed_secs();
	// slight error here
	start();
	return step;
}

/*!
	destructor
*/
g_timer::~g_timer()
{
	g_timer_destroy( TIMER() );
}

/*!
	At the end of an era (step) print the result and start the next era.
*/
void g_timer::step_print( str0 msg)
{
	gdouble step = elapsed_step();
	if( step < 0.01  )
		e_print(" STEP %7.4f %s\n", step, (STR0) msg );
	else
		e_print(" STEP %5.2f   %s\n", step, (STR0) msg );
}
