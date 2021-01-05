
#include "g_text_tag_table.h"
#include "g_text_buffer.h"
#include "w_text_view.h"

#include <unistd.h>
#include "e_print.h"

class w_text_view;


g_text_buffer::~g_text_buffer()
{
	gtk_text_buffer_delete_mark( buffer, mark_endpos );
	g_object_unref( mark_endpos );
	g_object_unref( buffer );
	mark_endpos = NULL;
}

/*!
	create a new g_text_table with a NEW shared tags table
*/
g_text_buffer::g_text_buffer()
{
	tags = new g_text_tag_table();
	buffer = gtk_text_buffer_new( tags ->TAG_TABLE() );
	init_mark_endpos();
};

/*!
	create a new g_text_table from a SHARED tags table
*/
g_text_buffer::g_text_buffer( g_text_tag_table * shared_tags )
{
	if(!shared_tags)
		shared_tags = new g_text_tag_table();
	tags = shared_tags;
	buffer = gtk_text_buffer_new( tags ->TAG_TABLE() );
	init_mark_endpos();
};

/*!
	create a wrapper for an existing buffer

	// write this when its needed - which isnt ever
	// at lest not until importing from someone-elses library

	g_text_buffer::g_text_buffer( GtkTextBuffer * _buffer )
	{
		assert( _buffer );
		buffer = _buffer();
		// wrap the existing tags table
		tags = new g_text_tag_table( gtk_text_buffer_get_tag_table( _buffer ));
		buffer = _buffer;
		g_object_ref( buffer );
	}
*/




void g_text_buffer::init_mark_endpos()
{
	const gchar *mark_name = "endpos";
	gboolean left_gravity = false; // mark stays on right

	gtk_text_buffer_get_end_iter(buffer, & iter );

	mark_endpos = gtk_text_buffer_create_mark(
		buffer,
		mark_name,
		&iter,
		left_gravity
	);
}

/*
*/
void g_text_buffer::clear()
{
	gtk_text_buffer_set_text (buffer, "", -1);
}

void g_text_buffer::append_8859( str0 str, int len )
{

	static buffer1 converter;
	if( len < 0 ) len = str.str_len();
	converter.put_nn_bytes_8859_to_utf8( len, str );
	append_utf8( converter );
	converter.clear();
}

void g_text_buffer::append_8859( const buffer1 & buff )
{
	int len = buff.nbytes_used;
	str0 txt = (const char *) buff.buff;
	append_8859( txt, len );
}

void g_text_buffer::append_utf8( const buffer1 & buff )
{
	append_utf8(
		(const char *) buff.buff,
		buff.nbytes_used
	);
}

void g_text_buffer::append_utf8( str0 str, int len )
{
	// ie already utf so unmodified
	gtk_text_buffer_get_iter_at_mark( buffer, &iter, mark_endpos );
	gtk_text_buffer_insert ( buffer, &iter, (STR0) str, len );
}

bool g_text_buffer::append_file_8859( str0 filename )
{
	if(!filename) return false;

	int fd = open( filename, O_RDONLY );
	if(fd < 0 ) {
		perror(filename);
		e_print("open %s - %m\n", (STR0) filename );
		return false;
	}
	int len = 4096;
	char b[len];
	while( len > 0 )
	{
		len = read( fd, b, len );
		append_8859( b, len );
	}
	close( fd );
	return true;
}

//////////////////////////////////

w_text_view * g_text_buffer::mk_view()
{
	return new w_text_view( this );
}

w_win_scrolled * g_text_buffer::mk_view_scrolled()
{
	w_win_scrolled * view_A = new w_win_scrolled(
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC
	);
	view_A->pack( mk_view() );
	return view_A;
}

