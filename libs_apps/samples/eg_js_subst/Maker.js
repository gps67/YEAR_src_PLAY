#!/usr/bin/env js_many

ACCTS = new Accts_Module()
// throws // ACCTS.make( "/home/gps/2000/src/libs_apps/_run", "will_fail" ) || ACCTS.exit()
ACCTS.make( "/home/gps/2000/src/libs_apps/_run", "js_many" ) || ACCTS.exit()
ACCTS.make( "/home/gps/2000/src/libs_apps/_run", "js_many" ) || ACCTS.exit()
// ACCTS.make( "/home/gps/2000/src/libs_apps/_run", "lots" ) || ACCTS.exit()

