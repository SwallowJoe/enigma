#pragma once

#include "include/private/base/EgAPI.h"

#include "src/gpu/lantern/glsl/EgGLSLShaderBuilder.h"

#include <string>
#include <vector>

class EgGLSLVertextGeometryShaderBuilder : public EgGLSLShaderBuilder {
public:
    void insertFunction(const char* functionDefintion) {
        this->functions().append(functionDefintion);
    }
};

class EG_API EgGLSLVertexShaderBuilder {

public:

private:
};