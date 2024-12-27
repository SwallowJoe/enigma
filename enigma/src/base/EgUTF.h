#pragma once

#include "include/private/base/EgAPI.h"
#include "include/private/base/EgTFitsIn.h"

// cstddef 是 C++ 语言标准库中的头文件，定义了一些
// 与 C 语言标准库中的头文件相同的函数和宏，但是放在了 std 命名空间中。
// stddef.h 是 C 语言标准库中的头文件，包含了一些与 C 语言中的数据类型有关的宏。
#include <cstddef>
// cstdint 是 C++ 语言标准库中的头文件，定义了一些
// 与 C 语言标准库中的头文件相同的函数和宏，但是放在了 std 命名空间中。
// stdint.h 是 C 语言标准库中的头文件，定义了一些与整数类型相关的宏。
#include <cstdint>


typedef int32_t EgUnichar;

namespace EgUTF {

/**
 * @brief 将 int32_t 类型的数字向左移动
 * @param value 要移动的数字
 * @param shift 移动的位数
 * @return 移动后的数字
 * @note 该函数不会检查 shift 是否为负数
 * @note 该函数不会检查 shift 是否大于 32
 * @note 该函数不会检查 value 是否为负数
 */
static constexpr inline int32_t left_shift(int32_t value, int32_t shift) {
    return (int32_t)((uint32_t)value << shift);
}

template <typename T>
static constexpr bool is_align2(T value) {
    return (value & 0x1) == 0;
}

template <typename T>
static constexpr bool is_align4(T value) {
    return (value & 0x3) == 0;
}

static constexpr inline bool utf16_is_high_surrogate(uint16_t c) {
    return (c & 0xFC00) == 0xD800;
}

static constexpr inline bool utf16_is_low_surrogate(uint16_t c) {
    return (c & 0xFC00) == 0xDC00;
}

/**
 * @brief 判断是否是 UTF-8 编码的前导字节
 * @param c 字符
 * @return 是否是 UTF-8 编码的前导字节
 * @note UTF-8 编码的前导字节范围是 0xC0 ~ 0xFD
 */
static int utf8_byte_type(uint8_t c) {
    if (c < 0x80) {
        return 1;
    } else if (c < 0xC0) {
        return 0;
    } else if (c >= 0xF5 || (c & 0xFE) == 0xC0) {
        return -1;
    } else {
        int value = (((0xE5 << 24) >> ((unsigned)c >> 4 << 1)) & 0x3) + 1;
        return value;
    }
}

static bool utf8_is_valid_leading_byte(int type) {
    return type > 0;
}

/**
 * @brief 判断是否是 UTF-8 编码的后续字节
 * @param c 字符
 * @return 是否是 UTF-8 编码的后续字节
 */
static bool utf8_byte_is_continuation(uint8_t c) {
    return utf8_byte_type(c) == 0;
}

/**
 * @brief 计算 UTF-8 编码的字符数
 * @param utf8 UTF-8 编码的字符串
 * @param byteLength 字符串长度
 * @return 字符数
 * @note 如果字符串长度为 0，则返回 0
 * @note 如果字符串长度不为 0，但是字符串不是有效的 UTF-8 编码，则返回 -1
 */
EG_API int CountUTF8(const char* utf8, size_t byteLength) {
    if (!utf8 && byteLength) {
        return -1;
    }
    int count = 0;
    const char* stop = utf8 + byteLength;
    while (utf8 < stop) {
        int type = utf8_byte_type(*(const uint8_t*)utf8);
        if (!utf8_is_valid_leading_byte(type) || utf8 + type > stop) {
            return -1;
        }
        while(type-- > 1) {
            ++utf8;
            if (!utf8_byte_is_continuation(*(const uint8_t*)utf8)) {
                return -1;
            }
        }
        ++utf8;
        ++count;
    }
    return count;
}

EG_API int CountUTF16(const char16_t* utf16, size_t charLength) {
    // TODO
    return 0;
}

EG_API int CountUTF32(const char32_t* utf32, size_t charLength) {
    // TODO
    return 0;
}

template <typename T>
static EgUnichar next_fail(const T** ptr, const T* end) {
    *ptr = end;
    return -1;
}

EG_API EgUnichar NextUTF8(const char** ptr, const char* end) {
    // TODO:
    return 0;
}

EG_API EgUnichar NextUTF16(const uint16_t** ptr, const uint16_t* end) {
    // TODO:

    return 0;
}

EG_API EgUnichar NextUTF32(const uint32_t** ptr, const uint32_t* end) {
    // TODO:
    return 0;
}

constexpr unsigned char kMaxBytesInUTF8Sequence = 4;

/**
 * @brief 将 Unicode 字符转换为 UTF-8 编码
 * @param unichar Unicode 字符
 * @param utf8 UTF-8 编码
 * @return UTF-8 编码的字节数
 */
EG_API size_t ToUTF8(EgUnichar unichar, char utf8[kMaxBytesInUTF8Sequence] = nullptr) {
    if ((uint32_t)unichar > 0x10FFFF) {
        return 0;
    }
    if (unichar <= 127) {
        if (utf8) {
            *utf8 = (char)unichar;
        }
        return 1;
    }
    char tmp[4];
    char* p = tmp;
    size_t count = 1;
    while (unichar > 0x7F >> count) {
        *p++ = (char)(0x80 | (unichar & 0x3F));
        unichar >>= 6;
        ++count;
    }
    if (utf8) {
        p = tmp;
        utf8 += count;
        while (p < tmp + count -1) {
            *--utf8 = *p++;
        }
        *--utf8 = (char)(~(0xFF >> count) | unichar);
    }
    return count;
}

/**
 * @brief 将 Unicode 字符转换为 UTF-16 编码
 * @param unichar Unicode 字符
 * @param utf16 UTF-16 编码
 * @return UTF-16 编码的字符数
 */
EG_API size_t ToUTF16(EgUnichar unichar, uint16_t utf16[2] = nullptr) {
    if ((uint32_t)unichar > 0x10FFFF) {
        return 0;
    }
    int extra = (unichar > 0xFFFF);
    if (utf16) {
        if (extra) {
            utf16[0] = (uint16_t)((0xD800 - 64) + (unichar >> 10));
            utf16[1] = (uint16_t)(0xDC00 | (unichar & 0x3FF));
        } else {
            utf16[0] = (uint16_t)unichar;
        }
    }
    return 1 + extra;
}

EG_API int UTF8ToUTF16(uint16_t* dst, int dstCapacity, const char src[], size_t scrByteLength) {
    // TODO:
    return 0;
}


EG_API int UTF16ToUTF8(char dst[], int dstCapacity, const uint16_t src[], int srcCharLength) {
    // TODO: 将 UTF-16 编码的字符串转换为 UTF-8 编码的字符串
    if (!dst) {
        dstCapacity = 0;
    }
    int dstLength = 0;
    const char* endDst = dst + dstCapacity;
    const uint16_t* endSrc = src + srcCharLength;

    return 0;
}

/**
 * @brief 判断是否是 UTF-16 编码的前导代理
 * @param c 字符
 * @return 是否是 UTF-16 编码的前导代理
 * @note UTF-16 编码的前导代理范围是 0xD800 ~ 0xDBFF
 */
static inline bool isLeadingSurrogateUTF16(uint16_t c) {
    return (c & 0xFC00) == 0xD800;
}

/**
 * @brief 判断是否是 UTF-16 编码的后尾代理
 * @param c 字符
 * @return 是否是 UTF-16 编码的后尾代理
 * @note UTF-16 编码的后尾代理范围是 0xDC00 ~ 0xDFFF
 */
static inline bool isTrailingSurrogateUTF16(uint16_t c) {
    return (c & 0xFC00) == 0xDC00;
}

};