cat <<\!

	${MAKECMDGOALS}

		passes ARGV to child make

all: subdirs

.PHONY: all subdirs clean
subdirs:
        ${MAKE} -C dir1 ${MAKECMDGOALS}
        ${MAKE} -C dir2 ${MAKECMDGOALS}
        ${MAKE} -C dir3 ${MAKECMDGOALS}

clean: subdirs

The list of suffixes for auto rules needs to be stated

.SUFFIXES: .SUFFIXES: .cxx .c .o .a .so .dll .exec 

.c.o:
	gcc $(OPTIM) $(INCLS) $(DEFS) -c $<

.o.exec: $(OBJS):
	$(CXX) $(OPTIM) -o $@ $< $(OBJS) $(LINKS) $(LIBS)

	$@	the item being made
	$^	all prerequisites (repeats removed)
	$+	all prerequisites (repeats kept) (see $^)
	$?	new prerequisites (need building)
	$<	first pre-requisite (see $^)

	$(@D)	dir part of $@
	$(@F)	file part of $@

	$%	VAGUE # bar.o from TARGET: foo.a(bar.o) (See Automatic Variables)

	$*	a.%.b ==> a.$*.b 

ifeq ($(WITH_PYTHON),1)
	...
else
	...
endif

FUNCTIONS

	# remove TABS to spaces
	# builtin strip
	# assign := # 
	OBJS_base2 := $(strip $(OBJS_base2))

	# run command shell
	# INCLS += $(shell `$(PKG_CONFIG) $(PKGS) --cflags`)


FUNCTIONS for LISTS

	# Makefile easily introduces SP by accident
	SP=$(empty) $(empty)# SINGLE SPACE
        COLON=:# no  spaces

	# TODO rename _PATH as _LIST 
	# even VPATH as VLIST # VPATH_LIST

	VPATH_LIST += ADIR # single SPACEs in lots of places
	VPATH+=$(subst $(SP),$(COLON),$(VPATH_LIST))
	CPLUS_INCLUDE_PATH+=$(subst $(SP),$(COLON),$(foreach sub,$(LIST_CPLUS_INCLUDE_
	PKG_CONFIG_PATH=$(subst $(SP),$(COLON),$(LIST_PKG_CONFIG_PATH):$(PKG_CONFIG_d











