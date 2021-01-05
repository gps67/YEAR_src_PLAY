#ifndef w_text_view_H
#define w_text_view_H

#include "w_widget0.h"
class g_text_tag_table;
#include "g_text_buffer.h"
#include "buffer1.h"
#include "w_win_scrolled.h"

/*!
	a text view is a view on a shared g_text_buffer,
	this is mostly about the packable window frame.

	It does join the view and the text, so things like
	internel scrolling are done here, and a few macros over the two

	The text_view is self-scrolling, but its USUALLY put into a
	scrolling frame (to get the scrollbars + semantics).
*/
class w_text_view : public w_widget0
{
 public:
	obj_hold<g_text_buffer> buff;

	w_text_view( g_text_buffer * buff );
	w_text_view( g_text_tag_table * tags );
	w_text_view();
	GtkTextView * TEXT_VIEW() { return GTK_TEXT_VIEW( w ); }
	g_text_buffer * get_text_buffer() { return buff; }
	g_text_tag_table * get_tag_table() { return buff->tags; }

	w_win_scrolled * mk_scroll_wrap();

	void scroll_to_endpos();
	void append_8859( str0 msg );
	void append_utf8( str0 msg );
	void append_8859( const buffer1 & b );
	void append_utf8( const buffer1 & b );

//	void e_print_from_8859( const char * fmt, va_list args );

	void set_font1( const char * str = "Serif 15" );
	void set_editable( bool editable )
	{
		gtk_text_view_set_editable( TEXT_VIEW(), editable );
	}

	void test1();

};

#endif

