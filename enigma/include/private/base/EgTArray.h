#pragma once

#include "include/private/base/EgTo.h"
#include "include/private/base/EgSpan.h"
#include "include/private/base/EgDebug.h"
#include "include/private/base/EgAssert.h"
#include "include/private/base/EgTypeTraits.h"
#include "include/private/base/EgAlignedStorage.h"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstring>


/**
 * @brief 模板类 `EgTArray`，用于表示一个动态数组。
 */
template <typename T, bool MEM_MOVE = eg_is_trivially_relocatable_v<T>>
class EgTArray {

public:
    /**
     * @brief 数组的元素类型。
     */
    using value_type = T;

    EgTArray() : mOwnMemory(true), mCapacity{0} {}
    explicit EgTArray(int reserverdCount) : EgTArray() {
        this->reserve_back(reserverdCount);
    }

    EgTArray(const EgTArray& other) : EgTArray(other.mData, other.mSize) {}

    EgTArray(EgTArray&& other) noexcept {
        if (other.mOwnMemory) {
            this->setData(other);
            other.setData({});
        } else {
            this->initData(other.mSize);
            other.move(mData);
        }
        mSize = std::exchange(other.mSize, 0);
    }
        

    EgTArray(const T* array, int count) {
        this->initData(count);
        this->copy(array);
    }

    EgTArray(std::initializer_list<T> list) : EgTArray(list.begin(), list.size()) {}

    EgTArray& operator=(const EgTArray& other) {
        if (this != &other) {
            this->clear();
            this->checkRealloc(other.mSize, kExactFit);
            mSize = other.mSize;
            this->copy(other.mData);
        }
        return *this;
    }

    EgTArray& operator=(EgTArray&& other) noexcept {
        if (this != &other) {
            this->clear();
            if (other.mOwnMemory) {
                if (mOwnMemory) {
                    free(mData);
                }
                mData = std::exchange(other.mData, nullptr);
                mCapacity = other.mCapacity;
                other.mCapacity = 0;
                mOwnMemory = true;
            } else {
                this->checkRealloc(other.mSize, kExactFit);
                mSize = other.mSize;
                other.move(mData);
            }
            mSize = std::exchange(other.mSize, 0);
        }
        return *this;
    }

    ~EgTArray() {
        this->destroyAll();
        if (mOwnMemory) {
            free(mData);
        }
    }

    void reset(int n) {
        EgAssert(n >= 0);
        this->clear();
        this->checkRealloc(n, kExactFit);
        mSize = n;
        for (int i = 0; i < n; ++i) {
            new (mData + i) T();
        }
    }

    void reset(const T* array, int count) {
        EgAssert(array != nullptr);
        EgAssert(count >= 0);
        this->clear();
        this->checkRealloc(count, kExactFit);
        mSize = count;
        this->copy(array);
    }

    void reserve(int count) {
        EgAssert(count >= 0);
        if (count > this->size()) {
            this->checkRealloc(count - this->size(), kGrowing);
        }
    }

    void reserve_back(int count) {
        EgAssert(count >= 0);
        if (count > 0) {
            this->checkRealloc(count, kGrowing);
        }
    }

    /**
     * @brief 移除并打乱数组中的元素
     * 
     * 该函数用于移除数组中指定位置的元素，并将其后面的元素向前移动，以填补移除元素后的空缺。
     * 
     * @param n 要移除的元素的索引
     */
    void removeShuffle(int n) {
        // 断言：确保要移除的元素索引小于数组的当前大小
        EgAssert(n < this->size());
        // 计算移除元素后的新数组大小
        int newCount = this->size() - 1;
        // 更新数组的当前大小
        mSize = newCount;
        // 调用元素的析构函数，销毁要移除的元素
        mData[n].~T();
        // 如果要移除的元素不是最后一个元素，则将其后面的元素向前移动
        if (n != newCount) {
            // 调用移动函数，将元素从索引n移动到索引newCount
            this->move(n, newCount);
        }
    }

