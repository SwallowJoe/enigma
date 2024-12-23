#pragma once

#include "include/core/EgTypes.h"
#include "include/core/EgScalar.h"
#include "include/core/EgPoint.h"
#include "include/core/EgSize.h"

struct EgRect;

struct EG_API EgIRect {
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

    static constexpr EgIRect MakeLTRB(int32_t left, int32_t top, int32_t right, int32_t bottom) {
        return EgIRect{ left, top, right, bottom };
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

    constexpr EgIPoint topRight() const { return {fRight, fTop}; }

    constexpr EgIPoint bottomLeft() const { return {fLeft, fBottom}; }

    constexpr EgIPoint bottomRight() const { return {fRight, fBottom}; }

    /**
     * @return 矩形中心点坐标
     */
    constexpr EgIPoint center() const { return {(fLeft + fRight) / 2, (fTop + fBottom) / 2}; }

    void setEmpty() { memset(this, 0, sizeof(*this)); }

    /**
     * @brief 设置矩形的位置和大小
     * @param left 矩形的左边界
     * @param top 矩形的上边界
     * @param right 矩形的右边界
     * @param bottom 矩形的下边界
     */
    void setLTRB(int32_t left, int32_t top, int32_t right, int32_t bottom) {
        fLeft = left;
        fTop = top;
        fRight = right;
        fBottom = bottom;
    }

    /**
     * @brief 设置矩形的左上角和宽高
     * @param x 矩形的左上角x坐标
     * @param y 矩形的左上角y坐标
     * @param width 矩形的宽度
     * @param height 矩形的高度
     */
    void setXYWH(int32_t x, int32_t y, int32_t width, int32_t height) {
        fLeft = x;
        fTop = y;
        fRight = x + width;
        fBottom = y + height;
    }

    /**
     * @brief 设置矩形位置在坐标原点，宽高为参数
     * @param width 矩形的宽度
     * @param height 矩形的高度
     */
    void setWH(int32_t width, int32_t height) {
        fLeft = 0;
        fTop = 0;
        fRight = width;
        fBottom = height;
    }

    /**
     * @brief 设置矩形位置在坐标原点，宽高为参数
     * @param size 矩形的大小
     */
    void setSize(EgSize size) {
        fLeft = 0;
        fTop = 0;
        fRight = fLeft + size.width();
        fBottom = fTop + size.height();
    }

    /**
     * @brief 设置矩形偏移，宽高不变
     *        如果dx是负数，那么返回的矩形相对原来的矩形向左移动
     *        如果dx是正数，那么返回的矩形相对原来的矩形向右移动
     *        如果dy是负数，那么返回的矩形相对原来的矩形向上移动
     *        如果dy是正数，那么返回的矩形相对原来的矩形向下移动
     *        注意：坐标系是向下为正，向右为正
     * @param dx 水平偏移量
     * @param dy 垂直偏移量
     * @return 偏移后的矩形
     */
    constexpr EgIRect makeOffsets(int32_t dx, int32_t dy) const {
        return {
            Eg32_sat_add(fLeft,  dx), Eg32_sat_add(fTop,    dy),
            Eg32_sat_add(fRight, dx), Eg32_sat_add(fBottom, dy)
        };
    }

    /**
     * @brief 设置矩形偏移，宽高不变，返回一个新的矩形
     *        如果offset.x()是负数，那么返回的矩形相对原来的矩形向左移动
     *        如果offset.x()是正数，那么返回的矩形相对原来的矩形向右移动
     *        如果offset.y()是负数，那么返回的矩形相对原来的矩形向上移动
     *        如果offset.y()是正数，那么返回的矩形相对原来的矩形向下移动
     *        注意：坐标系是向下为正，向右为正
     * @param offset 偏移量
     * @return 偏移后的矩形
     */
    constexpr EgIRect makeOffsets(const EgIVector& offset) const {
        return makeOffsets(offset.x(), offset.y());
    }

    /**
     * @brief 返回一个新的矩形，相对于原来的矩形会被缩放
     *        如果dx是负数，那么返回的矩形相对原来的矩形会更宽
     *        如果dx是正数，那么返回的矩形相对原来的矩形会更窄
     *        如果dy是负数，那么返回的矩形相对原来的矩形会更高
     *        如果dy是正数，那么返回的矩形相对原来的矩形会更矮
     *        注意：坐标系是向下为正，向右为正
     * @param dx 水平缩放大小
     * @param dy 垂直缩放大小
     * @return 缩放后的新矩形
     */
    EgIRect makeInset(int32_t dx, int32_t dy) const {
        return {
            Eg32_sat_add(fLeft,  dx), Eg32_sat_add(fTop,    dy),
            Eg32_sat_sub(fRight, dx), Eg32_sat_sub(fBottom, dy)
        };
    }

    /**
     * @brief 返回一个新的矩形，相对于原来的矩形会被缩放
     *        如果dx是负数，那么返回的矩形相对原来的矩形会更窄
     *        如果dx是正数，那么返回的矩形相对原来的矩形会更宽
     *        如果dy是负数，那么返回的矩形相对原来的矩形会更矮
     *        如果dy是正数，那么返回的矩形相对原来的矩形会更高
     *        注意：坐标系是向下为正，向右为正
     * @param dx 水平缩放大小
     * @param dy 垂直缩放大小
     * @return 缩放后的新矩形
     */
    EgIRect makeOutset(int32_t dx, int32_t dy) const {
        return {
            Eg32_sat_sub(fLeft,  dx), Eg32_sat_sub(fTop,    dy),
            Eg32_sat_add(fRight, dx), Eg32_sat_add(fBottom, dy)
        };
    }

    /**
     * @brief 偏移矩形
     * @param dx 水平偏移量
     *            如果dx是负数，那么矩形向左移动
     *            如果dx是正数，那么矩形向右移动
     * @param dy 垂直偏移量
     *            如果dy是负数，那么矩形向上移动
     *            如果dy是正数，那么矩形向下移动
     * @return 偏移后的矩形
     */
    void offset(int32_t dx, int32_t dy) {
        fLeft   = Eg32_sat_add(fLeft,   dx);
        fTop    = Eg32_sat_add(fTop,    dy);
        fRight  = Eg32_sat_add(fRight,  dx);
        fBottom = Eg32_sat_add(fBottom, dy);
    }

    /**
     * @brief 偏移矩形
     * @param delta 偏移量
     *        如果delta.x()是负数，那么矩形向左移动
     *        如果delta.x()是正数，那么矩形向右移动
     *        如果delta.y()是负数，那么矩形向上移动
     *        如果delta.y()是正数，那么矩形向下移动
     *        注意：坐标系是向下为正，向右为正
     * @return 偏移后的矩形
     */
    void offset(const EgIVector& delta) {
        offset(delta.fX, delta.fY);
    }

    /**
     * @brief 偏移矩形到指定位置
     * @param newX 新的左上角x坐标
     * @param newY 新的左上角y坐标
     */
    void offsetTo(int32_t newX, int32_t newY) {
        fRight  = Eg64_pin_to_s32((int64_t)fRight + newX - fLeft);
        fBottom = Eg64_pin_to_s32((int64_t)fBottom + newY - fTop);
        fLeft   = newX;
        fTop    = newY;
    }

    /**
     * @brief 缩放矩形
     * @param dx 水平缩放大小
     *            如果dx是负数，那么矩形会更宽
     *            如果dx是正数，那么矩形会更窄
     * @param dy 垂直缩放大小
     *            如果dy是负数，那么矩形会更高
     *            如果dy是正数，那么矩形会更矮
     */
    void inset(int32_t dx, int32_t dy) {
        fLeft   = Eg32_sat_add(fLeft,   dx);
        fTop    = Eg32_sat_add(fTop,    dy);
        fRight  = Eg32_sat_sub(fRight,  dx);
        fBottom = Eg32_sat_sub(fBottom, dy);
    }

    /**
     * @brief 缩放矩形
     * @param dx 水平缩放大小
     *            如果dx是负数，那么矩形会更窄
     *            如果dx是正数，那么矩形会更宽
     * @param dy 垂直缩放大小
     *            如果dy是负数，那么矩形会更矮
     *            如果dy是正数，那么矩形会更高
     */
    void outset(int32_t dx, int32_t dy) {
        fLeft   = Eg32_sat_sub(fLeft,   dx);
        fTop    = Eg32_sat_sub(fTop,    dy);
        fRight  = Eg32_sat_add(fRight,  dx);
        fBottom = Eg32_sat_add(fBottom, dy);
    }

    /**
     * @brief 调整矩形，调整后矩形的边界会被调整到指定的位置
     * @param dL 左边界调整量
     * @param dT 上边界调整量
     * @param dR 右边界调整量
     * @param dB 下边界调整量
     */
    void adjust(int32_t dL, int32_t dT, int32_t dR, int32_t dB) {
        fLeft   = Eg32_sat_add(fLeft,   dL);
        fTop    = Eg32_sat_add(fTop,    dT);
        fRight  = Eg32_sat_add(fRight,  dR);
        fBottom = Eg32_sat_add(fBottom, dB);
    }

    /**
     * @brief 判断点是否在矩形内部
     * @param x 点的x坐标
     * @param y 点的y坐标
     * @return 如果点在矩形内部，返回true，否则返回false
     */
    bool contains(int32_t x, int32_t y) const {
        return x >= fLeft && x < fRight && y >= fTop && y < fBottom;
    }

    /**
     * @brief 判断参数矩形是否在当前矩形内部
     *        注意：如果参数矩形或当前矩形为空，返回false
     * @param r 要判断的矩形
     * @return 如果参数矩形在当前矩形内部，返回true，否则返回false
     */
    bool contains(const EgIRect& r) const {
        return !r.isEmpty() && !this->isEmpty() &&
            fLeft <= r.fLeft && fTop <= r.fTop &&
            fRight >= r.fRight && fBottom >= r.fBottom;
    }

    inline bool containts(const EgRect& r) const;

    /**
     * @brief 判断参数矩形是否在当前矩形内部
     *        注意：如果参数矩形或当前矩形为空，则会被断言
     * @param r 要判断的矩形
     * @return 如果矩形相交，返回true，否则返回false
     */
    bool containsNoEmptyCheck(const EgIRect& r) const {
        EgAssert(fLeft < fRight && fTop < fBottom);
        EgAssert(r.fLeft < r.fRight && r.fTop < r.fBottom);

        return fLeft <= r.fLeft && fTop <= r.fTop &&
            fRight >= r.fRight && fBottom >= r.fBottom;
    }

    /**
     * @brief 判断参数矩形是否与当前矩形相交
     * @param r 要判断的矩形
     * @return 如果矩形相交，返回true，否则返回false
     */
    bool intersect(const EgIRect& r) {
        return this->intersect(*this, r);
    }

    /**
     * @brief 判断两个矩形是否相交
     * @param a 第一个矩形
     * @param b 第二个矩形
     * @return 如果矩形相交，返回true，否则返回false
     */
    bool intersect(const EgIRect& a, const EgIRect& b);

    static bool Intersect(const EgIRect& a, const EgIRect& b) {
        return EgIRect{}.intersect(a, b);
    }

    void join(const EgIRect& r);

    /**
     * @brief 交换矩形的左右边界
     *        如果左边界大于右边界，则交换左右边界
     *        如果上边界大于下边界，则交换上下边界
     *        注意：如果矩形为空，则不会被交换，交换后矩形宽高只能是非负数
     */
    void sort() {
        using std::swap;
        if (fLeft > fRight) swap(fLeft, fRight);
        if (fTop > fBottom) swap(fTop, fBottom);
    }

    /**
     * @brief 交换矩形的左右边界, 返回一个新的矩形
     *        如果左边界大于右边界，则交换左右边界
     *        如果上边界大于下边界，则交换上下边界
     * 
     * @return 交换后的新矩形
     */
    EgIRect makeSorted() const {
        return MakeLTRB(
            std::min(fLeft, fRight), std::min(fTop, fBottom),
            std::max(fLeft, fRight), std::max(fTop, fBottom)
        );
    }
};

struct EG_API EgRect {
    EgScalar fLeft;
    EgScalar fTop;
    EgScalar fRight;
    EgScalar fBottom;
    static constexpr EgRect MakeEmpty() {
        return { 0, 0, 0, 0 };
    }
    static constexpr EgRect MakeLRTB(EgScalar left, EgScalar top, EgScalar right, EgScalar bottom) {
        return { left, top, right, bottom };
    }
    static constexpr EgRect MakeXYWH(EgScalar x, EgScalar y, EgScalar width, EgScalar height) {
        return { x, y, x + width, y + height };
    }
    static constexpr EgRect MakeWH(EgScalar width, EgScalar height) {
        return { 0, 0, width, height };
    }
    static constexpr EgRect MakeSize(const EgSize& size) {
        return { 0, 0, size.width(), size.height() };
    }
    static constexpr EgRect MakeIWH(int32_t width, int32_t height) {
        return { 0, 0, EgIntToScalar(width), EgIntToScalar(height) };
    }

