#!/usr/bin/env python2

# translation of the simple.c test in the gtk+ distribution, using the
# new() function from gobject (this is an example of creating objects
# with the properties interface).

import gobject, gtk
import os

# CHEAT - add .profile style things to CALLING env, not indiv cmd
from sys import stdout, stderr, argv, path
path.insert(1, "/home/gps/bin" ) # 
print( "path", path )

class globals:
	""" This is a pointless class but it annotates global variables """
	pass

gbl = globals()
gbl.tt = gtk.Tooltips()

class Runnable:
	def __init__(self):
		self.cmd_exe = None # not in argv but often duplicated in argv0
		self.cmd_name = None # argv0
		self.cmd_args = [] # argv1 plus - the optional args!
		self.btn_label = None # so dump argv

		# self is not YET a button ...
		# see # panel.new_btn_Runnable( cmd )
		# btn.set_property("xalign",0)

	def get_exe( self ):
		exe = self.cmd_exe
		if not exe:
		  if 0:
		   exe = "/usr/bin/env"
		  else:
		   exe = self.cmd_name
		  # better lookup in path
		return exe

	def get_argv0( self ):
		argv0 = self.cmd_name
		if not argv0:
			argv0 = self.cmd_exe
			# better - resolve to basename ?
		return argv0

	def get_env( self ):
		return os.environ
		# return os.env

	def run_now( self, btn=None ):
		exe = self.get_exe()
		str2 = "" + exe
		argv = []
		argv.append( self.get_argv0() )
		for arg in self.cmd_args:
			str2 += " " + arg
			argv.append( arg )
		env = self.get_env()
		print( "\nRUNNING COMMAND:", str2 )
		# os.P_DETATCH # detech from console ...  etc ?
		# os.P_OVERLAY # exec not fork
		tag = os.spawnve( os.P_NOWAIT, exe, argv, env )
		print( "TAG =", tag )
		# os.execve( exe, argv, env )
		#
		# problem with WIN32 - waitpit() exists but 1 (red oct)
		# one pid only
		# UNIX allows PID(0) WINDOWS not
		# presumably could have O_NDELAY somewhere in WAITPID
		# tclsrc/win/tclWinPipe.c
		# WaitForSingleObject( handle. flags )
		# GetExitCodeProcess(infoPtr->hProcess, &exitCode);


	def get_tooltip( self ):
		str = ""
		str += self.get_argv0()
		for arg in self.cmd_args:
			str += "\n " + arg 
		return str

	def get_label( self ):
		if self.btn_label:
			return self.btn_label
		str = ""
		str += self.get_argv0()
		for arg in self.cmd_args:
			str += " " + arg 
		return str

class Runnable_xterm_mc_d1_d2(Runnable):
	""" run an xterm with mc inside it pointing at d1 d2

		This shows the generic need for button TYPES
		eg where get_argv() can be called at run time
		eg where buttons can be style-grouped - label
		eg ... too many variables!
	"""
	def __init__(cmd, d1, d2 = None ):
		if not d2:
			xd2 = "/home/gps/2000"
		Runnable.__init__( cmd )
		cmd.d1 = d1
		cmd.d2 = d2
		cmd.cmd_exe = "/usr/bin/xterm"
		cmd.cmd_args.append( "-e" )
		cmd.cmd_args.append( "mc" )
		cmd.cmd_args.append( d1 )
		if d2:
			cmd.cmd_args.append( d2 )
		# panel.new_btn_Runnable( cmd )

	def get_tooltip( self ):
		str = "Open a new xterm, running mc"
		str += "\n "
		str += self.d1
		if self.d2:
			str += "\n "
			str += self.d2
		return str

	def get_label( self ):
		if self.btn_label:
			return self.btn_label
		str = "xmc " + os.path.basename( self.d1 )
		return str

	def get_exe( self ):
		return "/usr/bin/xterm"


def cb_plus_btn_pressed( btn ):
	btn.cb_clicked()

def cb_destroy(*args):
	print( "Gtk destroy, emits destroy signal, calls this func" )
	print( "Calling window.hide()" )
	gbl.window_main.hide()
	print( "Calling gtk.main_quit()" )
	gtk.main_quit()
	print( "So main loop will exit - soon" )

class X_plus_vbox( gtk.VBox ):
	""" rename scrolled vbox """
	def __init__( self ):
		gtk.VBox.__init__( self )

	def add( self, btn ):
		gtk.VBox.add( self, btn )

