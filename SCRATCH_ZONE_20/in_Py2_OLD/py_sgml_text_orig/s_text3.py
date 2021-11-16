#!/usr/local/bin/python

from s_aopts import *
import os

class scan1:
	"""	a node being parse scanned from ESIS
		a queue in use
	"""

	def	__init__(self,txt, fd=-1):
		self.fd = fd
		self.lv = -1
		self.ln = 0
		self.txt = txt
	
	def scan_top_zone(self):
		str = self.fd.readline()
		if str == '': return None
		c = str[0]
		str = str[1:-1] # trim nl
		if   c == '(' :
			obj = self.scan_rend_sub_zone(str,[])
			return obj
		else:
			print "ERROR initial immediate ( expected"
			return None

	def scan_rend_sub_zone(self, name, my_atts):
		global is_pre
		atts = 	A_opts_list() # collected or sub-zone
		list = []	# sub-items or text found
##		print "scan_sub_zone", name
#		print "scan_sub_zone", name
		self.txt.zone_start( name, atts ) # HMMM

		while 1:
			str = self.fd.readline()
			if str == '': break
			c = str[0]
			str = str[1:-1] # trim nl
			if   c == '(' :
				self.txt.zone_start( str, atts ) 
				atts.set_null()
			elif c == ')' :
				self.txt.zone_end()
			elif c == '-' :
				self.txt.puts( str )
			elif c == '+' :
				self.txt.esis_newline()
				self.txt.puts( str )
			elif c == 'A' : 
				atts.add_from_esis_line(str)
			elif c == 'C' : return
			else:
				print "c = %s" % c
				print "s = %s" % str
		print "This code should never be reached"
		print "ILLEGAL BREAK IN WHILE LOOP"

global tk
tk = 9
if 1:
	from s_text2 import *
        W0 = Tk()
	tk = W0.tk
        B1 = Button( W0, { 'text': "Exit", 'command': sys.exit }) # OK
        B1.pack({'expand': 0, 'fill': 'x', 'side': 'top'})
        W1 = S_text2( W0 )
	W1.frame.pack({'expand': 1, 'fill': 'both', 'side': 'left'})
        W1.tag_setup()
#	fd = open( "ssr01.spout", "r" )
#	fd = open( "fa", "r" )
	filename = "ssr01.sgml.gz"
	seds="| sed -e 's/\\\\n/ \r+/g'| tr '\\015' '\\012'"
	cmd = "zcat "+filename+" | pr -t -e | nsgmls -D/home/gps/raven/dtd -C catalog1" 
	cmd = cmd + seds
#	os.system( cmd  +"|less")
#	exit
#	cmd = "zcat "+filename+" | nsgmls -D/home/gps/raven/dtd -C catalog1"
	print cmd
	fd = os.popen( cmd,  "r" )
	txt = W1

	scan = scan1(txt,fd)
	print "Creating widget.txt from fifo"
#	import profile
	obj = scan.scan_top_zone()
	fd.close()
	print "Loaded obj"

	#W1.test2()

	W0.mainloop()

	# NEVER REACHED

	if 0:
	    f = open( "ssr01.pick", "w" )
	    p = pickle.Pickler(f)
	    print "dumping"
	    p.dump(obj)
	    f.close()

if 0:
    g = open( "ssr01.pick", "r" )
    p2 = pickle.Unpickler(g)
    print "loading"
    obj = p2.load()
    g.close()



#profile.run("obj = scan.scan_top_zone()", "pro-file")
#obj = scan.scan_top_zone()
print "render"
obj.render('render','CtXt')
print "-"*40
#obj.render('render_sgml','CtXT')
print "render_tk"
obj.render('render_tk','CtXT')
# obj.prints()

"""
	word up qt_py people, you SHALL use SGML to markup your email!
	and also use SGML as a databse ! Stop laughing, this is serious!!

	But first I've go a pretty printer to sort out, then ...
"""
###
"""
	And don't forget that your run the python program to teach the
	robot what to do at certain times, it only runs once as python,
	the next time it runs as it's own FSM. However by sticking to the
	provided verbs you get the functionality (and class via dictionary)
	for more information, write the source
"""
