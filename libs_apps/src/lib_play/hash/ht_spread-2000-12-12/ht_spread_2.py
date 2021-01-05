#!/usr/bin/env python

import random

def rand( xs ):
	v = int( xs * random.random() )
	if( v == xs ): v = rand( xs )
	return v

def factorial( i ):
	result = 1.0
	while i>0:
		result *= i
		i -= 1
	return result

def p_zero():
	p0 =( nslot - 1 ) / nslot
	return p0 ** nitem

def experiment( nitem, nslot, show1, show2 ):
	# nitem *= 1.0
	# nslot *= 1.0

	slots = []
	for item in range(0, nslot): slots.append( 0 )

	# spray nitem values into nslot slots
	for item in range(0, nitem):
		s = rand( nslot )
		slots[ s ] += 1

	counts = []
	for item in range(0, nitem+1): counts.append( 0 )

	slots.sort()
	for s in range(0, nslot):
		counts[ slots[s] ] += 1

	sum = 0
	sumskips = 0.0
	for chainlen in range( 0, len(counts) ):
		slotcount = counts[chainlen]
		if slotcount:
			items = chainlen * slotcount
			sum += items
			sumskips += (chainlen-1)/2.0 * items
			if show1:
				print "chainlen = %2d slotcount = %3d items = %3d sum = %3d" % ( chainlen, slotcount, items, sum )
	avgskips = sumskips / nitem
	ratio = 2.0 * avgskips / (nitem/nslot)
	if show2:
		print "nitem/nslot = %3d/%3d = %4.1f avgskips = %5.2f ratio=%4.2f" % (
			nitem,
			nslot,
			nitem/nslot,
			avgskips,
			ratio
		)
	return avgskips

experiment(  10, 10, 0, 1 )
experiment( 100, 10, 0, 1 )
experiment( 400, 10, 0, 1 )
nslots = 50
for weight in [ 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.5, 3.0, 4.0, 5.0 ]:
	nitems = nslots * weight
	experiment( nitems, nslots, 0, 1 )

# guess what - it all adss up to avg len = items/slots

# print "p_zero =", p_zero()
# print "factorial(nslot) =", factorial(nslot)