    static constexpr EgRect Make(const EgIRect& irect) {
        return {
            EgIntToScalar(irect.fLeft), EgIntToScalar(irect.fTop),
            EgIntToScalar(irect.fRight), EgIntToScalar(irect.fBottom)
        };
    }

    constexpr EgScalar left() const { return fLeft; }
    constexpr EgScalar top() const { return fTop; }
    constexpr EgScalar right() const { return fRight; }
    constexpr EgScalar bottom() const { return fBottom; }
    constexpr EgScalar width() const { return fRight - fLeft; }
    constexpr EgScalar height() const { return fBottom - fTop; }
    constexpr EgScalar x() const { return fLeft; }
    constexpr EgScalar y() const { return fTop; }
    constexpr EgPoint topLeft() const { return {fLeft, fTop}; }
    constexpr EgPoint topRight() const { return {fRight, fTop}; }
    constexpr EgPoint bottomLeft() const { return {fLeft, fBottom}; }
    constexpr EgPoint bottomRight() const { return {fRight, fBottom}; }
    constexpr EgScalar centerX() const { return EgScalarHalf(fLeft) + EgScalarHalf(fRight); }
    constexpr EgScalar centerY() const { return EgScalarHalf(fTop) + EgScalarHalf(fBottom); }
    constexpr EgPoint center() const { return {centerX(), centerY()}; }
    bool isEmpty() const { return !(fLeft < fRight && fTop < fBottom); }
    bool isSorted() const { return fLeft <= fRight && fTop <= fBottom; }
    bool isFinite() const {
        float accum = 0;
        accum *= fLeft;
        accum *= fTop;
        accum *= fRight;
        accum *= fBottom;
        EgAssert(0 == accum || EgScalarIsNaN(accum));
        return !EgScalarIsNaN(accum);
    }

