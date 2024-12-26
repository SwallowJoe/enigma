#pragma once

#include "include/private/base/EgAssert.h"
#include "include/core/EgScalar.h"
#include "include/private/base/EgTo.h"

#include <string>
#include <string_view>
#include <cstdint>
#include <cstddef>
#include <atomic>

/**
 * @brief 检查字符串是否以指定前缀开头
 */
static inline bool EgStrStartsWith(const char[] str, const char[] prefix) {
    EgAssert(str != nullptr);
    EgAssert(prefix != nullptr);
    // strncmp 比较字符串前缀，返回 0 表示相等
    return !strncmp(str, prefix, strlen(prefix));
}

/**
 * @brief 检查字符串是否以指定字符开头
 */
static inline bool EgStrStartsWith(const char str[], const char prefixChar) {
    EgAssert(str != nullptr);

    // strncmp 比较字符串前缀，返回 0 表示相等
    return prefixChar == *str;
}


bool EgStrEndsWith(const char[] str, const char[] suffixStr);
bool EgStrEndsWith(const char[] str, const char suffixChar);

int EgStrStartsWithOneOf(const char str[], const char[] prefixes);

static inline int EgStrFind(const char str[], const char[] substr) {
    // strstr 查找子串
    // 如果找到子字符串，返回指向 haystack 中第一次出现 needle 的指针。
    // 如果没有找到子字符串，返回 nullptr。
    const char* found = strstr(str, substr);
    // 返回子串在原字符串中的位置
    // 如果找不到则返回 -1
    // &str[0] 表示字符串的首地址
    // found - &str[0] 表示子串在原字符串中的位置
    return found ? EgToInt(found - &str[0]) : -1;
}


static inline int EgStrFindLastOf(const char str[], const char[] substr) {
    // strrchr 查找子串
    // 如果找到子字符串，返回指向 haystack 中最后一次出现 needle 的指针。
    // 如果没有找到子字符串，返回 nullptr。
    const char* found = strrchr(str, substr);
    // 返回子串在原字符串中的位置
    // 如果找不到则返回 -1
    // &str[0] 表示字符串的首地址
    // found - &str[0] 表示子串在原字符串中的位置
    return found ? EgToInt(found - &str[0]) : -1;
}

static inline bool EgStrContains(const char str[], const char[] substr) {
    return EgStrFind(str, substr) != -1;
}

static inline bool EgStrContains(const char str[], const char substrChar) {
    char temp[2] = {substrChar, '\0'};
    return EgStrContains(str, temp);
}

static constexpr int kEgStrAppendU32_MaxSize = 10;
char* EgStrAppendU32(char dst[], uint32_t dec);

static constexpr int kEgStrAppendU64_MaxSize = 20;
char* EgStrAppendU64(char dst[], uint64_t dec, int minDigits);

static constexpr int kEgStrAppendS32_MaxSize = kEgStrAppendU32_MaxSize + 1;
char* EgStrAppendS32(char dst[], int32_t dec);

static constexpr int kEgStrAppendS64_MaxSize = kEgStrAppendU64_MaxSize + 1;
char* EgStrAppendS64(char dst[], int64_t dec, int minDigits);


static constexpr int kEgStrAppendScalar_MaxSize = 15;
char* EgStrAppendScalar(char dst[], EgScalar dec);


class EG_API EgString {
public:
                EgString();
    explicit    EgString(size_t len);
    explicit    EgString(const char[] str);
                EgString(const char[] str, size_t len);
                EgString(const EgString& other);
                EgString(EgString&& other) noexcept;
    explicit    EgString(const std::string& str);
    explicit    EgString(std::string_view str);
                ~EgString();

    bool isEmpty() const;
    size_t size() const;
    const char* c_str() const;
    const char* data() const;
    
    char operator[](size_t index) const;

    bool equals(const EgString& other) const;
    bool equals(const char[] str) const;
    bool equals(const char[] str, size_t len) const;

    bool startsWith(const char[] prefix) const {
        return EgStrStartsWith(mRec->data(), prefix);
    }

    bool startsWith(const char prefixChar) const {
        return EgStrStartsWith(mRec->data(), prefixChar);
    }

    bool endsWith(const char[] suffix) const {
        return EgStrEndsWith(mRec->data(), suffix);
    }

    bool endsWith(const char suffixChar) const {
        return EgStrEndsWith(mRec->data(), suffixChar);
    }

    bool contains(const char[] substr) const {
        return EgStrContains(mRec->data(), substr);
    }

    bool contains(const char substrChar) const {
        return EgStrContains(mRec->data(), substrChar);
    }

