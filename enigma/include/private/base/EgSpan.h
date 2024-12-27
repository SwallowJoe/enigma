#pragma once

#include "include/private/base/EgAssert.h"
#include "include/private/base/EgTo.h"

#include <cstddef>
#include <limits>
#include <initializer_list>
#include <iterator>
#include <utility>

#include <type_traits>

template <typename T>
class EgSpan {

public:
    constexpr EgSpan(): mPtr{nullptr}, mSize{0} {}

    template <typename Integer, std::enable_if_t<std::is_integral_v<Integer>, bool> = true>
    constexpr EgSpan(T* ptr, Integer size): mPtr{ptr}, mSize{EgToSizeT(size)} {
        EgAssert(size >= 0 && size <= kMaxSize);
    }

    template <typename U, typename = std::enable_if_t<std::is_same_v<const U, T>>>
    constexpr EgSpan(const EgSpan<U>& other): mPtr{other.data()}, mSize{other.size()} {}

    constexpr EgSpan(const EgSpan& other) : mPtr{other.mPtr}, mSize{other.mSize} {}

    template<size_t N> constexpr EgSpan(T (&arr)[N]) : mPtr{arr}, mSize{N} {}

    /**
     * template<typename Container>：这是一个模板声明，意味着 EgSpan 是一个模板类，可以
     * 接受任何类型的容器作为参数。Container 是一个占位符，代表任何满足一定条件的容器类型，
     * 比如有 std::data 和 std::size 函数的容器。
     */
    template<typename Container>
    /**
     * Container& c：构造函数接受一个名为 c 的容器的引用。使用引用是为了避免不必要的复制，并且允许构造函数接受容器的常量引用（通过在类模板声明中添加 const）。
     * 
     * EgSpan(std::data(c), std::size(c)) {}：这是一个成员初始化列表，它调用了 EgSpan 类的另一个构造函数（委托构造函数）。
     *                                       这个列表使用 std::data(c) 获取容器 c 的数据指针，和 std::size(c) 获取容器 c 的大小（元素数量），
     *                                       并将这些值传递给另一个构造函数
     */
    constexpr EgSpan(Container& c) : EgSpan(std::data(c), std::size(c)) {}

    EgSpan(std::initializer_list<T> list) : EgSpan(std::data(list), std::size(list)) {}

    constexpr EgSpan& operator=(const EgSpan& other) {
        mPtr = other.mPtr;
        mSize = other.mSize;
        return *this;
    }

    constexpr T& operator[](size_t index) const {
        EgAssert(index < mSize);
        return mPtr[index];
    }

    constexpr T& front() const { return mPtr[0]; }
    constexpr T& back() const { return mPtr[mSize - 1]; }
    constexpr T* begin() const { return mPtr; }
    constexpr T* end() const { return mPtr + mSize; }
    constexpr auto rbegin() const { return std::reverse_iterator<T*>(end()); }
    constexpr auto rend() const { return std::reverse_iterator<T*>(begin()); }
    constexpr T* data() const { return this->begin(); }
    constexpr bool empty() const { return mSize == 0; }
    constexpr size_t size() const { return mSize; }
    constexpr size_t size_bytes() const { return mSize * sizeof(T); }
    constexpr EgSpan<T> first(size_t prefixLen) const {
        EgAssert(prefixLen <= mSize);
        return EgSpan{mPtr, prefixLen};
    }
    constexpr EgSpan<T> last(size_t postfixLen) const {
        EgAssert(postfixLen <= mSize);
        return EgSpan{mPtr + mSize - postfixLen, postfixLen};
    }

    constexpr EgSpan<T> subspan(size_t offset) const {
        return this->subspan(offset, this->size() - offset);
    }

    constexpr EgSpan<T> subspan(size_t offset, size_t length) const {
        EgAssert(offset <= mSize);
        EgAssert(length <= mSize - offset);
        return EgSpan{mPtr + offset, length};
    }

private:
    static const constexpr size_t kMaxSize = std::numeric_limits<size_t>::max() / sizeof(T);
    T* mPtr;
    std::size_t mSize;
};

/**
 * @brief 推导指引：从容器类型推导出 EgSpan 的元素类型
 * 
 * 当使用容器类型实例化 EgSpan 时，这个推导指引会根据容器的 data() 函数返回的指针类型，
 * 推导出 EgSpan 的元素类型。
 * 
 * @tparam Container 容器类型
 */
template <typename Container>
/**
 * @brief 推导指引：从容器类型推导出 EgSpan 的元素类型
 * 
 * 当使用容器类型实例化 EgSpan 时，这个推导指引会根据容器的 data() 函数返回的指针类型，
 * 推导出 EgSpan 的元素类型。
 * 
 * @param c 容器引用
 * @return EgSpan<std::remove_pointer_t<decltype(std::data(std::declval<Container&>()))>> 
 *         推导得到的 EgSpan 类型
 */
EgSpan(Container&) -> EgSpan<std::remove_pointer_t<decltype(std::data(std::declval<Container&>()))>>;

/**
 * @brief 推导指引：从初始化列表类型推导出 EgSpan 的元素类型
 * 
 * 当使用初始化列表类型实例化 EgSpan 时，这个推导指引会根据初始化列表的 data() 函数返回的指针类型，
 * 推导出 EgSpan 的元素类型。
 * 
 * @tparam T 初始化列表中的元素类型
 */
template <typename T>
/**
 * @brief 推导指引：从初始化列表类型推导出 EgSpan 的元素类型
 * 
 * 当使用初始化列表类型实例化 EgSpan 时，这个推导指引会根据初始化列表的 data() 函数返回的指针类型，
 * 推导出 EgSpan 的元素类型。
 * 
 * @param list 初始化列表
 * @return EgSpan<std::remove_pointer_t<decltype(std::data(std::declval<std::initializer_list<T>>()))>> 
 *         推导得到的 EgSpan 类型
 */
EgSpan(std::initializer_list<T>) -> EgSpan<std::remove_pointer_t<decltype(std::data(std::declval<std::initializer_list<T>>()))>>;