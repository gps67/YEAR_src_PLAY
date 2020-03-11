#ifndef u8_u8_POOL_ITEM_H
#define u8_u8_POOL_ITEM_H

// namespace is CORE_PLUS

#include "CORE_PLUS/u16_hilo.h" // symb_link -> .

// suggest convert u16_hilo to u16_cpu or u32_cpu
// then convert u32_cpu into u32_cpu_var // ABCD_H ABCD_L ABCD_HL _X _xFFFF ...
// compile requires cpu_var is used
// compile requires cpu_var is loaded bytes swapped BSWAP

// for gcc use ctor(word) // bitfield cd in what was u32_abcd
// for gcc use ctor(word) // bitfield ab in what was u32_abcd // not avail
// compile _ab requires shift_16 or rotate_16 or bit_field_action // aligned

// usage: DATA STO STREAM += ... U8 U8 ...
// usage: CODE func_access_byte_A_getter _ABCD _0123 _getter _setter _CACHE
// usage: on modify set flag // on flag write back / send to next / filter

// so data declaration, for MMAP uses BYTES[addr]
// packed, uses CPU register containing DCBA ABCD CDAB

struct u8_u8 : public u16_ab {
 u8_u8( u16 ab_ ) {
  // ASM u16_ab = ab_;
 }

struct u8_u8 : public u32_cd {
 u
 u8_u8( u16 ab_ ) {
  // ASM u16_ab = ab_;
 }

struct u8_u8 : public u32_ab {
 u8_u8( u16 ab_ ) {
  // ASM u16_ab = ab_;
 }

};

#endif
