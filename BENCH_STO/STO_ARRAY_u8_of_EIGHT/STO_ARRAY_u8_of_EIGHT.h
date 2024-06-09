#ifndef STO_ARRAY_u8_of_EIGHT_H
#define STO_ARRAY_u8_of_EIGHT_H
namespace STO {

// typedef u32_u32 EIGHT_t; // UNION of 2 implementations that are proven SAME
// C uses u64 // even when u32_u32 //

typedef u64 EIGHT_t; // compules to a single word on AMD64 // edit for CT_ARCH

 struct ARRAY_u8_of_EIGHT {

 	EIGHT_t * ARRAY[ 256 ];	

	u8_then_1 N;

	u8_idx_t
	 u8_idx; // = 0 ; // P0P2 rules // N == 0 // future _idx is old N

	 // every ARRAY has a single builtin TEMP CSR // obtain LOCK get CSR

	 ARRAY_u8_of_EIGHT() {


	 	ARRAY = MMAP_alloc( sizeof(*ARRAY) ); // I always MEMSET_ZERO
		// sometime I HOPE ZERO and attempt to avoid DOUBLE_MEMSET //
		// the debugged version is MEMSET_ZERO // GUARANTEED //

	//	if(!ARRAY) return FAIL_FAILED(); // use NULL properly 
	//	if(!ARRAY) return FAIL_throw(); // retain NULL properly 
	//	if(!ARRAY)        FAIL_throw(); // retain NULL properly 
		if(!ARRAY) throw THROW_FAIL_FAILED(); // GEN this as default
		// errno and errm is already correctly pre_set
		// we just need to walk back the THROW or FAIL up the stack

		 u8_idx = 0 ;
	 }

	 bool OBTAIN_LOCK( WORD_t & WORD ) { // WORD 
	  // GLOBAL CODE LOCK
	  	LOCK_t LOCK( WORD );
		LOCK.use_CPU_test_and_set_mem_LOCK()
		if( LOCK.LOCK_OBTAINED() ) { // else was busy
			LOCK.sync_WORD(); // NOOP // already DONE //
			LOCK.register_LOCK_with_LOCK_BOX(); //
			LOCK.STO_WORD_and_DTOR(); // already DONE // SYNC
			return true; // LOCK obtained
		}
	  	if( WORD == 0 ) {
			WORD ++;
		} else {
		}
	  // GLOBAL_UN_LOCK
	 }
 };

 struct ARRAY_u8_of_ITEM : public ARRAY_u8_of_EIGHT {
	// we have only implemented EIGHT
	// because it is useful as u32_u32 or as UDEF_UDEF
	// even in ARM32 
	// even in AMD64 // u32_u32 // adjust HILO for access to WORD_UPPER
	// mode asif MEM // ASIF CPU_WORD // ASIF choice LIST_of_CHOICE // 
	// WEIGHTED PICK_WEIGHTED //
 };

 struct ARRAY_u8_of_ITEM {
 };


};
#endif
