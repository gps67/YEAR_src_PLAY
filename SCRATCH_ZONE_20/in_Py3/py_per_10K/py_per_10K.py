#!/usr/bin/env python

million = 1000000.0

n_million = 330.0
population = n_million * million
n1 = 450

n_per_population = n1 
n_per_million = n1 / n_million
n_per_100K = n_per_million / 10.0
n_per_10K = n_per_million / 100.0
n_per_cent = n_per_10K / 100.0

def print1( flt, str ):
	print( "%7.5f %s" % (flt, str ) )

print1( n_per_population, "n_per_population" )
print1( n_million, "population in millions" )
print1( n_per_million, "n_per_million" )
print1( n_per_100K, "n_per_100K" )
print1( n_per_10K, "n_per_10K" )
print1( n_per_cent, "n_per_cent" )

