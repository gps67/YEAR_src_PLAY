#! /usr/bin/env bash
# /home/gps/prefix/bin/bash
. ./go__fns

set_j_JOBS() {
	set_CPU_COUNT_J # ALIAS for JOBS but daft and unique
	decr_CPU_COUNT_J # one fewer for other users kernel etc
	JOBS="-j $CPU_COUNT_J"
	j_JOBS="-j $CPU_COUNT_J"
}


set_j_JOBS


echo j_JOBS "'$j_JOBS'"
