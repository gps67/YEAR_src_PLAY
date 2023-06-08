#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
	License: GPL www.gnu.org
	Author: Graham Swallow
	Date: March 2005
	Date: July 2023 # still keeping away from libs_apps # indie #


	PATH_TOOL is useful to add dirs to PATH or LD_LIBRARY_PATH

	PATH_TOOL pfx varname d1 [d2 ... ]
	PATH_TOOL add varname d1 [d2 ... ]
	PATH_TOOL del varname d1 [d2 ... ]

	if VAL=`PATH_TOOL $1 $2`
	then
		eval "$1=\"$VAL\""
	else
		: ...
	fi

	-dupes: allows duplicates, needed to build command lines
	-sep_space: use SP instead of ':', needed to build command lines
	-for_eval: outputs VAR='newval'
	-drop_no_dir: warn but remove absent dirs

	eval `PATH_TOOL -for_eval pfx "$@"`                                     

	
*/

const char * progname;

/*!
	print an error message and exit with 'Invalid argument'
*/
void usage( const char * msg )
{
	fprintf(stderr, "# %s USAGE ERROR %s TRY:\n", progname, msg );
	fprintf(stderr, "# PATH_TOOL [-for_eval] pfx|del|add varname [d1 d2 ... ]\n");
	fprintf(stderr, "# PATH_TOOL pfx varname [d1 d2 ... ]\n");
	fprintf(stderr, "# PATH_TOOL add varname [d1 d2 ... ]\n");
	fprintf(stderr, "# PATH_TOOL del varname [d1 d2 ... ]\n");
	fprintf(stderr, "# PATH_TOOL list varname\n");
	fprintf(stderr, "# PATH_TOOL FLAG -for_eval # outputs VAR=newval\n");
	fprintf(stderr, "# PATH_TOOL FLAG -sep_space # use space as separator \n");
	fprintf(stderr, "# PATH_TOOL FLAG -drop_no_dir # remove absent dirs\n");
	fprintf(stderr, "# PATH_TOOL FLAG -dupes # do not eliminate duplicates \n");
	exit(22);
}

/*!
	dir_list holds a list of unique strings which it copies and owns.
*/
struct dir_list
{
	int len;
	int len_alloc;
	char ** list;
	bool dupes;
	bool drop_no_dir;

	/*!
		constructor - init NULL
	*/
	dir_list()
	{
		len = 0;
		len_alloc = 0;
		list = NULL;
		dupes = false; // no duplicate dirs 
		drop_no_dir = false; // keep no_dir without checking
	}

	/*!
		destructor - free each string and the holding array
	*/
	~dir_list()
	{
		while( len > 0 )
		{
			len--;
			char * P = list[len];
			if(P) free( P );
			list[len] = NULL;
		}
		if( list ) free( list );
		list = NULL;
		len_alloc = 0;
	}

	/*!
		ensure that array has AT LEAST n slots
	*/
	void extend_to( int new_len_alloc )
	{
		if( new_len_alloc <= len_alloc ) return;
		new_len_alloc += 40;
		char ** list_old = list;
		list = (char ** ) malloc( sizeof(char*) * new_len_alloc );
		if(!list) throw "ENOMEM";
		for( int i = 0; i<len; i++ )
		{
			list[i] = list_old[i];
		}
		for( int i = len; i < new_len_alloc; i ++ )
		{
			list[i] = NULL;
		}
		if( list_old ) free( list_old );
		len_alloc = new_len_alloc;
	}

	bool expect_is_a_dir( const char * dir ) {
		struct stat st;
		if(-1==::stat( dir, &st )) {
			if( errno == ENOENT ) {
			  fprintf(stderr,"# WARN # expected DIR got ENOENT # '%s' \n", dir );
			} else {
			  fprintf(stderr,"# WARN # stat DIR got ERRNO %d '%s' \n", errno, dir ); 
			}
			return false;
		}
		mode_t m = st.st_mode;
		if(S_ISDIR(m)) return true;

		 fprintf(stderr,"# WARN # stat S_IS_DIR says NO # TODO follow links  # '%s' \n", dir ); 
		return false;
	}

	/*!
		add dir to end of list, or leave where found
	*/
	void add_dir( char * dir )
	{
		// NB called by init_list_from_aswas
		// sweep to find it as was
		if( !dupes )
			for( int i=0; i< len; i++ )
			{
				if( 0== strcmp( list[i], dir ) )
					return; // do not add_dir // 
			}
		if( drop_no_dir ) {
			if( expect_is_a_dir( dir )) {
				// is as it should be
				// stay
			} else {
				// already reported
				 fprintf(stderr,"# WARN # DROP non DIR # %s # \n", dir ); 
				// DROP so dropped from the list
				return;
			}
		}
		extend_to( len+1 );
		list[ len++ ] = strdup( dir );
	}

	/*!
		find dir in list and delete it (the string not rmdir!)
		OK if absent
	*/
	void del_dir( char * dir )
	{
		for( int i=0; i<len; i++ )
		{
			if( 0== strcmp( list[i], dir ) )
			{
				free( list[i] );
				list[i] = NULL;
				len--;
				while( i < len )
				{
					list[i] = list[i+1];
					i++;
				}
				return;
			}
		}
	}

