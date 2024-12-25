#pragma once

#include "include/private/base/EgAPI.h"
#include "include/private/base/EgTypeTraits.h"
#include "include/private/base/EgTArray.h"

#include <string>
#include <vector>

class EG_API EgGLSLShaderBuilder {

public:
    explicit EgGLSLShaderBuilder(EgGLSLProgramBuilder* builder);
    virtual ~EgGLSLShaderBuilder();


protected:
    enum {
        eExtensions,
        eDefinitions,
        ePrecisionQualifier,
        eLayoutQualifiers,
        eUniforms,
        eInputs,
        eOutputs,
        eFunctions,
        eMain,
        eCode,

        ePrealloc = eCode + 6,
    };

    std::string& main() { return mCode[eMain]; }
    std::string& code() { return mCode[eCode]; }


    EgTArray<ePrealloc, std::string> mShaderStrings;

    EgSTArray<1, std::string> mLayoutParams[];
};