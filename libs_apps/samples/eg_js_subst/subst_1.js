#!/usr/bin/env js_many

ACCTS = new Accts_Module()
ACCTS.make( "/home/gps/2000/src/libs_apps/_run", "js_many" ) || ACCTS.exit()


f1 = "tmp_f1.txt"
f2 = "tmp_f2.txt"
dry = new Array()
dry[ "a1" ] = "one"

ACCTS.subst_dry_f1_f2( dry, f1, f2 )