	/*!
		call add_dir( list_old[each] )
	*/
	void add_list( dir_list & list_old )
	{
		for( int i = 0; i< list_old.len; i++ )
		{
			add_dir( list_old.list[i] );
		}
	}

	/*!
		call add_dir( each_argv[i] )
	*/
	void add_list( int argc, char ** argv )
	{
		for( int i = 0; i< argc; i++ )
		{
			add_dir( argv[i] );
		}
	}

	/*!
		call del_dir( each_argv[i] )
	*/
	void del_list( int argc, char ** argv )
	{
		for( int i = 0; i< argc; i++ )
		{
			del_dir( argv[i] );
		}
	}

	/*!
		split var_val_buffer by sep, and add_dir( each )
	*/
	void add_list_buffer( char sep, char * var_val_buffer )
	{
		if(! var_val_buffer ) return;
		/*
			duplicate buffer to allow overwrite with NUL
		*/
		var_val_buffer = strdup( var_val_buffer );
		const char NUL = 0;
		/*
			build list_old from strings held in var_val_buffer
			overwrite buffer with NUL bytes over sep
			(not ideal but OK)
		*/
		char * p = var_val_buffer;
		while( p && *p )
		{
			char * dir = p;
			p = strchr( p, sep );
			if(p) {
				// truncate dir in word - overwrite buffer
				*p = NUL;
				p++;
			}
			add_dir( dir );
		}
		free( var_val_buffer );
	}

	/*!
		print the list of strings using separator
	*/
	void print( const char * sep_str_val, FILE * std_out )
	{
		const char * sep_str = "";
		for( int i=0; i < len; i++ )
		{
			fprintf(std_out, "%s%s", sep_str, list[i] );
			sep_str = sep_str_val;
		}
	}

};

#define XXX 0
#define PFX 1
#define ADD 2
#define DEL 3
#define LIST 4

int main( int argc, char ** argv )
{
	// parse argv, reducing argc
	progname = argv[0];
	argc--;
	argv++;

	// the PATH that we are building
	dir_list PATH_list;

	// set defaults before flags
	bool for_eval = false;
	char sep = ':';
	const char * sep_str_val = ":";

	// parse any flags
	while( argc && argv[0][0]=='-' )
	{
		if(0==strcmp( argv[0], "-for_eval" ))
		{
			for_eval = true;
		} else if(0==strcmp( argv[0], "-sep_space" ))
		{
			sep = ' ';
			sep_str_val = " ";
		} else if(0==strcmp( argv[0], "-drop_no_dir"))
	
		{
			PATH_list.drop_no_dir = true; // args arent dirs
		} else if(0==strcmp( argv[0], "-dupes" ))
		{
			PATH_list.dupes = true; // args arent dirs
		} else
			usage("bad switch");
		argc--;
		argv++;
	}

	if(!argc) usage( "missing opcode (del,pfx,...)" );
	char * opcodestr = argv[0];
	argc -= 1;
	argv += 1;

	int opcode = XXX;
	if(0==strcmp(opcodestr,"pfx")) opcode = PFX; else
	if(0==strcmp(opcodestr,"add")) opcode = ADD; else
	if(0==strcmp(opcodestr,"del")) opcode = DEL; else
	if(0==strcmp(opcodestr,"list")) opcode = LIST; else
	{
		fprintf(stderr,"# opcode == '%s' # %s \n", opcodestr, progname );
		usage( opcodestr );
	 // usage( "bad opcode" );
	}

	if(!argc) usage( "missing VARNAME" );
	char * varname = argv[0];
	argc -= 1;
	argv += 1;

	// now argc,argv == dirs to add

	// its OK for the var to not exist
	char * var_val_buffer = getenv( varname );

	switch(opcode) {
	 case PFX:
		PATH_list.add_list( argc, argv );
		PATH_list.add_list_buffer( sep, var_val_buffer );
	 break;
	 case ADD:
		PATH_list.add_list_buffer( sep, var_val_buffer );
		PATH_list.add_list( argc, argv );
	 break;
	 case DEL:
		PATH_list.add_list_buffer( sep, var_val_buffer );
		PATH_list.del_list( argc, argv );
	 break;
	 case LIST:
		fprintf(stdout,"\t");
		PATH_list.add_list_buffer( sep, var_val_buffer );
		sep_str_val = "\n\t";
	 break;
	}

	/*
		print new PATH
	*/
	if( for_eval )
	{
		/*
			most shells allow export V=val on the same line
			The export is only relevent when the var is new,
			otherwise how did this process get the old value !
			eval `PATH_TOOL -for_eval pfx "$@"`                                     
		*/
		fprintf( stdout, "export %s='", varname );
		PATH_list.print( sep_str_val, stdout );
		fprintf( stdout, "'" );
	} else {
		PATH_list.print( sep_str_val, stdout );
	}

	fprintf(stdout, "\n" ); // sh ignores trailing newline in backquotes
	fflush(0);
	return 0;
}

