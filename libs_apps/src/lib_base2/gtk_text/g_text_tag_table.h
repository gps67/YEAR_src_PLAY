#ifndef g_text_tag_table_H
#define g_text_tag_table_H

#include "w_widget0.h"

class w_text_view;

class g_text_tag_table : public obj_ref
{
	GtkTextTagTable *tag_table;
 public:
	GtkTextTag * tag1;
	GtkTextTagTable *TAG_TABLE()
	{
		return tag_table;
	}

	g_text_tag_table();
	GtkTextTag * lookup( str0 tagname );
	GtkTextTag * create( str0 tagname );
	GtkTextTag * create_anon();
	void remove( GtkTextTag * _tag1 = NULL );
	void set_tag1( GtkTextTag * _tag1 );

	void set_tag_prop( str0 prop, str0 val );
	void set_tag_prop( str0 prop, int val );
	void set_tag_prop_obj( str0 prop, GtkObject * val );

	// get is also possible
};

#endif

