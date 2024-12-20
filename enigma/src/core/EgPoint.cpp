
#define LOG_TAG "EgPoint"

#include "include/core/EgPoint.h"
#include "include/private/base/EgAssert.h"
#include "include/private/base/EgDebug.h"
/**
 * 计算两点之间的欧几里得距离。
 * @param dx 两点在 x 轴方向上的距离。
 * @param dy 两点在 y 轴方向上的距离。
 * @return 两点之间的距离，如果距离过大导致浮点数溢出，则返回饱和值。
 *
 * 该函数首先计算 dx 和 dy 的平方和，然后检查这个和是否为有限数。
 * 如果是有限数，它计算这个和的平方根，这表示两点之间的距离。
 * 如果不是有限数（即原点坐标导致溢出），则将 dx 和 dy 转换为双精度浮点数，
 * 重新计算平方和的平方根，并将结果转换回单精度浮点数。
 */
EgScalar EgPoint::length(EgScalar dx, EgScalar dy) {
    float mag2 = dx * dx + dy * dy;
    if (EgScalarIsFinite(mag2)) {
        return eg_float_sqrt(mag2);
    } else {
        double xx = dx;
        double yy = dy;
        return eg_double_to_float(sqrt(xx*xx + yy*yy));
    }
}

/**
 * 设置向量的长度并返回是否成功。
 * 
 * @tparam use_rsqrt 是否使用快速平方根倒数算法。
 * @param vec 指向要设置长度的向量的指针。
 * @param x 向量的 x 分量。
 * @param y 向量的 y 分量。
 * @param length 期望的向量长度。
 * @param orig_length 可选参数，用于返回原始向量长度。
 * @return 如果设置成功返回 true，否则返回 false。
 * 
 * 该函数首先计算向量的原始长度，然后根据期望长度计算缩放因子。
 * 如果缩放后的向量分量不是有限的，或者向量为零向量，则设置向量为零向量并返回 false。
 * 否则，设置向量的分量为缩放后的值，并返回 true。
 * 如果提供了 orig_length 参数，则将原始向量长度存储在其中。
 */
template <bool use_rsqrt> bool set_point_length(EgVector *vec, float x, float y,
                float length, float* orig_length = nullptr) {
    // 断言：如果使用快速平方根倒数算法，则 orig_length 必须为 nullptr
    EgAssert(!use_rsqrt || (orig_length == nullptr));

    // 将向量的分量转换为双精度浮点数
    double xx = x;
    double yy = y;

    // 计算向量的原始长度
    double dmag = sqrt(xx*xx + yy*yy);

    // 计算缩放因子
    double dscale = eg_ieee_double_divide(length, dmag);

    // 缩放向量的分量
    x *= dscale;
    y *= dscale;

    // 检查缩放后的向量分量是否有限，或者向量是否为零向量
    if (!eg_float_isfinite(x) || !eg_float_isfinite(y) || (x == 0 && y == 0)) {
        // 设置向量为零向量
        vec->set(0, 0);
        return false;
    }
    // 初始化向量长度变量
    float mag = 0;
    // 如果提供了 orig_length 参数，则将原始向量长度存储在其中
    if (orig_length) {
        mag = eg_double_to_float(dmag);
    }
    // 设置向量的分量
    vec->set(x, y);
     // 如果提供了 orig_length 参数，则将原始向量长度存储在其中
    if (orig_length) {
        *orig_length = mag;
    }
    // 返回设置成功
    return true;
}

/**
 * 归一化向量并返回其原始长度。
 * 
 * @param vec 指向要归一化的向量的指针。
 * @return 向量的原始长度，如果归一化失败则返回0。
 * 
 * 如果归一化成功，它返回向量的原始长度；否则，它返回0。
 */
EgScalar EgPoint::normalize(EgVector *vec) {
    float mag;
    if (set_point_length<false>(vec, vec->fX, vec->fY, 1.0f, &mag)) {
        return mag;
    } else {
        return 0;
    }
}

bool EgPoint::normalize() {
    return this->setLength(fX, fY, EG_Scalar1);
}

bool EgPoint::setNormalize(EgScalar x, EgScalar y) {
    return this->setLength(x, y, EG_Scalar1);
}

bool EgPoint::setLength(EgScalar length) {
    return this->setLength(fX, fY, length);
}

bool EgPoint::setLength(float x, float y, float length) {
    return set_point_length<false>(this, x, y, length);
}

void EgPoint::scale(float value, EgPoint *dst) const {
    dst->set(fX * value, fY * value);
}