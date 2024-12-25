#pragma once

#include "include/private/base/EgAssert.h"
#include "include/private/base/EgTFitsIn.h"

#include <cstddef>
#include <cstdint>

template <typename D, typename S> constexpr D EgTo(S s) {
    return EgAssert(EgTFitsIn<D>(s)), static_cast<D>(s);
}

template <typename S> constexpr int8_t EgToS8(S x) { return EgTo<int8_t>(x); }
template <typename S> constexpr int16_t EgToS16(S x) { return EgTo<int16_t>(x); }
template <typename S> constexpr int32_t EgToS32(S x) { return EgTo<int32_t>(x); }
template <typename S> constexpr int64_t EgToS64(S x) { return EgTo<int64_t>(x); }
template <typename S> constexpr uint8_t EgToU8(S x) { return EgTo<uint8_t>(x); }
template <typename S> constexpr uint16_t EgToU16(S x) { return EgTo<uint16_t>(x); }
template <typename S> constexpr uint32_t EgToU32(S x) { return EgTo<uint32_t>(x); }
template <typename S> constexpr uint64_t EgToU64(S x) { return EgTo<uint64_t>(x); }
template <typename S> constexpr int EgToInt(S x) { return EgTo<int>(x); }
template <typename S> constexpr unsigned EgToUInt(S x) { return EgTo<unsigned>(x); }
template <typename S> constexpr size_t EgToSizeT(S x) { return EgTo<size_t>(x); }

template <typename T> static constexpr bool EgToBool(const T& x) { return (bool)x; }


