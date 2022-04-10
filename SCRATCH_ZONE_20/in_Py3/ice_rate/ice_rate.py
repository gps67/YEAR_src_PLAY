#!/usr/bin/env python3

class y_mx_c:
	def __init__( self, _m, _c ):
		self.m = _m
		self.c = _c
	
	def y_from_x( self, x ):
		y = self.m * x + self.c
		return y
	
	def x_from_y( self, y ):
		x = ( y - self.c ) / self.m
		return x

def line_from_x1_y1_x2_y2( x1, y1, x2, y2 ):
	dx = x2 - x1
	dy = y2 - y1
	dy_dx = dy / dx
	y0 = y1 - x1 * dy_dx
	return y_mx_c( dy_dx, y0 )

t1=2011.878
t2=2014.634
t3=2020.662

v1=13.742
v2=15.05
v3=13.5855

# find nearest MAX and MIN to t3 # manually
v3_hi =  22.961 # 2020.29
v3_lo = 4.026 # 2020.72

v3_lift = v3_hi - v3 # +ve addition
v3_drop = v3_lo - v3 # -ve addition

t12 = (t1 + t2) / 2.0
v12 = (v1 + v2) / 2.0

line_mid = line_from_x1_y1_x2_y2( t12, v12, t3, v3 )
line_hi  = line_from_x1_y1_x2_y2( t12, v12+v3_lift, t3, v3+v3_lift)
line_low = line_from_x1_y1_x2_y2( t12, v12+v3_drop, t3, v3+v3_drop)

def pnt( val, desc ):
	print("%7.2f %s" % ( val, desc ))

def pnt2( X, Y, desc ):
	print("%7.2f %5.2f  %s" % ( X, Y, desc ))

pnt2( t1, v1, "left low" )
pnt2( t12, v12, "left mid - calculated" )
pnt2( t2, v2, "left high" )
pnt2( t3, v3, "right recent" )
print("")

pnt( line_mid.x_from_y( v12 ) , "t_mid calc" )
pnt( line_mid.y_from_x( t12 ) , "v_mid calc" )

pnt( line_low.x_from_y( v12 ) , "t_low calc" )
pnt( line_low.y_from_x( t12 ) , "v_low calc" )
print("")

T_LEFT = 2016
T_LEFT = 2008
T_LEFT = 2000
V_HI_LEFT = line_hi.y_from_x( T_LEFT )
V_MID_LEFT = line_mid.y_from_x( T_LEFT )
V_LOW_LEFT = line_low.y_from_x( T_LEFT )

V_ZERO = 0
T_HI_ZERO = line_hi.x_from_y( V_ZERO )
T_MID_ZERO = line_mid.x_from_y( V_ZERO )
T_LOW_ZERO = line_low.x_from_y( V_ZERO )

pnt2( T_LEFT,  V_HI_LEFT, "T V _ HI_LEFT" )
pnt2( T_LEFT, V_MID_LEFT, "T V _MID_LEFT" )
pnt2( T_LEFT, V_LOW_LEFT, "T V _LOW_LEFT" )
print("")
pnt2(  T_HI_ZERO, V_ZERO, "T V _ HI_ZERO" )
pnt2( T_MID_ZERO, V_ZERO, "T V _MID_ZERO" )
pnt2( T_LOW_ZERO, V_ZERO, "T V _LOW_ZERO" )
print("")

dv = ( v3 - v12 ) 
dt =( t3 - t12 )
dv_dt = dv / dt
pnt( dv_dt, "dv_dt" )
pnt( line_mid.m, "line_mid dv_dt" )

v_zero_mid = 0
t_zero_mid = t3 + (v_zero_mid-v3 / dv_dt )
v_zero_lo = 0
v3_lo = v3 - 8 # a guess
t_zero_lo = t3 + (v_zero_lo-v3_lo / dv_dt )
t_1970_mid = 1970
v_1970_mid = (t_1970_mid - t_zero_mid) * dv_dt
t_1970_lo = 1970
v_1970_lo = (t_1970_lo - t_zero_lo) * dv_dt + v3_lo

T1 = 2008
T2 = t_zero_lo
VM2 = (T1 - t_zero_lo) * dv_dt
VL2 = (T1 - t_zero_lo) * dv_dt + v3_lo
VM1 = (T1 - t_zero_lo) * dv_dt
VL1 = (T1 - t_zero_lo) * dv_dt + v3_lo

print("")
pnt( t1, "t1" )
pnt( v1, "v1" )
print("")
pnt( t2, "t2" )
pnt( v2, "v2" )
print("")
pnt( t3, "t3" )
pnt( v3, "v3" )
print("")
pnt( t_1970_mid, "t_1970_mid" )
pnt( v_1970_mid, "v_1970_mid" )
print("")
pnt( t_1970_lo, "t_1970_lo" )
pnt( v_1970_lo, "v_1970_lo" )
print("")
pnt( T1, "T1 pencil " )
print("")
pnt( VM1, "VM1 pencil " )
pnt( VL1, "VL1 pencil " )
print("")
pnt( t_zero_mid, "YM2 t_zero_mid - zero ice" )
pnt( t_zero_lo, "YL2 t_zero_lo - zero ice" )
