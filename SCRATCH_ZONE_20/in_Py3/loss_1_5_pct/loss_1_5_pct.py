#! /usr/bin/env python3

pct_1_5 = 1.5
pct_98_5 = 100.0 - pct_1_5
iters = 20
val_initial = 50.0
for iters in range(1,41):

	val_after = val_initial * ( pct_98_5 / 100.0 ) ** iters
	print(" %d %6.3f" % ( iters, val_after ))

