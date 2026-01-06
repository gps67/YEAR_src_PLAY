
#include <stdio.h>
#include <glib.h>
#include "g_rrr.h"

#include "key_file.h"
#include "dgb.h"
#include "dgb.h"

// #include "key_holder.h"
// #include "evp_cipher.h"
#include "util_buf.h"
#include "buffer2.h"

#include "dyn_array.h"

Key_file::~Key_file()
{
}

Key_file::Key_file( G_rrr & _g_rrr )
: g_rrr( _g_rrr )
{
	kf = g_key_file_new();
	g_key_file_set_list_separator( kf, ',');
}

bool Key_file::has_key( const char * group, const char * item )
{
	if( g_key_file_has_key(
		kf,
		group,
		item,
		grrr()
	)) return true;
	INFO("FALSE: %s %s %s", file_one.get_str(), group, item );
	return false;
}

bool Key_file::get_value(
	const char * group,
	const char * item,
	str1 & value,
	const char * deflt, // = NULL // "" means OK NULL means error
	bool is_multi_line
)
{
	if( is_multi_line ) {

		gsize length = 0;
		gchar ** blk = \
		 g_key_file_get_string_list( kf, group, item, &length, grrr() );
		if( blk ) {
			buffer2 joined;
			for(uint i = 0; i < length; i++ ) {
				joined.print("%s\n", blk[i]);
			}
			value.set( (STR0) joined );
			g_strfreev(blk);
			blk = NULL;
			return true;
		}
		
	} else {
		gchar * newstr = g_key_file_get_value( kf, group, item, grrr() );
		if( newstr ) {
			value.set( newstr );
			free( newstr );
			return true;
		}
	}
	if( g_rrr.is_key_not_found() ) {
		const char * msg = g_rrr.get_message();
		if( !deflt ) {
			// NULL means error (or careful OK)
			// do not change returned value
			// do not print any error (clear error ?)
			// return false
			WARN("%s %s (%s) ", group, item, msg  );
			return false;
			return g_rrr.only_print_gerror();
		}
		// "" is a value like any other, (you dont get it by accident)
		value = deflt; // copy
	//	WARN("%s %s USING DEFAULT %s (%s) ", group, item, (STR0) value, msg  );
		WARN("%s %s USING DEFAULT %s ", group, item, (STR0) value );
		g_rrr.clear_gerror_if_any();
		return false; // even though fn worked OK ? as intended?
		return true; // even though not found ?
	}
	// system error other than key not found
	// could even throw exception

	return g_rrr.only_print_gerror();
}

bool Key_file::set_value(
	const char * group,
	const char * item,
	str1 & value,
	bool is_multi_line
)
{
	const char * val = value.get_str();
	if(!val) {
		val = "";
	}
	if( is_multi_line ) {
		dyn_array<gchar *> ary;
		buffer2 copy;
		copy.set( value );
		copy.trailing_nul(); // must have had anyway
		char * P = (char *) copy.buff;
		char * L = NULL;
		while(*P) {
			if(!L) {
				L = P;
				ary.append( L );
			}
			if(*P == '\r') { *P = 0; }
			if(*P == '\n') { *P = 0;  L = NULL;}
			P++;
		}
		
		gsize len = ary.N();
		gchar ** strlist = (gchar **) ary.blk.buff;
		g_key_file_set_string_list(kf, group, item, strlist, len );
	} else {
		g_key_file_set_value(
			kf,
			group,
			item,
			val
		);
	}
	return true; // there was no fail condition
}

bool Key_file::del_item(
	const char * group,
	const char * item
)
{
	if(!g_key_file_remove_key(
		kf,
		group,
		item,
		grrr()
	))
		return g_rrr.only_print_gerror();
	return true;
}

bool Key_file::load_from_buf( blk1 & buf )
{
	GKeyFileFlags flags = ( GKeyFileFlags) (
		G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS );
	bool ok =
	g_key_file_load_from_data( kf, (STR0) buf.buff, buf.nbytes_used, flags, grrr() );
	if(ok) {
		if(file_one) INFO("%s",(STR0) file_one);
		return true;
	}
	FAIL("g_key_file_load_from_data()");
	if(file_one) WARN("%s",(STR0) file_one);
	return g_rrr.only_print_gerror();
}

