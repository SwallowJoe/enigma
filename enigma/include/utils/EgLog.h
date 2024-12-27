#pragma once

#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <mutex>

#define EG_DEBUG 1

#if !defined(EgDebugf)
    /**
     * 可变参数函数，用于格式化输出调试信息。
     * 
     * 该函数使用了C标准库中的可变参数列表（va_list）来处理可变数量的参数。
     * 它接受一个格式化字符串 `fmt` 和一系列参数，并将格式化后的字符串输出到标准输出。
     * 
     * @param fmt 格式化字符串，类似于 printf 中的格式说明符。
     * @param ... 可变数量的参数，用于填充格式化字符串中的占位符。
     */
    void EgDebugf(const char* fmt, ...);
#endif

#if defined(EG_DEBUG) && EG_DEBUG == 1
    #define EgDEBUGCODE(...) __VA_ARGS__
    #define EgDEBUGF(...)    EgDebugf(__VA_ARGS__)
#else
    #define EgDEBUGCODE(...)
    #define EgDEBUGF(...)
#endif


// 获取当前时间并格式化为字符串
static char* log_time(void) {

    // 线程局部存储的时间字符串缓冲区
    thread_local char szTime[26];

    // 定义一个 timeval 结构体，用于存储时间戳
    struct timeval tv {};
    // 定义一个 timezone 结构体，用于存储时区信息
    struct timezone tz {};
    // 获取当前时间戳和时区信息
    gettimeofday(&tv, &tz);

    // 将时间戳转换为本地时间
    struct tm* ptm = localtime(&tv.tv_sec);
    // 将本地时间格式化为字符串，格式为 "YYYY-MM-DD HH:MM:SS"
    strftime(szTime, sizeof(szTime) - 5, "%Y-%m-%d %H:%M:%S", ptm);
    // 计算毫秒数
    int millis = tv.tv_usec / 1000;

    // 将毫秒数格式化为字符串，格式为 ".SSS"
    snprintf(szTime + strlen(szTime), 5, ".%03d", millis);
    // 返回时间字符串
    return szTime;
}

static std::string getCurrentThreadIdHex() {
    std::stringstream ss;
    ss << std::hex << std::this_thread::get_id();
    return ss.str();
}

static int getCurrentTid() {
    static std::unordered_map<std::thread::id, int> threadIdMap;
    static std::mutex mutex;
    auto id = std::this_thread::get_id();

    std::lock_guard<std::mutex> lock(mutex);
    if (threadIdMap.find(id) == threadIdMap.end()) {
        threadIdMap[id] = threadIdMap.size();
    }
    return threadIdMap[id];
}

static std::string getCurrentThreadName() {
    char name[16]; // 线程名称的最大长度为16
    pthread_getname_np(pthread_self(), name, sizeof(name));
    return std::string(name);
}

static void __print(const std::stringstream& sstream) {
    std::cout << sstream.str() << std::endl << std::flush;
}

template<typename T>
static void __print(const std::stringstream& sstream, const T& value) {
    sstream << value << std::endl;
    std::cout << sstream.str() << std::flush;
}

template<typename T, typename... Args>
static void __print(const std::stringstream& sstream, const T& value, const Args&... args) {
    sstream << value;
    __print(sstream, args...);
}

template<typename T, typename... Args>
static void __println(const std::string& type, const T& value, const Args&... args) {
    std::stringstream sstream;

#ifndef LOG_TAG
#define LOG_TAG "NO_TAG"
#endif

    std::string tagStr(LOG_TAG);
    tagStr.substr(0, 10);
    tagStr.resize(10, ' ');

    sstream << log_time() << " " << getCurrentThreadName() << " " << type << "/" << tagStr << ": " << value;
    __print(sstream, args...);
}


/**
 * 打印日志信息
 * 示例用法：
 *    LOGD("Hello, world!");
 *    LOGD("Value: " << 42);
 *    LOGD("Value:" << "Hello");
 */
#if defined(EG_DEBUG) && EG_DEBUG == 1
    #define LOGV(...) {\
        std::ostringstream ostr;\
        ostr << __VA_ARGS__;\
        __println("V", ostr.str());\
    }

    #define LOGD(...) {\
        std::ostringstream ostr;\
        ostr << __VA_ARGS__;\
        __println("D", ostr.str());\
    }
    #define LOGI(...) {\
        std::ostringstream ostr;\
        ostr << __VA_ARGS__;\
        __println("I", ostr.str());\
    }
    #define LOGW(...) {\
        std::ostringstream ostr;\
        ostr << __VA_ARGS__;\
        __println("W", ostr.str());\
    }
    #define LOGE(...) {\
        std::ostringstream ostr;\
        ostr << __VA_ARGS__;\
        __println("E", ostr.str());\
    }
#else
    #define LOGV(...) {}
    #define LOGD(...) {}
    #define LOGI(...) {}
    #define LOGW(...) {}
    #define LOGE(...) {}
#endif