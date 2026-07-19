/// @file
/// @brief File with bit operations definitions.
#ifndef _16NAR_PLATFORM_MATH_BITS_H
#define _16NAR_PLATFORM_MATH_BITS_H

#include <cinttypes>

namespace _16nar::math
{

/// @brief Count number of trailing zeroes in a 64-bit number.
/// @param[in] x target number.
/// @return number of trailing zeroes or 0 if @b x is 0.
constexpr unsigned int ntz64( std::uint64_t x ) noexcept
{
    constexpr std::uint64_t debruijn{ 0x022fdd63cc95386d };
    constexpr unsigned int debruijn_table[ 64 ] = {
          0,  1,  2,  53, 3,  7,  54, 27, 4,  38, 41, 8,  34, 55, 48, 28,
          62, 5,  39, 46, 44, 42, 22, 9,  24, 35, 59, 56, 49, 18, 29, 11,
          63, 52, 6,  26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
          51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12
    };
    const auto lsb = x & -x;
    return debruijn_table[ ( lsb * debruijn ) >> 58 ];
}

} // namespace _16nar::math

#endif // #ifndef _16NAR_PLATFORM_MATH_BITS_H
