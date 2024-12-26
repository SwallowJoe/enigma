
#include "include/private/base/EgTo.h"
#include "include/private/base/EgAssert.h"

#include "include/core/EgString.h"

#include "src/base/EgSafeMath.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string_view>
#include <new>

static const size_t kBufferSize = 1024;

struct StringBuffer {
    char* mData;
    int mLength;
};

template <int SIZE>
static StringBuffer apply_format_string(const char* fmt,
                                        va_list args,
                                        char (&buffer)[SIZE],
                                        EgString* heapBuffer) {

    va_list argsCopy;
    va_copy(argsCopy, args);
    int outLen = std::vsnprintf(buffer, SIZE, fmt, args);
    if (outLen < 0) {
        va_end(argsCopy);
        return {buffer, 0};
    }

    if (outLen < SIZE) {
        va_end(argsCopy);
        return {buffer, outLen};
    }

    heapBuffer->set(nullptr, outLen);
    char* heapBufferDst = heapBuffer->data();
    va_end(argsCopy);
    return {heapBufferDst, outLen};
}

///////////////////////////////////////////////////////////////


bool EgStrEndsWith(const char str[], const char suffixStr[]) {
    EgAssert(str != nullptr);
    EgAssert(suffixStr != nullptr);

    size_t strLen = strlen(str);
    size_t suffixLen = strlen(suffixStr);


    return  strLen >= suffixLen &&
                !strncmp(str + strLen - suffixLen, suffixStr, suffixLen);
}

bool EgStrEndsWith(const char str[], const char suffixChar) {
    EgAssert(str != nullptr);

    size_t strLen = strlen(str);

    return strLen > 0 && str[strLen - 1] == suffixChar;
}

int EgStrStartsWithOneOf(const char str[], const char prefixes[]) {
    int index = 0;
    do {
        const char* limit = strchr(prefixes, '\0');
        if (!strncmp(str, prefixes, limit - prefixes)) {
            return index;
        }
        prefixes = limit + 1;
        index++;
    } while (prefixes[0]);
    return -1;
}

/**
 * @brief 将 int32_t 类型的数字转换为字符串
 * @param dst 字符串缓冲区
 * @param dec 要转换的数字
 * @return 转换后的字符串
 * @note 该函数不会检查缓冲区是否足够大
 * @note 该函数不会检查 dst 是否为 nullptr
 */
char* EgStrAppendU32(char dst[], uint32_t dec) {
    // 10 位数字的最大值是 4294967295
    char buffer[kEgStrAppendU32_MaxSize];
    // 从后往前填充数字
    char* p = buffer + sizeof(buffer);
    do {
        // 取余数
        *--p =  EgToU8('0' + dec % 10);
        // 取整数
        dec /= 10;
    } while (dec != 0);

    EgAssert(p >= buffer);
    // 计算拷贝的长度
    size_t cp_len = buffer + sizeof(buffer) - p;
    // 拷贝到目标缓冲区
    memcpy(dst, p, cp_len);
    // 移动目标缓冲区指针
    dst += cp_len;

    return dst;
}

/**
 * @brief 将 int32_t 类型的数字转换为字符串
 * @param dst 字符串缓冲区
 * @param dec 要转换的数字
 * @return 转换后的字符串
 */
char* EgStrAppendS32(char dst[], int32_t dec) {
    uint32_t udec = dec;
    // 如果是负数，先加上负号，然后取反加一
    // 为啥要取反加一呢？
    // 因为 int32_t 的最小值是 -2147483648，而 int32_t 的最大值是 2147483647
    // 所以 int32_t 的最小值的绝对值比最大值的绝对值大 1
    if (dec < 0) {
        *dst++ = '-';
        udec = ~udec + 1;
    }
    return EgStrAppendU32(dst, udec);
}

/**
 * @brief 将 uint64_t 类型的数字转换为字符串
 * @param dst 字符串缓冲区
 * @param dec 要转换的数字
 * @param minDigits 最小位数
 * @return 转换后的字符串
 * @note 该函数不会检查缓冲区是否足够大
 * @note 该函数不会检查 dst 是否为 nullptr
 * @note 该函数不会检查 minDigits 是否为负数
 */
char* EgStrAppendU64(char dst[], uint64_t dec, int minDigits) {
    char buffer[kEgStrAppendU64_MaxSize];
    char* p = buffer + sizeof(buffer);
    do {
        *--p = EgToU8('0' + (int32_t)(dec % 10));
        dec /= 10;
        minDigits--;
    } while (dec != 0);

    while (minDigits > 0) {
        *--p = '0';
        minDigits--;
    }

    EgAssert(p >= buffer);
    size_t cp_len = buffer + sizeof(buffer) - p;
    memcpy(dst, p, cp_len);
    dst += cp_len;

    return dst;
}

