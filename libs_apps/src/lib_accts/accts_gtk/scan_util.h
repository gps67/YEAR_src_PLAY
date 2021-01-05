#ifndef scan_util_H
#define scan_util_H

#include "ints.h"

namespace ACCTS {

class Scan_Util : public GRP_lib_accts
{
 public:
	bool run_str( const char * name, const char * cmd );
	bool run_str_quiet( const char * cmd );
	bool scan_into_file( const char * filebase, const char * comment );
	bool scan_into_jpeg( const char * filebase, const char * comment );
	bool scanimage_tiff( const char * filebase );
	bool reduce_convert_display( const char * filebase, const char * comment );
	bool reduce_convert_display_ext( const char * filebase, const char * comment, const char * ext ); // ext is not .tiff OK
	bool reduce_colours_add_comment_tiff( const char * filebase, const char * comment );
	bool convert_compress_tiff_png( const char * filebase );
	bool convert_compress_tiff_jpeg( const char * filebase );
	bool remove_tiff( const char * filebase );
	bool display( const char * filebase, const char * dotext );
	bool display_png( const char * filebase );
	bool display_jpeg( const char * filebase );
	bool list_ext( const char * filebase, const char * dotext );
};

}; // NAMESPACE

#endif
