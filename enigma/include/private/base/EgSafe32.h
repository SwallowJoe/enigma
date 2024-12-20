#pragma once

#include "include/private/base/EgMath.h"
#include <cstdint>

static constexpr int32_t Eg64_pin_to_s32(int64_t v) {
    return v < EG_MinS32 ? EG_MinS32 : (v > EG_MaxS32 ? EG_MaxS32 : (int32_t)v);
}

static constexpr int32_t Eg32_sat_add(int32_t a, int32_t b) {
    return Eg64_pin_to_s32((int64_t)a + (int64_t)b);
}

static constexpr int32_t Eg32_sat_sub(int32_t a, int32_t b) {
    return Eg64_pin_to_s32((int64_t)a - (int64_t)b);
}