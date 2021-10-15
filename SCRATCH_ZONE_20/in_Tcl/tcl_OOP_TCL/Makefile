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
	Module_objs += TCL_PTR_t.o
	Module_objs += TCL_LIST_t.o
	Module_objs += TCL_HELP.o

# // edit any .h // recompile everything

	Module_hdrs += $(Module).h
	Module_hdrs += OBJ_decoder.h
	Module_hdrs += TCL_PTR_t.h
	Module_hdrs += TCL_LIST_t.h
	Module_hdrs += TCL_HELP.h

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
