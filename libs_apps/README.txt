
lib_apps is several apps and their libraries, in one code pool.

	License:
		(Library) L-GPL see
		http://ftp.gnu.org/gnu/LGPL
	runs on
		WIN32, Linux, UNIX, ... it used to and shall again
	uses
		the Gtk GUI-Toolkit (V2, not V1)
	NO	the POST datafile (mapped data files) (bundled)
	()	the PDFLIB library (optional - GPL usage)
	()	the JavaScript implementation from Mozilla (not js2)
	written in
		C++
	home pages:
		https://github.com/gps67/YEAR_src_PLAY
		# YEAR-src_PLAY is an even wider libs_apps
		# it was getting messy inside, so new experiments moved out
		# libs_apps is one dir that is sometimes used by adjacent dirs
		# this probably requires /home/gps/ to be a link so that
		# /home/gps/YEAR_src_PLAY/libs_apps/ exists
		# soz #
	Does:
		various things,
		lib_base - reference counted objects, buffers, etc
		lib_base2 - javascript, pdf, 
		lib_gtk1 - a wrap on Gtk, lib_gtk2 = more specific to app
		lib_accts - UK accounting reports over CVS files
		lib_media - controls a linux mixer, xawtv-x11, radio, lirc, cd
		sar_plot - parse/plot SVR4 sar report

License: LGPL = (Library) L-GPL = http://ftp.gnu.org/gnu/LGPL

	You can use this LGPL software in commercial products,
	(hereby, without asking) as long as you comply with the L-GPL.

	NOTE: this package links against some non LGPL modules,
	you may need to be sure that you dont use them, and/or switch them off.

	If you fully comply with the spirit and letter of the source
	side of the LGPL, I am not worried about the linking side of the LGPL.

	To be safe, put your modified copy of libs_apps on your web-site.
	Optionally email me, so that I know about it. Then you can link static
	or apply DRM signatures to your binaries, or whatever troubles you.

	To go L-GPL without this let-off, simply link your app against
	the shared object DLL's. Any header info that is inlined,
	was presumably designed to be inlined (by C++ by default),
	and still allows use of replacement DLL's (which dont
	differ by much, as far as the inlined stuff goes).

	If you have a speed need, you might release two copies
	of your app. One linked statically, one against the DLL's.
	That 'allows users to run with a adjusted DLL' if they chose,
	or not if they want the speed. They should both work similarly!

