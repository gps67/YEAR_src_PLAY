#!/usr/bin/env sh

	PATH=$PATH:

	prog_syslog5.exec $0 0 USER INFO "SH message without pid"
	prog_syslog5.exec $0 1 USER INFO "SH message with pid - pointless (PPID pref)"