class plus_vbox( gtk.ScrolledWindow ):
	""" rename scrolled vbox """
	def __init__( self ):
		gtk.ScrolledWindow.__init__( self )
		self.set_hscrollbar_policy()
		self.vbox = gtk.VBox()
		# self.add( self.vbox )
		self.add_with_viewport( self.vbox )
		# NO # self.set_property("shadow-type", gtk.SHADOW_NONE )
		# NO # self.set_border_width(0)
		# NOT # FB # self.set_has_frame(False)
		# TOPLEVEL ONLY # self.set_decorated(False)

	def set_hscrollbar_policy( self ):
		V = gtk.POLICY_AUTOMATIC
		H = gtk.POLICY_ALWAYS
		n = gtk.POLICY_NEVER
		# H = n
		self.set_policy( H, V )

	def add( self, btn ):
		# gtk.VBox.add( self, btn )
		self.vbox.add( btn )

	def set_width( self ):
		pass

class plus_btn( gtk.Button ):
	def __init__( self, label, cmd ):
		if 1:
			# gtk (?pygtk?) uses "_" to underline fast key letters
			label = label.replace("_", "__")
		if 1:
			gtk.Button.__init__( self, label )
		else:
			# this didnt work
			gtk.Button.__init__( self )
			self.label = label
		self.cmd = cmd
		self.connect("clicked", cb_plus_btn_pressed)
		self.show()

	def cb_clicked( btn ):
		# print( "PLUS btn =", btn )
		# print( "PLUS msg =", btn.msg )
		print( "PLUS cmd =", btn.cmd )
		# call the callback
		# pass the btn so that it can be annotated
		# pass other stuff (the original X11 event) (or not)
		btn.cmd.run_now( btn )
		# print( "window =", gbl.window_main )

class btn_factory:

	def __init__( self, window ):
		self.curr = None
		self.window = window
		vbox = plus_vbox()
		self.vbox = vbox
		window.add(vbox)
		vbox.show()

	def new_btn( self, label, cmd ):
		button = plus_btn( label, cmd )
		self.vbox.add( button )
		# button.connect("clicked", cb_plus_btn_pressed)
		# button.msg = "Whatever-II"

		if 1:
			self.set_colours( button )

		return button

	def new_btn_Runnable( self, cmd ):
		# print( "cmd =", cmd )
		# label = cmd.get_label()
		btn = self.new_btn( cmd.get_label(), cmd )
		str = cmd.get_tooltip()
		gbl.tt.set_tip( btn, str, None )
		gbl.tt.enable()

		# this shows how a factory uses styles from pebbles
		# just as the cmd_factory uses run_in_xterm style
		# yes this is leading to fragmentation 
		btn.set_property("xalign",0)
		return btn

	def set_colours( self, b ):
		
		colr_green  = gtk.gdk.color_parse( "green")
		colr_purple = gtk.gdk.color_parse( "purple")
		colr_blue   = gtk.gdk.color_parse( "blue")
		colr_yellow = gtk.gdk.color_parse( "yellow")
		colr_white  = gtk.gdk.color_parse( "white")
		
		b.modify_bg( gtk.STATE_NORMAL,      colr_green )	# 
		b.modify_bg( gtk.STATE_ACTIVE,      colr_purple )	# 
		b.modify_bg( gtk.STATE_PRELIGHT,    colr_blue )	# 
		b.modify_bg( gtk.STATE_SELECTED,    colr_yellow )	# 
		b.modify_bg( gtk.STATE_INSENSITIVE, colr_white )	# 

