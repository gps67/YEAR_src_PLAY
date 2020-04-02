#ifndef ASM_u32_hilo_H
#define ASM_u32_hilo_H

/*
	all vary lovely but want PRIVATE macros
	not callable nor needed elsewhere
	SO have to declare the class tree to get derived u32_hilo
	WANT fast, as included EVERYWHERE inline


	struct WORD_ABCD {

	  // as loaded from DISK to CPU to MEM to REG
	  // literally in buffer as = "abcd"
	  u32 word_abcd; /* private */

	  operator u32 () {
	    return WORD.get_as_u32();
	  }

	  operator = (u32 val) {
	    return WORD.set_from_u32(val);
	  }

	};
	
	SEE the cast is in the WORD code
	ALSO no casts to lohi, but that is also avail for other projects
	ALSO this is to go into files conditionally included?

	TODO: reserve WORD as u32 extend with WORD64
*/

#if ON_PLATFORM_hilo

    // NO BYTE SWAP required
    u32  u32_from_hilo( u32 hilo ) { return hilo; }
    u32  hilo_from_u32( u32 val ) { return hilo; }

#elif ON_PLATFORM_lohi // BYTE_SWAP required
# if  ON_PLATFORM_lohi_BSWAP_i486 // using ASM BSWAP

    u32  u32_from_hilo( u32 hilo ) { return ASM_bytes_swap( hilo ); }
    u32  hilo_from_u32( u32 val ) { return ASM_bytes_swap( hilo ); }

# else // on 386 but not using ASM BSWAP

   #include <arpa/inet.h>
    u32  u32_from_hilo( u32 hilo ) { return ntohl( hilo ); }
    u32  hilo_from_u32( u32 val ) { return ntohl( hilo ); }

# endif // BYTE_SWAPPED with/without ASM

#else
#error ON_PLATFORM_byte_order_UNSET
#endif

 struct WORD {
  static
    u32 
   	    u32_from_hilo( u32 hilo ) {
    	    // ON PLATFORM //
    	    // ON_HILO
    		return hilo; // no CAST this is the cast, reinterpret is OK
    	    // ON_LOHI
     	    // ON_i486
     	    // ON_x64
    		return ASM_bytes_swap( hilo );
     	    // ON_Default
		// include <arpa/inet.h>
    		return ntohl( hilo );
	     }
  static
   void
   	hilo_from_cpu( u32 val ) {
	// ON PLATFORM
	 //ON_HILO
	 		
	}
   	

     	    // ON_ARM_32
     	    // ON_ARM_64
     	    // ON_i386_no_BSWAP
		// include <arpa/inet.h>
    		return ntohl( hilo );
 };

 struct WORD_ABCD : WORD {
  union {
  	u32 word;
	...
  };
  	// NO // u32 hilo_from_abcd() { return word; }
  	// NO // u32 lohi_from_abcd() { return word; }

 };

 struct u32_hilo : WORD_ABCD {

 	u32_hilo &	// AUTO ADD // return *this;
	operator = ( u32 val ) {
		word = hilo_from_u32( val );
		return * this;	// FILTER VIEW // UN AUTO ADD
	}

	operator u32 () (
		return u32_from_hilo( word );
	}

 };



		/*
			class u32_hilo : public WORD_BASE {
				// BASE // WORD word;
	/* C++ return SELF& */		operator = ( u32 ) {
	{
					return u32_FROM_u32_hilo( word ) {
					 ON_LOHI return ASM_byte_swap( word )
					 ON_HILO return DATA_DIRECT( word )
					}
				}
				operator = u32() {
			}
		...
#endif
#ifndef ASM_byte_swap_H
#define ASM_byte_swap_H

#ifndef SIMPLIFIED_API_SUMMARY // DIAG

 USAGE	{
  OBJECT [
  	RO_Object_In_Buffer * Obj = PKT_IN_Object.CSR 
	// CSR //
 		u32_hilo N_hilo = CSR.N_items_in_list;
		const u32 N = N_hilo; // CAST (u32) FROM WORD u32_hilo;


		// const u32 N = N_hilo;
		N = u32_FROM_u32_hilo( N_hilo );

		N = u32_FROM_u32_hilo( N_hilo );
		N = return ASM_byte_swap( hilo );

inline u32 u32_FROM_u32_hilo( u32 hilo ) {
}
 }

	// require GCC ASM "=r" malarky
	// require i486 // i386 lacks bswap
	// require typedef int u32;

inline void ASM_byte_swap_in_situ( u32 & situ ) {
	// TODO // check that IN_SITU & can go to register
 // ok  __asm__("bswapl %0" : "=r" (situ) : "0" (situ));
 // ok  __asm__("bswapl %0" : "+r" (situ) );
        __asm__("bswapl %[io32]" : [io32] "+r" (situ) );
	// TODO // "bswapl %[insitu]" : [insitu]"+r" (situ) //)
	// 0 means like %0
	// http://www.ethernut.de/en/documents/arm-inline-asm.html
	// http://ericw.ca/notes/a-tiny-guide-to-gcc-inline-assembly.html
	// REV is ARM reverse swap bytes
}

inline u32 ASM_byte_swap( u32 insitu ) {
	// C // insitu is already a copy
	ASM_byte_swap_in_situ( insitu );
	// __asm__("bswapl %0" : "=r" (insitu) : "0" (insitu));
	return insitu; // now lohi
}

inline u32 u32_FROM_u32_hilo( u32 hilo ) {
	return ASM_byte_swap( hilo );
}

#else // ACTUAL CODE //

	// require GCC ASM "=r" malarky
	// require i486 // i386 lacks bswap
	// require typedef int u32;

inline void ASM_byte_swap_in_situ( u32 & situ ) {
	// TODO // check that IN_SITU & can go to register
 // ok  __asm__("bswapl %0" : "=r" (situ) : "0" (situ));
 // ok  __asm__("bswapl %0" : "+r" (situ) );
        __asm__("bswapl %[io32]" : [io32] "+r" (situ) );
	// TODO // "bswapl %[insitu]" : [insitu]"+r" (situ) //)
	// 0 means like %0
	// http://www.ethernut.de/en/documents/arm-inline-asm.html
	// http://ericw.ca/notes/a-tiny-guide-to-gcc-inline-assembly.html
	// REV is ARM reverse swap bytes
}

inline u32 ASM_byte_swap( u32 insitu ) {
	// C // insitu is already a copy
	ASM_byte_swap_in_situ( insitu );
	// __asm__("bswapl %0" : "=r" (insitu) : "0" (insitu));
	return insitu; // now lohi
}

inline u32 u32_FROM_u32_hilo( u32 hilo ) {
	return ASM_byte_swap( hilo );
}

#endif // ACTUAL CODE
#endif
