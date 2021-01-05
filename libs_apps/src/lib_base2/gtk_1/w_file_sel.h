#ifndef w_file_sel_H
#define w_file_sel_H

#include "w_widget0.h"
class str2;
class w_btn_base;

/*!
	a file selection widget (DOCS NEEDE)
*/
class w_file_sel : public w_widget0
{ 
public:
	GtkFileSelection * FILE_SEL() { return (GtkFileSelection *) w; }
	w_btn_base * btn_compression;
	bool compression_option;
	w_file_sel(
		w_widget0 * mainwin,
		str0 title,
		str2 * filename
	);

	void add_thing( w_widget0 * thing );
	void add_thing2();
	void add_compression_option();
	bool get_compression_option();
	void set_filename( str0 filename );
	str0 get_filename();
	void connect_ok( // ok button is not the top widget
		SignalFunc func,
		obj_ref * app
	) ;
	void connect_cancel( // cancel button is not the top widget
		SignalFunc func,
		obj_ref * app
	) ;
	// gets vague here
	static void destroyed(
		GtkWidget * btn,
		w_file_sel * self
	) ;
	static void cancelled(
		GtkWidget * btn,
		w_file_sel * self
	) ;
	static void selected(
		GtkWidget * btn,
		w_file_sel * self
	) ;
	void connect();
	void set_transient_for( w_widget0 * mainwin );

};

#endif
