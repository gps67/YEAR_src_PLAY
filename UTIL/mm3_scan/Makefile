#!/usr/bin/make -f
# needs to be gmake for += feature
# using += when not necessary allows later additions!

	CXX = g++
#	CXX = /tools/2002/ccs3/bin/g++
#	OPTIM += -O2
	OPTIM += -g

	INCL += -I./scan_nl
	INCL += -I./map_file
	INCL += -I./own_str
	INCL += -I./dbg
	INCL += -I./cset
	INCL += -I./p0p2
	INCL += -I./mm3_file

	OBJS += misc.o
	OBJS += map_file/map_file.o
#	OBJS += mm3_file/mm3_file.o
	OBJS += mm3_file/mm3_file_scan.o
	OBJS += mm3_file/mm3_file_data.o
	OBJS += mm3_file/mm3_file_tbl.o
	OBJS += scan_mm3.o

	PROG = scan_mm3.exec

	M1 = 0001
	M2 = 0102
	M3 = 0203
	M4 = 0304
	# MM3FILE = 2k01.mm3
	# MM3FILE = 9899a2.mm3
	# MM3FILE = ir9798.mm3

all: $(PROG)
# all: $(M1).csv
all: $(M2).csv
all: $(M3).csv
all: $(M4).csv

$(M1).csv: $(PROG) Makefile
$(M2).csv: $(PROG) Makefile
$(M3).csv: $(PROG) Makefile
$(M4).csv: $(PROG) Makefile

.SUFFIXES: .cxx .o .mm3 .csv

.cxx.o:
	$(CXX) $(OPTIM) $(INCL) -o $@ -c $<
 
.mm3.csv: $(PROG) Makefile
	./$(PROG) $< > $@
	# less -S '+/ x. ' $@

clean:
	rm -f  $(PROG) $(OBJS) fa tclIndex rpt1.csv

# optim is irrel for ld but possible 
$(PROG): $(OBJS)
	$(CXX) $(OPTIM) -o $@ $+

# # rpt1.tcl is the output from the C file
# $(MM3FILE).csv: $(PROG)  $(MM3FILE).mm3 Makefile
# 	./$(PROG) $(MM3FILE).mm3 > $@
# 	less -S '+/ x. ' $@

# rpt2.tcl is the code that generates the CSV file
# it needs an auto_path that works, so build the index of procs
tclIndex: rpt2.tcl
	tclsh ./mki.tcl

