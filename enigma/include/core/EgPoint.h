#pragma once

#include "include/core/EgTypes.h"
#include "include/core/EgScalar.h"
#include "include/private/base/EgSafe32.h"
#include "include/private/base/EgAssert.h"

#include <cstdint>

struct EgIPoint;

typedef EgIPoint EgIVector;

struct EgIPoint {
    int32_t fX;
    int32_t fY;

    static constexpr EgIPoint Make(int32_t x, int32_t y) {
        return {x, y};
    }

    constexpr int32_t x() const { return fX; }
    constexpr int32_t y() const { return fY; }

    bool isZero() const { return (fX|fY) == 0; }

    void set(int32_t x, int32_t y) {
        fX = x;
        fY = y;
    }

    EgIPoint operator-() const {
        return {-fX, -fY};
    }

    void operator+=(const EgIVector& v) {
        fX = Eg32_sat_add(fX, v.fX);
        fY = Eg32_sat_add(fY, v.fY);
    }

    void operator-=(const EgIVector& v) {
        fX = Eg32_sat_sub(fX, v.fX);
        fY = Eg32_sat_sub(fY, v.fY);
    }

    bool equals(int32_t x, int32_t y) const {
        return fX == x && fY == y;
    }

    friend bool operator==(const EgIPoint& a, const EgIPoint& b) {
        return a.fX == b.fX && a.fY == b.fY;
    }

    friend bool operator!=(const EgIPoint& a, const EgIPoint& b) {
        return a.fX != b.fX || a.fY != b.fY;
    }

    friend EgIPoint operator-=(const EgIPoint& a, const EgIPoint& b) {
        return { Eg32_sat_sub(a.fX, b.fX), Eg32_sat_sub(a.fY, b.fY) };
    }

    friend EgIPoint operator+=(const EgIPoint& a, const EgIPoint& b) {
        return { Eg32_sat_add(a.fX, b.fX), Eg32_sat_add(a.fY, b.fY) };
    }
};

struct EgPoint;

typedef EgPoint EgVector;

struct EG_API EgPoint {
    EgScalar fX;
    EgScalar fY;

    /**
     * 创建一个点。
     */
    static constexpr EgPoint Make(EgScalar x, EgScalar y) {
        return {x, y};
    }

    constexpr EgScalar x() const { return fX; }
    constexpr EgScalar y() const { return fY; }

    /**
     * 判断点是否为零点。
     */
    bool isZero() const { return (0 == fX) & (0 == fY); }

    /**
     * 设置点的坐标。
     */
    void set(EgScalar x, EgScalar y) {
        fX = x;
        fY = y;
    }

    /**
     * 设置点的坐标。
     */
    void set(const EgPoint& p) {
        fX = p.fX;
        fY = p.fY;
    }

    /**
     * 设置点的坐标。
     */
    void iset(int32_t x, int32_t y) {
        fX = EgIntToScalar(x);
        fY = EgIntToScalar(y);
    }

    /**
     * 设置点的坐标。
     */
    void iset(const EgIPoint& p) {
        fX = EgIntToScalar(p.fX);
        fY = EgIntToScalar(p.fY);
    }

    /**
     * 设置点的坐标为其绝对值。
     * @param p 参考一个 EgPoint 对象，其坐标将被取绝对值。
     *
     * 该函数接受一个 EgPoint 类型的引用 p，并将 fX 和 fY 的值设置为它们的绝对值。
     */
    void setAbs(const EgPoint& p) {
        fX = EgScalarAbs(p.fX);
        fY = EgScalarAbs(p.fY);
    }

    /**
     * 偏移点的坐标。
     */
    void offset(EgScalar dx, EgScalar dy) {
        fX += dx;
        fY += dy;
    }

    /**
     * @brief 将一组 EgPoint 对象按指定的 dx 和 dy 值进行偏移。
     *
     * 这个静态方法遍历一个 EgPoint 对象数组，并对每个点应用指定的偏移量。
     *
     * @param points 要偏移的 EgPoint 对象数组。
     * @param count 数组中 EgPoint 对象的数量。
     * @param dx 沿 x 轴应用的偏移值。
     * @param dy 沿 y 轴应用的偏移值。
     */
    static void offset(EgPoint points[], int count, EgScalar dx, EgScalar dy) {
        for (int i = 0; i < count; i++) {
            points[i].offset(dx, dy);
        }
    }

