#pragma once

#include "include/private/base/EgAPI.h"

class EG_API EgShader {

public:
    virtual bool isOpaque() const { return false; }
    
private:

};