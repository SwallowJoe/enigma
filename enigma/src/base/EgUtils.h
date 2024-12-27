#pragma once

#include <algorithm>

template <typename T>
static constexpr const T& EgTPin(const T& value, const T& min, const T& max) {
    return std::min(std::max(value, min), max);
}

namespace EgHexadecimalDigits {
    extern const char gUpper[16];
    extern const char gLower[16];
}