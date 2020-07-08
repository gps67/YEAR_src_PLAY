cddb_live=audio.cddb
cddb_temp=audio.cddb_
. ./.env1
echo "Running cd-diskid gets the cddb sign and track times"
./make_new_cddbfile.py > $cddb_temp
hissy_fit --cddb_summary $cddb_temp
ask_do mv $cddb_temp $cddb_live
