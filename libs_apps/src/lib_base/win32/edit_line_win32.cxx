
#include "edit_line.h"
// #include "MY_WIN32.h"
#include <windows.h>

// #include <histedit.h>
#include "dgb_fail.h"

using namespace EL;

// #############################################################
// #############################################################
//
// openvpn ssuggested these modes, seem good (with line editing)
// TODO move to a base class
//
//
bool
get_console_input_win32(
	const char *prompt,
	const bool echo,
	char *input,
	const int capacity
)
{
	//
	// OPPORTUNITY TO PROCESS SOME OTHER EVENT (stopping for keyboard)
	//

	// guarantee
	input[0] = '\0';

	HANDLE in = INVALID_HANDLE_VALUE;
	HANDLE err = INVALID_HANDLE_VALUE;
	DWORD len = 0;

	DWORD flags_save = 0;
	DWORD flags_mode = 0;
	flags_mode |= ENABLE_LINE_INPUT;
	flags_mode |= ENABLE_PROCESSED_INPUT;
	if (echo)
		flags_mode |= ENABLE_ECHO_INPUT;

	// ASSERT (prompt);
	// ASSERT (input);
	// ASSERT (capacity > 0);


	in = GetStdHandle (STD_INPUT_HANDLE);
	err= GetStdHandle (STD_ERROR_HANDLE);
	// err = get_orig_stderr ();

	if(in  == INVALID_HANDLE_VALUE ) return false;
	if(err == INVALID_HANDLE_VALUE ) return false;
	bool is_console = (GetFileType (in) == FILE_TYPE_CHAR);
	if (GetConsoleMode (in, &flags_save))
	{
		// COMPLAIN
		is_console = false;
	}

	if(!WriteFile (err, prompt, strlen(prompt), &len, NULL))
	{
		// lookes like an error to me
		// COMPLAIN
		// return false ; // ??
	}

	int status = 0;

	// switch some ConsoleModeFlags
	//
	if (is_console)
	{
		if (!GetConsoleMode (in, &flags_save))
		{
			is_console = 0;
			// COMPLAIN above should have detected it
			// FOLLOWING will fail similarly
		}
		SetConsoleMode (in, flags_mode);
	}

	status = ReadFile (in, input, capacity, &len, NULL);


	if (!echo)
		WriteFile (err, "\r\n", 2, &len, NULL);
	if (is_console)
		SetConsoleMode (in, flags_save);

	//      string_null_terminate (input, (int)len, capacity);
	//      chomp (input);
	//      if (status && !win32_service_interrupt (&win32_signal))
	//        return true;
	return true;
}


// #############################################################
// #############################################################



edit_line_win32::
edit_line_win32( const char * progname )
{
}

edit_line_win32::
~edit_line_win32()
{
}

/*!
 * 	gets a line
 */
bool
edit_line_win32::
gets_PS1() // into buff
{
	PromptString = PS1;
	buff.clear();

	const char *prompt = get_prompt();
	const bool echo = true;
	buff.get_space( 200 );
	char input[ 200];
	const int capacity = sizeof( input );
	if( !get_console_input_win32 (prompt, echo, input, capacity) )
	{
		/// 
		return false;
	}
	buff.put( input );
	buff.trim_trailing_gaps();
	return true;
}

/*!
 * 	gets a continuation line
 */
bool
edit_line_win32::
gets_PS2_append() // onto buff
{
	// do not clear buff
	// amybe add a space for the EOLN
	buff.get_space( 200 );
	PromptString = PS2;
	const char *prompt = get_prompt();
	const bool echo = true;
	buff.get_space( 200 );
	char input[ 200];
	const int capacity = sizeof( input );
	if( !get_console_input_win32 (prompt, echo, input, capacity) )
	{
		/// 
		return false;
	}
	buff.put( " " );
	buff.put( input );
	buff.trim_trailing_gaps();
	return true;
}



/*!
 * 	gets the prompt string - internal
 */
const char *
edit_line_base::
get_prompt()
{
	return (STR0) * PromptString;
}
