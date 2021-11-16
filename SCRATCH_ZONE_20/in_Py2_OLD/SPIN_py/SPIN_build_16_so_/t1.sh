xml=/tools/2003/xml
spin=..
set -x

gcc \
	-I/tools/2003/py/include/python2.2 \
	-export-dynamic \
	t1_main.c \
	-I$xml/include/OpenSP \
	-L$xml/lib \
	-losp \
	$spin/SP_lib/spin_py.so \
	/tools/2003_obj/py/Python-2.2.2/libpython2.2.a \
	-ldl \
	-lpthread \
	-lutil \
	-lm \
	-Wl,-rpath=$xml/lib \

#

# c++  -Xlinker -export-dynamic -o python \
#