    int find(const char[] substr) const {
        return EgStrFind(mRec->data(), substr);
    }

    int findLastOf(const char[] substr) const {
        return EgStrFindLastOf(mRec->data(), substr);
    }

    friend bool operator==(const EgString& lhs, const EgString& rhs) {
        return lhs.equals(rhs);
    }

    friend bool operator!=(const EgString& lhs, const EgString& rhs) {
        return !lhs.equals(rhs);
    }

    EgString& operator=(const EgString& other);
    EgString& operator=(EgString&& other) noexcept;
    EgString& operator=(const char[] str);

    char* data();
    char& operator[](size_t index) { return data()[index]; }

    void reset();
    void resize(size_t len);

    void set(const EgString& src) { *this = src; }
    void set(const char[] str);
    void set(const char[] str, size_t len);
    void set(std::string_view str) { this->set(str.data(), str.size()); }

    void insert(size_t offset, const char[] str);
    void insert(size_t offset, const char[] str, size_t len);
    void insert(size_t offset, const EgString& str) { this->insert(offset, str.c_str(), str.size()); }
    void insert(size_t offset, const std::string_view str) { this->insert(offset, str.data(), str.size()); }
    void insertUnichar(size_t offset, char unichar);
    void insertS32(size_t offset, int32_t value);
    void insertS64(size_t offset, int64_t value, int minDigits = 0);
    void insertU32(size_t offset, uint32_t value);
    void insertU64(size_t offset, uint64_t value, int minDigits = 0);
    void insertScalar(size_t offset, EgScalar value);
    void insertHex(size_t offset, uint32_t value, int minDigits = 0);

    void append(const char[] str) { this->insert((size_t)-1, str); }
    void append(const char[] str, size_t len) { this->insert((size_t)-1, str, len); }
    void append(const EgString& str) { this->insert((size_t)-1, str.c_str(), str.size()); }
    void append(const std::string_view str) { this->insert((size_t)-1, str.data(), str.size()); }
    void appendUnichar(char unichar) { this->insertUnichar((size_t)-1, unichar); }
    void appendS32(int32_t value) { this->insertS32((size_t)-1, value); }
    void appendS64(int64_t value, int minDigits = 0) { this->insertS64((size_t)-1, value, minDigits); }
    void appendU32(uint32_t value) { this->insertU32((size_t)-1, value); }
    void appendU64(uint64_t value, int minDigits = 0) { this->insertU64((size_t)-1, value, minDigits); }
    void appendScalar(EgScalar value) { this->insertScalar((size_t)-1, value); }
    void appendHex(uint32_t value, int minDigits = 0) { this->insertHex((size_t)-1, value, minDigits); }

    void prepend(const char[] str) { this->insert(0, str); }
    void prepend(const char[] str, size_t len) { this->insert(0, str, len); }
    void prepend(const EgString& str) { this->insert(0, str.c_str(), str.size()); }
    void prepend(const std::string_view str) { this->insert(0, str.data(), str.size()); }
    void prependUnichar(char unichar) { this->insertUnichar(0, unichar); }
    void prependS32(int32_t value) { this->insertS32(0, value); }
    void prependS64(int64_t value, int minDigits = 0) { this->insertS64(0, value, minDigits); }
    void prependU32(uint32_t value) { this->insertU32(0, value); }
    void prependU64(uint64_t value, int minDigits = 0) { this->insertU64(0, value, minDigits); }
    void prependScalar(EgScalar value) { this->insertScalar(0, value); }
    void prependHex(uint32_t value, int minDigits = 0) { this->insertHex(0, value, minDigits); }

    void printf(const char[] fmt, ...);
    

    void remove(size_t offset, size_t len);

    EgString& operator+=(const char[] str) { this->append(str); return *this; }
    EgString& operator+=(const EgString& str) { this->append(str); return *this; }
    EgString& operator+=(const std::string_view str) { this->append(str); return *this; }

    void swap(EgString& other);
private:
    struct Rec {
        public:
            constexpr Rec(uint32_t len, uint32_t refCnt): mLength(len), mRefCount(refCnt) {}

            static std::shared_ptr<Rec> Make(const char str[], size_t len);
            char* data() { return mBeginningOfData; }
            const char* data() const { return mBeginningOfData; }
            void ref() const;
            void unref() const;
            bool unique() const;

            uint32_t mLength;
        private:
            mutable std::atomic<uint32_t> mRefCount;
            char mBeginningOfData[1] = { '\0' };

            void operator delete(void* ptr) {
                ::operator delete(ptr);
            }
    };

    std::shared_ptr<Rec> mRec;
    const EgString& validate() const { return *this; }

    static const Rec kEmptyRec;
};