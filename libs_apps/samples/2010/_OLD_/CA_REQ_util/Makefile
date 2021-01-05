#!/usr/bin/make -f
# external users of the libs_apps code can do this:
# include /home/gps/2004/src/libs_apps/MK_ext_lib/MK_with_libs_apps

	TGT=WIN32
	TGT=LGX

# NO needs code from other makefile
#	SubDirs+=../fd_api
#	VPATH+=:../fd_api_buf
#	VPATH+=:../fd_sel_ssl

#	INCL += -I ../fd_api
#	INCL += -I ../fd_sel_ssl
# NO	LIBS += -L ../fd_api

	include ./MK/MK_LIBS_APPS
	include ./MK/MK_SUFFIXES

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

	PROG1 =mk_req# NOSP
	# PROG1 =ssl_bio_test# NOSP

# .exec files are used to make new makefiles easier ? and WIN32 too

	EXE_1 =$(PROG1).exec# NOSP

# one way of making all targets

	EXELIST += $(EXE_1)

	OBJS += X509_util_NAME.o
	OBJS += X509_util_EXT.o
	OBJS += X509_VALS_base.o
	OBJS += X509_VALS_SITE_funcs.o
	OBJS += X509_util_REQ.o

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

default_test: $(EXE_1)
	./$<
	./client_req_view.sh

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

## $(EXE_1): $(OBJS)
$(EXE_1): $(PROG1).o $(OBJS)

zap:
	rm -rf $(EXELIST) *.o client_key.pem client_req.pem

#
# DEPENDENCIES
#

