#ifndef KERNEL_INCLUDE_BIT_HPP_
#define KERNEL_INCLUDE_BIT_HPP_

#include <defines.hpp>
#include <todo.hpp>
#include <types.hpp>

/**
 * TODO: add tests of this header asap
 */
TODO_BY_THE_END_OF_MILESTONE0

// ------------------------------
// Various defines
// ------------------------------

static constexpr u64 kBitMask4  = 0xF;
static constexpr u64 kBitMask8  = 0xFF;
static constexpr u64 kBitMask16 = kBitMask8 | (kBitMask8 << 8);
static constexpr u64 kBitMask32 = kBitMask16 | (kBitMask16 << 16);
static constexpr u64 kBitMask64 = kBitMask32 | (kBitMask32 << 32);

template <typename NumT> static constexpr NumT kLsb = 1;

template <typename NumT> static constexpr NumT kMsb = kLsb<NumT> << sizeof(NumT) * 8 - 1;

// ------------------------------
// Functions
// ------------------------------

template <typename NumT> FAST_CALL void SetBit(NumT &num, const u16 bit) { num |= kLsb<NumT> << bit; }

template <typename NumT> FAST_CALL void ClearBit(NumT &num, const u16 bit) { num &= ~(kLsb<NumT> << bit); }

template <typename NumT> FAST_CALL void SwitchBit(NumT &num, const u16 bit) { num ^= kLsb<NumT> << bit; }

template <typename NumT> FAST_CALL void SetBitValue(NumT &num, const u16 bit, const bool val)
{
    ClearBit(num, bit);
    num |= static_cast<NumT>(val) << bit;
}

#endif // KERNEL_INCLUDE_BIT_HPP_
