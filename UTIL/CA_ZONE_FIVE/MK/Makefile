#!/usr/bin/make -f
# external users of the libs_apps code can do this:
# include /home/gps/2004/src/libs_apps/MK_ext_lib/MK_with_libs_apps

	# we are in $T/obj/.

	T=..
	MK_ext_lib=/home/gps/YEAR/src/PLAY/libs_apps/MK_ext_lib

	CXX = g++
	OPTIM += -g 
#	OPTIM += T_IS_$T

# UNUSED see INCL or todo INCL_LIST
#	INCLS += -I./
#	INCLS += -I../
	INCLS += -I$T/src/
#	INCLS += -I../src/

	DEFS = 

	#
	LIST_VPATH +=$T
	LIST_VPATH +=$T/src
#	LIST_VPATH +=$T/src/some_sub_dir
	LIST_VPATH+=:$T/objs
#	LIST_VPATH+=:.

#	include ../MK/Makefile_libs_apps
	include $(MK_ext_lib)/MK_with_libs_apps # TODO put back there
# which includes these
#	include $(MK_ext_lib)/MK_SUFFIXES
#	include $(MK_ext_lib)/MK_LISTS_

# done in MK_with_libs_apps
#	TGT=WIN32
#	TGT=LGX

#	INCL += -I ../fd_api
#	INCL += -I ../fd_sel_ssl
# NO	LIBS += -L ../fd_api

	PKGS += libs_apps_base1
	PKGS += libs_apps_base2
	PKGS += libs_apps_uses_ssl
#	PKGS += libs_apps_mozjs

# names of the programs (but not with .exec)

	PROG1 =mk_req# NOSP
	# PROG1 =ssl_bio_test# NOSP

# .exec files are used to make new makefiles easier ? and WIN32 too

	EXE_1 =$(PROG1).exec# NOSP

# one way of making all targets

	EXELIST += $(EXE_1)

#	OBJS += SITE_X509_VALS_base.o
#	OBJS += SITE_X509_layout.o
#	OBJS += X509_VALS_base.o
#	OBJS += X509_util_EXT.o
#	OBJS += X509_util_NAME.o
#	OBJS += X509_util_NID.o # entirely optional

	# tree layout of CA_ _ZERO _ONE _ZONE [_VPN_level_4] then C_
	# each CA_ZONE is given to the local admin to look after
	# you could have a CA_ for each category of C_  but ... too many ?
	# LAPTOP || USER || SERVER || ITEM
	# some items use RSA not X509, that comes for ride
	# some items use RSA elsewhere (not within itself) eg db_of_config
	
	# task is the process of issuing one of those X509's
	# particularly getting VALS into the REQ
	# signing with the up-tree CA_
	# and saving in a knowable filename
	# TODO pass files
	# TODO pass from ...
	# TODO multi example C_
	# TODO stream from SQL

	OBJS += MYSITE_X509_layout.o
	OBJS += MYSITE_CA_task.o
	OBJS += cfg_CA_1.o

#	OBJS += MYSITE_X509_VALS.o
#	OBJS += X509_util_CERT.o
 ##	OBJS += X509_REQ_util.o
 ##	OBJS += X509_ISS_util.o

#	OBJS += SSL_global.o
#	OBJS += X509_cert.o
#	OBJS += fd_ssl_client.o

#	OBJS += P5.o		# P5_N = 5 allowing P4<P5_N
#	OBJS += P0P4.o		# array of P0 P1 P2 P3 P4 (not post office)
#	OBJS += dgb.o		# FAIL WARN COMPLAIN TRACE

#	OBJS += $(PROG1).o	# DONT - its doubled!

# OBJS is used in all (possibly via _a)
# no need for PROG1.o
# except when deleting *.o

#
# DEFAULT COMES FIRST
#

default: default_test

defaults2: show_vars

show_vars:
	# NOWT TO DO HERE
	#
	# VPATH
	# $(VPATH)
	#
	# CXX
	# $(CXX)
	#
	# OPTIM
	# $(OPTIM)
	#
	# INCL
	# $(INCL)
	#
	# DEFS
	# $(DEFS)
	#

default_test: $(EXE_1)
	./$<
	# NO # ./client_req_view.sh
	# NOW BUILTIN to save files # ./client_iss_view.sh

CA_ROOT: $(EXE_1)
	./$(EXE_1) CA_ROOT

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
	rm -rf $(EXELIST) *.o client_key.pem client_req.pem

#
# DEPENDENCIES
#

