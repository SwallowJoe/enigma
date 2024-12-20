#pragma once

#if !defined(EG_IMPLEMENTATION)
    #define EG_IMPLEMENTATION 0
#endif

#if !defined(EG_API)
    #if defined(_MSC_VER)
        #if EG_IMPLEMENTATION
            #define EG_API __declspec(dllexport)
        #else
            #define EG_API __declspec(dllimport)
        #endif
    #else
        // 确保了即使在默认隐藏符号的情况下，标记为 EG_API 的符号仍然是对外可见
        #ifdef __GNUC__
            #define EG_API __attribute__((visibility("default")))
        #else
            #define EG_API
        #endif
    #endif
#else
    #define EG_API
#endif

// #undef EG_API
// #define EG_API 