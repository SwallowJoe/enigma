#include "include/core/EgRect.h"

bool EgIRect::intersect(const EgIRect& a, const EgIRect& b) {
    EgIRect tmp = {
        std::max(a.fLeft,    b.fLeft),
        std::max(a.fTop,     b.fTop),
        std::min(a.fRight,   b.fRight),
        std::min(a.fBottom,  b.fBottom)
    };
    if (tmp.isEmpty()) {
        return false;
    }
    *this = tmp;
    return true;
}

void EgIRect::join(const EgIRect& r) {
    if (r.isEmpty()) {
        return;
    }
    if (isEmpty()) {
        *this = r;
        return;
    }
    fLeft   = std::min(fLeft,   r.fLeft);
    fTop    = std::min(fTop,    r.fTop);
    fRight  = std::max(fRight,  r.fRight);
    fBottom = std::max(fBottom, r.fBottom);
}

////////////////////////////////////////////////////////

void EgRect::toQuad(EgPoint quad[4]) const {
    EgAssert(quad);

    quad[0].set(fLeft, fTop);
    quad[1].set(fRight, fTop);
    quad[2].set(fRight, fBottom);
    quad[3].set(fLeft, fBottom);
}


#include "src/base/EgVx.h"

bool EgRect::setBoundsCheck(const EgPoint pts[], int count) {
    EgAssert((pts) && (count > 0) || count == 0);

    if (count == 0) {
        setEmpty();
        return true;
    }

    egvx::float4 min, max;
    if (count & 1) {
        min = max = egvx::float2::Load(pts).xyxy();
        pts += 1;
        count -= 1;
    } else {
        min = max = egvx::float4::Load(pts);
        pts += 2;
        count -= 2;
    }

    egvx::float4 accum = min * 0;
    while (count) {
        egvx::float4 xy = egvx::float4::Load(pts);
        accum = accum * xy;
        min = egvx::min(min, xy);
        max = egvx::max(max, xy);
        pts += 2;
        count -= 2;
    }

    const bool all_finite = all(accum * 0 == 0);
    if (all_finite) {
        setLTRB(std::min(min[0], min[2]), std::min(min[1], min[3]),
                std::max(max[0], max[2]), std::max(max[1], max[3]));
    } else {
        setEmpty();
    }
    return all_finite;   
}

void EgRect::setBoundsNoCheck(const EgPoint pts[], int count) {
    if (!setBoundsCheck(pts, count)) {
        setLTRB(EG_ScalarNaN, EG_ScalarNaN, EG_ScalarNaN, EG_ScalarNaN);
    }
}

#define CHECK_INTERSECT(al, at, ar, ab, bl, bt, br, bb) \
    EgScalar L = std::max(al, bl);                      \
    EgScalar T = std::max(at, bt);                      \
    EgScalar R = std::min(ar, br);                      \
    EgScalar B = std::min(ab, bb);                      \
    do { if (!(L < R && T < B)) return false; } while (0)

bool EgRect::intersect(const EgRect& r) {
    CHECK_INTERSECT(r.fLeft, r.fTop, r.fRight, r.fBottom, fLeft, fTop, fRight, fBottom);
    setLTRB(L, T, R, B);
    return true;
}

bool EgRect::intersect(const EgRect& a, const EgRect& b) {
    CHECK_INTERSECT(a.fLeft, a.fTop, a.fRight, a.fBottom, b.fLeft, b.fTop, b.fRight, b.fBottom);
    setLTRB(L, T, R, B);
    return true;
}

void EgRect::join(const EgRect& r) {
    if (r.isEmpty()) {
        return;
    }
    if (isEmpty()) {
        *this = r;
    } else {
        fLeft   = std::min(fLeft,   r.fLeft);
        fTop    = std::min(fTop,    r.fTop);
        fRight  = std::max(fRight,  r.fRight);
        fBottom = std::max(fBottom, r.fBottom);
    }
}

/////////////////////////////////////////////////////////


void EgRect::dump(bool asHex) const {
    // TODO:
}