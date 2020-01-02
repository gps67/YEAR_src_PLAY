
#include "X_STUBS.h"
#include "X_ret_err.h"
#include "buffer1.h"

#include <X11/X.h> // to get BadFont recognised
// #include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <X11/Xos.h>
// #include <X11/Xatom.h>
// #include <X11/keysym.h>

namespace WAX {

#define ERR( name, desc ) case name : s = #name " " desc ; break;

const char * name_of_X_err( buffer1 & temp, int err  )
{
	int n = temp.nbytes_used;
	const char * s = "OK";
	switch( err ) {
	 // case BadAlloc: s = "BadAlloc"; break;
	 // case BadFont:  s = "BadFont";  break;
	 // case BadGC:    s = "BadGC";    break;
ERR( Success,    "/* everything's okay */")

ERR( BadRequest, "/* bad request code */" )
ERR( BadValue,   "/* int parameter out of range */" )
ERR( BadWindow,  "/* parameter not a Window */" )
ERR( BadPixmap,  "/* parameter not a Pixmap */" )
ERR( BadAtom,    "/* parameter not an Atom */" )
ERR( BadCursor,  "/* parameter not a Cursor */" )
ERR( BadFont,    "/* parameter not a Font */" )
ERR( BadMatch,   "/* parameter mismatch */" )
ERR( BadDrawable,"/* parameter not a Pixmap or Window */" )
ERR( BadAccess,  "/* depending on context: see X.h */" )

ERR( BadAlloc,   "/* insufficient resources */" )
ERR( BadColor,   "/* no such colormap */" )
ERR( BadGC,      "/* parameter not a GC */" )
ERR( BadIDChoice,"/* choice not in range or already used */" )
ERR( BadName,    "/* font or color name doesn't exist */" )
ERR( BadLength,  "/* Request length incorrect */" )
ERR( BadImplementation, "/* server is defective */" )

// ERR( BadAccess,  "/* depending on context: see X.h */" )
				 /*
                                 - key/button already grabbed
                                 - attempt to free an illegal
                                   cmap entry
                                - attempt to store into a read-only
                                   color map entry.
                                - attempt to modify the access control
                                   list from other than the local host.
                                */

	 default:
	 	s = "DEFAULT_ERROR";
		INFO("%d %s", BadAlloc, "BadAlloc" ); // 11
		INFO("%d %s", BadFont,  "BadFont" ); // 7
		INFO("%d %s", BadGC,    "BadGC" ); // 13
	}
	temp.printf( "%s", s );
	return (STR0) temp;
}

const char * name_of_X_err( int err  )
{
	static buffer1 temp;
	temp.clear();
	return name_of_X_err( temp, err  );
}

}; // namespace
