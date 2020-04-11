#include <stdint.h>

inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}

// supposedly GCC + SLANG recognise this as rotate right
// r == 0 is problematic
// (-r & 63)



