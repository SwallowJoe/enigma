#pragma once

#include "include/private/base/EgFloatingPoint.h"

typedef float EgScalar;

#define EgScalarAbs(x)          eg_float_abs(x)

#define EgIntToScalar(x)        static_cast<EgScalar>(x)
#define EgIntToFloat(x)         static_cast<float>(x)
#define EgScalarTruncToInt(x)   eg_float_saturate2int(x)