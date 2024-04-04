#!/tools/CTXT/tcl/bin/tclsh

proc DIR_NAME_CN_key {CN} { file join ca $CN private }
proc FILENAME_CN_key {CN} { file join ca $CN private $CN.key }
proc FILENAME_CN_crt {CN} { file join ca $CN $CN.crt }
proc FILENAME_CN_csr {CN} { file join ca $CN $CN.csr }
