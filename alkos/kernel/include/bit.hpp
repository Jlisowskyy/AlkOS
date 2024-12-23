#ifndef KERNEL_INCLUDE_BIT_HPP_
#define KERNEL_INCLUDE_BIT_HPP_

#include <types.hpp>

static constexpr u64 kBitMask4 = 0xF;
static constexpr u64 kBitMask8 = 0xFF;
static constexpr u64 kBitMask16 = kBitMask8 | (kBitMask8 << 8);
static constexpr u64 kBitMask32 = kBitMask16 | (kBitMask16 << 16);
static constexpr u64 kBitMask64 = kBitMask32 | (kBitMask32 << 32);

#endif // KERNEL_INCLUDE_BIT_HPP_
