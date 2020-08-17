
# GLOBAL - move to namespace X

set G_counter_N 0
proc G_counter {pfx} {
 global G_counter_N
 set G $G_counter_N
 incr G_counter_N
 return "$pfx$G"
}

proc mk_id {} { G_counter id_ } ;# id_99
proc mk_idx {} { G_counter } ;# it is actually a str %d idx 99