bool Key_file::load_from_file( const char * filename, bool error_if_absent )
{
	GKeyFileFlags flags = ( GKeyFileFlags) (
		G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS );
	// keep failed attempt ... ?
	file_one = filename;
	bool ok = g_key_file_load_from_file( kf, filename, flags, grrr() );
	if(ok) {
		INFO("%s",filename);
		return true;
	}
	if( error_if_absent ) {
		FAIL("g_key_file_load_from_file()");
		INFO("%s",filename);
		return g_rrr.only_print_gerror();
	} else {
		INFO("filename=%s, error_if_absent=%d", filename, error_if_absent );
		g_rrr.clear_gerror_if_any();
		return false;
		return g_rrr.only_print_gerror();
	}
}

/*!
	slightly wasteful, to copy a mem string into a mem buffer.
	Could use str1, but want to be closer to using the same malloc/free
	less bad if using evp_cipher_bases buffer 
*/
bool Key_file::save_into_buf( blk1 & buf )
{
	if(file_one) INFO("%s",(STR0) file_one);
	gsize len = 0;
	char * newstr = g_key_file_to_data( kf, & len, grrr() );
	if( ! newstr ) {
		WARN("bad news from g_key_file_to_data()");
		return g_rrr.only_print_gerror();
	}

	if(!buf.set( newstr, len )) return FAIL("buffer.set");;
	free( newstr );
	return true;
}

bool Key_file::save_into_file( const char * filename )
{
#warning dont save unenctypted by accident
	if(! filename ) {
		FAIL( "NULL filename ");
		return false;
	}

	gsize len = 0;
	char * newstr = g_key_file_to_data( kf, & len, grrr() );
	if( ! newstr ) {
		return g_rrr.only_print_gerror();
	}

	str1 text;
	text.set( newstr );
	free( newstr );

	STEP( "%s", filename );

	FILE * f = fopen( filename, "wb" );
	if(!f) {
		FAIL( "%s", filename );
		return false;
	}

	gsize len2 = fwrite( newstr, 1, len, f );
	if( len2 != len ) {
		FAIL( "%s", filename );
		fclose( f );
		return false;
	}
	fclose( f );
	return true;
}

bool Key_file::set_comment_for_item(
	const char * group,
	const char * item,
	const char * cmnt
)
{
	// glib adds # and does CRLF
	// but it looks very dense
	// so add a leading space, and some gaps
	//
	buffer2 buf;
	buf.print("\n %s\n", cmnt );
	cmnt = (STR0) buf;

	if( g_key_file_set_comment( kf, group, item, cmnt, grrr() )) return true;
	return g_rrr.only_print_gerror();
}

bool Key_file::set_comment_for_group(
	const char * group,
	const char * cmnt
)
{
	const char * L = "| ";
	const char * R = " |";
	L=" ";
	R="";

	buffer2 buf;
	buf.print("\n%s%s%s\n%s%s%s  %s\n%s%s%s\n",
	 L, group, R,
	 L, group, R, cmnt,
	 L, group, R
	);
	cmnt = (STR0) buf;

	const char * item = NULL;
	if(!g_key_file_has_group( kf, group )) {
		str1 empty;
		const char * fakeitem = "FAKE";
		set_value( group, fakeitem, empty );
		del_item( group, fakeitem );
	}
	if(!g_key_file_set_comment( kf, group, item, "", grrr() )) return false;
	if(!g_key_file_set_comment( kf, group, item, "", grrr() )) return false;
	if( g_key_file_set_comment( kf, group, item, cmnt, grrr() )) return true;
	if(!g_key_file_set_comment( kf, group, item, "", grrr() )) return false;
	return g_rrr.only_print_gerror();
}

bool Key_file::set_comment_for_file(
	const char * cmnt
)
{
	buffer2 buf;
	buf.print("\n\n %s\n", cmnt );
	cmnt = (STR0) buf;

	const char * group = NULL;
	const char * item = NULL;
	if( g_key_file_set_comment( kf, group, item, cmnt, grrr() )) return true;
	return g_rrr.only_print_gerror();
}


bool Key_file::bind(
	bool write,
	const char * group,
	const char * item,
	str1 & value,
	const char * cmnt_if_absent,
	bool is_multi_line
)
{
	bool ok = true;
	if( ! has_key( group, item ) ) {
		if(!set_value( group, item, value, is_multi_line )) return false;
		if( cmnt_if_absent )
			ok = set_comment_for_item( group, item, cmnt_if_absent );
		// have just written value so return (even for read!)
		// semanitic is that value is not changed - AND is added
		// so always initialise value before binding it
		return ok;
	}
	if(write) {
		if(!set_value( group, item, value, is_multi_line )) ok = false;
	} else {
		if(!get_value( group, item, value, NULL, is_multi_line )) ok = false;
	}
	return ok;
}
