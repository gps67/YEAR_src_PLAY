#ifndef audacity_mod_script_pipe_H
#define audacity_mod_script_pipe_H

#include "str1.h"
#include "fd_hold.h"

/*
	audacity provides a API == "mod-script-pipe"

	client opens two PIPES READ and WRITE
	server names those files the same

	client sends ENQ and expects ACK
	client receives ACK // SEGMENT CHOPPED TEXT EOLN used
	limit ENQ to 1.5K X 1024; // entire file streams // need 2K not 2M
	quota monitor resource used { int nbytes } { nbytes < 100 * 1024 }
	// ON DETECT { nbytes < K8 } // WARN only VECTOR CALLED limit 2G i48
	// ON DETECT { nbytes < 20 } // WARN long word DETECT MATCH set_FGBG
	// ON DETECT { nbytes < u24_MAX_INT } // WARN still using i64_MAX_INT
	// client sends 100 bytes gets back 3K //
*/


class fd_PIPE_base : public fd_hold_1 {
 public:
};

//	audacity_mod_script_pipe.h

class audacity_mod_script_pipe {
 protected:
 public:
 	str1 filename_READ;
 	str1 filename_WRITE;
	obj_hold<fd_PIPE_base> fd_READ; 
	obj_hold<fd_PIPE_base> fd_WRITE;

 	audacity_mod_script_pipe() {
		set_filename_PAIR_to_default();
	}

	bool set_filename_PAIR_to_default();

 	virtual bool open_pipe();
 	virtual bool close_pipe();
};

//	audacity_mod_script_pipe.h

#endif
