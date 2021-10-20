#!/usr/bin/make -f

# where is tclsh rooted and where are its include files

#	Tcl_prefix=/usr
	Tcl_prefix=/tools/x86_64/tcl

#	Tcl_prefix=/usr/local
#	Tcl_prefix=/opt9906/tcl
#	Tcl_prefix=/tools/2000/tcl

	Tcl_includes += -I$(Tcl_prefix)/include
	Tcl_includes += -I$(Tcl_prefix)/include/tcl

	Module=tcl_oop_tcl
	Module_objs += $(Module).o 
	Module_objs += OBJ_decoder.o
	Module_objs += TCL_PTR.o
	Module_objs += TCL_LIST.o
	Module_objs += TCL_HELP.o
	Module_objs += TCL_HASH.o
	Module_objs += TCL_DICT.o
	Module_objs += TCL_MATCHER.o
	Module_objs += TCL_TYPE.o

# // edit any .h // recompile everything

	Module_hdrs += $(Module).h
	Module_hdrs += OBJ_decoder.h
	Module_hdrs += TCL_PTR.h
	Module_hdrs += TCL_LIST.h
	Module_hdrs += TCL_HELP.h
	Module_hdrs += TCL_HASH.h
	Module_hdrs += TCL_DICT.h
	Module_hdrs += TCL_MATCHER.h
	Module_hdrs += TCL_TYPE.h

	VPATH_LIST += ./src
	VPATH_LIST += ../src
	VPATH+=$S$(foreach sub,$(VPATH_LIST),:$(sub))
#	VPATH+=$S$(foreach sub,$(VPATH_LIST),:$S/$(sub))


.SUFFIXES: .o .cxx .so .exec .h

test: $(Module).so
	nm $(Module).so | grep Init
	tclsh test_$(Module).tcl 2>&1 | tee fa 
	@echo '---'

# all of the .o depend on all of the .h
# but this did not work somehow
$(Module_objs): $(Module_hdrs)

$(Module).so:  $(Module_objs)
	g++ -shared -o $@ $(Module_objs)

.cxx.o:
	g++ -g -O2 -fPIC -c $< $(Tcl_includes)

clean:
	rm -f $(Module).so $(Module_objs) fa fb