/**
 * @brief 将 int64_t 类型的数字转换为字符串
 * @param dst 字符串缓冲区
 * @param dec 要转换的数字
 * @param minDigits 最小位数
 * @return 转换后的字符串
 * @note 该函数不会检查缓冲区是否足够大
 */
char* EgStrAppendS64(char dst[], int64_t dec, int minDigits) {
    uint64_t udec = dec;
    if (dec < 0) {
        *dst++ = '-';
        udec = ~udec + 1;
    }
    return EgStrAppendU64(dst, udec, minDigits);
}

/**
 * @brief 将 EgScalar 类型的数字转换为字符串
 * @param dst 字符串缓冲区
 * @param dec 要转换的数字
 * @return 转换后的字符串
 * @note 该函数不会检查缓冲区是否足够大
 * @note 该函数不会检查 dst 是否为 nullptr
 */
char* EgStrAppendScalar(char dst[], EgScalar dec) {
    char buffer[kEgStrAppendScalar_MaxSize];
    char* p = buffer + sizeof(buffer);
    do {
        *--p = EgToU8('0' + (int32_t)(dec % 10));
        dec /= 10;
    } while (dec != 0);

    EgAssert(p >= buffer);
    size_t cp_len = buffer + sizeof(buffer) - p;
    memcpy(dst, p, cp_len);
    dst += cp_len;

    return dst;
}

///////////////////////////////////////////////////////////////

const EgString::Rec EgString::kEmptyRec(0, 0);

#define SizeOfRec()  (kEmptyRec.data() - (char*)&kEmptyRec)

/**
 * @brief 将size_t类型的值转换为uint32_t类型的值
 * @param value 要转换的值
 * @return 转换后的值
 */
static uint32_t trim_size_t_to_u32(size_t value) {
    if (sizeof(size_t) > sizeof(uint32_t)) {
        if (value > UINT32_MAX) {
            return UINT32_MAX;
        }
    }
    return EgToU32(value);
}

/**
 * @brief 检查是否可以将 base 和 extra 相加
 * @param base 基数
 * @param extra 额外的数
 * @return 可以相加的数
 */
static size_t check_add32(size_t base, size_t extra) {
    EgAssert(base <= UINT32_MAX);
    if (sizeof(size_t) > sizeof(uint32_t)) {
        if (base + extra > UINT32_MAX) {
            extra = UINT32_MAX - base;
        }
    }
    return extra;
}

/**
 * @brief 创建一个 EgString::Rec 对象
 * @param str 字符串
 * @param len 字符串长度
 * @return 创建的 EgString::Rec 对象
 */
std::shared_ptr<EgString::Rec> EgString::Rec::Make(const char str[], size_t len) {
    if (len == 0) {
        return std::make_shared<Rec>(const_cast<Rec*>(&kEmptyRec));
    }
    EgSafeMath safe;
    uint32_t stringLen = safe.castTo<uint32_t>(len);
    size_t allocationSize = safe.add(len, SizeOfRec() + sizeof(char));
    allocationSize = safe.alignUp(allocationSize, 4);
    EgAssert(safe.ok());

    void* memory = ::operator new(allocationSize);
    std::shared_ptr<Rec> rec(new(memory) Rec(stringLen, 1));
    if (str) {
        memcpy(rec->data(), str, len);
    }
    rec->data()[len] = '\0';
    return rec;
}

void EgString::Rec::ref() const {
    if (this == &EgString::kEmptyRec) {
        return;
    }
    EgAssertRelease(mRefCnt.fetch_add(1, std::memory_order_relaxed));
}

void EgString::Rec::unref() const {
    if (this == &EgString::kEmptyRec) {
        return;
    }
    int32_t oldRefCnt = mRefCnt.fetch_sub(1, std::memory_order_acq_rel);
    EgAssert(oldRefCnt);
    if (oldRefCnt == 1) {
        this->~Rec();
        ::operator delete(const_cast<Rec*>(this));
    }
}

bool EgString::Rec::unique() const {
    return mRefCnt.load(std::memory_order_acquire) == 1;
}

///////////////////////////////////////////////////////////////

EgString::EgString() : mRec(const_cast<Rec*>(&kEmptyRec)) {}

EgString::EgString(size_t len) {
    mRec = Rec::Make(nullptr, len);
}

EgString::EgString(const char str[]) {
    size_t len = str ? strlen(str) : 0;
    mRec = Rec::Make(str, len);
}

EgString::EgString(const char str[], size_t len) {
    mRec = Rec::Make(str, len);
}