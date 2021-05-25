#ifndef _MUFFIN_CORE_COMMON_H_
#define _MUFFIN_CORE_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#if defined(__GNUC__)
#   define MUF_COMPILER_GCC
#elif defined(__MSC_VER)
#   define MUF_COMPILER_MSVC
#elif defined(__BORLANDC__)
#   define MUF_COMPILER_BORLANDC
#elif defined(__clang__)
#   define MUF_COMPILER_CLANG
#else
#   define MUF_COMPILER_UNKNOWN
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#   define MUF_PLATFORM_WIN32
#elif defined(__linux__) || defined(linux) || defined(__linux)
#   define MUF_PLATFORM_LINUX
#elif defined(__ANDROID__)
#   define MUF_PLATFORM_ANDROID
#elif defined(__APPLE__) || defined(__MACH__)
#   define MUF_PLATFORM_MACOS
#endif

#if INTPTR_MAX == INT64_MAX
#   define MUF_ARCHITECTURE_X86_64
#elif INTPTR_MAX == INT32_MAX
#   define MUF_ARCHITECTURE_X86
#endif

#if defined(__cplusplus)
#   define MUF_EXTERN extern "C"
#else
#   define MUF_EXTERN extern
#endif

#if __STDC_VERSION__ >= 199901L
#   define MUF_INLINE inline
#else
#   define MUF_INLINE
#endif

#if !defined(NDEBUG)
#   define MUF_DEBUG
#endif

#if defined(MUF_COMPILER_GCC) || defined(MUF_COMPILER_CLANG)
#   if defined(MUF_PLATFORM_WIN32)
#       define MUF_IMPORT __declspec(dllimport)
#       define MUF_EXPORT __declspec(dllexport)
#       define MUF_HIDDEN static
#   elif defined(MUF_PLATFORM_LINUX)
#       define MUF_IMPORT __attribute__((visibility("default")))
#       define MUF_EXPORT __attribute__((visibility("default")))
#       define MUF_HIDDEN __attribute__((visibility("hidden")))
#   endif
#elif defined(MUF_COMPILER_MSVC)
#   define MUF_IMPORT __declspec(dllimport)
#   define MUF_EXPORT __declspec(dllexport)
#   define MUF_HIDDEN static
#else
#   define MUF_IMPORT
#   define MUF_EXPORT
#   define MUF_HIDDEN static
#endif

#if defined(MUF_LIB_EXPORT)
#   define MUF_API MUF_EXTERN MUF_EXPORT
#   define MUF_INTERNAL MUF_HIDDEN
#else
#   define MUF_API MUF_EXTERN
#   define MUF_INTERNAL static
#endif

typedef signed char         muf_i8;
typedef short               muf_i16;
typedef int                 muf_i32;
typedef long long           muf_i64;

typedef unsigned char       muf_u8;
typedef unsigned short      muf_u16;
typedef unsigned int        muf_u32;
typedef unsigned long long  muf_u64;

typedef float               muf_f32;
typedef double              muf_f64;

#if defined(__cplusplus)
    typedef bool muf_bool;
#   define MUF_TRUE     true
#   define MUF_FALSE    false
#else
    typedef muf_i32 muf_bool;
#   define MUF_TRUE     1
#   define MUF_FALSE    0
#endif

typedef unsigned char   muf_byte;
typedef unsigned char   muf_uchar;
typedef char            muf_char;
typedef wchar_t         muf_wchar;

#if defined(MUF_ARCHITECTURE_X86_64)
    typedef muf_u64     muf_usize;
    typedef muf_i64     muf_isize;
    typedef muf_i64     muf_diff;
#   define MUF_SIZEOF_USIZE 8
#   define MUF_SIZEOF_ISIZE 8
#else
    typedef muf_u32     muf_usize;
    typedef muf_i32     muf_isize;
    typedef muf_i32     muf_diff;
#   define MUF_SIZEOF_USIZE 4
#   define MUF_SIZEOF_ISIZE 4
#endif

typedef muf_usize   muf_index;
typedef muf_diff    muf_offset;

typedef void* muf_unknownptr;
typedef void* muf_rawptr;

/*typedef union MufHandle_u {
    muf_rawptr asRawptr;
    muf_u32 asU32;
    muf_i32 asI32;
    muf_u64 asU64;
    muf_i64 asI64;
} MufHandle;*/


