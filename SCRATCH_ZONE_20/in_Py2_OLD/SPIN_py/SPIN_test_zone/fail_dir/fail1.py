#!/usr/bin/env python2

import sys
import posix

from sys import stdout, stderr, argv, path
p0 = path[0]
# path.insert(1, p0+"/../SP_lib/" ) # .so
path.insert(1, p0+"/../lib_base/" ) # used

from spin_eh import spin_py

# libs_apps provides a better used gdb system
# which gets invoked instead of a core dump
# Its also good to invoke intentionally.
# edit code to select GUI insight or gdb text

print "# a gdb command worth trying:"
print "b posix_listdir"
print "..."

spin_py.GDB_INVOKE();

# something to debug

print posix.listdir("/")
