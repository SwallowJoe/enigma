#include "include/core/EgBlendMode.h"


bool EgBlendMode_ShouldPreScaleCoverage(EgBlendMode mode, bool rgb_coverage) {
    // The most important things we do here are:
    //   1) never pre-scale with rgb coverage if the blend mode involves a source-alpha term;
    //   2) always pre-scale Plus.
    //
    // When we pre-scale with rgb coverage, we scale each of source r,g,b, with a distinct value,
    // and source alpha with one of those three values.  This process destructively updates the
    // source-alpha term, so we can't evaluate blend modes that need its original value.
    //
    // Plus always requires pre-scaling as a specific quirk of its implementation in
    // EgRasterPipeline.  This lets us put the clamp inside the blend mode itself rather
    // than as a separate stage that'd come after the lerp.
    //
    // This function is a finer-grained breakdown of EgBlendMode_SupportsCoverageAsAlpha().
    switch (mode) {
        case EgBlendMode::gDst:        // d              --> no sa term, ok!
        case EgBlendMode::gDstOver:    // d + s*inv(da)  --> no sa term, ok!
        case EgBlendMode::gPlus:       // clamp(s+d)     --> no sa term, ok!
            return true;

        case EgBlendMode::gDstOut:     // d * inv(sa)
        case EgBlendMode::gSrcATop:    // s*da + d*inv(sa)
        case EgBlendMode::gSrcOver:    // s + d*inv(sa)
        case EgBlendMode::gXor:        // s*inv(da) + d*inv(sa)
            return !rgb_coverage;

        default: break;
    }
    return false;
}

// Users of this function may want to switch to the rgb-coverage aware version above.
bool EgBlendMode_SupportsCoverageAsAlpha(EgBlendMode mode) {
    return EgBlendMode_ShouldPreScaleCoverage(mode, false);
}

bool EgBlendMode_AsCoeff(EgBlendMode mode, EgBlendModeCoeff* src, EgBlendModeCoeff* dst) {
    struct CoeffRec {
        EgBlendModeCoeff fSrc;
        EgBlendModeCoeff fDst;
    };

    static constexpr CoeffRec gCoeffs[] = {
        // src coeff                  dst coeff                   blend func
        //-----------------------     ------------------------    ---------------
        { EgBlendModeCoeff::gZero,    EgBlendModeCoeff::gZero }, // clear
        { EgBlendModeCoeff::gOne,     EgBlendModeCoeff::gZero }, // src
        { EgBlendModeCoeff::gZero,    EgBlendModeCoeff::gOne  }, // dst
        { EgBlendModeCoeff::gOne,     EgBlendModeCoeff::gISA  }, // src-over
        { EgBlendModeCoeff::gIDA,     EgBlendModeCoeff::gOne  }, // dst-over
        { EgBlendModeCoeff::gDA,      EgBlendModeCoeff::gZero }, // src-in
        { EgBlendModeCoeff::gZero,    EgBlendModeCoeff::gSA   }, // dst-in
        { EgBlendModeCoeff::gIDA,     EgBlendModeCoeff::gZero }, // src-out
        { EgBlendModeCoeff::gZero,    EgBlendModeCoeff::gISA  }, // dst-out
        { EgBlendModeCoeff::gDA,      EgBlendModeCoeff::gISA  }, // src-atop
        { EgBlendModeCoeff::gIDA,     EgBlendModeCoeff::gSA   }, // dst-atop
        { EgBlendModeCoeff::gIDA,     EgBlendModeCoeff::gISA  }, // xor

        { EgBlendModeCoeff::gOne,     EgBlendModeCoeff::gOne  }, // plus
        { EgBlendModeCoeff::gZero,    EgBlendModeCoeff::gSC   }, // modulate
        { EgBlendModeCoeff::gOne,     EgBlendModeCoeff::gISC  }, // screen

    };

    if (mode > EgBlendMode::gScreen) {
        return false;
    }
    if (src) {
        *src = gCoeffs[static_cast<int>(mode)].fSrc;
    }
    if (dst) {
        *dst = gCoeffs[static_cast<int>(mode)].fDst;
    }
    return true;
}

const char* EgBlendMode_Name(EgBlendMode blendMode) {
    switch (blendMode) {
        case EgBlendMode::gClear:       return "clear";
        case EgBlendMode::gSrc:         return "Src";
        case EgBlendMode::gDst:         return "Dst";
        case EgBlendMode::gSrcOver:     return "SrcOver";
        case EgBlendMode::gDstOver:     return "DstOver";
        case EgBlendMode::gSrcIn:       return "SrcIn";
        case EgBlendMode::gDstIn:       return "DstIn";
        case EgBlendMode::gSrcOut:      return "SrcOut";
        case EgBlendMode::gDstOut:      return "DstOut";
        case EgBlendMode::gSrcATop:     return "SrcATop";
        case EgBlendMode::gDstATop:     return "DstATop";
        case EgBlendMode::gXor:         return "Xor";
        case EgBlendMode::gPlus:        return "Plus";
        case EgBlendMode::gModulate:    return "Modulate";
        case EgBlendMode::gScreen:      return "Screen";
        case EgBlendMode::gOverlay:     return "Overlay";
        case EgBlendMode::gDarken:      return "Darken";
        case EgBlendMode::gLighten:     return "Lighten";
        case EgBlendMode::gColorDodge:  return "ColorDodge";
        case EgBlendMode::gColorBurn:   return "ColorBurn";
        case EgBlendMode::gHardLight:   return "HardLight";
        case EgBlendMode::gSoftLight:   return "SoftLight";
        case EgBlendMode::gDifference:  return "Difference";
        case EgBlendMode::gExclusion:   return "Exclusion";
        case EgBlendMode::gMultiply:    return "Multiply";
        case EgBlendMode::gHue:         return "Hue";
        case EgBlendMode::gSaturation:  return "Saturation";
        case EgBlendMode::gColor:       return "Color";
        case EgBlendMode::gLuminosity:  return "Luminosity";
        default: return "Unknown";
    }
}