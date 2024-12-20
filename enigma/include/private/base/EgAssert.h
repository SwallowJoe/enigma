#pragma once

#include <cstdio>

#ifndef EG_ABORT
    #define EG_DUMP_LINE_FORMAT "%s(%d)"
    #define EG_ABORT(message, ...) \
        do { \
            fprintf(stderr, "ASSERT: " message "\n", ##__VA_ARGS__); \
            fprintf(stderr, "ASSERT: " EG_DUMP_LINE_FORMAT "\n", __FILE__, __LINE__); \
            abort(); \
        } while (0)
#endif

#define EgAssertRelease(cond) \
    static_cast<void>( (cond) ? (void)0 : [] { EG_ABORT("assert(%s)", #cond); }() )

#if defined(EG_DEBUG)
    #define EgAssert(cond) EgAssertRelease(cond)
    #define EgAssertf(cond, fmt, ...) EgAssertRelease((cond) ? true : (EG_ABORT(fmt, ##__VA_ARGS__), false))
    #define EgDebugFail(message) EG_ABORT(message)
    #define EgDebugFailf(fmt, ...) EG_ABORT(fmt, ##__VA_ARGS__)
    #define EgAssertResult(cond) EgAssert(cond)
#else
    #define EgAssert(cond) static_cast<void>(0)
    #define EgAssertf(cond, fmt,...) static_cast<void>(0)
    #define EgDebugFail(message)
    #define EgDebugFailf(fmt,...)
    #define EgAssertResult(cond) if (cond) {} do {} while(false)
#endif