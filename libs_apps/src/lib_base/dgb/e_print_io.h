#ifndef e_print_gui_or_not_H
#define e_print_gui_or_not_H

#include "obj_ref.h"
#include "blk1.h"
#include "buffer2.h"

/*!
	a virtual class that lib_base can use (with or without gui)

	like g_print, e_print() has its own buffers.
	Then calls write_UTF8( blk ) or (str)

	TODO: maybe think of a virtual console
	where gtk_text can also act as a readline keyboard

	For now, just enough to make it compile.
*/
class e_print_io_INTERFACE : public obj_ref
{

	// buff is either 8859 or utf8 depending on IO not APP
//	buffer2 buff;
 public:
	bool prefers_8859; // if not a FLAG_BIT in obj header
	/*
		the caller is supposed to know this mode
		and call the fastest routine, but it doesnt have to.

		The caller can work in its own CSET and e_print
		must adapt to it.

		app_prefers_8859( false ) may or may not
		change the mode of the console, but it should
		probably change the default mode of the buffer(s)

		However, if e_print has its own mode, then the io's mode
		is a good place to translate stuff en-masse
	*/

	/*CTOR*/ e_print_io_INTERFACE();
	/*DTOR*/ virtual ~ e_print_io_INTERFACE();

 private:

	virtual bool write_BYTES( blk1 & blk );
 public:

	virtual bool write_ASCII( blk1 & blk );
	virtual bool write_8859( blk1 & blk );
	virtual bool write_UTF8( blk1 & blk );
// repeats
	virtual bool write_UTF8( STR0 str );

	virtual void app_prefers_8859( bool latin15 );
	virtual void shutdown();

	virtual void show_unhide() {}

	virtual bool set_as_glib_print_handler();

	virtual void switch_gui_onoff( bool onoff );
	virtual void switch_gui_onoff_toggle();
};

extern obj_hold<e_print_io_INTERFACE> e_print_io;

#endif
