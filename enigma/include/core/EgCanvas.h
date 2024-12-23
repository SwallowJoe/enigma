#pragma once

#include "include/private/base/EgAPI.h"

#include "include/core/EgBlendMode.h"
#include "include/core/EgColor.h"
#include "include/core/EgPaint.h"

class EG_API EgCanvas {

public:

    ////////////////////  Draw API //////////////////////
    void drawColor(EgColor4f& color, EgBlendMode mode = EgBlendMode::gScrOver);

    void drawColor(EgColor color, EgBlendMode mode = EgBlendMode::gScrOver) {
        drawColor(EgColor4f::FromColor(color), mode);
    }

    void clear(EgColor color) {
        clear(EgColor4f::FromColor(color));
    }

    void clear(EgColor4f& color) {
        drawColor(color, EgBlendMode::gSrc);
    }

    void discard() { this->onDiscard(); }

    void drawPaint(const EgPaint& paint);

    /////////////////////////////////////////////////////
protected:
    virtual void onDiscard();
private:

};