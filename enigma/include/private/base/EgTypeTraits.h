#pragma once

#include <memory>
#include <type_traits>

/**
 * is_trivially_copyable 是 C++ 中的一个类型萃取（type trait），它用于确定一个类型是否可以被平凡地复制（trivially copied）。
 * 一个类型被认为是平凡可复制的，如果满足以下条件：
 * 1. 平凡构造（Trivially Constructible）：
 *      类型具有平凡构造函数，即构造函数不执行任何操作，或者构造函数的操作可以被忽略（比如，构造函数体内只有默认构造函数的调用）
 * 2. 平凡析构（Trivially Destructible）：
 *      类型具有平凡析构函数，即析构函数不执行任何操作。
 * 3. 平凡赋值（Trivially Assignable）：
 *      类型具有平凡赋值操作，即赋值操作可以被忽略（比如，赋值操作体内只有成员的赋值）。
 * 4. 标准布局（Standard Layout）：
 *      类型是标准布局类型，意味着其布局在所有包含它的类的上下文中都是相同的，并且遵循特定的规则（比如，没有虚拟成员函数，没有数据成员的继承等）。
 * 5. 没有虚函数或虚拟基类（No Virtual Functions or Virtual Base Classes）：
 *      类型不包含虚函数或虚拟基类，这意味着它不涉及多态行为
 * 6. 没有非平凡的成员（No Non-Trivial Members）：
 *      类型的非静态成员也是平凡可复制的
 * 7. 没有引用成员（No Reference Members）：
 *      类型不能包含引用类型的成员。
 * 8. 没有数组成员，其长度不是常量表达式（No Array Members with Non-Constant Size）：
 *      类型不能包含长度不是编译时常量的数组类型的成员。
 * 
 * 如果一个类型满足以上所有条件，那么它就是平凡可复制的。对于这样的类型，可以使用 memcpy 或 memmove 来复制对象，而不需要调用构造函数、析构函数或赋值操作符。
 */

// 模板结构体 eg_has_trivially_relocatable_member
// 用于检查类型 T 是否具有 trivially relocatable 成员
// 默认情况下，假设类型 T 没有 trivially relocatable 成员
template<typename, typename = void>
struct eg_has_trivially_relocatable_member : std::false_type {};

// 模板结构体 eg_is_trivially_relocatable
// 用于检查类型 T 是否为 trivially relocatable
// 通过 std::disjunction 组合 std::is_trivially_copyable 和 eg_has_trivially_relocatable_member 的结果
// 即如果 T 是 trivially copyable 或者具有 trivially relocatable 成员，则认为 T 是 trivially relocatable
template<typename T>
struct eg_is_trivially_relocatable
        : std::disjunction<std::is_trivially_copyable<T>, eg_has_trivially_relocatable_member<T>> {};

// 模板结构体 eg_is_trivially_relocatable
// 用于检查类型 T 是否为 trivially relocatable
// 特化版本，当 T 是 std::unique_ptr 时，假设它是 trivially relocatable
template<typename T>
struct eg_is_trivially_relocatable<std::unique_ptr<T>> : std::true_type {};

// 模板结构体 eg_has_trivially_relocatable_member
// 特化版本，当类型 T 具有名为 eg_is_trivially_relocatable 的嵌套类型时，
// 使用该嵌套类型来判断 T 是否具有 trivially relocatable 成员
template<typename T>
struct eg_has_trivially_relocatable_member<T, std::void_t<typename T::eg_is_trivially_relocatable>>
        : T::eg_is_trivially_relocatable {};


// 模板变量 eg_is_trivially_relocatable_v
// 用于检查类型 T 是否为 trivially relocatable
// 通过 eg_is_trivially_relocatable<T>::value 获取结果
template <typename T>
inline constexpr bool eg_is_trivially_relocatable_v = eg_is_trivially_relocatable<T>::value;