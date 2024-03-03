#ifndef LOHI_checks_H
#define LOHI_checks_H

// ON_HILO
// ON_LOHI_using_BSWAP
// ON_LOHI

#if not defined(__amd64__)
#warning "not on-amd64"
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
# error this is untested outside of AMD_64
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

bool CT_RT_TEST_HILO();

#endif // MODULE


