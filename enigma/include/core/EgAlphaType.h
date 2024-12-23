#pragma once

enum EgAlphaType: int {
    gUnknown_EgAlphaType,
    gOpaque_EgAlphaType,
    gPremul_EgAlphaType,
    gUnpremul_EgAlphaType,
    gLastEnum_EgAlphaType   = gUnpremul_EgAlphaType,
};

static inline bool EgAlphaTypeIsOpaque(EgAlphaType at) {
    return gOpaque_EgAlphaType == at;
}