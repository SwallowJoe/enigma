#pragma once

#include "include/private/base/EgAssert.h"
#include "include/private/base/EgTFitsIn.h"

#include <cstddef>
#include <cstdint>
#include <climits>

class EgSafeMath {

public:
    EgSafeMath() = default;

    bool ok() const { return mOk; }
    explicit operator bool() const { return mOk; }

    size_t mul(size_t x, size_t y) {
        return sizeof(size_t) == sizeof(uint64_t) ? mul64(x, y) : mul32(x, y);
    }

    size_t add(size_t x, size_t y) {
        size_t result = x + y;
        mOk = mOk && (result >= x);
        return result;
    }

    int addInt(int a, int b) {
        if (b < 0 && a < std::numeric_limits<int>::min() - b) {
            mOk = false;
            return a;
        } else if (b > 0 && a > std::numeric_limits<int>::max() - b) {
            mOk = false;
            return a;
        }
        return a + b;
    }

    /**
     * @brief 对齐 x 到 alignment 的倍数
     * @param x 要对齐的数
     * @param alignment 对齐的倍数
     * @return 对齐后的数
     * @note alignment 必须是 2 的幂
     * @note 如果 alignment 是 0，则返回 x
     */
    size_t alignUp(size_t x, size_t alignment) {
        // alignment 必须是 2 的幂
        EgAssert(alignment && !(alignment & (alignment - 1)));

        // 对齐 x 到 alignment 的倍数
        return add(x, alignment - 1) & ~(alignment - 1);
    }

    /**
     * @brief 强制转换 value 为 T 类型
     * @param value 要转换的值
     * @return 转换后的值
     * @note 如果 value 无法转换为 T 类型，则 mOk 为 false
     */
    template <typename T> T castTo(size_t value) {
        if (!EgTFitsIn<T>(value)) {
            mOk = false;
        }
        return static_cast<T>(value);
    }

    static size_t Add(size_t x, size_t y) {
        EgSafeMath safe;
        size_t sum = safe.add(x, y);
        return safe.ok() ? sum : SIZE_MAX;
    }

    static size_t Mul(size_t x, size_t y) {
        EgSafeMath safe;
        size_t product = safe.mul(x, y);
        return safe.ok() ? product : SIZE_MAX;
    }

    static size_t Align4(size_t x) {
        EgSafeMath safe;
        return safe.alignUp(x, 4);
    }

private:

    /**
     * @brief 32 位乘法
     * @param x 乘数
     * @param y 乘数
     * @return 乘积
     * @note 如果乘积超过 32 位，则 mOk 为 false
     */
    uint32_t mul32(uint32_t x, uint32_t y) {
        uint64_t bx = x;
        uint64_t by = y;
        uint64_t result = bx * by;
        mOk &= result >> 32 == 0;
        return (uint32_t)result;
    }

    /**
     * @brief 64 位乘法
     * @param x 乘数
     * @param y 乘数
     * @return 乘积
     * @note 如果乘积超过 64 位，则 mOk 为 false
     * @note 如果 x 和 y 都小于等于 2^32，则直接返回 x * y
     * @note 如果 x 和 y 有一个大于 2^32，则使用分解乘法
     *       分解乘法原理：
     *       x = x1 * 2^32 + x0
     *       y = y1 * 2^32 + y0
     *       x * y = (x1 * 2^32 + x0) * (y1 * 2^32 + y0)
     *            = x1 * y1 * 2^64 + (x1 * y0 + x0 * y1) * 2^32 + x0 * y0
     */
    uint64_t mul64(uint64_t x, uint64_t y) {
        if (x <= (std::numeric_limits<uint64_t>::max() >> 32)
                && y <= (std::numeric_limits<uint64_t>::max() >> 32)) {
            return x * y;
        }
        auto hi = [](uint64_t v) { return v >> 32; };
        auto lo = [](uint64_t v) { return v & 0xFFFFFFFF; };

        uint64_t lx_ly = lo(x) * lo(y);
        uint64_t hx_ly = hi(x) * lo(y);
        uint64_t lx_hy = lo(x) * hi(y);
        uint64_t hx_hy = hi(x) * hi(y);

        uint64_t result = 0;
        result = this->add(lx_ly, (hx_ly << 32));
        result = this->add(result, (lx_hy << 32));

        // 如果 x 和 y 都大于 2^32，则需要判断是否溢出
        mOk &= (hx_hy + (hx_ly >> 32) + (lx_hy >> 32)) == 0;

        return result;
    }
    bool mOk = true;
};