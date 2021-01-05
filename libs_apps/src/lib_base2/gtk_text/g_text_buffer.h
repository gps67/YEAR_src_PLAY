#ifndef g_text_buffer_H
#define g_text_buffer_H

#include "w_widget0.h"
#include "g_text_tag_table.h"
#include "obj_hold.h"
#include "str2.h"

class w_text_view;
// #include "w_text_view.h" // v simple holder
#include "w_win_scrolled.h"
#include "buffer1.h"

/*!
	g_text_buffer is the centre class, with all the functions,
	but the caller must retain each VIEW

	It is created from a SHARED tags table, but if you dont want
	shared tags, pass NULL, and a private (initial shared) will be created

	One text buffer can have any number of views, showing the same data

	This provides a load of extra hooks, specific marks, iter help
*/
class g_text_buffer : public obj_ref
{
	void init_mark_endpos();
	GtkTextIter iter; // temp thing
public:
	GtkTextBuffer *buffer;
	obj_hold<g_text_tag_table> tags;
	GtkTextMark *	mark_endpos;

	~g_text_buffer();
	g_text_buffer(); // create a new tags table
	g_text_buffer( g_text_tag_table * shared_tags );

	w_text_view * mk_view();
	w_win_scrolled * mk_view_scrolled();


	void set_text( str0 str, int len = -1 )
	{
		gtk_text_buffer_set_text ( buffer, (const char *)str, len );
	}
	void clear();
	void append_utf8( const buffer1 & buff );
	void append_8859( const buffer1 & buff );
	void append_8859( str0 str, int len = -1 );
	void append_utf8( str0 str, int len = -1 );
	bool append_file_8859( str0 filename );
	bool append_file( str0 filename )
	{
		return append_file_8859( filename );
	}
	bool append_file( str2 * filename )
	{
		return append_file( * filename );
	}

};

#endif

