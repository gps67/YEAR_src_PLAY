#!/usr/bin/env python

# exp and log # log natural # e not 10 #
from math import exp, log

# pick 2 genuinely representative points from your exp graph
# nb there is no arbitrary zero, zero is zero, (subtract it first before ratio)
# avoid divide by zero, by avoiding flat lines
# write numbers as floats, to save confusion

million = 1000.0 * 1000 
billion = 1000.0 * 1000 * 1000

t2=2018.0
v2=600.0

t1=1980.0
v1=100.0

t2 = 2022.0
v2 = 2.09 * million # meme claims house price

t1 = 1976.0
v1 = 58 * 1000.0

# here is how you compute the n'th root of ratio

n_years = t2 - t1 # unit of time is year
factor_n_years = v2 / v1 # total ratio factor of growth, over n years

factor2 = 2.0 # to calculate doubling time
log_factor2 = log( factor2 ) # a constant

log_factor_n_years = log( factor_n_years )
log_factor_1_year = log_factor_n_years / n_years # nth root
factor_1_year = exp( log_factor_1_year )

n_years_doubling = log_factor2 / log_factor_1_year
rate_percent = (factor_1_year * 100.0) - 100.0

print
print ("A %3.1f %% inflation rate doubles every %4.1f years" % (rate_percent, n_years_doubling ))
print ("Over %3.1f years costs multiplied by %4.1f" % (n_years, factor_n_years))
print

# if you want to start with the percent
# then simple algebra works on the above
# to compute the nth power of 1.03 (from 3%)


# nb 'e' is 2.71 which is the compound interest version of 2.0 (100%)

exit()

print (rate_percent, "rate_percent")
print (n_years_doubling, "n_years_doubling")
print (n_years, "n_years")
print (factor_n_years, "factor_n_years")

exit()

rate_percent = 1
factor_1_year = (100.0 + rate_percent) / 100.0
log_factor_1_year = log( factor_1_year )

n_years = 10 * 1000
log_factor_n_years = log_factor_1_year * n_years
factor_n_years = exp ( log_factor_n_years )

print (rate_percent, "percent ***", n_years, "n_years, factor_n_years =", factor_n_years)

v2 = 10 * billion
v1 = 1.0
factor_n_years = v2 / v1
n_years = n_years

log_factor_n_years = log( factor_n_years )
log_factor_1_year = log_factor_n_years / n_years
factor_1_year = exp( log_factor_1_year )
rate_percent = (factor_1_year * 100.0) - 100.0
print (rate_percent, "percent ***", n_years, "n_years, factor_n_years =", factor_n_years)
