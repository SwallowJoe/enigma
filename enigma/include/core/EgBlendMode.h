#pragma once

#include "include/private/base/EgAPI.h"

enum class EgBlendMode {
    gClear,
    gSrc,
    gDst,
    gSrcOver,
    gDstOver,
    gSrcIn,
    gDstIn,
    gSrcOut,
    gDstOut,
    gSrcATop,
    gDstATop,
    gXor,
    gPlus,
    gModulate,
    gScreen,

    gOverlay,
    gDarken,
    gLighten,
    gColorDodge,
    gColorBurn,
    gHardLight,
    gSoftLight,
    gDifference,
    gExclusion,
    gMultiply,

    gHue,
    gSaturation,
    gColor,
    gLuminosity,

    gLastCoeffMode          = gScreen,
    gLastSeparableMode      = gMultiply,
    gLastMode               = gLuminosity,
};


static constexpr int gEgBlendModeCount = static_cast<int>(EgBlendMode::gLastMode) + 1;

enum class EgBlendModeCoeff {
    gZero,  // 0
    gOne,   // 1
    gSC,    // src color
    gISC,   // inverse src color
    gDC,    // dst color
    gIDC,   // inverse dst color
    gSA,    // src alpha
    gISA,   // inverse src alpha
    gDA,    // dst alpha
    gIDA,   // inverse dst alpha

    gCoeffCount
};

EG_API bool EgBlendMode_AsCoeff(EgBlendMode mode, EgBlendModeCoeff* src, EgBlendModeCoeff* dst);

EG_API const char* EgBlendMode_Name(EgBlendMode blendMode);