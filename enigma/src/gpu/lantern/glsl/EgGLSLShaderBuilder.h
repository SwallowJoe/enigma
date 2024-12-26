#pragma once

#include "include/private/base/EgAPI.h"
#include "include/private/base/EgTypeTraits.h"
#include "include/private/base/EgTArray.h"

#include "src/gpu/lantern/glsl/EgGLSLProgramBuilder.h"

#include <string>
#include <vector>

class EG_API EgGLSLShaderBuilder {

public:
    explicit EgGLSLShaderBuilder(EgGLSLProgramBuilder* program);
    virtual ~EgGLSLShaderBuilder();

    EgGLSLShaderBuilder& addExtension(const char* extension);
    EgGLSLShaderBuilder& addDefinition(const char* definition);
    EgGLSLShaderBuilder& addPrecisionQualifier(const char* precisionQualifier);
    EgGLSLShaderBuilder& addLayoutQualifier(const char* layoutQualifier);
    EgGLSLShaderBuilder& addUniform(const char* uniform);
    EgGLSLShaderBuilder& addInput(const char* input);
    EgGLSLShaderBuilder& addOutput(const char* output);
    EgGLSLShaderBuilder& addFunction(const char* function);
    EgGLSLShaderBuilder& addMain(const char* main);
    EgGLSLShaderBuilder& addCode(const char* code);

    EgGLSLShaderBuilder& defineConstant(const char* type, const char* name, const char* value);
    EgGLSLShaderBuilder& defineConstant(const char* name, int value);
    EgGLSLShaderBuilder& defineConstant(const char* name, float value);

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


    std::string& extensions() { return mShaderStrings[eExtensions]; }
    std::string& main() { return mShaderStrings[eMain]; }
    std::string& code() {
        return mShaderStrings[eCode];
    }

    EgGLSLProgramBuilder* mProgram;
    std::string mCompilerString;
    EgTArray<ePrealloc, std::string> mShaderStrings;

    EgSTArray<1, std::string> mLayoutParams[];
};