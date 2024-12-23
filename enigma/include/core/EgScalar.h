#pragma once

#include "include/private/base/EgAssert.h"
#include "include/private/base/EgFloatingPoint.h"

typedef float EgScalar;

///////////////////////////////////////////////////////////////////////

#define EG_Scalar1                  1.0f
#define EG_ScalarHalf               0.5f
#define EG_ScalarPi                 3.14159265358979323846f
#define EG_ScalarPiOver2            (EG_ScalarPi * 0.5f)
#define EG_ScalarE                  2.7182818284590452354f

#define EG_ScalarMax                3.402823466e+38F
#define EG_ScalarMin                -EgScalarMax
#define EG_ScalarInfinity           EG_FloatInfinity
#define Eg_ScalarNegativeInfinity   EG_FloatNegativeInfinity
#define EG_ScalarNaN                EG_FloatNaN

#define EG_ScalarNearlyZero         (EG_Scalar1 / (1 << 12))
#define EG_ScalarSinCosNearZero     (EG_Scalar1 / (1 << 16))

///////////////////////////////////////////////////////////////////////

#define EgScalarFloorToScalar(x)    eg_float_floor(x)
#define EgScalarCeilToScalar(x)     eg_float_ceil(x)
#define EgScalarRoundToScalar(x)    eg_float_round(x)

#define EgScalarFloorToInt(x)       eg_float_floor2int(x)
#define EgScalarCeilToInt(x)        eg_float_ceil2int(x)
#define EgScalarRoundToInt(x)       eg_float_round2int(x)

#define EgScalarCopySign(x, y)      eg_float_copysign(x, y)
#define EgScalarPow(base, exponent) eg_float_pow(base, exponent)
#define EgScalarSqrt(x)             eg_float_sqrt(x)
#define EgScalarSin(x)              eg_float_sin(x)
#define EgScalarCos(x)              eg_float_cos(x)
#define EgScalarTan(x)              eg_float_tan(x)
#define EgScalarAtan2(y, x)         eg_float_atan2(y, x)
#define EgScalarAtan(x)             eg_float_atan(x)
#define EgScalarAsin(x)             eg_float_asin(x)
#define EgScalarExp(x)              eg_float_exp(x)
#define EgScalarLog(x)              eg_float_log(x)
#define EgScalarMod(x, y)           eg_float_mod(x, y)
#define EgScalarAbs(x)              eg_float_abs(x)

#define EgIntToScalar(x)            static_cast<EgScalar>(x)
#define EgIntToFloat(x)             static_cast<float>(x)
#define EgScalarTruncToInt(x)       eg_float_saturate2int(x)

#define EgScalarAve(a, b)           ((a) + ((b) - (a)) * EG_ScalarHalf)
#define EgScalarHalf(a)             ((a) * EG_ScalarHalf)

#define EgDegreesToRadians(degrees) ((degrees) * (EG_ScalarPi / 180.0f))
#define EgRadiansToDegrees(radians) ((radians) * (180.0f / EG_ScalarPi))
#define EgScalarIsZero(v)           ((v) == 0.0f)

static inline bool EgScalarIsFinite(EgScalar v) {
    return eg_float_isfinite(v);
}

static inline bool EgScalarIsNaN(EgScalar v) {
    return eg_float_isnan(v);
}

static inline EgScalar EgScalarSquare(EgScalar v) {
    return v * v;
}


static inline bool EgScalarsEqual(const EgScalar a[], const EgScalar b[], int count) {
    EgAssert(count >= 0);
    for (int i = 0; i < count; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}