
#include "dir_list.h"

#include <stdlib.h>	// free
#include <stdio.h>
#include <string.h>


	/*!
	*/
	void dir_reader_base::set_dir_name( const char * name )
	{
		if(dir_name) free(dir_name);
		dir_name = NULL;
		dir_name_len = 0;
		if(name) {
			dir_name = strdup( name );
			dir_name_len = strlen( dir_name );
		}
	}

	/*!
	*/
	dir_reader_base::dir_reader_base()
	: item()
	{
		skip_dotdot = true;
		skip_dot_all = false;
		clear();
	}

	/*!
	*/
	void dir_reader_base::clear()
	{
		dir = NULL;
		dp = NULL;
		dir_name = NULL;
		dir_name_len = 0;
		item.clear();
	}

	/*!
	*/
	dir_reader_base::~dir_reader_base()
	{
		close();
	}

	/*!
	*/
	bool dir_reader_base::open( const char * path )
	{
		close();
		set_dir_name( path );
		dir = opendir( path );
		if(!dir)
		{
			perror( path );
			return false;
		}
		return true;
	}

	/*!
	*/
	void dir_reader_base::close()
	{
		if( dir )
		{
			closedir( dir );
			dir = NULL;
		}
		if( dp )
		{
			dp = NULL;
		}
		set_dir_name(NULL);
		clear();
	}

	/*!
		advance the dir reader to the next name (maybe skip dots)

		do not stat the name
	*/
	bool dir_reader_base::next_name()
	{
		if(!dir) return false;
		dp = readdir( dir );
		if(!dp) return false;
		if( skip_dot_all )
		{
			if( *name() == '.' ) return next_name();
		}
		if( skip_dotdot )
		{
			if( 0==strcmp( name(), "." ) ) return next_name();
			if( 0==strcmp( name(), ".." ) ) return next_name();
		}
		return true;
	}

	/*!
		advance to and stat the next item in the dir
	*/
	bool dir_reader_base::next()
	{
		if(!next_name()) return false;

		/*
			compute the full relative/absolute pathname
		*/
		int l = strlen( dp->d_name ) + dir_name_len + 1 + 10;
		char filename[ l ];
		sprintf( filename, "%s/%s", dir_name, dp->d_name );
		/*
			The stat can fail when a name is found but the
			file has just been deleted. Skip it.
		*/
		if(! item.stat( filename )) return next();

		return true;
	}

	/*!
	*/
	const char * dir_reader_base::name()
	{
		return dp->d_name;
	}

