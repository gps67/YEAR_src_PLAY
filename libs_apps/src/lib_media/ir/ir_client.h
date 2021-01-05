#ifndef ir_client_H
#define ir_client_H

#include "fd_sel_line.h"
#include "str_builder.h"
#include "str1.h"
#include "obj_list.h"
#include "dyn_array.h"
#include "word_splitter.h"

/*!
	IR_client can send commands to lircd (www.lirc.org) and
	parse the response codes aynchronously.
*/
class IR_client : public fd_sel_line
{
	//! The response parser state is whats next ....
	enum t_waiting_for
	{
		W_BEGIN,	//!<	idle state
		W_COMMAND,	//!<	the EXACT original request string
		W_STATUS,	//!<	SUCCESS or FAIL
		W_DATA,		//!< W_DATA_OR_END
		W_N,		//!<	line count of data lines
		W_DATA_LINE,	//!<	error message || dataline (pref not KEYWORD)
		W_END,		//!<	Matches BEGIN
		W_RECOVERY	//!<	parser is in fail/recovery looking for END/BEGIN
	};

	//! The response data is ...
	enum t_msg_type
	{
		M_NONE,
		M_LIST_REMOTES,
		M_LIST_BUTTONS,
		M_LIST_REMOTE_BUTTON,
		M_SEND_ONCE,
		M_SEND_START,
		M_SEND_STOP,
		M_RECEIVED,
		M_SIGHUP
	};

	str1 dev_lircd;
	word_splitter resp_cmd;
	str_builder resp_cmd_line;
	str_builder resp_data;
	obj_list<str2>	word_list;
	int N_DATA_expected;
	int N_DATA_received;
	bool status_ok;
	buffer2 buf1;

 public:

	//! The parser state is waiting_for ... W_BEGIN
	t_waiting_for waiting_for;
	t_msg_type msg_type;
	
	IR_client( str0 DEV_LIRCD );
	bool check_connected();
	bool reconnect();
	void pre_BEGIN();
	bool req_SEND_ONCE( str0 rem, str0 btn );
	bool req_SEND_START( str0 rem, str0 btn );
	bool req_SEND_STOP( str0 rem, str0 btn );
	bool req_LIST_remotes(); // and tell who ??
	bool req_LIST_buttons( str0 rem ); // and tell who ??
	void test1();
	void goto_recovery_mode( str0 line, str0 error );
	void got_line( str0 line, int len );
	void got_eof();
	void got_data(void*, int);
	void got_error();

	virtual void got_input_button(
		str0 hex,
		str0 count,
		str0 button,
		str0 remote
	);

	void send_commands( str0 remote, str0 commands );

	virtual void got_success();
	virtual void got_bad_something();
	virtual void got_bad_directive();
	virtual void got_bad_remote();
	virtual void got_bad_button();
	virtual void got_bad_other();
	virtual void got_sighup();
	virtual void got_remotes();
	virtual void got_buttons();
	virtual void got_remote_button();
	virtual void got_recovery();
	virtual void got_received();

} ;
#endif
