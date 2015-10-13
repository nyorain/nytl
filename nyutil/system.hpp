#pragma once

namespace nyutil
{

/* possible macros: NYUTIL_OS_NAME NYUTIL_COMPILER_NAME
 * OS: NYUTIL_OS_64, NYUTIL_OS_WINDOWS, NYUTIL_OS_WINDOWS64, NYUTIL_OS_LINUX, NYUTIL_OS_ANDROID, NYUTIL_OS_MACOS, NYUTIL_OS_IOS, NYUTIL_OS_BSD, NYUTIL_OS_UNKNOWN
 *     NYUTIL_ANDROID_VERION
 * Compiler: NYUTIL_COMPILER_64, NYUTIL_COMPILER_INTEL, NYUTIL_COMPILER_MSC, NYUTIL_COMPILER_GNU, NYUTIL_COMPILER_CLANG, NYUTIL_COMPILER_BORLAND, NYUTIL_COMPILER_MINGW, NYUTIL_COMPILER_MINGW64
 *           NYUTIL_GNU_VERSION, NYUTIL_MSC_VERSION, NYUTIL_INTEL_VERSION, NYUTIL_CLANG_VERSION, NYUTIL_MINGW32_VERSION, NYUTIL_MINGW64_VERSION
 */

//http://sourceforge.net/p/predef/wiki
//OS//////////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(__WIN32__)
    #define NYUTIL_OS_WINDOWS 1
    #define NYUTIL_OS_NAME "Windows"

    #ifdef _WIN64
        #define NYUTIL_OS_64 1
        #define NYUTIL_OS_WINDOWS64 1
    #endif // _WIN64

#elif defined(__linux__) || defined(linux) || defined(__linux)
    #define NYUTIL_OS_LINUX 1

    #ifdef __ANDROID__
        #define NYUTIL_OS_ANDROID 1
        #define NYUTIL_OS_NAME "Android"
    #else
        #define NYUTIL_OS_NAME "Linux"
    #endif //__ANDROID__

#elif defined(__APPLE__) || defined(__MACH__) || defined(macintosh) || defined(Macintosh)
    #define NYUTIL_OS_MACOS 1

    #include "TargetConditionals.h"
    #ifdef TARGET_OS_IPHONE
        #define NYUTIL_OS_IOS 1
        #define NYUTIL_OS_NAME "ios"
    #else
        #define NYUTIL_OS_NAME "MacOS"
    #endif //IOS

#elif defined(__FreeBSD__) || defined(__OpenBSD__)
    #define NYUTIL_OS_BSD 1
    #define NYUTIL_OS_NAME "BSD"

#else
    #define NYUTIL_OS_UNKOWN 1
    #define NYUTIL_OS_NAME "Unknown"

    //#warning NYUTIL::SYSTEM: unknown or unsupported OS

#endif //OS



//Compiler/////////////////////////////////////////////////////////////////////////
#if defined(__BORLANDC__)
    #define NYUTIL_COMPILER_BORLAND 1
    #define NYUTIL_COMPILER_NAME "Borland"


#elif defined(__MINGW32__)
    #define NYUTIL_COMPILER_MINGW 1
    #define NYUTIL_COMPILER_NAME "MINGW"

    #define NYUTIL_MINGW32_VERSION (_MINGW32_MAJOR_VERSION * 100 + _MINGW32_MINOR_VERSION)

    #if defined(__MINGW64__)
        #define NYUTIL_COMPILER_64 1
        #define NYUTIL_COMPILER_MINGW64 1
        #define NYUTIL_MINGW64_VERSION (_MINGW64_MAJOR_VERSION * 100 + _MINGW64_MINOR_VERSION)
    #endif

#elif defined(__clang__)
    #define NYUTIL_COMPILER_CLANG 1
    #define NYUTIL_COMPILER_NAME "Clang"

    #define NYUTIL_CLANG_VERSION ( __clang_major__ * 100 + __clang_minor__)

#elif defined(__INTEL_COMPILER)
    #define NYUTIL_COMPILER_INTEL 1
    #define NYUTIL_COMPILER_NAME "Intel"

    #define NYUTIL_INTEL_VERSION __INTEL_COMPILER

#elif defined(_MSC_VER)
    #define NYUTIL_COMPILER_MSC 1
    #define NYUTIL_COMPILER_NAME "MSC"

    #define NYUTIL_MSC_VERSION _MSC_VER


#elif defined(__GNUC__)
    #define NYUTIL_COMPILER_GNU 1
    #define NYUTIL_COMPILER_NAME "GNU"

    #define NYUTIL_GNU_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

#else
    #define NYUTIL_COMPILER_UNKNOWN 1
    #define NYUTIL_COMPILER_NAME "Unknown"

    //#warning NYUTIL::SYSTEM: unknown or unsupported Compiler

#endif //Compiler



//utility functions
constexpr const char* getOSName()
{
    return NYUTIL_OS_NAME;
}

constexpr const char* getCompilerName()
{
    return NYUTIL_COMPILER_NAME;
}

constexpr bool is64Bit()
{
    #ifdef NYUTIL_OS_64
        return 1;
    #else
        return 0;
    #endif // NYUTIL_OS_64
}

}
