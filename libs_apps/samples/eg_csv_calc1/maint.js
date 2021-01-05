#!/usr/bin/env js_accts

/*
	Eight flats share the insurance and maintenence costs
	of a 110 year old sand-castle. How much do they have to
	pay each month to prevent it crumbling?
*/

ACCTS = new Accts_Module()

dset = ACCTS.add_dset()

COSTS = dset.add_table( "COSTS" )
COSTS.add_type_col( "key", "key")
COSTS.add_type_col( "str", "amount")
COSTS.add_type_col( "percent", "percent")

cost_list = new Array()
cost_sum = 0.0

ins = 2208.0

function COST_ITEM( key, amnt )
{
	this.key = key
	this.amnt = amnt
}

function COST( key, amnt )
{
	cost_list.push( new COST_ITEM( key, amnt ))
	cost_sum += amnt
}

/*
	The AA say a car cost 3-4 K
*/

COST( "AXA-Ins", ins/8 )
COST( "maintenence", 75 * 12 - ins/8 )
COST( "MortgagueAssur", 12 * 80 )
COST( "MortgagueInt", 60 * 1000 * 0.05 )
COST( "MOT.DISC", 110 + 390 )
COST( "Car Loan", 52 * 5 )
COST( "tpft", 300 )
COST( "Petrol", 52 * 15 )
COST( "Food", 52 * 30 )
COST( "counciltax", 600 )
COST( "gas_elec_water", 365 + 20.21 * 8 )
COST( "phoneline", 120 )
COST( "TVPlus", (9.33 *12) + 12*5 )

COST( "Other", 10 * 1000 - cost_sum )

fixed = cost_sum
COST( "total", cost_sum )
cost_sum = fixed

for( i in cost_list ) {
	cost = cost_list[i]
	row = COSTS.add_row( cost.key )
	row.key = cost.key
	row.amount = Math.round( cost.amnt )
	row.percent = Math.round( cost.amnt * 100 / cost_sum )
	// row.percent = cost.amnt * 100 / cost_sum 
}

MAINT = dset.add_table( "MAINT" )
MAINT.add_type_col( "key", "key")
MAINT.add_type_col( "str", "SO")
MAINT.add_type_col( "str", "Y1")
MAINT.add_type_col( "str", "maint")
MAINT.add_type_col( "str", "year")
MAINT.add_type_col( "str", "total")
MAINT.add_type_col( "str", "Job1")
MAINT.add_type_col( "str", "Job2")
MAINT.add_type_col( "str", "Job3")
MAINT.add_type_col( "str", "Job4")
MAINT.add_type_col( "str", "Job5")

function P( amnt )
{
	return Math.round( amnt )
}

function Calc_Monthly( m1 )
{
	var row = MAINT.add_row("+") // gets an anonymous key
	m2 = m1 - i2
	row.SO		 = m1
	row.Y1		 = m1 * 12
	row.maint	 = m2 
	row.year	 = m2 * 12
	sum              = m2 * 96
	row.total	 = sum
	row.Job1	 = sum / 2
	row.Job2	 = sum / 4
	row.Job3	 = sum / 8
	row.Job4	 = sum / 16
	row.Job5	 = sum / 16
}

i2 = ins / 96
Calc_Monthly( i2 )
step = 10
step = 5
for( var m1 = 90; m1 >= 45; m1 -= step ) {
	Calc_Monthly( m1 )
}

dset.save_file("maint.csv")






