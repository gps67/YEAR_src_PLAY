# near http://sourceforge.net/docman/display_doc.php?docid=13826&group_id=50913
# Project: js32

CC   = mingw32-gcc.exe
WINDRES = windres.exe
RES  = js.res
# SRCDIR	= ../../../Development/SpiderMonkey/js/src
SRCDIR	= ../js/src
OBJECTS = \
	$(SRCDIR)/jsapi.o \
	$(SRCDIR)/jsarena.o \
	$(SRCDIR)/jsarray.o \
	$(SRCDIR)/jsatom.o \
	$(SRCDIR)/jsbool.o \
	$(SRCDIR)/jscntxt.o \
	$(SRCDIR)/jsdate.o \
	$(SRCDIR)/jsdbgapi.o \
	$(SRCDIR)/jsdhash.o \
	$(SRCDIR)/jsdtoa.o \
	$(SRCDIR)/jsemit.o \
	$(SRCDIR)/jsexn.o \
	$(SRCDIR)/jsfun.o \
	$(SRCDIR)/jsgc.o \
	$(SRCDIR)/jshash.o \
	$(SRCDIR)/jsinterp.o \
	$(SRCDIR)/jslock.o \
	$(SRCDIR)/jslog2.o \
	$(SRCDIR)/jslong.o \
	$(SRCDIR)/jsmath.o \
	$(SRCDIR)/jsnum.o \
	$(SRCDIR)/jsobj.o \
	$(SRCDIR)/jsopcode.o \
	$(SRCDIR)/jsparse.o \
	$(SRCDIR)/jsprf.o \
	$(SRCDIR)/jsregexp.o \
	$(SRCDIR)/jsscan.o \
	$(SRCDIR)/jsscope.o \
	$(SRCDIR)/jsscript.o \
	$(SRCDIR)/jsstr.o \
	$(SRCDIR)/jsutil.o \
	$(SRCDIR)/jsxdrapi.o \
	$(SRCDIR)/prmjtime.o \
	$(RES)

LIBS =  --add-stdcall-alias -L. -lfdlibm
BIN  = js.dll
#CFLAGS = -DXP_PC -DEXPORT_JS_API -D_WINDOWS -DWIN32 -D_MINGW -s
CFLAGS = -DXP_WIN -DEXPORT_JS_API -D_WINDOWS -DWIN32 -D_MINGW -s

DLLWRAP=dllwrap.exe
DEFFILE=libjs.def
STATICLIB=libjs.a

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $*.c

$(BIN): $(OBJECTS)
	$(CC) -shared -o $@ \
	-Wl,--output-def,$(DEFFILE) \
	-Wl,--out-implib,$(STATICLIB) \
	$(OBJECTS) \
	$(LIBS)

$(RES): 
	$(WINDRES) -i $(SRCDIR)/js3240.rc -I rc -o $(RES) -O coff 
