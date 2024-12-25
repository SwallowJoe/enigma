#pragma once

#include "include/private/base/EgCPUTypes.h"

#include "include/core/EgColor.h"
#include "include/core/EgScalar.h"
#include "include/core/EgTypes.h"
#include "include/core/EgRect.h"
#include "include/core/EgColorSpace.h"

class EG_API EgPaint {

public:
    EgPaint();
    explicit EgPaint(const EgColor4f& color, EgColorSpace* colorSpace = nullptr);
    EgPaint(const EgPaint& paint);
    EgPaint(EgPaint&& paint);

    ~EgPaint();

    EgPaint& operator=(const EgPaint& paint);
    EgPaint& operator=(EgPaint&& paint);

    EG_API friend bool operator==(const EgPaint& a, const EgPaint& b);
    EG_API friend bool operator!=(const EgPaint& a, const EgPaint& b) {
        return !(a == b);
    }

    void reset();

    bool isAntiAlias() const {
        return fWidth > 0;
    }

private:
    EgColor4f       fColor4f;
    EgScalar        fWidth;
    EgScalar        fMiterLimit;
};