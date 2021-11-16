<fa awk '
BEGIN {
	N = 0
	SUM = 0.0
	N2 = 0
	SUM2 = 0.0
	avg = 0
	avg2 = 0
	hz = 0
}
{
	v = $5
	# 2/3 of reading are below 0.7 sec (avg 0.65) and worked OK
	if( v < 700000 ) {
		SUM = SUM + v
		N = N + 1
		avg = SUM * 1.0
		avg = avg / N
		hz = 1000000.0/avg
	} else {
		SUM2 = SUM2 + v
		N2 = N2 + 1
		avg2 = SUM2 * 1.0
		avg2 = avg2 / N2
	}
	printf("%7d %4d %9.4f %4d %9.4f %9.4f \n", v, N, avg, N2, avg2, hz )
}'
