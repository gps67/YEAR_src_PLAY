#include "w_file_sel.h"
#include "w_btn_base.h"
#include "str2.h"

/*!
	create and show a file selector dialog
*/
w_file_sel::w_file_sel(
	w_widget0 * mainwin,
	str0 title,
	str2 * filename
)
{
	e_print( "# OLD CODE # gtk_file_selection_new() # use GtkFileChooser instead \n");
#warning "# OLD CODE # gtk_file_selection_new() # use GtkFileChooser instead"
	w_set( gtk_file_selection_new ( title ));
	compression_option = TRUE;
	set_filename( * filename );
	connect();
	show();
	if(mainwin) set_transient_for( mainwin );
	// TEST // add_thing2();
	// CALLER DOES // add_compression_option();
}

/*!
	Add any thing to the dialog
	LURK-TODO: retain item
*/
void w_file_sel::add_thing( w_widget0 * thing )
{
	int TODO_use_w_container;
	gtk_container_add(
		GTK_CONTAINER( FILE_SEL()->main_vbox),
		thing->outer_packable()
	);
	thing->show();
}

/*!
	TEST - add_thing() called twice
*/
void w_file_sel::add_thing2()
{
	w_btn_base * btn1 = new w_btn_base( "B-One", NULL, NULL );
	w_btn_base * btn2 = new w_btn_base( "B-Two", NULL, NULL );
	add_thing( btn1 );
	add_thing( btn2 );
}

/*!
	set the initial selected filename
*/
void w_file_sel::set_filename( str0 filename )
{
	if( filename )
	gtk_file_selection_set_filename( FILE_SEL(), (STR0) filename );
}

/*!
	get the filename (as a temp?)
*/
str0 w_file_sel::get_filename()
{
	// gtk uses a statically allocated string
	return gtk_file_selection_get_filename( FILE_SEL() );
}

/*!
	connect the ok buttons "clicked" to action AND THEN destroy
*/
void w_file_sel::connect_ok( // ok button is not the top widget
	SignalFunc func,
	obj_ref * app
) {
	gtk_signal_connect(
		GTK_OBJECT (FILE_SEL()->ok_button),
		"clicked",
		(GtkSignalFunc) func,
		app
	);
	// then ensure the dialog is closed
	// you can only call connect_ok ONCE !!
	gtk_signal_connect_object(
		GTK_OBJECT (FILE_SEL()->ok_button),
		"clicked",
		(GtkSignalFunc) gtk_widget_destroy,
		GTK_OBJECT (w)
	);
}

/*!
	connect the "cancel" buttons "clicked" (but not destroy?)
*/
void w_file_sel::connect_cancel( // cancel button is not the top widget
	SignalFunc func,
	obj_ref * app
) {
	gtk_signal_connect(
		GTK_OBJECT (FILE_SEL()->cancel_button),
		"clicked",
		(GtkSignalFunc) func,
		app
	);
}

/*!
	after gtk has destroyed the widget, it calls this to tell C++
*/
// gets vague here
// static
void w_file_sel::destroyed(
	GtkWidget * btn,
	w_file_sel * self
) {
	fprintf(stderr,"File Selector Destroyed\n");
	if(self)
		self->w_set_NULL();
	// cancelled( btn, self );
}

/*!
	UNCALLED ?
*/
// static
void w_file_sel::cancelled(
	GtkWidget * btn,
	w_file_sel * self
) {
	fprintf(stderr,"File Selector Cancelled\n");
}

/*!
*/
// static
void w_file_sel::selected(
	GtkWidget * btn,
	w_file_sel * self
) {
	fprintf(stderr,"File Selected '%s'\n", (STR0) self->get_filename());
	// gtk_widget_destroy( (GtkWidget*)self->FILE_SEL() );
}

/*!
*/
void w_file_sel::connect()
{
	gtk_signal_connect(
		GTK_OBJECT(w),
		"destroy",		// from Gtk core
		(GtkSignalFunc) destroyed,
		this
	);
	gtk_signal_connect(
		GTK_OBJECT (FILE_SEL()->ok_button),
		"clicked",
		(GtkSignalFunc) selected,
		this
	);
/*
	the clicked action MUST be already defined
	this is then called after that
	gtk_signal_connect_object(
		GTK_OBJECT (FILE_SEL()->ok_button),
		"clicked",
		(GtkSignalFunc) gtk_widget_destroy,
		GTK_OBJECT (w)
	);
*/

	/* Connect the cancel_button to destroy the widget */
	gtk_signal_connect_object(
		GTK_OBJECT( FILE_SEL()->cancel_button ),
		"clicked",
		(GtkSignalFunc) gtk_widget_destroy,
		GTK_OBJECT (w)
	);
}

/*!
*/
static void compression_button_pressed(
	GtkWidget * w,
	obj_ref * app
) {
	w_file_sel * fsel = (w_file_sel *) app;
	if(! fsel ) return;
	fsel->compression_option = ! fsel->compression_option;
	char * state = (char *) "OFF";
	if( fsel->compression_option )
		state = (char *) "ON";
	static char message[30];
	sprintf( (char *) &message, "Compression (is %s)", state );
	fsel->btn_compression->set_text( (str0)(char *) &message );
	
}

/*!
*/
void w_file_sel::add_compression_option()
{
	compression_option = FALSE; // then switched ON
	btn_compression = new w_btn_base(
		"Compression", // immediately replaced
		this,
		(SignalFunc) compression_button_pressed
	);
	add_thing( btn_compression );
	compression_button_pressed( NULL, this ); // fake call
}

/*!
*/
bool w_file_sel::get_compression_option()
{
	return compression_option;
}

/*!
*/
void w_file_sel::set_transient_for( w_widget0 * mainwin )
{
	if(!mainwin) return;
	GtkWidget * top = gtk_widget_get_toplevel( mainwin->w );
	gtk_window_set_transient_for( GTK_WINDOW(w), GTK_WINDOW( top ) );
}



