#! /usr/bin/env python

# approx weight of atmosphere as feet
atm_water_feet = 33
atm_water_mm = atm_water_feet * 12 * 25.4
million = 1000.0 * 1000.0
co2_rate = 420.0 / million

co2_water_mm = co2_rate * atm_water_mm

print("co2_water_mm", co2_water_mm )

