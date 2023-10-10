xml=/tools/2003/xml
pfx_py2=/usr
pfx_py2=/tools/x86_64/py2
libpython2a=$pfx_py2/lib/libpython2.7.a
pfx_osp=/home/gps/G/prefix/SP
#        libosp.so.4 => /home/gps/G/prefix/SP/lib/libosp.so.4 (0x00007f7c2a488000)

main1=t1_main
exe1=$main1.elf

spin=..
set -x

gcc \
	-I$pfx_py2/include/python2.7 \
	-export-dynamic \
	$main1.c \
	-o $exe1 \
	-I$pfx_osp/include/OpenSP \
	-L$pfx_osp/lib \
	-losp \
	$spin/SP_lib/spin_py.so \
	$libpython2a \
	-ldl \
	-lpthread \
	-lutil \
	-lm \
	-Wl,-rpath=$pfx_osp/lib \
	-Wl,-rpath=$pfx_py2/lib \
|| exit

ldd $exe1
./$exe1
#

# c++  -Xlinker -export-dynamic -o python \
#
ask_do rm ./$exe1