    friend bool operator==(const EgRect& a, const EgRect& b) {
        return EgScalarsEqual((const EgScalar*)&a, (const EgScalar*)&b, 4);
    }

    friend bool operator!=(const EgRect& a, const EgRect& b) {
        return !EgScalarsEqual((const EgScalar*)&a, (const EgScalar*)&b, 4);
    }

    void toQuad(EgPoint quad[4]) const;

    void setEmpty() { *this = MakeEmpty(); }

    void set(const EgIRect& src) {
        fLeft   = EgIntToScalar(src.fLeft);
        fTop    = EgIntToScalar(src.fTop);
        fRight  = EgIntToScalar(src.fRight);
        fBottom = EgIntToScalar(src.fBottom);
    }

    void setLTRB(EgScalar left, EgScalar top, EgScalar right, EgScalar bottom) {
        fLeft   = left;
        fTop    = top;
        fRight  = right;
        fBottom = bottom;
    }

    void setBounds(const EgPoint pts[], int count) {
        (void)this->setBoundsCheck(pts, count);
    }

    bool setBoundsCheck(const EgPoint pts[], int count);

    void setBoundsNoCheck(const EgPoint pts[], int count);

    void set(const EgPoint& p0, const EgPoint& p1) {
        fLeft   = std::min(p0.fX, p1.fX);
        fTop    = std::min(p0.fY, p1.fY);
        fRight  = std::max(p0.fX, p1.fX);
        fBottom = std::max(p0.fY, p1.fY);
    }

