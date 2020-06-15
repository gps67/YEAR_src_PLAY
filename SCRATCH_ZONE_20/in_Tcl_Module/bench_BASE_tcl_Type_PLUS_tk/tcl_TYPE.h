#ifndef tcl_TYPE_H
#define tcl_TYPE_H

# or move this code aboce
#include "tcl_N_bytes" // AUTO.h tcl has own N_str_
namespace tcl_TYPE { # one of 256 UDEF BASE tcl_TYPE_t

# import some modules into SCOPE = up_space // before namespace
# or move this code aboce

 # FRAG GEN # %s_t # FILTER VIEW REVIEW
 struct UDEF_tcl_TYPE_from_Tcl_Obj_HEAD
 	Tcl_Obj * _Tcl_Obj; // PLUS SELF WORD OBJ2 "Alias"
	// ASSERT_NO_MORE_DATA // WORD is WORD is_ PTR2
	 UDEF_tcl_TYPE_from_Tcl_Obj_HEAD( Tcl_Obj * _Obj )
	 : _Tcl_Obj( _Obj )
	 {
	 	// DO NOTHING oafter that assignment
	 }

	 UDEF_tcl_TYPE_from_Tcl_Obj_HEAD()
	 : _Tcl_Obj( NULL )
	 {
	 	// maybe tell debugger // entire parallel build
		// pupular inline PREBUILT // switched // fast wrap CALLA CALLB
	 }

	 /*

 typedef struct Tcl_Obj
 {
	/*
		use the inline C/C++ refcount macro functions
		ref_incr();
		ref_decr();
		which use the same as TCL 8.6 // and ancients too 8.0

		nb Tcl defers actual DTOR until next REPL_SCRIPT_STATEMENT
		so cann pass zero ref thing around on stack
		but it is better to match python and ref_incr on stack
		opt add to snow_plough_list # keep_until
		snow_plough_list = SP_safe_stack_sync_list_drop_later
		sync_list += flush_sync_list_item // call_back_delete_me_now

		SCRIPT_STEP {
		 keep_list = new_keep_list( the_old_one ) // 
		 keep_list.get_space( N_items ) // 1024 // reuse page
		 keep_list.add_all_new_items_to_keep_list() // register

		 callback add_item( item ) {
		  u32 idx = N_items ++; // [0[i]n [0[n
		  // [0[i[n
		  // from [0] upto not incl n
		  // DIAG = AUTO ITEM [0] [i] but not [N] 
		  // DIAG P0P2 but as IDX0 IXD2 = IDX0 + N
		}

		 callback add_item_to_extra_list( item ) {
		 }

		... script step does it's stuff
		# add new ITEM via ALLOC to keep_list
		# sometimes
		# add ITEM to PLUS_LIST # fast_safe_exit
		# add ITEM to TASK_LIST # nudge them occasionally

		foreach idx item plus_list {
		 not_ref_decr_not_final( item ) // flush buffers // SCRIPT
		 # DONT item = NULL ; // rest of object still needs recycling
		 # plus_list is a fast_safe_exit # with plan_ahead # !!
		 # only a few items subscribe to going out of SCOPE
		 # CACHE can age components, keeping advantage
		 # PREP graceful_close_messages # QUEUE #dd
		 # 
		}
		keep_list.N_items = 0; // tis now

		foreach idx item keep_list {
		 ref_decr_final( item )
		 item = NULL ; // DROP from keep_list by overwriting MEM
		}
		keep_list.N_items = 0; // tis now

	*/
	int refCount;

	char *bytes;
	int length;

	// int length; // USES 4 GETS 8
	// means there is an exploitable HOLE_u32

	const Tcl_ObjType *typePtr;

		/*
		 const Tcl_ObjType *typePtr;
		 const Tcl_ObjType_UDEF_me *typePtr_CTBL;
		 // CTBL is VTBL in any dialect of C 
		 // XTBL is X_any
		 // STBL is SYMBOL // from_lookup(STR0)
		 // CAST from Tcl_ObjType // RAW Tcl API into _ENHANCED
		 // TEST_can_CAST // KNOW_ _ISNT _CANT _INTO _FROM
		 // BASE_from_import _inherit _derive _refer_
		*/

	union {
	  // TWO WORDS PTR1 PTR2 
	  // or any other use of that many bytes
	  // N_bytes but bytes_N_bytes STR_N BUF_N not N_STR 

	  // u32 only uses first HALF_WORD
	  long longValue;

	  // f64 uses first WORD // second SPACE for you :-)
	  double doubleValue;

	  // strange alias added to union, sample madness
	  void *otherValuePtr;

	  // errm RTFM the builtin types available
	  Tcl_WideInt wideValue;

	  // TWO WORDS as TWO PTRS
	  // WORD OPCODE
	  // WORD PAYLOAD
	  // -pair-
	  // WORD EXPR_lhs
	  // WORD EXPR_rhs
	  // ---- note lhs rhs are REALLY limited, no extra PLUS core SELF 
	  // ---- tcl_TYPE picks DECODER == "OPCODE LHS RHS // CODE_POINT"
	  // ---- OPCODE = OPCODES[ u3 ] --- ZERO means SELF_ID_BIND_ZAP
	  // ---- OPCODE = u3 += CODE_POINT _HERE --- ONE means ITEM( key )
	  // ---- OPCODE = u3 += CODE_POINT _HERE --- ONE means CTOR( SCRIPT }
	  // SCRIPT = _HERE.SCRIPT // self evidential declarations float
	  // SCRIPT = EXPR_lhs
	  // PAYLOAD = EXPR_rhs // and any CT aliases you might hear in DIALECTo
	  //
	  struct {
	    void *ptr1;
	    void *ptr2;
	  } twoPtrValue;

	  struct {
	    void *ptr;
	    unsigned long value;
	  } ptrAndLongRep;

	} internalRep;
	} Tcl_Obj;


	 */
 };

 struct UDEF_tcl_TYPE_t : UDEF_tcl_TYPE_from_Tcl_Obj_HEAD {
 }
 struct tcl_ITEM_t : WORD("Tcl_Obj * ITEM;") PTR(u48_u16) 
 struct tcl_TYPE_t : UDEF_tcl_TYPE CACHE_SYMBOL // OPCODETOKEN_ARGS

 struct tcl_ITEM_t : WORD("Tcl_Obj * ITEM;") PTR(u48_u16) 
 struct tcl_TYPE_t : UDEF_tcl_TYPE CACHE_SYMBOL // OPCODETOKEN_ARGS

  N_bytes_t	N_Bytes;
  N_bytes_t &	Name = N_bytes

  u32 PTR1 ; // LOHI //
  u32 PTR2 ; // LOHI //

  Set_From_STR0 STR0_t _str0;
  Set_From_TOKEN u32 _TOKEN // compiler step required _TOKEN = _u64;

 }; // struct

}; // namespace
#endif
