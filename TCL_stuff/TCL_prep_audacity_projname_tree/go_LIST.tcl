#!/usr/bin/env tclsh9

# find all the scripts
set D1 /mnt/sda2/x79_A2/G/YEAR_TREE_2025/REC/REC_2025/REC_AUD_2025/REC_AUD_2025_08_Aug/WHERE_THE_SCRIPTS_ARE
set D1 .
lappend auto_path $D1 .

MODE DONE
MODE TODO

L0 - A_DIR This ROOT dir is almost outside UDEF
# A_DIR is actually created in /tmp/ (see script)
# so beware of reboot losing ALL your work

# we require all previous headings or JUMP past N
# we use a SWITCH MODE to skip already done areas

L1 1 L1 ARTIST number _PROVIDED_ 

P A paragraph of descriptive text - goes where 

 H2 - H2 ALBUM dir dash meant auto number

   T3 - T3 TITLE of the track and TITLE of PROJECT
   T3 - level 3 auto numbering
   T3 - actual text here

# TITLE of TRACK is also projname as TITLE.aup
# suggest you save as .aup3
# suggest you touch a tile called _projname.aup3 
# and be aware of GUI offering list that ENTER picks one of !!

# PROJECT is per TRACK not per ALBUM
# you can change that

P Another paragraph of descriptive text - goes where 
# basically ignored but it spans the void of tree to document

 H2 - LEVEL 2 dir dash meant auto number

   T3 - etc
   T3 - etc etc
MODE DONE
   T3 - etc etc skip though

# can do this anythime at T3
calc_file_name ;# prints it out

puts "#- EOF -#"