    bool isEmpty() const { return mSize == 0; }

    T& push_back() {
        void* newT = this->push_back_raw(1);
        return *new T(newT);
    }

    T& push_back(const T& t) {
        void* newT = this->push_back_raw(1);
        return *new (newT) T(t);
    }

    T& push_back(T&& t) {
        void* newT = this->push_back_raw(1);
        return *new (newT) T(std::move(t));
    }

    template<class... Args> T& emplace_back(Args&&... args) {
        void* newT = this->push_back_raw(1);
        return *new (newT) T(std::forward<Args>(args)...);
    }

    T* pusb_back_n(int n) {
        EgAssert(n >= 0);
        T* newTs = TCast(this->push_back_raw(n));
        for (int i = 0; i < n; ++i) {
            new (&newTs[i]) T;
        }
        return newTs;
    }

    T* pusb_back_n(int n, const T& t) {
        EgAssert(n >= 0);
        T* newTs = TCast(this->push_back_raw(n));
        for (int i = 0; i < n; ++i) {
            new (&newTs[i]) T(t);
        }
        return newTs;
    }

    T* push_back_n(int n, const T t[]) {
        EgAssert(n >= 0);
        this->checkRealloc(n, kGrowing);
        T* end = this->end();
        for (int i = 0; i < n; ++i) {
            new (end + i) T(t[i]);
        }
        mSize += n;
        return end;
    }

    T* move_back_n(int n, T* t) {
        EgAssert(n >= 0);
        this->checkRealloc(n, kGrowing);
        T* end = this->end();
        for (int i = 0; i < n; ++i) {
            new (end + i) T(std::move(t[i]));
        }
        mSize += n;
        return end;
    }

    void pop_back() {
        EgAssert(mSize > 0);
        --mSize;
        mData[mSize].~T();
    }

    void pop_back_n(int n) {
        EgAssert(n >= 0);
        EgAssert(size() >= n);
        int i = mSize;
        while (i-- > mSize - n) {
            mData[i].~T();
        }
        mSize -= n;
    }

    void resize_back(int newCount) {
        EgAssert(newCount >= 0);
        if (newCount > this->size()) {
            this->push_back_n(newCount - this->size());
        } else if (newCount < this->size()) {
            this->pop_back_n(this->size() - newCount);
        }
    }

    void swap(EgTArray& other) {
        using std::swap;
    
        if (this == &other) {
            return;
        }
        if (mOwnMemory && other.mOwnMemory) {
            swap(mData, other.mData);
            swap(mSize, other.mSize);
            auto allocCount = mCapacity;
            mCapacity = other.mCapacity;
            other.mCapacity = allocCount;
        } else {
            EgTArray copy(std::move(other));
            other = std::move(*this);
            *this = std::move(copy);
        }
    }

    T* begin() { return mData; }
    T* end() { return mData + mSize; }
    const T* begin() const { return mData; }
    const T* end() const { return mData + mSize; }
    T* data() { return mData; }
    const T* data() const { return mData; }
    int size() const { return mSize; }
    size_t size_bytes() const { return mSize * sizeof(T); }
    void resize(size_t count) { this->resize_back(EgToInt(count)); }
    void clear() {
        this->destroyAll();
        mSize = 0;
    }

    void shrink_to_fit() {
        if (!mOwnMemory || size() == mCapacity) {
            return;
        }
        if (size() == 0) {
            free(mData);
            mData = nullptr;
            mCapacity = 0;
        } else {
            EgSpan<std::byte> allocation = Allocate(size());
            this->move(TCast(allocation.data()));
            if (mOwnMemory) {
                free(mData);
            }
            this->setDataFromBytes(allocation);
        }
    }

    T& operator[](int index) {
        EgAssert(index >= 0);
        EgAssert(index < this->size());
        return mData[index];
    }

    const T& operator[](int index) const {
        EgAssert(index >= 0);
        EgAssert(index < this->size());
        return mData[index];
    }

