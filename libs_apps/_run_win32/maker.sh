
# run this on win98 but within the MSYS environment
# MSYS is from www.mingw.org

cd ../_obj_win32
set -x
make  WITH_GTK=2 WITH_MYSQL=0 WIN32=1 -f ../MK/Makefile ${@:-lots}
