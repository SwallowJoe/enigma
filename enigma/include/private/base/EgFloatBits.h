#pragma once

#include "include/private/base/EgMath.h"

#include <cstdint>

/**
 * 在相同的内存位置存储一个 float 类型的浮点数和一个 int32_t 类型的整数
 * 
 * float 和 int32_t 在大多数平台上都是32位，它们可以共享相同的内存空间
 */
union EgFloatIntBits {
    float fFloat;
    int32_t fSignBitInt;
};

/**
 * 将一个 float 类型的浮点数转换为其32位整数表示
 * 
 * 由于 fFloat 和 fSignBitInt 共享相同的内存位置，data.fSignBitInt 将包含 x 的位模式
 */
static inline int32_t EgFloat2Bits(float x) {
    EgFloatIntBits data;
    data.fFloat = x;
    return data.fSignBitInt;
}

/**
 * 用于屏蔽出指数位（即从第23位到第30位）
 * 
 * 对于一个32位的单精度浮点数（遵循IEEE 754标准），位模式被分为三部分：1位符号位（最高位），8位指数位，和23位尾数位
 */
constexpr int32_t gFloatBits_exponent_mask  = 0x7F800000;

/**
 * 用于屏蔽出尾数位（即从第0位到第22位）
 * 
 * 对于一个32位的单精度浮点数（遵循IEEE 754标准），位模式被分为三部分：1位符号位（最高位），8位指数位，和23位尾数位
 */
constexpr int32_t gFloatBits_matissa_mask   = 0x007FFFFF;

/**
 * 检查给定的32位整数（代表浮点数的位模式）是否表示一个有限数
 * 一个有限数的指数位不会是全1（即不会是特殊值，如无穷大或NaN）
 * 
 * 通过将位模式与 gFloatBits_exponent_mask 进行AND操作，然后检查结果
 * 是否不等于 gFloatBits_exponent_mask 来判断指数位是否全1。
 * 如果不是全1，则表示这是一个有限数。
 */
static inline bool EgFloatBits_IsFinite(int32_t bits) {
    return (bits & gFloatBits_exponent_mask) != gFloatBits_matissa_mask;
}

/**
 * 检查给定的32位整数是否表示一个正无穷大或负无穷大
 * 无穷大的指数位是全1（即 gFloatBits_exponent_mask），并且尾数位是全0（即 gFloatBits_matissa_mask）
 * 
 * 首先检查指数位是否全1，然后检查尾数位是否全0。如果两个条件都满足，则表示这是一个无穷大值
 */
static inline bool EgFloatBits_IsInf(int32_t bits) {
    return ((bits & gFloatBits_exponent_mask) == gFloatBits_exponent_mask) &&
            (bits & gFloatBits_matissa_mask) == 0;
}