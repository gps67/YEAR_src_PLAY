#!/usr/bin/awk -f
#!/usr/bin/gawk -f

# fortunately the data starts at day 0 of 1979
# but the average for the current incomplete year will be wrong


BEGIN {

	# SET NAMES OF FIELDS IN A TABLE
# LOOKING FOR # create an array of strings
# https://www.gnu.org/software/gawk/manual/html_node/Arrays.html

	COL_NAME[0] = "YEAR_MM_DD"
	COL_NAME[1] = "CAR_MILES_was"
	COL_NAME[2] = "CAR_MILES_did"
	COL_NAME[3] = "CAR_MILES_now"
	COL_NAME[4] = "PENCE_PER_MILE"
	COL_NAME[5] = "label"
	COL_NAME[6] = "POUNDS_per_LITRE"
	COL_NAME[7] = "label"
	COL_NAME[8] = "POUNDS_per_GALLON"
	COL_NAME[9] = "label"
	COL_NAME[10] = "MPG"
	COL_NAME[11] = "label"
	COL_NAME[12] = "lp100"
	COL_NAME[13] = "label"
	COL_NAME[14] = "had_receipt" # ie not a guess
	COL_NAME[13] = "CMNT MULTI-WORD fields all the way to EOLN" # TEXT

	process_data_file()
}
# that was BEGIN { SCRIPT }

function process_data_file()
{
	F = "mpg_table.txt"

	CMD = "zcat " F 
	CMD = "cat " F 
	LNO = 0
	while(( CMD | getline ) > 0 ) {
		LNO ++
		if(LNO == 1 ) continue
		# if( LNO > 7) break

		YEAR_MM_DD = $1
		CAR_MILES_was = $2
		CAR_MILES_did = $3
		CAR_MILES_now = $4
		PENCE_PER_MILE = $5
		label = $6
		POUNDS_per_LITRE = $7
		label = $8
		POUNDS_per_GALLON = $9
		label = $10
		MPG = $11
		label = $12
		lp100 = $13
		label = $14
		had_receipt = $15

		print( YEAR_MM_DD miles, PENCE_PER_MILE )
		
		# convert day of year to fraction of year
	}
	exit
}

{
	print "## EVERY LINE"
	exit(22)
}