    static void offset(EgPoint points[], int count, const EgVector& d) {
        offset(points, count, d.fX, d.fY);
    }


    /**
     * 计算到原点之间的欧几里得距离。
     * @return 两点之间的距离，如果距离过大导致浮点数溢出，则返回饱和值。
     */
    EgScalar length() const {
        return EgPoint::length(fX, fY);
    }

    /**
     * 计算到原点之间的欧几里得距离。
     */
    EgScalar distanceToOrigin() const { return length(); }

    bool normalize();

    bool setNormalize(EgScalar x, EgScalar y);

    bool setLength(EgScalar length);

    bool setLength(EgScalar x, EgScalar y, EgScalar length);

    /**
     * 缩放点的坐标。
     * @param value 缩放因子。
     * @param dst 指向存储缩放结果的点的指针。
     *
     * 该函数接受一个 EgScalar 类型的缩放因子 value 和一个指向 EgPoint 对象的指针 dst。
     * 它将当前点的坐标按 value 进行缩放，并将结果存储在 dst 指向的点中。
     */
    void scale(EgScalar value, EgPoint* dst) const;

    /**
     * 缩放点的坐标。
     * @param value 缩放因子。
     *
     * 该函数接受一个 EgScalar 类型的缩放因子 value，并将当前点的坐标按 value 进行缩放。
     * 缩放后的结果将存储在当前点中。
     */
    void scale(EgScalar value) { scale(value, this); }


    /**
     * 取反点的坐标。
     * 
     * 该函数将当前点的坐标取反，即将 fX 和 fY 的值变为其相反数。
     */
    void negate() {
        fX = -fX;
        fY = -fY;
    }

    /**
     * 取反点的坐标。
     * 
     * 该函数将当前点的坐标取反，即将 fX 和 fY 的值变为其相反数。
     */
    EgPoint operator-() const {
        return {-fX, -fY};
    }

    void operator+=(const EgPoint& p) {
        fX += p.fX;
        fY += p.fY;
    }

    void operator-=(const EgPoint& p) {
        fX -= p.fX;
        fY -= p.fY;
    }

    EgPoint operator*(EgScalar scale) const {
        return {fX * scale, fY * scale};
    }

    EgPoint& operator*=(EgScalar scale) {
        fX *= scale;    
        fY *= scale;
        return *this;
    }

    bool isFinite() const {
        EgScalar accum = 0;
        accum *= fX;
        accum *= fY;
        EgAssert(0 == accum || EgScalarIsNaN(accum));

        return !EgScalarIsNaN(accum);
    }

    bool equals(EgScalar x, EgScalar y) const {
        return fX == x && fY == y;
    }

    friend bool operator==(const EgPoint& a, const EgPoint& b) {
        return a.fX == b.fX && a.fY == b.fY;
    }

    friend bool operator!=(const EgPoint& a, const EgPoint& b) {
        return a.fX!= b.fX || a.fY!= b.fY;
    }

    friend EgVector operator-(const EgPoint& a, const EgPoint& b) {
        return { a.fX - b.fX, a.fY - b.fY };
    }

    friend EgPoint operator-=(const EgPoint& a, const EgPoint& b) {
        return { a.fX - b.fX, a.fY - b.fY };
    }

    friend EgPoint operator+=(const EgPoint& a, const EgPoint& b) {
        return { a.fX + b.fX, a.fY + b.fY };
    }

    static EgScalar length(EgScalar dx, EgScalar dy);

    static EgScalar normalize(EgVector* v);

    static EgScalar distance(const EgPoint& a, const EgPoint& b) {
        return length(a.fX - b.fX, a.fY - b.fY);
    }

    static EgScalar dotProduct(const EgPoint& a, const EgPoint& b) {
        return a.fX * b.fX + a.fY * b.fY;
    }

    static EgScalar crossProduct(const EgPoint& a, const EgPoint& b) {
        return a.fX * b.fY - a.fY * b.fX;
    }

    EgScalar cross(const EgVector& vec) const {
        return EgPoint::crossProduct(*this, vec);
    }

    EgScalar dot(const EgVector& vec) const {
        return EgPoint::dotProduct(*this, vec);
    }
};