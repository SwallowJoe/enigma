#pragma once

#include <cstddef>
#include <iterator>

/**
 * @brief 模板类 `EgAlignedStorage`，用于在内存中存储对齐的对象。
 *        N 表示要存储的对象数量。
 *        T 表示要存储的对象类型。
 */
template <int N, typename T>
class EgAlignedStorage {
public:
    /**
     * @brief 默认构造函数，初始化存储对象
     */
    EgAlignedStorage() {}

    /**
     * @brief 移动构造函数，禁止使用
     */
    EgAlignedStorage(EgAlignedStorage&&) = delete;

    /**
     * @brief 拷贝构造函数，禁止使用
     */
    EgAlignedStorage(const EgAlignedStorage&) = delete;

    /**
     * @brief 移动赋值运算符，禁止使用
     */
    EgAlignedStorage& operator=(EgAlignedStorage&&) = delete;

    /**
     * @brief 拷贝赋值运算符，禁止使用
     */
    EgAlignedStorage& operator=(const EgAlignedStorage&) = delete;

    /**
     * @brief 获取存储对象的指针
     * @return 指向存储对象的指针
     */
    void* get() { return mStorage; }

    /**
     * @brief 获取存储对象的常量指针
     * @return 指向存储对象的常量指针
     */
    const void* get() const { return mStorage; }

    /**
     * @brief 获取存储对象的字节数组指针
     * @return 指向存储对象的字节数组指针
     */
    std::byte* data() { return mStorage; }

    /**
     * @brief 获取存储对象的常量字节数组指针
     * @return 指向存储对象的常量字节数组指针
     */
    const std::byte* data() const { return mStorage; }

    /**
     * @brief 获取存储对象的大小
     * @return 存储对象的大小
     */
    size_t size() const { return std::size(mStorage); }

private:
    /**
     * @brief 存储对象的字节数组，使用 alignas(T) 进行对齐
     */
    alignas(T) std::byte mStorage[sizeof(T) * N];
};