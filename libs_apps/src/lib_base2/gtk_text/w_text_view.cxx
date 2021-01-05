
#include "w_text_view.h"
#include "g_text_buffer.h"
#include "e_print.h"
#include "dgb.h"

w_text_view::w_text_view( g_text_buffer * _buff )
{
	buff = _buff;
	w = gtk_text_view_new_with_buffer( buff->buffer );
}

w_text_view::w_text_view( g_text_tag_table * tags )
{
	buff = new g_text_buffer( tags );
	w = gtk_text_view_new_with_buffer( buff->buffer );
}

w_text_view::w_text_view()
{
	buff = new g_text_buffer();
	w = gtk_text_view_new_with_buffer( buff->buffer );
	errno_zero();
}

void w_text_view::scroll_to_endpos()
{
	gtk_text_view_scroll_to_mark(
		TEXT_VIEW(),
		buff->mark_endpos,
		0.05,
		false,
		0.9, 0.9
	);
}

void w_text_view::append_8859( str0 msg )
{
	buff->append_8859( msg );
	scroll_to_endpos();
}

void w_text_view::append_utf8( str0 msg )
{
	buff->append_utf8( msg );
	scroll_to_endpos();
}


void w_text_view::append_8859( const buffer1 & b )
{
	buff->append_8859( b );
	scroll_to_endpos();
}

void w_text_view::append_utf8( const buffer1 & b )
{
	buff->append_utf8( b );
	scroll_to_endpos();
}

w_win_scrolled * w_text_view::mk_scroll_wrap()
{
	w_win_scrolled * wrap =
	new w_win_scrolled(
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC
	);
	wrap->pack( this );
	return wrap;
}

void w_text_view::set_font1( const char * str )
{
	errno_zero();
#ifdef WITH_GTK2
	PangoFontDescription * font_desc;
	font_desc = pango_font_description_from_string ( str );
	char * msg = pango_font_description_to_string( font_desc );
	INFO("PANGO FONT: %s (%s)", msg, str );
	g_free( msg );
	gtk_widget_modify_font( GTK_WIDGET(TEXT_VIEW()), font_desc);
	pango_font_description_free (font_desc);
#endif
	errno_zero();
}

////////////////

void w_text_view::test1()
{
	g_text_tag_table * tags = get_tag_table();
        tags->create("red");
        tags->set_tag_prop( "foreground", "#FF0000" );
        tags->create("green");
        tags->set_tag_prop( "foreground", "#00FF00" );
        tags->create("blue");
        tags->set_tag_prop( "foreground", "blue" );

	GtkTextBuffer *text_buffer = buff->buffer;

	GtkTextIter iter1;
	GtkTextIter iter2;

	GtkTextTag *tag;

	append_8859( "Hello One Two Three four Five\n" );

	/// iter2 = buff->iter; // bit field copy - BEFORE ADD TEXT
	gtk_text_buffer_get_iter_at_offset( text_buffer, &iter2, 0 );	

	iter1 = iter2;
	gtk_text_iter_forward_word_end( &iter2 );
	// NOW iter1..iter2 is the first word
	tag = tags->lookup( "red" );
	if(!tag) return;
	gtk_text_buffer_apply_tag( text_buffer, tag, &iter1, &iter2 );

	iter1 = iter2;
	gtk_text_iter_forward_word_end( &iter2 );
	tag = tags->lookup( "green" );
	if(!tag) return;
	gtk_text_buffer_apply_tag( text_buffer, tag, &iter1, &iter2 );
	// NOW the second word is green

	iter1 = iter2;
	gtk_text_iter_forward_word_end( &iter2 );
	tag = tags->lookup( "blue" );
	if(!tag) return;
	gtk_text_buffer_apply_tag( text_buffer, tag, &iter1, &iter2 );
	// NOW the third word is green // still grey after that

	/* insert new text at iter1 to see what colour it becomes */

	// without this back 2 chars, it came out as the default colour
	// which is a clue to where tags are ??
	gtk_text_iter_backward_chars( & iter1, 3 );
	gtk_text_buffer_insert( text_buffer, & iter1, "PLUS", 4 );
	gtk_text_iter_backward_chars( & iter1, 2 );
	gtk_text_buffer_insert( text_buffer, & iter1, "ADDED", 5 );


}
















