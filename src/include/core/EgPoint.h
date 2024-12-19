#pragma once

#include "EgTypes.h"
#include "include/core/EgScalar.h"
#include "include/private/base/EgSafe32.h"

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

    static constexpr EgPoint Make(EgScalar x, EgScalar y) {
        return {x, y};
    }

    constexpr EgScalar x() const { return fX; }
    constexpr EgScalar y() const { return fY; }

    bool isZero() const { return (0 == fX) & (0 == fY); }

    void set(EgScalar x, EgScalar y) {
        fX = x;
        fY = y;
    }

    void iset(int32_t x, int32_t y) {
        fX = EgIntToScalar(x);
        fY = EgIntToScalar(y);
    }

    void iset(const EgIPoint& p) {
        fX = EgIntToScalar(p.fX);
        fY = EgIntToScalar(p.fY);
    }

    void setAbs(const EgPoint& p) {
        
    }
};