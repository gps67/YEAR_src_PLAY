#!/usr/bin/make -f
# external users of the libs_apps code can do this:
# include /home/gps/2004/src/libs_apps/MK_ext_lib/MK_with_libs_apps

	TGT=WIN32
	TGT=LGX

# NO needs code from other makefile
#	SubDirs+=../fd_api
#	VPATH+=:../fd_api_buf
#	VPATH+=:../fd_sel_ssl
	# still not working so a TODO ...
	LIST_VPATH+=:../objs
	LIST_VPATH+=:./objs
	LIST_VPATH+=:..
	LIST_VPATH+=:.

#	INCL += -I ../fd_api
#	INCL += -I ../fd_sel_ssl
# NO	LIBS += -L ../fd_api

	include ./MK/MK_LIBS_APPS
	include ./MK/MK_SUFFIXES
	include ./MK/MK_LISTS_

	PKG_CONFIG += pkg-config 
	PKG_CONFIG += --define-variable=TGT=$(TGT)

	PKGS += link_with_base1
	PKGS += link_with_base2
	PKGS += libs_apps_uses_ssl
#	PKGS += libs_apps_uses_js2

	INCL += `$(PKG_CONFIG) $(PKGS) --cflags`
	LIBS += `$(PKG_CONFIG) $(PKGS) --libs`
       
	CXX = g++
	OPTIM += -g

# names of the programs (but not with .exec)

	PROG1 =vnc_pass_test# NOSP
	# PROG1 =ssl_bio_test# NOSP

# .exec files are used to make new makefiles easier ? and WIN32 too

	EXE_1 =$(PROG1).exec# NOSP

# one way of making all targets

	EXELIST += $(EXE_1)

#	OBJS += MYSITE_CA_task.o
#	OBJS += MYSITE_X509_layout.o

# OBJS is used in all (possibly via _a)
# no need for PROG1.o
# except when deleting *.o

#
# DEFAULT COMES FIRST
#

default_test: $(EXE_1)
	./$<
	# NO # ./client_req_view.sh
	# NOW BUILTIN to save files # ./client_iss_view.sh

test99:
	sh -c 'echo $$VPATH'

exe1: $(EXE_1)

default1_test: P $(EXE_1)
	./$<

P5.o:P5.h
P0P4.o: P0P4.h

exe: $(EXE_1)

P:
	#
	# PKG_CONFIG_PATH=# dirs used to find pkgconfig files
	# $(PKG_CONFIG_PATH)
	var_list PKG_CONFIG_PATH
	#
	# The INCLUDE settings come from this commands output
	#
	$(PKG_CONFIG) $(PKGS) --cflags
	#
	# LINKING options
	#
	$(PKG_CONFIG) $(PKGS) --libs
	#
	#

## $(EXE_1): $(OBJS) # NO - now using explicit $(PROG1).o to be sure

$(EXE_1): $(PROG1).o $(OBJS) # make the main first

# $(EXE_1): $(OBJS) $(PROG1).o # make the main last!
# that doesnt work - .exe probably depends on .o so its first!
# the benefit is sorting out the synax of a (new) basic .h
# dont believe me ? drop the ';' from a class decl

zap:
	rm -rf $(EXELIST) *.o 

#
# DEPENDENCIES
#

