# from http://sourceforge.net/docman/display_doc.php?docid=13826&group_id=50913
# Project: fdlibm

CC = mingw32-gcc.exe
# SRCDIR	= ../../../Development/SpiderMonkey/js/src/fdlibm
SRCDIR	= ../js/src/fdlibm

OBJECTS= \
	$(SRCDIR)/w_sqrt.o \
	$(SRCDIR)/e_pow.o \
	$(SRCDIR)/e_sqrt.o \
	$(SRCDIR)/k_standard.o \
	$(SRCDIR)/s_atan.o \
	$(SRCDIR)/s_finite.o \
	$(SRCDIR)/s_isnan.o \
	$(SRCDIR)/s_matherr.o \
	$(SRCDIR)/s_rint.o \
	$(SRCDIR)/s_scalbn.o \
	$(SRCDIR)/w_atan2.o \
	$(SRCDIR)/e_atan2.o \
	$(SRCDIR)/s_fabs.o \
	$(SRCDIR)/s_copysign.o \
	$(SRCDIR)/w_pow.o \
	$(SRCDIR)/s_lib_version.o

LIBS =	-llibm
BIN  = fdlibm.lib
CFLAGS = -D_IEEE_LIBM -DWIN32 -D_WINDOWS -s

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $*.c

$(BIN): $(OBJECTS)
	ar r $(BIN) $(OBJECTS)
	ranlib $(BIN)

