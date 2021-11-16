from app_item import app_item, libs_apps_lib_item, libs_apps_app_item

################ libs apps #################

tv_rem = libs_apps_app_item( "tv_rem" )
tv_rem.lang = "C++"
tv_rem.desc_line = "switch tv channel "
tv_rem.desc_text = """
	This is a major panel to watch TV.
	
	It uses lirc to send InfraRed to the set-top-cable-box,
	and the VCR.
	
	It uses /dev/radio to tune FM stations
	
	It uses xawtv to watch TV, and uses X11 atoms to send signals
	in the same way that xawtv-remote does. (switch channels)
	
	It uses /dev/mixer, (OSS emul if ALSA) to track volumes
	on a per-channel basis, and it does quite a good job at
	fading over the nasty CLICK that xawtv/bttv used
	to have when changing channel.
	
	It can run /dev/cdrom. but that needs a bit more work.
"""

accts_gtk = libs_apps_app_item( "accts_gtk" )
accts_gtk.lang = "C++ gtk"
accts_gtk.desc_line = "A table of financial accounts data and UK reports "
accts_gtk.desc_text = """
	This is a cross between a spreadsheet and a database,
	with the rules in the code.
	
	It produces a nifty companies house report in PDF (still with the
	old API, its replacement is postponed again).
"""

sar_plot = libs_apps_app_item( "sar_plot" )
sar_plot.lang = "C++"
sar_plot.desc_line = "Gather parse store and plot SAR -AMS data from Solaris, HP/UX, True64, AIX, ...  "
sar_plot.desc_text = """
	sar is a standard UNIX utility on Solaris, HP/UX, ... 
	which collects System Activity samples every 60 seconds
	and outputs a wierd ascii report.
	
	That can be gathered by a script, gzip compressed, and collected.
	(See sar_gather, tarball in ../samples/.)
	
	Another machine can then parse the text, and build up a binary
	array of floating points in compressed blobs in SQL tables.
	
	They can then be plotted, along with data from other data collectors.
	
	The current version works well, for several years of 'batch collected' data.
	Future developments will move to online collection streams with
	re-transmission options for LAN down times.
"""

sar_parse = libs_apps_app_item( "sar_parse" )
sar_parse.lang = "C++"
sar_parse.desc_line = "parse a SAR report "
sar_parse.desc_text = """
	sar_parse reads the ASCII and gets it to the database.
"""

sar_data = libs_apps_lib_item( "sar_data" )
sar_data.lang = "C++"
sar_data.desc_line = "code concepts for the SAR data software"
sar_data.desc_text = """
	SAR data gets held in memory, in POST files, in remote SQL
	(MySQL or Postgres), all accessed through the dset_api.
"""

wclient = libs_apps_app_item( "wclient" )
wclient.lang = "C++"
wclient.desc_line = "client/server socket test code"
wclient.desc_text = """
	I dont do threading, mostly because of its slow-down overhead.
	Instead, I do event driven IO. Usually one packet contains
	a complete action. If that action blocks, and you have
	many connections, you should have many processes (apache style).

	wclient is a test frame for the HTTP client and server code,
	also a few other things in libs_apps.
"""

accts_js = libs_apps_app_item( "js_many" )
accts_js.lang = "C++"
accts_js.desc_line = "JavaScript with many plugins, such as accts_js"
accts_js.desc_text = """
	js_many is a JavaScript interpreter that can import modules
	(compiled in if not dynamically loaded DLL), so that you can
	load the ACCTS module, or the XXX module. 

	I've written C++ modules for Tcl and Python, so why-o-why
	am I going down the JavaScript path? -- Tcl does have a
	secure model, but doesnt have struct-objects which auto-know
	when to call delete. Python has wonderful struct-objects, but
	doesnt have a secure model, and I want to be able
	to send scripts to the accountant that cant reformat the disk.

	Hindsight, says "create a Tcl struct object extension",
	but Tcl continues to refuse to add ClientData to Tcl_ObjType
	(actually two needed, ClientData_of_Module, ClientData_of_Type
	- or overload freeIntRepProc for the first, and extend struct
	for second).
"""

################ libs apps #################

lib_base = libs_apps_lib_item( "lib_base" )
lib_base.lang = "C++"
lib_base.desc_line = "a basic library of buffers, objects, fd, event-driven-io"
lib_base.desc_text = """
	This is a library with core types such as reference-counted
	objects, smart pointers to them, buffers that stretch,
	event-driven file-descriptors, unix-time, etc (COMING SOON: u32_hilo i32_abcd)
"""

lib_base2 = libs_apps_lib_item( "lib_base2" )
lib_base2.lang = "C++"
lib_base2.desc_line = "extended library of specific features: js gtk pdf ssl"
lib_base2.desc_text = """
	Things like PDF, X11, Gtk, JavaScript, OpenSSL. If your app needs
	JS-instrumentation-scripting, here is one way of adding it.
	The SSL is much stonger, now that I have a use for it.
"""

lib_play = libs_apps_lib_item( "lib_play" )
lib_play.lang = "C++"
lib_play.desc_line = "play area - move to IDEAS"
lib_play.desc_text = """
	These are IDEAS that are not yet fully grown, but might have
	proof of concept use elsewhere within libs_apps.
"""

lib_media = libs_apps_lib_item( "lib_media" )
lib_media.lang = "C++"
lib_media.desc_line = "access to the sound mixer, radio, infra-red, ..."
lib_media.desc_text = """
	This wraps the Linux kernel access to TV cards with a radio on
	them (the TV is viewed using xawtv, but the radio is direct).
	It also contains wrappers to control the sound mixer, and
	LIRC infra-red transmitter (I channel hop using my keypad,
	and pause the VCR with a mouse)
"""

lib_inet = libs_apps_lib_item( "lib_inet" )
lib_inet.lang = "C++"
lib_inet.desc_line = "internet ftp, http, ..."
lib_inet.desc_text = """
	The HTTP code works well. The ftp is a bit of a placeholder.
	The HTTP code uses event driven IO, WITHOUT THREADS, to handle
	multiple connections as a client or server. A second area
	will be needed for servlet style.
"""

lib_accts = libs_apps_lib_item( "lib_accts" )
lib_accts.lang = "C++"
lib_accts.desc_line = "code concepts for the accounting software"
lib_accts.desc_text = """
	This splits the ACCTS application(s) into its components.
	Thats a set of tables of rows (of spec) of cols
	(of spec). Its also the JavaScript API to the data, the GUI
	frames, reports and a possible new tbl2 format.

	It generates a rather smart PDF report.
"""

sar_parse = app_item( "BLOG_PSG_TOKEN" )
sar_parse.lang = "utf8-en"
sar_parse.desc_line = "some BLOGS from whenever"
sar_parse.desc_text = """
	This is a rough collection of stuff
"""

