#!/usr/bin/env python

enc1 = "utf-8" # default
enc2 = "latin1" 

list_enc = [ enc1, enc2 ]

list_pairs = [
	("e_acute ","é"),
	("n_titlde","ñ")
]
for e in list_enc:
 for (name,s) in list_pairs:
 	b = s.encode(e)
 	print("%s '%s'.encode(%s) STR --> BYTES %s" %( name, s, e, b ))
 	continue

 	print("\nenc %s" %(e) )
 	print("name %s str '%s' type %s" %( name, s, type(s) ))
 	print("%s s.encode(%s)  (%s) type %s" %( name, e, b, type(b) ))