    T& at(int index) {
        EgAssert(index >= 0);
        EgAssert(index < this->size());
        return mData[index];
    }

    const T& at(int index) const {
        EgAssert(index >= 0);
        EgAssert(index < this->size());
        return mData[index];
    }

    T& front() {
        EgAssert(mSize > 0);
        return mData[0];
    }

    const T& front() const {
        EgAssert(mSize > 0);
        return mData[0];
    }

    T& back() {
        EgAssert(mSize > 0);
        return mData[mSize - 1];
    }

    const T& back() const {
        EgAssert(mSize > 0);
        return mData[mSize - 1];
    }

    T& fromBack(int index) {
        EgAssert(index >= 0);
        EgAssert(index < this->size());
        return mData[mSize - 1 - index];
    }

    const T& fromBack(int index) const {
        EgAssert(index >= 0);
        EgAssert(index < this->size());
        return mData[mSize - 1 - index];
    }

    bool operator==(const EgTArray<T, MEM_MOVE>& right) const {
        int leftCount = this->size();
        if (leftCount != right.size()) {
            return false;
        }
        for (int index = 0; index < leftCount; ++index) {
            if (mData[index]!= right.mData[index]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const EgTArray<T, MEM_MOVE>& right) const {
        return !(*this == right);
    }

    int capacity() const { return mCapacity; }

protected:

    template <int InitialCapacity>
    EgTArray(EgAlignedStorage<InitialCapacity, T>* storage, int size = 0) {
        static_assert(InitialCapacity >= 0);
        EgAssert(size >= 0);
        EgAssert(storage->get() != nullptr);

        if (size > InitialCapacity) {
            this->initData(size);
        } else {
            this->setDataFromBytes(*storage);
            mSize = size;

            mOwnMemory = false;
        }
    }

    template <int InitialCapacity>
    EgTArray(const T* array, int size, EgAlignedStorage<InitialCapacity, T>* storage) : EgTArray {storage, size} {
        this->copy(array);
    }

private:
    static constexpr double kExactFit = 1.0;
    static constexpr double kGrowing = 1.5;

    static constexpr int kMinHeapAllocCount = 8;

    static constexpr int kMaxCapacity = EgToInt(std::min(SIZE_MAX / sizeof(T), (size_t)INT_MAX));


    void setDataFromBytes(EgSpan<std::byte> allocation) {
        T* data = TCast(allocation.data());
        size_t size = std::min(allocation.size() / sizeof(T), EgToSizeT(kMaxCapacity));
        setData(EgSpan<T>(data, size));
    }

    void setData(EgSpan<T> array) {
        mData = array.data();
        mCapacity = EgToInt(array.size());
        mOwnMemory = true;
    }

    static T* TCast(void* buffer) {
        return (T*)buffer;
    }

    size_t bytes(int n) const {
        EgAssert(n <= kMaxCapacity);
        return EgToSizeT(n) * sizeof(T);
    }

    static EgSpan<std::byte> Allocate(int capacity, double growthFactor = 1.0) {
        EgAssert(capacity >= 0);
        if (capacity == 0) {
            return {};
        }
        size_t bytes = std::min(EgToSizeT(capacity) * sizeof(T), SIZE_MAX);
        return {new std::byte[bytes], bytes};
    }

    void initData(int count) {
        this->setDataFromBytes(Allocate(count));
        mSize = count;
    }

     /**
     * @brief 销毁数组中的所有元素
     * 
     * 该函数遍历数组中的所有元素，并调用它们的析构函数来销毁它们。
     */
    void destroyAll() {
        // 如果数组不为空
        if (!this->isEmpty()) {
            // 获取数组的起始指针
            T* cursor = this->begin();
            // 获取数组的结束指针
            const T* const end = this->end();
            // 遍历数组中的所有元素
            do {
                // 调用当前元素的析构函数
                cursor->~T();
                // 移动指针到下一个元素
                ++cursor;
            // 直到指针到达数组的结束位置
            } while (cursor < end);
        }
    }

    void copy(const T* src) {
        if constexpr (std::is_trivially_copyable_v<T>) {
            if (!this->isEmpty() && src != nullptr) {
                std::memcpy(mData, src, this->size_bytes());
            }
        } else {
            for (int i = 0; i < mSize; ++i) {
                new (mData + i) T(src[i]);
            }
        }
    }

    void move(int dst, int src) {
        if constexpr (MEM_MOVE) {
            memcpy(static_cast<void*>(&mData[dst]),
                    static_cast<const void*>(&mData[src]),
                    sizeof(T)); 
        } else {
            new (&mData[dst]) T(std::move(mData[src]));
            mData[src].~T();
        }
    }

    void move(void* dst) {
        if constexpr (MEM_MOVE) {
            memcpy(dst, mData, this->bytes(mSize));
        } else {
            for (int i = 0; i < this->size(); ++i) {
                new (static_cast<char*>(dst + this->bytes(i))) T(std::move(mData[i]));
                mData[i].~T();
            }
        }
    }

    void* push_back_raw(int n) {
        this->checkRealloc(n, kGrowing);
        void* newT = mData + mSize;
        mSize += n;
        return newT;
    }

    void checkRealloc(int delta, double growthFactor) {
        EgAssert(delta >= 0);
        EgAssert(mCapacity >= 0);
        EgAssert(mSize >= 0);

        if (this->capacity() - mSize >= delta) {
            return;
        }

        if (delta > kMaxCapacity - size()) {
            throw std::bad_alloc();
        }

        const int newCount = mSize + delta;
        EgSpan<std::byte> allocation = Allocate(newCount, growthFactor);
        this->move(TCast(allocation.data()));
        if (mOwnMemory) {
            free(mData);
        }
        this->setDataFromBytes(allocation);
        EgAssert(this->capacity() >= newCount);
        EgAssert(mData != nullptr);
    }

    T *mData{nullptr};
    int mSize{0};
    uint32_t mOwnMemory : 1;
    uint32_t mCapacity : 31;
};


template <typename T,  bool MEM_MOVE>
static inline void swap(EgTArray<T, MEM_MOVE>& left, EgTArray<T, MEM_MOVE>& right) {
    left.swap(right);
}

template <int N, typename T, bool MEM_MOVE = eg_is_trivially_relocatable_v<T>>
class EgSTArray : private EgAlignedStorage<N, T> , public EgTArray<T, MEM_MOVE> {

private:
    static_assert(N >= 0);
    using STORAGE   = EgAlignedStorage<N, T>;
    using BASE      = EgTArray<T, MEM_MOVE>;
public:
    EgSTArray() : STORAGE{}, BASE {static_cast<STORAGE*>(this)} {}

    EgSTArray(const T* array, int count)
        : STORAGE{}, BASE {array, count, static_cast<STORAGE*>(this)} {}

    EgSTArray(std::initializer_list<T> data): EgSTArray(data.begin(), EgToInt(data.size())) {}

    explicit EgSTArray(int reserveCount): EgSTArray() {
        this->reserve_back(reserveCount);
    }

    EgSTArray(const EgSTArray& other) : EgSTArray() { *this = other; }
    explicit EgSTArray(const BASE& other) : EgSTArray() { *this = other; }

    EgSTArray(EgSTArray&& other) : EgSTArray() { *this = std::move(other); }
    explicit EgSTArray(BASE&& other) : EgSTArray() { *this = std::move(other); }

    EgSTArray& operator=(const EgSTArray& other) {
        BASE::operator=(other);
        return *this;
    }

    EgSTArray& operator=(BASE&& other) {
        BASE::operator=(std::move(other));
        return *this;
    }

    EgSTArray& operator=(EgSTArray&& other) {
        BASE::operator=(std::move(other));
        return *this;
    }

    EgSTArray& operator=(const BASE& other) {
        BASE::operator=(other);
        return *this;
    }

    using BASE::data;
    using BASE::size;
};