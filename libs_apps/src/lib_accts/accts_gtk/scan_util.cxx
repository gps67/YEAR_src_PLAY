
#include <stdio.h>
#include <errno.h>

#include <string.h>
#include <ctype.h>
#include <stdlib.h> // atoi

#include "scan_util.h"
#include "file_alloc.h"

namespace ACCTS {

	bool Scan_Util::run_str( const char * name, const char * cmd )
	{
		printf( "%s \n", cmd );
		printf( "%s ...", name );
		fflush(0);
		if( run_str_quiet( cmd ) )
		{
			printf(" OK\n");
			return true;
		} else {
			perror( name );
			return false;
		}
	}

	bool Scan_Util::run_str_quiet( const char * cmd )
	{
#if 1
		int t = system( cmd );
#else
		int t = 0;
#endif
		if( t ) 
		{
			errno = t;
			return false;
		}
		return true;
	}

	bool Scan_Util::scan_into_file( const char * filebase, const char * comment )
	{
		// THIS IS THE DEFAULT FUNCTION - 2008-12-31 - or so
		// DAMN it isnt - reduce_convert_display - is the default
		printf("scan_into_file( %s, '%s' )\n", filebase, comment );	
		// return true;
		if(! scanimage_tiff( filebase ) ) return false;
#if 1
		if(! reduce_convert_display( filebase, comment ) ) return false;
#else
		// this is never run !
		if(! reduce_colours_add_comment_tiff( filebase, comment ) ) return false;
		if(! convert_compress_pnm_png( filebase ) ) return false;
		display_png( filebase );
		// this is never run !
#endif
		return true;
	}

	bool Scan_Util::scan_into_jpeg( const char * filebase, const char * comment )
	{
		// copied from scan_into_file 
		// but jpeg instead of png
		printf("scan_into_jpeg( %s, '%s' )\n", filebase, comment );	
		// return true;
		if(! scanimage_tiff( filebase ) ) return false;
		if(! reduce_convert_display_ext( filebase, comment, ".jpeg" ) ) return false;
		// GONE // if(! reduce_colours_add_comment_tiff( filebase, comment ) ) return false;
		return true;
	}

	bool Scan_Util::scanimage_tiff( const char * filebase )
	{

		char buf[380];
	
		sprintf( buf,
			"scanimage"
			" -d artec_eplus48u:"
			" --mode Color"
			" --depth 16"
			" --resolution 100"
			" -x 216 -y 297"
			" --format=tiff" 
			" > %s.tiff",
			filebase
		);
		if( !run_str( "scanimage", buf )) return false;
		list_ext( filebase, ".tiff" );
		return true;
	}

	bool Scan_Util::reduce_convert_display( const char * filebase, const char * comment )
	{
		return reduce_convert_display_ext( filebase, comment, ".png" );
	}
	bool Scan_Util::reduce_convert_display_ext(
		const char * filebase,
		const char * comment,
		const char * dot_ext
	)
	{
		char buf[1380];
		sprintf( buf,
		"(\n"

			"gm"
			" convert"
			" -sharpen 4"
			" -colors 14"
			" -depth 8"
			" -resize 850x1169"
			" -comment"
			" \"%s\""
			" %s.tiff"
			" %s%s"
		" && "
			"rm -f"
			" %s.tiff"

		" && "

			"gm display"
			" %s%s"

		"\n ) &",
	//
			comment,	// convert - comment
			filebase,	// convert from %s.tiff

			filebase,	// convert into %s%s
			dot_ext,	// ".png",

			filebase,	// rm %s.tiff

			filebase,	// gm display %s%s
			dot_ext		// ".png"

		);
		if(!run_str( "reduce_convert_display", buf )) return false;
		return true;
	}

	bool Scan_Util::reduce_colours_add_comment_tiff( const char * filebase, const char * comment )
	{
		char buf[380];
		sprintf( buf,
			"mogrify"
			" -sharpen 4"
			" -quality 0"
			" -colors 14"
			" -depth 8"
			" -comment"
			" \"%s\""
			" %s.tiff",
			comment,
			filebase
		);
		if(!run_str( "mogrify", buf )) return false;
		list_ext( filebase, ".tiff" );
		return true;
	}

	bool Scan_Util::convert_compress_tiff_png( const char * filebase )
	{
		char buf[380];
		sprintf( buf,
			"gm convert"
			" -quality 100"
			" %s.tiff"
			" %s.png",
			filebase,
			filebase
		);
		if(!run_str( "convert", buf )) return false;
		remove_tiff( filebase );
		list_ext( filebase, ".png" );
		return true;
	}

	bool Scan_Util::convert_compress_tiff_jpeg( const char * filebase )
	{
		char buf[380];
		sprintf( buf,
			"gm convert"
			" -quality 100"
			" %s.tiff"
			" %s.jpeg",
			filebase,
			filebase
		);
		if(!run_str( "convert", buf )) return false;
		remove_tiff( filebase );
		list_ext( filebase, ".jpeg" );
		return true;
	}

	bool Scan_Util::remove_tiff( const char * filebase )
	{
		char buf[380];
		sprintf( buf,
			"rm -f"
			" %s.tiff",
			filebase
		);
		if(!run_str( "rm", buf )) return false;
		return true;
	}

	bool Scan_Util::display( const char * filebase, const char * dotext )
	{
		char buf[380];
		sprintf( buf,
			"display"
			" %s%s"
			" &",
			filebase,
			dotext
		);
		if(!run_str( "display", buf )) return false;
		return true;
	}

	bool Scan_Util::display_png( const char * filebase )
	{
		return display( filebase, ".png" );
	}

	bool Scan_Util::display_jpeg( const char * filebase )
	{
		return display( filebase, ".jpeg" );
	}

	bool Scan_Util::list_ext( const char * filebase, const char * dotext )
	{
		char buf[380];
		sprintf( buf,
			"ls -l"
			" %s%s",
			filebase,
			dotext
		);
		// if(!run_str( "ls", buf )) return false;
		if(!run_str_quiet( buf )) return false;
		return true;
	}

}; // NAMESPACE

