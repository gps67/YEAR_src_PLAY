#ifdef WIN32 

// THIS SNIPPET (C) GPS - freely re-usable

#include <windows.h>
#include "word_splitter.h"
extern int main( int, char ** );

#define MAXPATHLEN 300

int WINAPI
WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
)
{
	word_splitter splitter;
        TCHAR program[MAXPATHLEN];


	// got this WIN32 command from a jc gtk-list suggestion
	// havent tried it yet (blind coding)
        GetModuleFileName( hInstance, program, sizeof(program) );

	splitter.split_by_spaces_and_quotes( lpCmdLine );
        int argc = splitter.word_list.N()+1; // [0] is program
	char * argv[ argc + 1 + 2 ]; // trailing NULL plus XS of 2
	argv[0] = (char *) program;
	for(int i=1; i<argc; i++ )
	{
		// should be using STR0(s) not (STR0)s
		argv[i] = (char *) STR0( splitter.word_list[i-1] );
		// argv[i] = (STR0) splitter.word_list[i-1];
	}
        argv[ argc ] = NULL;

        return( main( argc, argv ) );
}
#endif /* WIN32 */
