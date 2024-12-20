#pragma once

#include "include/core/EgTypes.h"
#include "include/core/EgScalar.h"
#include "include/core/EgPoint.h"

struct EgRect;

struct EgIRect {
    int32_t fLeft;  // 矩形的左边界
    int32_t fTop;   // 矩形的上边界
    int32_t fRight; // 矩形的右边界
    int32_t fBottom;// 矩形的下边界

    constexpr bool isEmpty() const {
        return fLeft >= fRight || fTop >= fBottom;
    }
 
    /**
     * 创建一个空的矩形
     */
    static constexpr EgIRect MakeEmpty() {
        return {0, 0, 0, 0};
    }

    /**
     * @brief 从左上角和右下角创建一个矩形
     * @param left 矩形的左上角x坐标
     * @param top 矩形的左上角y坐标
     * @param right 矩形的右下角x坐标
     * @param bottom 矩形的右下角y坐标
     * @return 一个矩形
     */
    static constexpr EgIRect Make(int32_t left, int32_t top, int32_t right, int32_t bottom) {
        return {left, top, right, bottom};
    }

    /**
     * @brief 从左上角和宽高创建一个矩形
     * @param left 矩形的左上角x坐标
     * @param top 矩形的左上角y坐标
     * @param width 矩形的宽度
     * @param height 矩形的高度
     * @return 一个矩形
     */
    static constexpr EgIRect MakeWH(int32_t left, int32_t top, int32_t width, int32_t height) {
        return {left, top, left + width, top + height};
    }

    /**
     * @brief 从左上角和宽高创建一个矩形
     * @param point 矩形的左上角坐标
     * @param width 矩形的宽度
     * @param height 矩形的高度
     * @return 一个矩形
     */
    static constexpr EgIRect MakeWH(const EgIPoint& point, int32_t width, int32_t height) {
        return {point.x(), point.y(), point.x() + width, point.y() + height};
    }

    /**
     * 矩形左边界x坐标
     */
    constexpr int32_t left() const { return fLeft; }

    /**
     * 矩形上边界y坐标
     */
    constexpr int32_t top() const { return fTop; }

    /**
     * 矩形右边界x坐标
     */
    constexpr int32_t right() const { return fRight; }

    /**
     * 矩形下边界y坐标
     */
    constexpr int32_t bottom() const { return fBottom; }

    /**
     * 矩形宽度
     */
    constexpr int32_t width() const { return fRight - fLeft; }

    /**
     * 矩形高度
     */
    constexpr int32_t height() const { return fBottom - fTop; }

    constexpr int32_t x() const { return fLeft; }
    constexpr int32_t y() const { return fTop; }

    constexpr EgIPoint topLeft() const { return {fLeft, fTop}; }
};