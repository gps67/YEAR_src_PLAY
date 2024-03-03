#ifndef ASM_hilo_H
#define ASM_hilo_H


// ON_HILO
// ON_LOHI_using_BSWAP
// ON_LOHI

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
# define ON_HILO
#else
# define ON_LOHI
#endif

#if not defined(ON_LOHI)
#warning "not on-LOHI"
#endif

#if not defined(__GNUC__)
#warning "not on-GNUC"
#endif

#if not defined(__i386__)
// #warning "not on-i386" // amd64 is NOT
#endif

#if defined(__i386__)
#warning "IS on-i386 - bswap didnt exist" // but 486 does !!
#endif

#if not defined(__amd64__)
#warning "not on-amd64"
#endif

#if defined(ON_LOHI) && defined(__GNUC__) && defined(__i386__)
// hope not ACTUAL i386 without bswap
#  define ON_LOHI_using_BSWAP

# elif defined(ON_LOHI) && defined(__GNUC__) && defined(__amd64__)
// more work for 64 bit but ints are 32 bit
#  define ON_LOHI_using_BSWAP

# elif defined(ON_HILO) 
// ARM has asm REV but GCC builtin will know that
# else
#warning "ON_LOHI needs to detect CPU i486 but not i386"
# endif

// testing i386 ntohl
// undef ON_LOHI_using_BSWAP

// on this box it should be
#if not defined(ON_LOHI_using_BSWAP)
#warning "###### not ON_LOHI_using_BSWAP #######"
#endif

// CALL_BYTE_SWAP_32( v ) ASM_byte_swap_32
// CALL_BYTE_SWAP_32( v ) __builtin_bswap32() GCC
// CALL_BYTE_SWAP_32( v ) 
// 3 ways to define it

# ifdef ON_LOHI_using_BSWAP
#  define CALL_BYTE_SWAP_16(V) ASM_byte_swap_16( V )
#  define CALL_BYTE_SWAP_32(V) ASM_byte_swap_32( V )
#  define CALL_BYTE_SWAP_64(V) ASM_byte_swap_64( V )

// #warning "###### GOOD ... ASM #######"

  inline void ASM_byte_swap_16_in_situ( u16 & situ ) {
        __asm__("bswap %[io16]" : [io16] "+r" (situ) );
  }

  inline void ASM_byte_swap_32_in_situ( u32 & situ ) {
        __asm__("bswapl %[io32]" : [io32] "+r" (situ) );
  }

  inline void ASM_byte_swap_64_in_situ( u64 & situ ) {
        __asm__("bswapq %[io64]" : [io64] "+r" (situ) );
  }

 // C // in-situ - but of a copy on the stack // optimised out

  inline u16 ASM_byte_swap_16( u16 val ) {
        ASM_byte_swap_16_in_situ( val );
        return val;
  }

  inline u32 ASM_byte_swap_32( u32 val ) {
	// C // val is already a copy
        ASM_byte_swap_32_in_situ( val );
	// now byte swapped
        return val;
  }

  inline u64 ASM_byte_swap_64( u64 val ) {
	// C // val is already a copy
        ASM_byte_swap_64_in_situ( val );
	// now byte swapped
        return val;
  }

# elif defined (__GNUC__)
# define	CALL_BYTE_SWAP_32(V) __builtin_bswap32( V )

# elif defined ON_LOHI // where ntohl(v) does byte swap
#  		include <arpa/inet.h>
# define	CALL_BYTE_SWAP_32(V) ntohl( V )
 
# else

// ON_HILO
// not GNUC
// MS has _byteswap_ulong
// LLVM has __builtin_bswap32
#error "ON_HILO needs _byteswap()"
#endif

///////////////////////////////////////////

inline u32  lohi_from_hilo( u32 hilo ) { return CALL_BYTE_SWAP_32( hilo ); }
inline u32  hilo_from_lohi( u32 lohi ) { return CALL_BYTE_SWAP_32( lohi ); }

#if defined ON_HILO
inline u32  u32_from_hilo( u32 hilo ) { return hilo; }
inline u32  hilo_from_u32( u32 val  ) { return val; }
#else
inline u32  u32_from_hilo( u32 hilo ) { return CALL_BYTE_SWAP_32( hilo ); }
inline u32  hilo_from_u32( u32 val  ) { return CALL_BYTE_SWAP_32( val ); }
#endif

///////////////////////////////////////////

 struct u32_hilo {

  u32 word_abcd;

  inline operator u32 () {
    return u32_from_hilo( word_abcd );
  }

  inline u32_hilo &
  operator = (u32 val) {
    word_abcd = hilo_from_u32( val );
    return *this;
  }
 };

 struct i32_hilo : u32_hilo {

  // i32 makes default casting ambiguous // so avoid future lock-in

  inline operator i32 () {
	  return (i32) u32_from_hilo( word_abcd );
  }

  inline u32_hilo &
  operator = (i32 val) {
	  word_abcd = hilo_from_u32( (u32) val );
	  return *this;
  }

 };

///////////////////////////////////////////
 template <class U>
 struct hilo {
  U val;

  U word_abcd;

  inline operator U () {
   // required overloaded  U _from_hilo(u16|i16|u32|i32|u64|i64)
    return u32_from_hilo( word_abcd );
  }

  inline u32_hilo &
  operator = (U val) {
   // required overloaded  U _hilo_from_(u16|i16|u32|i32|u64|i64)
    word_abcd = hilo_from_u32( val );
    return *this;
  }
 };
///////////////////////////////////////////

/*
 struct u32_lohi { // avail but unused

  u32 word_abcd;

  inline operator u32 () {
    return u32_from_lohi( word_abcd );
  }

  inline u32_lohi &
  operator = (u32 val) {
    word_abcd = lohi_from_u32( val );
    return *this;
  }

  inline operator i32 () { return (i32) u32_from_lohi( word_abcd ); }

  inline u32_lohi &
  operator = (i32 val) { word_abcd = lohi_from_u32( (i32) val ); return *this; }

 };
*/

///////////////////////////////////////////



#include "LOHI_checks.h"

#endif // MODULE