def create_window_main():
	gbl.window_main = gobject.new(
		gtk.Window,
		type=gtk.WINDOW_TOPLEVEL,
		title='Menu_App.py',
		allow_grow=False, # gtk.FALSE,
		allow_shrink=False, # gtk.FALSE,
		border_width=10
	)
	gbl.window_main.connect("destroy", cb_destroy)

	# no effect - 
	# gbl.window_main.set_default_size( 540, 480 )

	# this worked
	gbl.window_main.set_geometry_hints(
		min_height=480,
		min_width=200,
		max_height=2048,
		max_width=2048
	)
	
	s1 = gobject.new( gtk.Style )
	# s1.modify_bg( "green")
	# NO # f1 = gobject.new( gtk.Font )

	panel = btn_factory( gbl.window_main )

	# /usr/bin/env
	#	argv0 is 'env'
	#	argv1 is future argv0
	#	argv2 ...

	# searched/xterm -t ... -e prog0 a1 a2 a3

	# press_return_to_continue_at_end = wrapped within xterm!

	# stdout to text widget { own or shared ? }

	# PID to collector of PIDS, CTRL-C-ALLOWED

	# filename ==> tooltip{ MIME_PROG ... }

	# RUN_ON_AS_IN {
	#
	# rsh ... xauth-xterm ... prtc
	#
	# }

	# ENV { set_env_to_SOMETHING }

	# ASK_DO ...

	cmd = Runnable()
	cmd.cmd_exe = "/etc/ppp/dial_ppp" # not in argv but often duplicated in argv0
	cmd.cmd_name = None # argv0
	cmd.cmd_args.append( "xlog" )
	cmd.btn_label = None # so dump argv
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/etc/ppp/dial_ppp"
	cmd.cmd_args.append( "cut" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/etc/ppp/dial_ppp"
	cmd.cmd_args.append( "id" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/etc/ppp/dial_ppp"
	cmd.cmd_args.append( "uklinux" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/usr/bin/env"
	cmd.cmd_args.append( "ASK_DO sudo" )
	cmd.cmd_args.append( "init" )
	cmd.cmd_args.append( "0" )
	panel.new_btn_Runnable( cmd )

	# cd_dir = HOME/2000/...
	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "abiword" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "xdosemu" )
	panel.new_btn_Runnable( cmd )

	# cd_dir = HOME/audio/mp3_V4 or ..
	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "xmms" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/home/gps/2000/src/SCRATCH_SCRIPTS/in_TCL/tcl_eth_bytes/proc_net_ppp.tcl"
	cmd.cmd_name = "proc_net_ppp.tcl"
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "gdf.tcl" )
	panel.new_btn_Runnable( cmd )

	d1 = "/home/gps/2000/src/pygtk_menu_app"
	cmd = Runnable()
	cmd.cmd_exe = d1 + "/menu_app.py"
	cmd.cmd_name = "menu_app.py"
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.btn_label = "mc menu_app.py not func"
	cmd.cmd_exe = "/usr/bin/xterm"
	cmd.cmd_args.append( "-e" )
	cmd.cmd_args.append( "mc" )
	cmd.cmd_args.append( d1 )
	cmd.cmd_args.append( "/tools/2000/py" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable_xterm_mc_d1_d2( d1, "/tools/2000/py" )
	panel.new_btn_Runnable( cmd )

	for d in [
		"/home/gps/2000/Accts_CD_Prep_1",
		"/home/gps/2000/Accts_CD_Prep_1/src_PRIV_Company",
		"/home/gps/2000/Accts_CD_Prep_1/src_PRIV_Company/PRIV_paye/P11_pay",
		"/home/gps/2000/Accts_CD_Prep_1/src_PRIV_Company/PRIV_CH_CSV",
		"/home/gps/2000/Accts_CD_Prep_1/src_PRIV_Company/PRIV_samples",
		"/home/gps/2000/Accts_CD_Prep_1/src_PRIV_Company/2006-2007/images",
	]:
		panel.new_btn_Runnable( Runnable_xterm_mc_d1_d2( d ))

	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "xdosemu" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "tv_rem" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "xmms" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "alevt" )
	panel.new_btn_Runnable( cmd )

	cmd = Runnable()
	cmd.cmd_exe = "/bin/sh"
	cmd.cmd_args.append( "-c" )
	cmd.cmd_args.append( "wireshark" )
	panel.new_btn_Runnable( cmd )

	l1 = [ 
		"py",
		"libs",
		"mbr",
	]
	for d in l1:
		panel.new_btn_Runnable( Runnable_xterm_mc_d1_d2(
			"/tools/2000/" + d
		))

	l1 = [ 
		"HTDOCS",
		"ftp_2006",
		"ftp_2007",
		"Office_2006",
		"HOME_2006",
		"src_PRIV",
	]
	for d in l1:
		panel.new_btn_Runnable( Runnable_xterm_mc_d1_d2(
			"/home/gps/2000/" + d
		))

	l1 = [ 
		"src_PRIV",
		"X", "X", "X", "X", "X", "X", "X", "X",
		"X", "X", "X", "X", "X", "X", "X", "X",
		"X", "X", "X", "X", "X", "X", "X", "X",
	]
	# if True:
	if False:
	 for d in l1:
	  panel.new_btn_Runnable( Runnable_xterm_mc_d1_d2(
	  "/home/gps/2000/" + d
	  ))

	l1 = [ 
		"scan",
		"libs_apps",
		"libs_apps/src",
		"SPIN_py",
		"IDEAS",
		"SCRATCH_SCRIPTS",
		"mp3_hissy_fit",
		"HTDOCS",
		"HTDOCS/SNAKE",
	]

	for d in l1:
		panel.new_btn_Runnable( Runnable_xterm_mc_d1_d2(
			"/home/gps/2000/src/" + d
		))


	gbl.window_main.show_all()

# GTK_STATE_NORMAL
# State during normal operation.
# GTK_STATE_ACTIVE
# State of a currently active widget, such as a depressed button.
# GTK_STATE_PRELIGHT
# State indicating that the mouse pointer is over the widget
# and the widget will respond to mouse clicks.
# GTK_STATE_SELECTED
# State of a selected item, such the selected row in a list.
# GTK_STATE_INSENSITIVE
# State indicating that the widget is unresponsive to user actions. 

create_window_main()
gtk.main()

