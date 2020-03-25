/*
	tcl_syslog.c
	report an error via the syslog mechanism
	man syslogd
	man syslog

	The syslog stuff is in LIB1, this calls that.
	This shows you how easy it is to write a Tcl module.

	Note the use of CreateCommand, not CreateObjCommand, which
	Tcl provides with strings, not objects.
*/

#include <tcl.h> /* TCL */

/*
	the "main" function that does the work - but not TCL aware
*/

extern "C" {
	int syslog_main( int, const char **);
}

/*
	the TCL function that calls the non-tcl program (linked in).
	In (my) common usage, stderr will be /dev/null, but if it fails
	repeatably, you might find the error (saves variable length
	string manipulation and buffer overflows)
*/

static int tcl_syslog (
        ClientData clientData,
        Tcl_Interp *interp,
        int argc,
        const char **argv
){
	if( syslog_main( argc, argv )) {
		Tcl_AppendResult( interp,
			argv[0],
			": usage error see stderr",
			(char *) NULL);
		return TCL_ERROR;
	}
	return TCL_OK;
}

/*
	The module _Init function that declares the callable TCL proc
	(nb as char * not as Tcl_Object *)

	You could also add the pkg_provide bits, but 'load' does
	a similar effect itself. The namespace is auto_created.
*/

extern "C"
int Syslog_Init( Tcl_Interp *interp )
{
   Tcl_CreateCommand(
      interp,
      "syslog::syslog",
      tcl_syslog,
      (ClientData) 0,
      (Tcl_CmdDeleteProc *) NULL
    );
    return TCL_OK;
}
