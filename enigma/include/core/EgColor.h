#pragma once

#include "include/private/base/EgCPUTypes.h"
#include "include/private/base/EgAssert.h"
#include "include/private/base/EgAPI.h"

#include "include/core/EgAlphaType.h"

#include <array>
#include <cstdint>

typedef uint8_t EgAlpha;

typedef uint32_t EgColor;

static constexpr inline EgColor EgColorSetARGB(U8CPU a, U8CPU r, U8CPU g, U8CPU b) {
    return EgAssert(a <= 255 && r <= 255 && g <= 255 && b <= 255),
            (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

#define EgColorSetRGB(r, g, b)      EgColorSetARGB(0xFF, r, g, b)

#define EgColorGetA(color)          (((color) >> 24) & 0xFF)

#define EgColorGetR(color)          (((color) >> 16) & 0xFF)

#define EgColorGetG(color)          (((color) >> 8) & 0xFF)

#define EgColorGetB(color)          (((color) >> 0) & 0xFF)

/**
 * @brief 设置颜色的Alpha通道值
 * 
 * 该函数接受一个颜色值和一个Alpha通道值，返回一个新的颜色值，其中Alpha通道被设置为指定的值。
 * 
 * @param c 输入的颜色值
 * @param a 要设置的Alpha通道值（0-255）
 * @return EgColor 设置了Alpha通道后的新颜色值
 */
static constexpr inline EgColor EgColorSetA(EgColor c, U8CPU a) {
    // 清除原始颜色值的Alpha通道（将其设置为0）
    // 然后将新的Alpha通道值左移24位（移到最高字节）
    // 最后将两者进行按位或操作，以设置新的Alpha通道值
    return (c & 0x00FFFFFF) | (a << 24);
}

constexpr EgAlpha EG_AlphaTransparent       = 0x00;
constexpr EgAlpha EG_AlphaOpaque            = 0xFF;


constexpr EgColor EG_ColorTransparent       = EgColorSetARGB(0x00, 0x00, 0x00, 0x00);
constexpr EgColor EG_ColorBlack             = EgColorSetARGB(0xFF, 0x00, 0x00, 0x00);
constexpr EgColor EG_ColorDkGray            = EgColorSetARGB(0xFF, 0x44, 0x44, 0x44);
constexpr EgColor EG_ColorGray              = EgColorSetARGB(0xFF, 0x88, 0x88, 0x88);
constexpr EgColor EG_ColorLtGray            = EgColorSetARGB(0xFF, 0xCC, 0xCC, 0xCC);
constexpr EgColor EG_ColorWhite             = EgColorSetARGB(0xFF, 0xFF, 0xFF, 0xFF);
constexpr EgColor Eg_ColorRed               = EgColorSetARGB(0xFF, 0xFF, 0x00, 0x00);
constexpr EgColor Eg_ColorGreen             = EgColorSetARGB(0xFF, 0x00, 0xFF, 0x00);
constexpr EgColor Eg_ColorBlue              = EgColorSetARGB(0xFF, 0x00, 0x00, 0xFF);
constexpr EgColor Eg_ColorYellow            = EgColorSetARGB(0xFF, 0xFF, 0xFF, 0x00);
constexpr EgColor Eg_ColorCyan              = EgColorSetARGB(0xFF, 0x00, 0xFF, 0xFF);
constexpr EgColor Eg_ColorMagenta           = EgColorSetARGB(0xFF, 0xFF, 0x00, 0xFF);

template <EgAlphaType gAT>
struct EgRGBA4f {
    float fR;
    float fG;
    float fB;
    float fA;

    bool operator==(const EgRGBA4f& other) const {
        return fA == other.fA && fR == other.fR && fG == other.fG && fB == other.fB;
    }

    bool operator!=(const EgRGBA4f& other) const {
        return !(*this == other);
    }
    
    EgRGBA4f operator*(float scale) const {
        return { fR * scale, fG * scale, fB * scale, fA * scale};
    }

    EgRGBA4f operator*(const EgRGBA4f& scale) {
        return { fR* scale.fR, fG * scale.fG, fB * scale.fB, fA * scale.fA };
    }

    const float* vec() const { return &fR; }

    float* vec() { return &fR; }

    std::array<float, 4> array() const { return {fR, fG, fB, fA}; }

    float operator[](int index) const {
        EgAssert(index >= 0 && index < 4);

        return this->vec()[index];
    }

    bool isOpaque() const {
        EgAssert(fA <= 1.0f && fA >= 0.0f);
        return fA == 1.0f;
    }

    bool fitsInBytes() const {
        EgAssert(fA >= 0.0f && fA <= 1.0f);
        return fR >= 0.0f && fR <= 1.0f &&
               fG >= 0.0f && fG <= 1.0f &&
               fB >= 0.0f && fB <= 1.0f;
    }

    static EgRGBA4f FromColor(EgColor color);

    EgColor toEgColor() const;

    // static EgRGBA4f FromPMColor(EgPMColor);

    EgRGBA4f<gPremul_EgAlphaType> premul() const {
        static_assert(gAT == gUnpremul_EgAlphaType, "");
        return { fR*fA, fG*fA, fB*fA, fA};
    }

    EgRGBA4f<gUnpremul_EgAlphaType> unpremul() const {
        static_assert(gAT == gPremul_EgAlphaType, "");
        if (fA == 0.0f) {
            return { 0,0,0,0 };
        } else {
            float invAlpha = 1 / fA;
            return { fR * invAlpha, fG * invAlpha, fB * invAlpha, fA };
        }
    }

    uint32_t toBytes_RGBA() const;

    static EgRGBA4f FromBytes_RGBA(uint32_t color);

    EgRGBA4f makeOpaque() const {
        return { fR, fG, fB, 1.0f };
    }
};

using EgColor4f = EgRGBA4f<gUnpremul_EgAlphaType>;

template <> EG_API EgColor4f    EgColor4f::FromColor(EgColor);
template <> EG_API EgColor      EgColor4f::toEgColor() const;
template <> EG_API uint32_t     EgColor4f::toBytes_RGBA() const;
template <> EG_API EgColor4f    EgColor4f::FromBytes_RGBA(uint32_t color);

namespace EgColors {

    constexpr EgColor4f gTransparent    = {0,       0,      0,      0};
    constexpr EgColor4f gBlack          = {0,       0,      0,      1};
    constexpr EgColor4f gDkGray         = {0.25f,   0.25f,  0.25f,  1};
    constexpr EgColor4f gGray           = {0.50f,   0.50f,  0.50f,  1};
    constexpr EgColor4f gLtGray         = {0.75f,   0.75f,  0.75f,  1};
    constexpr EgColor4f gWhite          = {1,       1,      1,      1};
    constexpr EgColor4f gRed            = {1,       0,      0,      1};
    constexpr EgColor4f gGreen          = {0,       1,      0,      1};
    constexpr EgColor4f gBlue           = {0,       0,      1,      1};
    constexpr EgColor4f gYellow         = {1,       1,      0,      1};
    constexpr EgColor4f gCyan           = {0,       1,      1,      1};
    constexpr EgColor4f gMagenta        = {1,       0,      1,      1};
  
};

