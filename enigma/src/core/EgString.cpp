
#include "include/private/base/EgTo.h"
#include "include/private/base/EgAssert.h"

#include "include/core/EgString.h"

#include "src/base/EgSafeMath.h"
#include "src/base/EgUTF.h"
#include "src/base/EgUtils.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <string_view>
#include <new>
#include <utility>

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
    if (EgScalarIsNaN(dec)) {
        strcpy(dst, "NaN");
        return dst + 3;
    }

    if (!EgScalarIsFinite(dec)) {
        if (dec > 0) {
            strcpy(dst, "Inf");
            return dst + 3;
        } else {
            strcpy(dst, "-Inf");
            return dst + 4;
        }
    }

    static const char gFormat[] = "%.8g";
    char buffer[kEgStrAppendScalar_MaxSize + 1];
    int len = snprintf(buffer, sizeof(buffer), gFormat, dec);
    memcpy(dst, buffer, len);
    EgAssert(len <= kEgStrAppendScalar_MaxSize);
    return dst + len;
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
        return MakeEmpty();
    }
    EgSafeMath safe;
    uint32_t stringLen = safe.castTo<uint32_t>(len);
    size_t allocationSize = safe.add(len, SizeOfRec() + sizeof(char));
    allocationSize = safe.alignUp(allocationSize, 4);
    EgAssert(safe.ok());

    void* memory = ::operator new(allocationSize);
    std::shared_ptr<Rec> rec(new(memory) Rec(stringLen, 1), [memory](Rec* rec) {
        rec->unref();
        ::operator delete(memory);
    });
    if (str) {
        memcpy(rec->data(), str, len);
    }
    rec->data()[len] = '\0';
    return rec;
}

