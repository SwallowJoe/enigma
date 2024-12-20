#pragma once

#include <cstdint>
#include <climits>

static constexpr int16_t EG_MaxS16 = INT16_MAX;
static constexpr int16_t EG_MinS16 = -EG_MaxS16;

static constexpr int32_t EG_MaxS32 = INT32_MAX;
static constexpr int32_t EG_MinS32 = -INT32_MAX;
static constexpr int32_t EG_NaN32 = INT32_MIN;

static constexpr int64_t EG_MaxS64 = INT64_MAX;
static constexpr int64_t EG_MinS64 = -INT64_MAX;

template <typename T> constexpr inline bool EgIsPow2(T value) {
    return (value & (value -1)) == 0;
}
