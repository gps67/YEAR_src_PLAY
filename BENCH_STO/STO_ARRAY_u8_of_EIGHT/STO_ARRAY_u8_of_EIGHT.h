#ifndef STO_ARRAY_u8_of_EIGHT_H
#define STO_ARRAY_u8_of_EIGHT_H
namespace STO {

// typedef u32_u32 EIGHT_t; // UNION of 2 implementations that are proven SAME
// C uses u64 // even when u32_u32 //

// typedef u32 WORD_t; // CPU specific
   typedef u64 WORD_t; // MEM align EIGHT_t or FOUR_t // u32_u32 //


 struct LOCK_t {
 //	u64 LOCK_WORD; // on AMD
 //	u32 LOCK_WORD; // on AMD
	WORD_t LOCK_WORD;
 };

typedef u64 EIGHT_t; // compules to a single word on AMD64 // edit for CT_ARCH

 struct ARRAY_u8_of_EIGHT {

 	EIGHT_t * ARRAY[ 256 ];	

	// u9 N; //
	typedef u16 u9; 	// is_wider // BEWARE when N needs 17'th bit
	typedef u9  u9_or_i10_or_wider; //
	typedef u9 u8_then_1; // u9 or i10 or wider
	// u9_N_t //
	u8_then_1 N;

	u8_idx_t
	 u8_idx; // = 0 ; // P0P2 rules // N == 0 // future _idx is old N

	 // every ARRAY has a single builtin TEMP CSR // obtain LOCK get CSR

	 ARRAY_u8_of_EIGHT() N(0), ARRAY(NULL) {


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
		LOCK.use_CPU_test_and_set_MEM_lock(); // ALLOC the BIT
		if( LOCK.LOCK_OBTAINED() ) { // else was busy
			LOCK.sync_WORD(); // NOOP // already DONE // KNOW ...
			LOCK.register_LOCK_with_LOCK_BOX(); //
			LOCK.STO_WORD_and_DTOR(); // already DONE // SYNC
			return true; // LOCK obtained
		}
		} else {
			return FAIL_FAILED(); // already reported
		}
	  // GLOBAL_UN_LOCK
	 }

	 bool RELEASE_LOCK( WORD_t & WORD ) { // WORD 
	  	LOCK_t LOCK( WORD );
		LOCK.RELEASE(); // SAME_ALLOC_BIT CT_RT CODE_POINT KNOWN BIT
		return true;
	 };
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
