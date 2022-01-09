#ifndef _MUFFIN_CORE_COMMON_H_
#define _MUFFIN_CORE_COMMON_H_

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#if !defined(MUF_DEBUG)
#   if !defined(NDEBUG)
#       define MUF_DEBUG
#   endif
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

#define MUF_UNUSED(_variable) ((void) _variable)

#if defined(_countof)
#   define MUF_COUNTOF(_array) _countof(_array)
#else
#   define MUF_COUNTOF(_array) (sizeof(_array) / sizeof(_array[0]))
#endif

#if defined(MUF_COMPILER_GCC)
#   if defined(MUF_ARCHITECTURE_X86_64) || defined(MUF_ARCHITECTURE_X86)
#       define MUF_DEBUGBREAK() { __asm__ __volatile__("int $03"); }
#   endif
#elif defined(MUF_COMPILER_MSVC)
#   define MUF_DEBUGBREAK() __debugbreak()
#elif defined(MUF_COMPILER_CLANG)
#   define MUF_DEBUGBREAK() __builtin_trap()
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
#   define PRIusize PRIu64
#   define PRIisize PRIi64
#   define PRIdiff  PRIi64
#   define PRIindex PRIu64
#else
    typedef muf_u32     muf_usize;
    typedef muf_i32     muf_isize;
    typedef muf_i32     muf_diff;
#   define MUF_SIZEOF_USIZE 4
#   define MUF_SIZEOF_ISIZE 4
#   define PRIusize PRIu32
#   define PRIisize PRIi32
#   define PRIdiff  PRIi32
#   define PRIindex PRIu32
#endif

typedef muf_usize   muf_index;
typedef muf_diff    muf_offset;

typedef void*       muf_unknownptr;
typedef const void* muf_cunknownptr;
typedef void*       muf_rawptr;
typedef const void* muf_crawptr;

typedef muf_u32 muf_result;

void _mufAssertFormatPrint(const muf_char *filename, muf_index line,
    const muf_char *expression, const muf_char *format, ...);