    void setXYWH(EgScalar x, EgScalar y, EgScalar width, EgScalar height) {
        fLeft   = x;
        fTop    = y;
        fRight  = x + width;
        fBottom = y + height;
    }

    void setWH(EgScalar width, EgScalar height) {
        fLeft   = 0;
        fTop    = 0;
        fRight  = width;
        fBottom = height;
    }

    void setIWH(int32_t width, int32_t height) {
        fLeft   = 0;
        fTop    = 0;
        fRight  = EgIntToScalar(width);
        fBottom = EgIntToScalar(height);
    }

    constexpr EgRect makeOffset(EgScalar dx, EgScalar dy) const {
        return MakeLRTB(
            fLeft + dx, fTop + dy,
            fRight + dx, fBottom + dy
        );
    }

    constexpr EgRect makeOffset(const EgVector& v) const {
        return makeOffset(v.fX, v.fY);
    }

    EgRect makeInset(EgScalar dx, EgScalar dy) const {
        return MakeLRTB(
            fLeft + dx, fTop + dy,
            fRight - dx, fBottom - dy
        );
    }

    EgRect makeOutset(EgScalar dx, EgScalar dy) const {
        return MakeLRTB(
            fLeft - dx, fTop - dy,
            fRight + dx, fBottom + dy
        );
    }

    void offset(EgScalar dx, EgScalar dy) {
        fLeft   += dx;
        fTop    += dy;
        fRight  += dx;
        fBottom += dy;
    }

    void offset(const EgVector& v) {
        offset(v.fX, v.fY);
    }

    void offsetTo(EgScalar newLeft, EgScalar newTop) {
        fRight  += newLeft - fLeft;
        fBottom += newTop - fTop;
        fLeft   = newLeft;
        fTop    = newTop;
    }

    void inset(EgScalar dx, EgScalar dy) {
        fLeft   += dx;
        fTop    += dy;
        fRight  -= dx;
        fBottom -= dy;
    }