typedef union MufHandle_u {
    muf_rawptr  _ptr;
    muf_u32     _u32;
    muf_i32     _i32;
    muf_u64     _u64;
    muf_i64     _i64;
} MufHandle;

#define _MUF_HANDLE_CREATE_DEF(Active, Type) \
    MUF_INLINE MufHandle mufCreateHandle_##Active(Type value) { return (MufHandle) { ._##Active = value }; }

    _MUF_HANDLE_CREATE_DEF(ptr, muf_rawptr);
    _MUF_HANDLE_CREATE_DEF(u32, muf_u32);
    _MUF_HANDLE_CREATE_DEF(i32, muf_i32);
    _MUF_HANDLE_CREATE_DEF(u64, muf_u64);
    _MUF_HANDLE_CREATE_DEF(i64, muf_i64);
#undef _MUF_HANDLE_CREATE_DEF

#define mufCreateHandle(Suffix, Value) mufCreateHandle_##Suffix(Value)

#define mufCastHandle(Suffix, Handle) Handle._##Suffix 

#define MUF_NULL_HANDLE ((MufHandle) { 0 })

typedef muf_i32(*MufComparator)(const muf_rawptr, const muf_rawptr);
typedef muf_bool(*MufEqualityComparator)(const muf_rawptr, const muf_rawptr);
typedef void(*MufDestructor)(muf_rawptr);

#define MUF_RAWPTR_AT(Ptr, Stride, Count) ((muf_rawptr)(((muf_byte *)(Ptr)) + (Stride) * (Count)))
#define MUF_RAWPTR_TYPE_AT(Ptr, Type, Count) (Type*)(MUF_RAWPTR_AT(Ptr, sizeof(Type), Count))

MUF_INTERNAL void _mufAssertFormatPrint(const char *filename, muf_index line,
    const char *expression, const char *format, ...);

#if defined(MUF_DEBUG)
#   define MUF_ASSERT(Expr) \
        do { if (!(Expr)) { _mufAssertFormatPrint(__FILE__, __LINE__, #Expr, "Assertion failed"); abort(); } } while (0)
#   define MUF_FASSERT(Expr, Fmt, ...) \
        do { if (!(Expr)) { _mufAssertFormatPrint(__FILE__, __LINE__, #Expr, (Fmt), ##__VA_ARGS__); abort(); } } while (0)
#   define MUF_CHECK(Expr) \
        do { if (!(Expr)) { _mufAssertFormatPrint(__FILE__, __LINE__, #Expr, "Check failed"); } } while (0)
#   define MUF_FCHECK(Expr, Fmt, ...) \
        do { if (!(Expr)) { _mufAssertFormatPrint(__FILE__, __LINE__, #Expr, (Fmt), ##__VA_ARGS__); } } while (0)
#   define MUF_UNREACHABLE() \
        do { _mufAssertFormatPrint(__FILE__, __LINE__, "", "Unreachable"); abort(); } while (0)
#else
#   define MUF_ASSERT(Expr)
#   define MUF_FASSERT(Expr, Fmt, ...)
#   define MUF_CHECK(Expr)
#   define MUF_FCHECK(Expr, Fmt, ...)
#   define MUF_UNREACHABLE()
#endif

MUF_API muf_bool mufEqual_i8(const muf_rawptr a, const muf_rawptr b);
MUF_API muf_bool mufEqual_i16(const muf_rawptr a, const muf_rawptr b);
MUF_API muf_bool mufEqual_i32(const muf_rawptr a, const muf_rawptr b);
MUF_API muf_bool mufEqual_i64(const muf_rawptr a, const muf_rawptr b);

MUF_API muf_bool mufEqual_u8(const muf_rawptr a, const muf_rawptr b);
MUF_API muf_bool mufEqual_u16(const muf_rawptr a, const muf_rawptr b);
MUF_API muf_bool mufEqual_u32(const muf_rawptr a, const muf_rawptr b);
MUF_API muf_bool mufEqual_u64(const muf_rawptr a, const muf_rawptr b);

MUF_API muf_bool mufEqual_str(const muf_rawptr a, const muf_rawptr b);

enum MufResultCode_Common_e {
    MUF_OK
} MufResultCode_Common;

#endif