#if defined(MUF_DEBUG)
#   define MUF_ASSERT(_expr) \
        do { if (!(_expr)) { _mufAssertFormatPrint(__FILE__, __LINE__, #_expr, "Assertion failed"); MUF_DEBUGBREAK(); } } while (0)
#   define MUF_FASSERT(_expr, _fmt, ...) \
        do { if (!(_expr)) { _mufAssertFormatPrint(__FILE__, __LINE__, #_expr, (_fmt), ##__VA_ARGS__); MUF_DEBUGBREAK(); } } while (0)
#   define MUF_CHECK(_expr) \
        do { if (!(_expr)) { _mufAssertFormatPrint(__FILE__, __LINE__, #_expr, "Check failed"); } } while (0)
#   define MUF_FCHECK(_expr, _fmt, ...) \
        do { if (!(_expr)) { _mufAssertFormatPrint(__FILE__, __LINE__, #_expr, (_fmt), ##__VA_ARGS__); } } while (0)
#   define MUF_UNREACHABLE() \
        do { _mufAssertFormatPrint(__FILE__, __LINE__, NULL, "Unreachable"); MUF_DEBUGBREAK(); } while (0)
#   define MUF_FUNREACHABLE(_fmt, ...) \
        do { _mufAssertFormatPrint(__FILE__, __LINE__, NULL, _fmt, ##__VA_ARGS__); MUF_DEBUGBREAK(); } while (0)
#else
#   define MUF_ASSERT(_expr)
#   define MUF_FASSERT(_expr, _fmt, ...)
#   define MUF_CHECK(_expr)
#   define MUF_FCHECK(_expr, _fmt, ...)
#   define MUF_UNREACHABLE()
#endif

#define MUF_STRUCT_TYPEDEF(_typeName) typedef struct _typeName##_s _typeName
#define MUF_UNION_TYPEDEF(_typeName) typedef union _typeName##_u _typeName
#define MUF_ENUM_TYPEDEF(_typeName) typedef enum _typeName##_e _typeName

#define MUF_ENUM_UNKNOWN(_enumName) _##_enumName##_UNKNOWN_ = 0U
#define MUF_ENUM_COUNT(_enumName) _##_enumName##_COUNT_

typedef enum MufHandleType_e {
    MUF_HANDLE_TYPE_PTR,
    MUF_HANDLE_TYPE_U32,
    MUF_HANDLE_TYPE_I32,
    MUF_HANDLE_TYPE_U64,
    MUF_HANDLE_TYPE_I64
} MufHandleType;

typedef struct MufHandle_s {
    muf_rawptr  _ptr;
    muf_u32     _u32;
    muf_i32     _i32;
    muf_u64     _u64;
    muf_i64     _i64;
} MufHandle;

#define MUF_HANDLE_DEF(_name) typedef union _name##_u { \
    muf_rawptr  _ptr; \
    muf_u32     _u32; \
    muf_i32     _i32; \
    muf_u64     _u64; \
    muf_i64     _i64; \
} _name;

#define mufMakeHandle(_handleType, _suffix, _value) ((_handleType) { ._##_suffix = _value })
#define mufNullHandle(_handleType) ((_handleType) { 0 })
#define mufIsNullHandle(_handle) (_handle._ptr == 0)
#define mufHandleCastPtr(_type, _handle) ((_type *)_handle._ptr)
#define mufHandleCastRawptr(_handle) (_handle._ptr)
#define mufHandleCastU32(_handle) (_handle._u32)
#define mufHandleCastI32(_handle) (_handle._i32)
#define mufHandleCastU64(_handle) (_handle._u64)
#define mufHandleCastI64(_handle) (_handle._i64)
#define mufHandleCastCommon(_handle) ((MufHandle *) (&_handle))

typedef struct MufPixels_s {
    muf_u32     width;
    muf_u32     height;
    muf_usize   pixelSize;
    muf_rawptr  pixels;
} MufPixels;

#define MUF_U8_MAX  (255)
#define MUF_U16_MAX (65535)
#define MUF_U32_MAX (0xFFFFFFFFU)
#define MUF_U64_MAX (0xFFFFFFFFFFFFFFFFULL)

#define MUF_I8_MAX  (127)
#define MUF_I16_MAX (32767)
#define MUF_I32_MAX (2147483647)
#define MUF_I64_MAX (9223372036854775807LL)

#define MUF_I8_MIN  (-128)
#define MUF_I16_MIN (-32768)
#define MUF_I32_MIN (-2147483647 - 1)
#define MUF_I64_MIN (-9223372036854775807LL - 1)

#define MufGeneratorFunc(_name, _retType) _retType(*_name)(void)
#define MufUnaryFunc(_name, _retType, _paramType) _retType(*_name)(_paramType) 
#define MufBinaryFunc(_name, _retType, _paramType) _retType(*_name)(_paramType, _paramType)
#define MufBinary2Func(_name, _retType, _paramType0, _paramType1) _retType(*_name)(_paramType0, _paramType1)

typedef MufBinaryFunc(MufComparator, muf_i32, muf_crawptr);
typedef MufBinaryFunc(MufEqualityComparator, muf_bool, muf_crawptr);
typedef MufUnaryFunc(MufInitializer, void, muf_rawptr);
typedef MufUnaryFunc(MufDestructor, void, muf_rawptr);

#define MUF_RAWPTR_AT(_ptr, _stride, _count) ((muf_rawptr)(((muf_byte *)(_ptr)) + (_stride) * (_count)))
#define MUF_RAWPTR_TYPE_AT(_ptr, _type, _count) (_type*)(MUF_RAWPTR_AT(_ptr, sizeof(_type), _count))

MUF_API muf_bool mufEqual_i8 (muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_i16(muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_i32(muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_i64(muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_u8 (muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_u16(muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_u32(muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_u64(muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_str(muf_crawptr a, muf_crawptr b);
MUF_API muf_bool mufEqual_ptr(muf_crawptr a, muf_crawptr b);

void mufPrintln(const muf_char *format, ...);

/* | (_major) 10 bits | (_minor) 10 bits | (_patch) 12 bits | */
typedef muf_u32 MufVersion;

#define mufCreateVersion(_major, _minor, _patch) (((_major) << 22) | ((_minor) << 12) | (_patch))
#define mufVersionGetMajor(_version) ((muf_u32)((_version) >> 22))
#define mufVersionGetMinor(_version) ((muf_u32)(((_version) >> 12)) & 0x3FF)
#define mufVersionGetPatch(_version) ((muf_u32)((_version) & 0xFFF))

MUF_API void mufVersionToString(MufVersion version, muf_char *strBuf);

#endif