    void outset(EgScalar dx, EgScalar dy) {
        fLeft   -= dx;
        fTop    -= dy;
        fRight  += dx;
        fBottom += dy;
    }

    bool intersect(const EgRect& r);
    bool intersect(const EgRect& a, const EgRect& b);
private:
    static bool Intersects(EgScalar al, EgScalar at, EgScalar ar, EgScalar ab,
                           EgScalar bl, EgScalar bt, EgScalar br, EgScalar bb) {
        EgScalar L = std::max(al, bl);
        EgScalar R = std::min(ar, br);
        EgScalar T = std::max(at, bt);
        EgScalar B = std::min(ab, bb);
        return L < R && T < B;
    }
public:
    bool intersects(const EgRect& r) const {
        return Intersects(fLeft, fTop, fRight, fBottom,
                         r.fLeft, r.fTop, r.fRight, r.fBottom);
    }

    static bool Intersects(const EgRect& a, const EgRect& b) {
        return Intersects(a.fLeft, a.fTop, a.fRight, a.fBottom,
                         b.fLeft, b.fTop, b.fRight, b.fBottom);
    }

    void join(const EgRect& r);

    void joinNonEmptyArg(const EgRect& r) {
        EgAssert(!r.isEmpty());
        if (fLeft >= fRight || fTop >= fBottom) {
            *this = r;
        } else {
            joinPossibiltyEmptyRect(r);
        }
    }

    void joinPossibiltyEmptyRect(const EgRect& r) {
        fLeft = std::min(fLeft, r.fLeft);
        fTop = std::min(fTop, r.fTop);
        fRight = std::max(fRight, r.fRight);
        fBottom = std::max(fBottom, r.fBottom);
    }

    bool contains(EgScalar x, EgScalar y) const {
        return x >= fLeft && x < fRight && y >= fTop && y < fBottom;
    }

    bool contains(const EgRect& r) const {
        return !r.isEmpty() &&!this->isEmpty() &&
            fLeft <= r.fLeft && fTop <= r.fTop &&
            fRight >= r.fRight && fBottom >= r.fBottom;
    }

    bool contains(const EgIRect& r) const {
        return !r.isEmpty() && !this->isEmpty() &&
            fLeft <= EgIntToScalar(r.fLeft) && fTop <= EgIntToScalar(r.fTop) &&
            fRight >= EgIntToScalar(r.fRight) && fBottom >= EgIntToScalar(r.fBottom);
    }

    void round(EgIRect* dst) {
        EgAssert(dst);
        dst->setLTRB(
            EgScalarRoundToInt(fLeft), EgScalarRoundToInt(fTop),
            EgScalarRoundToInt(fRight), EgScalarRoundToInt(fBottom)
        );
    }

    void roundOut(EgIRect* dst) {
        EgAssert(dst);
        dst->setLTRB(
            EgScalarFloorToInt(fLeft), EgScalarFloorToInt(fTop),
            EgScalarCeilToInt(fRight), EgScalarCeilToInt(fBottom)
        );
    }

    void roundOut(EgRect* dst) {
        EgAssert(dst);
        dst->setLTRB(
            EgScalarFloorToScalar(fLeft), EgScalarFloorToScalar(fTop),
            EgScalarCeilToScalar(fRight), EgScalarCeilToScalar(fBottom)
        );
    }

    void roundIn(EgIRect* dst) {
        EgAssert(dst);
        dst->setLTRB(
            EgScalarCeilToInt(fLeft), EgScalarCeilToInt(fTop),
            EgScalarFloorToInt(fRight), EgScalarFloorToInt(fBottom)
        );
    }

    void sort() {
        using std::swap;
        if (fLeft > fRight) swap(fLeft, fRight);
        if (fTop > fBottom) swap(fTop, fBottom);
    }

    void setSize(const EgSize& size) {
        fRight = fLeft + size.width();
        fBottom = fTop + size.height();
    }

    EgRect makeSorted() const {
        return MakeLRTB(
            std::min(fLeft, fRight), std::min(fTop, fBottom),
            std::max(fLeft, fRight), std::max(fTop, fBottom)
        );
    }

    const EgScalar* asScalars() const {
        return &fLeft;
    }

    void dump(bool asHex) const;
    void dump() const { dump(false); }
    void dumpHex() const { dump(true); }
};

inline bool EgIRect::containts(const EgRect& r) const {
    return !r.isEmpty() && !this->isEmpty() &&
        (EgScalar)fLeft <= r.fLeft && (EgScalar)fTop <= r.fTop &&
        (EgScalar)fRight >= r.fRight && (EgScalar)fBottom >= r.fBottom;
}