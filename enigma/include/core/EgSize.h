#pragma once

#include "include/core/EgScalar.h"

#include <cstdint>

struct EgISize {
    int32_t fWidth;
    int32_t fHeight;
    static constexpr EgISize Make(int32_t width, int32_t height) {
        return {width, height};
    }
    static constexpr EgISize MakeEmpty() {
        return {0, 0};
    }
    constexpr int32_t width() const { return fWidth; }
    constexpr int32_t height() const { return fHeight; }

    void set(int32_t width, int32_t height) {
        fWidth = width;
        fHeight = height;
    }

    bool isEmpty() const {
        return fWidth <= 0 || fHeight <= 0;
    }

    void setEmpty() {
        fWidth = 0;
        fHeight = 0;
    }

    bool isZero() const {
        return fWidth == 0 && fHeight == 0;
    }

    bool equals(int32_t width, int32_t height) const {
        return fWidth == width && fHeight == height;
    }

    friend bool operator==(const EgISize& a, const EgISize& b) {
        return a.fWidth == b.fWidth && a.fHeight == b.fHeight;
    }
    friend bool operator!=(const EgISize& a, const EgISize& b) {
        return a.fWidth != b.fWidth || a.fHeight != b.fHeight;
    }

    constexpr int32_t area() const {
        return fWidth * fHeight;
    }
};

/************************************************************/

struct EgSize {
    EgScalar fWidth;
    EgScalar fHeight;
    static constexpr EgSize Make(EgScalar width, EgScalar height) {
        return {width, height};
    }
    static constexpr EgSize MakeEmpty() {
        return {0, 0};
    }
    static constexpr EgSize Make(const EgISize& size) {
        return {EgIntToScalar(size.fWidth), EgIntToScalar(size.fHeight)};
    }
    constexpr EgScalar width() const { return fWidth; }
    constexpr EgScalar height() const { return fHeight; }
    void set(EgScalar width, EgScalar height) {
        fWidth = width;
        fHeight = height;
    }
    bool isEmpty() const {
        return fWidth <= 0 || fHeight <= 0;
    }
    void setEmpty() {
        fWidth = 0;
        fHeight = 0;
    }
    bool isZero() const {
        return fWidth == 0 && fHeight == 0;
    }
    bool equals(EgScalar width, EgScalar height) const {
        return fWidth == width && fHeight == height;
    }
    friend bool operator==(const EgSize& a, const EgSize& b) {
        return a.fWidth == b.fWidth && a.fHeight == b.fHeight;
    }
    friend bool operator!=(const EgSize& a, const EgSize& b) {
        return a.fWidth!= b.fWidth || a.fHeight!= b.fHeight;
    }
    constexpr EgScalar area() const {
        return fWidth * fHeight;
    }
    EgISize toRound() const {
        return {EgScalarRoundToInt(fWidth), EgScalarRoundToInt(fHeight)};
    }

    EgISize toCeil() const {
        return {EgScalarCeilToInt(fWidth), EgScalarCeilToInt(fHeight)};
    }

    EgISize toFloor() const {
        return {EgScalarFloorToInt(fWidth), EgScalarFloorToInt(fHeight)};
    }
};