BUILD/INSTALL:

	Currently the WIN32 builds are bit_rot_broken

	For an easy life create /home/gps/ as a SYMB link to (...)
	I know that is naff, but your life will be easier

	Without it a few symb-links and a few makefile rules might break

	you MUST create symb-links and edit the Makefiles, see docs/Build.txt
	you will probably need to install various -dev packages
	and or build external packages that it uses

	I am moving AWAY from pdflib to use the internal SPOUT
	but that is not done, by some code, and it is easier to
	build with too many packages, than edit them out

	SpiderMonkey is the usual beligerant self.
	I claim to use it, but the API changed,
	and I have not the patience to redo it.
	Even the minimalist redo, is blocked by the fact
	that SpiderMonkey needs the very latest rust.
	It can run with 1.75 (which Ubuntu has)
	it DEMANDS 1.76 (and I bet for no good reason)
	THat too would only tak moments to add, but meh

	this is not as easy as the usual configure/make, sorry,
	and I build on a non-standard prefix, which may help admins
	of non Linux boxes (HP, SUN, AIX, ... ) and non-root users

	NEWS: pkg-config can be used by clients, you edit its files.pc instead
	edit lib_pkgconfig/* for what it USES, when linking

	OLD_NEWS pkg-config is used more, with lots of pseudo rules,
	eg libs_apps_uses_libpdf.pc 

tv_rem
	tv_rem is a tv remote to watch TV, RADIO, CD, ... 

	It controls xawtv (which does the hard work), drives the kernel radio,
	has sound-card mixer channels and sends commands to lirc
	(which does the hard work) to control external devices
	(VCR, NTL-Cable, ...) over infra-red.

	There is a document that tells you how I built and set up an
	infrared transmitter diode on the serial port, as suggested
	by www.lirc.org. With the furniture carefully placed in range,
	I dont use any other remote!

	It has per-channel volume factors, so switching between a quiet
	channels and others doesnt upset the neighbours. It also
	fades in TV-CARD channels switches, because BTTV had a really
	horrid click. (External Cable channel switches dont need a fader).
	BTTV is now fixed (it has its own fader).

	The parameters are currently hardcoded into tv_radio_data.cxx
	but you can easily replace them with the radio frequencies you
	can receive, channel names, and VCR/IR command strings.

	TODO: 
		The CD-player needs a bit more work.
		/dev/cd ioctls block - requiring separate thread
		Mplayer not even started
		MP3 library player not even started
		TV player would be nice
		(external progs should at least switch between)
		xmltv - tv listings
		Tune Radio (manual frequency)
		Turn Radio OFF ?? (allow external prog)

accts_gtk
	accts_gtk is a program to edit a small set of business accounts,
	and print reports to HTML and PDF.

	It has many fine ideas, but needs something more, or different.
	Problems include the real-world problem being absurdly complicated,
	eg merging UK PAYE tax tables but keeping them separate.

accts_js
js_accts
js_many
js_XXX

	js_accts is JavaScript for accts_gtk (development)

	JavaScript is potentially secure, when Python isnt.
	That might allow agencies to run clients setup scripts.
	Other options include Java, Tcl, ...

	http://www.javascriptkit.com/
	http://www.mozilla.org/docs/web-developer/

wclient (a test prog)
	wclient is an asynchronus TCP client/server (tester/development).

	There is also an http server, neither is much of anything,
	but the underlying event driven library io is quite good,
	and its moving in the right direction (TODO: faster).

	TODO:
		cddb client
		ftp client
		cookies
		ssl
		ssl - learn mode, with replay scripts
		yahoo parsing, NWA parsing, 
		wget replacement
		web-site-upload (diff)

	NB There is also wclient-like code in IDEAS/plus/slog1

text_gtk
	text_gtk is a test program (development)

	This is barely started, but the library has a gprint()
	that writes to a text widget (used by all other gtk apps).

sar_parse
	sar_parse parses an HP/UX gzipped 'sar -AMS' file and writes it to
	a POST database mmap'd file, alternatively a PostGreSQL or MySQL db.

sar_plot
	sar_plot loads data from sar_parse's database and plots it on-screen.

	TODO: time_sampled as well as time_series data
	TODO: ...

sar_gather
	sar_gather (.tgz in samples) gathers data to be parsed and plotted

	Its a shell script that you install on HP/UX or Solaris,
	then call back each week to harvest the collected data.

Licenses

	All my code is Library LGPL, which means you can CALL it from
	almost any License you like, with very little admin.

	The proper L-GPL says (effectively) that you must link against
	the DLL/SO dynamically-linked/libraries/shared-objects.
	Thats not so difficult.

	I call/use/combine-with other source software,
	you must also comply with those licenses.
	I chose the components, because they allow, at least GPL.

	If an externally authored module has been included (eg POST),
	it remains under its original license, its just bundled for
	convenience.


	POST datafile
		BSD-style free license

	PDFLIB
		You may develop free software with PDFlib,
		provided you make your source code available.
		www.pdflib.com

		the newer license emphasises GPL but not LGPL

		PDFLIB will (eventually) be phased out because it forces GPL.

	JS
		get it from www.mozilla.org
		http://en.wikipedia.org/wiki/SpiderMonkey

	Gtk+
		LGPL from www.gtk.org

	libmysql
		forces GPL (or similar FLOSS) even for the client!
		you can edit mysql out of the Makefile and use postgres

	Giving-It-Back
		Taking POST as an example:

		Any changes to the POST code (directory), are immediately
		offered back to the original author, or well known team,
		under the same licence as it was received.

		Physically doing so can be optimised to reduce email noise,
		and can be fixed by anyone at a later date.


See the docs subdir.

