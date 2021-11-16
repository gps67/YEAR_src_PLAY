#./tdoc_gen.tcl >tdoc.dtd
#./tdoc_gen.tcl 2>&1 >tdoc.dtd  | less
./tdoc_gen.tcl 2>&1 | tee tdoc.dtd | less
