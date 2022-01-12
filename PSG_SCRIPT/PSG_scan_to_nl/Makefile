# /usr/bin/make
#!/tools/2000/ccs/bin/make -f
#!/usr/bin/make -f
# needs to be gmake for += feature
# using += when not necessary allows later additions!

	CXX = g++
#	CXX = /tools/2004/ccs3/bin/g++
#	OPTIM += -O2
#	OPTIM += -O1
	OPTIM += -O0
	OPTIM += -g

	# to link against LIBSAPPS you must match its settings
	# then its as easy as including this ..
	# and a few other things
	#
#	include ../MK_LIBSAPPS
	include ../Makefile_libs_apps

	subdir_list += ..
	subdir_list += ../more_bits
	subdir_list += ../scan_lang
	subdir_list += ../scan_file
	subdir_list += ../prog_main
	subdir_list += ../lex_lookup
	subdir_list += ../XML_DTD
	subdir_list += ../XML_DOC
	subdir_list += ../XML_Parse
	subdir_list += ../LEX_PLEX

        SP=$(empty) $(empty)# SINGLE SPACE
        COLON=:# no  spaces
        VPATH=$(subst $(SP),$(COLON),$(subdir_list))
	export VPATH
	CPLUS_INCLUDE_PATH+=$(VPATH)
	export CPLUS_INCLUDE_PATH

	INCL += $(INCLS)	# set by libsapps

#	GCC_RPATHS += -Wl,--rpath=$(LIBSAPPS)/lib
# 	LD_RPATHS += -rpath=$(LIBSAPPS)/lib


	OBJS += scan_to_nl_P.o
	OBJS += scan_to_nl_base_report.o
	OBJS += scan_to_nl_base.o
	OBJS += scan_to_nl_plus.o
	OBJS += scan_to_nl.o
	OBJS += scan_nl_c_lex.o
	OBJS += scan_nl_c_psg.o
	OBJS += scan_nl_xml_lex.o

##	OBJS += own_str.o
	OBJS2 += file_scan_base.o
	OBJS += map_file.o
#	OBJS += stbl_spelling.o

	OBJS += scan_file_base.o
	OBJS2 += scan_c1.o
##	OBJS2 += scan_data.o

	OBJS2 += XML_Parse.o
	OBJS2 += XML_DTD.o
	OBJS2 += XML_DOC.o
	OBJS3 += scan_xml_1.o
	OBJS3 += lex_lookup.o

	OBJS4 += proc_parts/proc_partitions.o

	EXEC=.exec
#	PROG1 = scan_mm3
	NAME2 = c1
	NAME3 = x1
	PROG2 = $(NAME2)$(EXEC)
	PROG3 = $(NAME3)$(EXEC)
	PROG4 = scan_proc_partitions

	LIBscan_a = libscan.a

	M1 = 0001
	M2 = 0102
	M3 = 0203
	M4 = 0304
	# MM3FILE = 2k01.mm3
	# MM3FILE = 9899a2.mm3
	# MM3FILE = ir9798.mm3

default3: test3

default2: test2

include .deps

default: $(PROG1) test1

all: $(PROG1)
# all: $(M1).csv
all: $(M2).csv
all: $(M3).csv
all: $(M4).csv

$(M1).csv: $(PROG1) Makefile
$(M2).csv: $(PROG1) Makefile
$(M3).csv: $(PROG1) Makefile
$(M4).csv: $(PROG1) Makefile

# .deps: Makefile # too many

.deps:
	$(CXX) -MM $(INCL) *.cxx */*.cxx > $@

.SUFFIXES: .cxx .o .mm3 .csv

.cxx.o:
	$(CXX) $(OPTIM) $(INCL) -o $@ -c $<
 
test1: $(PROG1) Makefile
	./$(PROG1) $(M1).mm3 2>&1 | less
 
test2: $(PROG2) Makefile
	./$(PROG2) ../test/test2.c1 2>&1 # | less
 
test3: $(PROG3) Makefile
	./$(PROG3) ../test/test3.xml 2>&1 | less
 
ins3: $(PROG3) Makefile
	insight --args ./$(PROG3) ../test/test3.xml 2>&1 | less
 
test4: $(PROG4) Makefile
	./$(PROG4) /proc/partitions 2>&1 | less

.mm3.csv: $(PROG1) Makefile
	./$(PROG1) $< > $@
	# less -S '+/ x. ' $@

clean:
	rm -f  $(PROG1) $(PROG2) $(PROG3) $(PROG4) $(NAME1).o $(NAME2).o $(NAME3).o $(NAME4).o $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS) fa tclIndex rpt1.csv $(LIBscan_a)

$(LIBscan_a): $(OBJS1) $(OBJS2) $(OBJS) $(OBJS3)
	ar -curs $@ $?

# optim is irrel for ld but possible 
$(PROG1): $(NAME1).o  $(LIBscan_a)
	$(CXX) $(OPTIM) -o $@ $+ $(LIBS)

$(PROG2): $(NAME2).o $(LIBscan_a)
	@# . /tools/2000/fns_var_add; var_list PKG_CONFIG_PATH
	$(CXX) $(OPTIM) -o $@ $+ $(LIBS)

$(PROG3): $(NAME3).o $(LIBscan_a)
	@# . /tools/2000/fns_var_add; var_list PKG_CONFIG_PATH
	@# pkg-config libs_apps_base2 --cflags
	@# pkg-config libs_apps_base2 --libs
	$(CXX) $(OPTIM) -o $@ $+ $(LIBS)

$(PROG4): $(NAME4).o $(LIBscan_a)
	$(CXX) $(OPTIM) -o $@ $+ $(LIBS)

# rpt2.tcl is the code that generates the CSV file
# it needs an auto_path that works, so build the index of procs
tclIndex: rpt2.tcl
	tclsh ./mki.tcl

