#include "include/utils/EgLog.h"

#include <time.h>
#include <sys/time.h>
#include <cstring>
#include <sstream>
#include <thread>
#include <iostream>
#include <stdarg.h>

/**
 * 可变参数函数，用于格式化输出调试信息。
 * 
 * 该函数使用了C标准库中的可变参数列表（va_list）来处理可变数量的参数。
 * 它接受一个格式化字符串 `fmt` 和一系列参数，并将格式化后的字符串输出到标准输出。
 * 
 * @param fmt 格式化字符串，类似于 printf 中的格式说明符。
 * @param ... 可变数量的参数，用于填充格式化字符串中的占位符。
 */
void EgDebugf(const char* fmt, ...) {
    // 定义一个 va_list 类型的变量 args，用于存储可变参数列表
    va_list args;
    // 初始化 args，使其指向 fmt 之后的第一个参数
    va_start(args, fmt);
    // 使用 vprintf 函数将格式化字符串和可变参数列表输出到标准输出
    vprintf(fmt, args);
    // 清理 args，释放其占用的资源
    va_end(args);
}