std::shared_ptr<EgString::Rec> EgString::Rec::MakeEmpty() {
    return std::make_shared<Rec>(0, 0);
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

EgString::EgString() : mRec(Rec::MakeEmpty()) {}

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

EgString::EgString(const EgString& other) : mRec(other.validate().mRec) {}

EgString::EgString(EgString&& other) noexcept : mRec(std::move(other.validate().mRec)) {
    other.mRec = Rec::MakeEmpty();
}

EgString::EgString(const std::string& str) {
    mRec = Rec::Make(str.c_str(), str.size());
}

EgString::EgString(std::string_view str) {
    mRec = Rec::Make(str.data(), str.size());
}

EgString::~EgString() {
    this->validate();
}

bool EgString::equals(const EgString& other) const {
    return mRec == other.mRec || this->equals(other.c_str(), other.size());
}

bool EgString::equals(const char str[]) const {
    return this->equals(str, str ? strlen(str) : 0);
}

bool EgString::equals(const char str[], size_t len) const {
    EgAssert(str != nullptr || len == 0);

    return mRec->mLength == len && !memcmp(mRec->data(), str, len);
}

EgString& EgString::operator=(EgString&& other) {
    this->validate();
    if (mRec != other.mRec) {
        this->swap(other);
    }
    return *this;
}

EgString& EgString::operator=(const EgString& other) {
    this->validate();
    mRec = other.mRec;
    return *this;
}

EgString& EgString::operator=(const char str[]) {
    this->validate();
    mRec = Rec::Make(str, str ? strlen(str) : 0);
    return *this;
}

void EgString::reset() {
    this->validate();
    mRec = Rec::MakeEmpty();
}

/**
 * @brief 获取字符串的数据
 */
char* EgString::data() {
    this->validate();
    // 如果字符串长度为 0，则返回空指针
    if (mRec->mLength) {
        // 如果字符串不唯一，则复制一份数据
        if (!mRec->unique()) {
            mRec = Rec::Make(mRec->data(), mRec->mLength);
        }
    }
    return mRec->data();
}

void EgString::resize(size_t len) {
    len = trim_size_t_to_u32(len);
    if (0 == len) {
        this->reset();
    } else if (mRec->unique() && ((len >> 2) <= (mRec->mLength >> 2))) {
        char* p = data();
        p[len] = '\0';
        mRec->mLength = len;
    } else {
        EgString tmp(len);
        char* dst = tmp.data();
        int copyLen = std::min<uint32_t>(len, size());
        memcpy(dst, c_str(), copyLen);
        dst[copyLen] = '\0';
        this->swap(tmp);
    }
}

void EgString::set(const char str[]) {
    this->set(str, str ? strlen(str) : 0);
}

void EgString::set(const char str[], size_t len) {
    len = trim_size_t_to_u32(len);
    if (len == 0) {
        this->reset();
    } else if (mRec->unique() && ((len >> 2) <= (mRec->mLength >> 2))) {
        char* p = data();
        if (str) {
            memcpy(p, str, len);
        }
        p[len] = '\0';
        mRec->mLength = len;
    } else {
        EgString tmp(str, len);
        this->swap(tmp);
    }
}

void EgString::insert(size_t offset, const char str[]) {
    this->insert(offset, str, str ? strlen(str) : 0);
}

/**
 * @brief 在字符串中插入字符串
 * @param offset 插入的位置
 * @param str 要插入的字符串
 * @param len 要插入的字符串长度
 * @note 如果 offset 大于字符串长度，则插入到字符串末尾
 * @note 如果 len 为 0，则不插入
 */
void EgString::insert(size_t offset, const char str[], size_t len) {
    if (len) {
        size_t length = size();
        if (offset > length) {
            offset = length;
        }

        len = check_add32(offset, len);
        if (0 == len) {
            return;
        }

        if (mRec->unique() && (length >> 2) == (length + len >> 2)) {
            char* dst = data();
            if (offset < length) {
                memmove(dst + offset + len, dst + offset, length - offset);
            }
            memcpy(dst + offset, str, len);
            dst[length + len] = '\0';
            mRec->mLength = EgToU32(length + len);
        } else {
            EgString tmp(length + len);
            char* dst = tmp.data();
            if (offset > 0) {
                memcpy(dst, mRec->data(), offset);
            }
            memcpy(dst + offset, str, len);
            if (offset < mRec->mLength) {
                memcpy(dst + offset + len,
                        mRec->data() + offset,
                        mRec->mLength - offset);
            }
            swap(tmp);
        }
    }
}

void EgString::insertUnichar(size_t offset, char unichar) {
    char buffer[EgUTF::kMaxBytesInUTF8Sequence];
    size_t len = EgUTF::ToUTF8(unichar, buffer);
    if (len) {
        this->insert(offset, buffer, len);
    }
}

void EgString::insertS32(size_t offset, int32_t value) {
    char buffer[kEgStrAppendS32_MaxSize];
    char* end = EgStrAppendS32(buffer, value);
    this->insert(offset, buffer, end - buffer);
}

void EgString::insertS64(size_t offset, int64_t value, int minDigits) {
    char buffer[kEgStrAppendS64_MaxSize];
    char* end = EgStrAppendS64(buffer, value, minDigits);
    this->insert(offset, buffer, end - buffer);
}

void EgString::insertU32(size_t offset, uint32_t value) {
    char buffer[kEgStrAppendU32_MaxSize];
    char* end = EgStrAppendU32(buffer, value);
    this->insert(offset, buffer, end - buffer);
}

void EgString::insertU64(size_t offset, uint64_t value, int minDigits) {
    char buffer[kEgStrAppendU64_MaxSize];
    char* end = EgStrAppendU64(buffer, value, minDigits);
    this->insert(offset, buffer, end - buffer);
}

void EgString::insertHex(size_t offset, uint32_t hex, int minDigits) {
    minDigits = std::max(0, std::min(8, minDigits));

    char buffer[8];
    char* p = buffer + sizeof(buffer);
    do {
        *--p = EgHexadecimalDigits::gUpper[hex & 0xF];
        hex >> 4;
        minDigits--;
    } while (hex != 0);

    while (--minDigits >= 0) {
        *--p = '0';
    }

    EgAssert(p >= buffer);
    this->insert(offset, p, buffer + sizeof(buffer) - p);
}


void EgString::insertScalar(size_t offset, EgScalar value) {
    char buffer[kEgStrAppendScalar_MaxSize];
    char* end = EgStrAppendScalar(buffer, value);
    this->insert(offset, buffer, end - buffer);
}

void EgString::remove(size_t offset, size_t length) {
    size_t size = this->size();
    if (offset >= size) {
        return;
    }

    if (length > size - offset) {
        length = size - offset;
    }

    EgAssert(length <= size);
    EgAssert(offset <= size - length);

    if (length > 0) {
        EgString tmp(size - length);
        char* dst = tmp.data();
        const char* src = this->c_str();

        if (offset > 0) {
            memcpy(dst, src, offset);
        }

        size_t tail = size - offset - length;
        if (tail > 0) {
            memcpy(dst + offset, src + offset + length, tail);
        }

        this->swap(tmp);
    }
}

void EgString::swap(EgString& other) {
    this->validate();
    other.validate();

    std::swap(mRec, other.mRec);
}

void EgString::printf(const char fmt[], ...) {
    va_list args;
    va_start(args, fmt);
    this->printfVAList(fmt, args);
    va_end(args);
}

void EgString::printfVAList(const char fmt[], va_list args) {
    char stackBuffer[kBufferSize];
    StringBuffer buffer = apply_format_string(fmt, args, stackBuffer, this);

    if (buffer.mData == stackBuffer) {
        this->set(buffer.mData, buffer.mLength);
    }
}