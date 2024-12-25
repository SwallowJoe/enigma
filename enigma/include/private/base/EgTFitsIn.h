#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

/**
 * @brief 模板结构体 `eg_strip_enum`，用于去除枚举类型的包装。
 * 
 * 如果 `T` 是枚举类型，则 `eg_strip_enum<T>::type` 是 `T` 的底层类型。
 * 否则，`eg_strip_enum<T>::type` 就是 `T` 本身。
 */
template <typename T, class Enable = void>
struct eg_strip_enum {
    // 如果 `T` 不是枚举类型，则 `type` 就是 `T` 本身。
    typedef T type;
};

/**
 * @brief 模板结构体 `eg_strip_enum` 的特化版本，用于处理枚举类型。
 * 
 * 如果 `T` 是枚举类型，则 `eg_strip_enum<T>::type` 是 `T` 的底层类型。
 */
template <typename T>
struct eg_strip_enum<T, typename std::enable_if<std::is_enum<T>::value>::type> {
    // 如果 `T` 是枚举类型，则 `type` 是 `T` 的底层类型。
    typedef typename std::underlying_type<T>::type type;
};

/**
 * @brief 模板函数 `EgTFitsIn`，用于检查一个值是否可以安全地转换为另一个类型。
 * 
 * 该函数检查 `src` 是否可以安全地转换为类型 `D`。
 * 如果 `src` 是 `S` 类型的整数或枚举，并且 `D` 是整数或枚举，则该函数返回 `true`，否则返回 `false`。
 * 
 * @tparam D 目标类型。
 * @tparam S 源类型。
 * @param src 要检查的值。
 * @return 如果 `src` 可以安全地转换为 `D`，则返回 `true`，否则返回 `false`。
 */
template <typename D, typename S>
static constexpr inline
typename std::enable_if<(std::is_integral<S>::value || std::is_enum<S>::value) &&
                        (std::is_integral<D>::value || std::is_enum<D>::value), bool>::type
EgTFitsIn(S src) {
    // 使用 `eg_strip_enum` 去除 `S` 和 `D` 的枚举包装。
    using Sa = typename eg_strip_enum<S>::type;
    using Da = typename eg_strip_enum<D>::type;

    // 如果 `Sa` 是有符号的，`Da` 是无符号的，并且 `Sa` 的大小小于等于 `Da` 的大小，则检查 `src` 是否非负。
    return
    (std::is_signed<Sa>::value && std::is_unsigned<Da>::value && sizeof(Sa) <= sizeof(Da)) ?
        (S)0 <= src:

    // 如果 `Da` 是有符号的，并且 `Da` 的大小小于等于 `Sa` 的大小，则检查 `src` 是否小于等于 `Da` 的最大值。
    (std::is_signed<Da>::value && std::is_signed<Da>::value && sizeof(Da) <= sizeof(Sa))?
        src <= (S)std::numeric_limits<Da>::max():

    // 否则，检查 `src` 是否可以无损地转换为 `D`。
    (S)(D) src == src;
}
    