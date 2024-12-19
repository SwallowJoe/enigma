#pragma once

#include "include/private/base/EgFloatBits.h"
#include <cfloat>
#include <cstdint>
#include <cstring>
#include <cmath>

constexpr float EG_FloatSqrt2   = 1.41421356f;
constexpr float EG_FloatPI      = 3.14159265f;
constexpr double EG_DoublePI    = 3.14159265325979323846264338327950288;

/**
 * 计算单精度浮点数的平方根。
 * @param x 单精度浮点数。
 * @return x 的平方根。
 */
#define eg_float_sqrt(x)            sqrtf(x)


/**
 * 计算单精度浮点数的正弦值。
 * @param x 单精度浮点数，表示弧度。
 * @return x 的正弦值。
 */
#define eg_float_sin(x)             sinf(x)

/**
 * 计算单精度浮点数的余弦值。
 * @param x 单精度浮点数，表示弧度。
 * @return x 的余弦值。
 */
#define eg_float_cos(x)             cosf(x)

/**
 * 计算单精度浮点数的正切值。
 * @param x 单精度浮点数，表示弧度。
 * @return x 的正切值。
 */
#define eg_float_tan(x)             tanf(x)

/**
 * 计算单精度浮点数的向下取整值。
 * @param x 单精度浮点数。
 * @return 不大于 x 的最大整数。
 */
#define eg_float_floor(x)           floorf(x)

/**
 * 计算单精度浮点数的向上取整值。
 * @param x 单精度浮点数。
 * @return 不小于 x 的最小整数。
 */
#define eg_float_ceil(x)            ceilf(x)

/**
 * 计算单精度浮点数的小数部分的截断值。
 * @param x 单精度浮点数。
 * @return x 的小数部分，去掉整数部分。
 */
#define eg_float_trunc(x)           truncf(x)

/**
 * 计算单精度浮点数的反余弦值（arc cosine）。
 * @param x 单精度浮点数，值应在 [-1, 1] 范围内。
 * @return x 的反余弦值，以弧度表示。
 */
#define eg_float_acos(x)            acosf(x)

/**
 * 计算单精度浮点数的反正弦值（arc sine）。
 * @param x 单精度浮点数，值应在 [-1, 1] 范围内。
 * @return x 的反正弦值，以弧度表示。
 */
#define eg_float_asin(x)            asinf(x)

/**
 * 计算两个单精度浮点数的反正切值（atan2），即 y/x 的形式。
 * @param y 分子，单精度浮点数。
 * @param x 分母，单精度浮点数，不能为零。
 * @return 以 y 为分子，x 为分母的反正切值，结果以弧度表示，范围在 -π 到 π 之间。
 * 
 * 该函数计算两个参数 y 和 x 的比值的反正切值，同时考虑了 y 和 x 的符号，以确定正确的象限。
 */
#define eg_float_atan2(y, x)        atan2f(y, x)

/**
 * 计算单精度浮点数的绝对值。
 * @param x 单精度浮点数。
 * @return x 的绝对值。
 */
#define eg_float_abs(x)             fabsf(x)

/**
 * 返回一个浮点数，其绝对值与 x 相同，符号与 y 相同。
 * @param x 单精度浮点数，其绝对值将被复制。
 * @param y 单精度浮点数，其符号将被用于结果。
 * @return 一个单精度浮点数，具有 x 的绝对值和 y 的符号。
 *
 * 该函数比较 y 的符号，如果 y 是正数，返回 x 的绝对值；
 * 如果 y 是负数，返回 x 绝对值的负数。如果 x 是 NaN 或者 y 是 NaN，
 * 则结果也是 NaN。如果 x 是零，返回零，符号由 y 的符号决定。
 */
#define eg_float_copysign(x, y)     copysignf(x, y)

/**
 * 计算单精度浮点数的模（取余）。
 * @param x 被除数，单精度浮点数。
 * @param y 除数，单精度浮点数。
 * @return x 除以 y 的余数。
 */
#define eg_float_mod(x,y)           fmodf(x, y)

/**
 * 计算单精度浮点数的指数值。
 * @param x 单精度浮点数，指数的底数 e。
 * @return e 的 x 次幂。
 */
#define eg_float_exp(x)             expf(x)

/**
 * 计算单精度浮点数的自然对数值。
 * @param x 单精度浮点数，必须大于 0。
 * @return x 的自然对数值。
 */
#define eg_float_log(x)             logf(x)

/**
 * 将角度转换为弧度。
 * @param degrees 角度值，以度为单位。
 * @return 相同的值，转换为弧度。
 *
 * 该函数将度转换为弧度，使用转换因子 π/180。
 */
constexpr float eg_float_degrees_to_radians(float degrees) {
    return degrees * (EG_FloatPI / 180);
}

/**
 * 将弧度转换为角度。
 * @param radians 弧度值。
 * @return 相同的值，转换为度。
 *
 * 该函数将弧度转换为度，使用转换因子 180/π。
 */
constexpr float eg_float_radians_to_degrees(float radians) {
    return radians * (180 / EG_FloatPI);
}

/**
 * 检查给定的值是否表示一个正无穷大或负无穷大
 */
static inline bool eg_float_isinf(float x) {
    return EgFloatBits_IsInf(EgFloat2Bits(x));
}

/**
 * 判断给定数值是否为NaN
 */
static inline bool eg_float_isnan(float x) {
    return !(x == x);
}

#define EG_MAXS32FitsInFloat        2147483520
#define EG_MINS32FitsInFloat        -EG_MAXS32FitsInFloat

#define EG_MAXS64FitsInFloat        (EG_MaxS64 >> (63-24) << (63-24))   // 0x7fffff8000000000
#define EG_MINS64FitsInFloat        -EG_MAXS64FitsInFloat

/**
 * 将单精度浮点数饱和转换为整数。
 * @param x 单精度浮点数。
 * @return 转换后的整数。
 *
 * 该函数将输入的浮点数 x 限制在可表示为 int32_t 类型的范围内。
 * 如果 x 超过 int32_t 类型的最大值 EG_MAXS32FitsInFloat，则返回 EG_MAXS32FitsInFloat。
 * 如果 x 小于 int32_t 类型最小值 EG_MINS32FitsInFloat，则返回 EG_MINS32FitsInFloat。
 * 否则，返回 x 四舍五入到最近的整数。
 */
static inline int eg_float_saturate2int(float x) {
    x = x < EG_MAXS32FitsInFloat ? x : EG_MAXS32FitsInFloat;
    x = x > EG_MINS32FitsInFloat ? x : EG_MINS32FitsInFloat;
    return (int)x;
}