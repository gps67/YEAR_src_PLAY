#include "w_window.h"
#include "pix_holder.h"
#include "dgb_fail.h"
#include "buffer2.h"
#include <ctype.h> // toupper

bool w_window::set_icon_from_file( const char * filename )
{
	INFO("loading %s", filename );

	if(!gtk_window_set_icon_from_file(
		Window(),
		filename,
		G_rrr::get_global_GError()
	)) {
		return FAIL("gtk says no");
		// INFO("... %s", __BASE_FILE__ );
		INFO("filename = %s", filename );
	}

	return true;
}

/*!
	convert title to UPPER and set_title - ASCII / 8859 only
*/
void w_window::set_title_upper( str0 title )
{
	buffer2 buf;
	const char * s = title;
	while( char c = * s++ ) {
		c = toupper(c);
		buf.put( c );
	}
	set_title( buf );
}
