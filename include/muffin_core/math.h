#ifndef _MUFFIN_CORE_MATH_H_
#define _MUFFIN_CORE_MATH_H_

#include <math.h>
#include <stdint.h>

#include "common.h"

/// The value of pi
#define MUF_PI      3.141592653589793116

/// The value of pi/2
#define MUF_PI_HALF 1.570796326794896558

/// The value of pi/4
#define MUF_PI_QUAD 0.785398163397448279

/// The value of e
#define MUF_EXP     2.718281828459045091

/// The value of epsilon
#define MUF_EPSILON (1.0e-05)

/**
 * @brief Get the larger of two floating-point numbers
 * @param value1 First floating-pointer number
 * @param value2 Second floating-pointer number
 * @return The larger one
 */
static MUF_INLINE muf_f32 mufMaxf(muf_f32 value1, muf_f32 value2) {
    return fmaxf(value1, value2);
}

/**
 * @brief Get the smaller of two floating-point numbers
 * @param value1 First floating-pointer number
 * @param value2 Second floating-pointer number
 * @return The smaller one
 */
static MUF_INLINE muf_f64 mufMaxlf(muf_f64 value1, muf_f64 value2) {
    return fmax(value1, value2);
}

static MUF_INLINE muf_f32 mufMinf(muf_f32 value1, muf_f32 value2) {
	return fminf(value1, value2);
}

static MUF_INLINE muf_f64 mufMinlf(muf_f64 value1, muf_f64 value2) {
	return fmin(value1, value2);
}

/**
 * Get the larger of two numbers
 * @note The complex expression of Value1 or Value2 may
 *       lead to redundant calculations and unexpected result
 * @param Value1 First number
 * @param Value2 Second number
 * @return The larger one
 */
#define mufMax(Value1, Value2) ((Value1) > (Value2) ? Value1 : Value2)

/**
 * Get the smaller of two numbers
 * @note The complex expression of Value1 or Value2 may
 *       lead to redundant calculations and unexpected result
 * @param Value1 First number
 * @param Value2 Second number
 * @return The smaller one
 */
#define mufMin(Value1, Value2) ((Value1) < (Value2) ? Value1 : Value2)

/**
 * Clamp the value within the range of [min, max]
 * @param Value The number
 * @param MinV The lower limit of range
 * @param MaxV the upper limit of range
 * @return The result value in [min, max]
 */
#define mufClamp(Value, MinV, MaxV) (mufMax(mufMin(Value, MaxV), MinV))

static MUF_INLINE muf_f32 mufClampf(muf_f32 value, muf_f32 minv, muf_f32 maxv) {
	return mufMaxf(mufMinf(value, maxv), minv);
}

static MUF_INLINE muf_f64 mufClamplf(muf_f64 value, muf_f64 minv, muf_f64 maxv) {
	return mufMaxlf(mufMinlf(value, maxv), minv);
}

/**
 * Clamp(Value, 0, 1)
 * @param Value the number
 * @return The clamp result
 */
#define mufSaturate(Value) mufClamp(Value, 0, 1)

/**
 * Get the sign of number. The number must be signed
 * @param Value The signed number
 * @return -1 if the value less than 0, 0 if the value equals to 0 and 1 if the value larger than 0
 */
#define mufSign(Value) (((Value) > 0) - ((Value) < 0))

/**
 * Get the absolute value
 * @param Value The number
 * @return The absolute value
 */
#define mufAbs(Value) ((Value) < 0 ? -(Value) : (Value))

#define mufAbsf(Value) ((muf_f32) fabs(Value))

#define mufAbslf(Value) fabs(Value)

#define mufSwapInt(A, B) (((A) ^ (B)) && ((B) ^= (A) ^= (B), (A) ^= (B)))

#define mufSwap(Type, A, B) do { Type tmp = A; A = B; B = tmp; } while (0)

MUF_API muf_bool mufFloatEqualEpsilon(muf_f64 a, muf_f64 b, muf_f64 epsilon);

MUF_API muf_u32 mufBitsFindFirstSet(muf_u32 bits);
MUF_API muf_bool mufBitsTest(muf_u32 bits, muf_u32 index);
MUF_API void mufBitsSet(muf_u32 *bits, muf_u32 index);
MUF_API void mufBitsReset(muf_u32 *bits, muf_u32 index);
MUF_API muf_bool mufBitsFlip(muf_u32 *bits, muf_u32 index);

/**
 * @brief Convert degrees to radians
 * @param[in] degrees The angle expressed in degrees
 * @return The angle expressed in radians
 */
MUF_API muf_f32 mufToRadians(muf_f32 degrees);

/**
 * @brief Convert radians to degrees
 * @param[in] radians The angle expressed in radians
 * @return The angle expressed in degrees
 */
MUF_API muf_f32 mufToDegrees(muf_f32 radians);

/**
 * @brief Get the closest power of two
 * @param[in] value A unsigned integer
 * @return The closest power of two
 */
MUF_API muf_u64 mufCeilPower2(muf_u64 value);

typedef struct MufMT19937_s {
	muf_u32 seed;
	muf_u32 mt[624];
	muf_u32 index;
} MufMT19937;

/**
 * @brief Create a 32-bit random integer generator using MT19337
 * @param[in] seed The random seed
 * @return The mt19937 generator
 */
MUF_API MufMT19937 *mufCreateMT19937(muf_u32 seed);

/**
 * @brief Destroy the random integer generator
 * @param generator The mt19937 generator
 */
MUF_API void mufDestroyMT19937(MufMT19937 *generator);

/**
 * @brief Reset a random seed for the random integer generator. If the given 
 *        seed is same as before, it will reset the states of generator
 * @param generator The mt19937 generator
 * @param seed The new random seed
 */
MUF_API void mufMT19937SetSeed(MufMT19937 *generator, muf_u32 seed);

/**
 * @brief Get the next random 32-bit integer
 * @param generator The mt19937 generator
 * @return A random 32-bit integer
 */
MUF_API muf_u32 mufMT19937Next(MufMT19937 *generator);

MUF_API muf_i32 mufRandomRange(MufMT19937 *generator, muf_i32 minValue, muf_i32 maxValue);

MUF_API muf_f32 mufRandomRangef(MufMT19937 *generator, muf_f32 minValue, muf_f32 maxValue);

MUF_API void mufShuffle(MufMT19937 *generator, const muf_rawptr data, muf_usize elementSize, muf_usize count);

MUF_API void mufSetRandomSeed(muf_i32 seed);
MUF_API muf_i32 mufGetRandomSeed(void);
MUF_API muf_i32 mufRandom(muf_i32 minValue, muf_i32 maxValue);

typedef struct MufVec2_s {
    muf_f32 x, y;
} MufVec2;

typedef struct MufVec3_s {
    muf_f32 x, y, z;
} MufVec3;

typedef struct MufVec4_s {
    muf_f32 x, y, z, w;
} MufVec4;

/**
 * @brief Create a 2-dimensional vector using two floating-pointer numbers
 * @param x The x component of vector
 * @param y The y component of vector
 * @return The 2-dimensional vector
 */
MUF_API MufVec2 mufCreateVec2(muf_f32 x, muf_f32 y);

/**
 * @brief Create a 3-dimensional vector using three floating-pointer numbers
 * @param x The x component of vector
 * @param y The y component of vector
 * @param z The z component of vector
 * @return The 3-dimensional vector
 */
MUF_API MufVec3 mufCreateVec3(muf_f32 x, muf_f32 y, muf_f32 z);

/**
 * @brief Create a 4-dimensional vector using four floating-pointer numbers
 * @param x The x component of vector
 * @param y The y component of vector
 * @param z The z component of vector
 * @param w The w component of vector
 * @return The 4-dimensional vector
 */
MUF_API MufVec4 mufCreateVec4(muf_f32 x, muf_f32 y, muf_f32 z, muf_f32 w);

/**
 * @brief Load values for a 2-dimensional vector from memory
 * @note  If the given values doesn't have enough numbers, the result is undefined
 * @param[in] v The target vector
 * @param[in] values The values
 */
MUF_API void mufVec2Load(MufVec2 *v, const muf_f32 *values);

/**
 * @brief Load values for a 3-dimensional vector from memory. If the given
 *        values doesn't have enough numbers, the result is undefined
 * @param v The target vector
 * @param values The values
 */
MUF_API void mufVec3Load(MufVec3 *v, const muf_f32 *values);

/**
 * @brief Load values for a 4-dimensional vector from memory. If the given
 *        values doesn't have enough numbers, the result is undefined
 * @param[in] v The target vector
 * @param[in] values The values
 */
MUF_API void mufVec4Load(MufVec4 *v, const muf_f32 *values);

MUF_API muf_bool mufVec2Equal(muf_crawptr v1, muf_crawptr v2);
MUF_API muf_bool mufVec3Equal(muf_crawptr v1, muf_crawptr v2);
MUF_API muf_bool mufVec4Equal(muf_crawptr v1, muf_crawptr v2);

MUF_API muf_index mufVec2Hash(muf_crawptr v);
MUF_API muf_index mufVec3Hash(muf_crawptr v);
MUF_API muf_index mufVec4Hash(muf_crawptr v);

MUF_API void mufVec2ToArray(MufVec2 vec, muf_f32 *array);
MUF_API void mufVec3ToArray(MufVec3 vec, muf_f32 *array);
MUF_API void mufVec4ToArray(MufVec4 vec, muf_f32 *array);

MUF_API MufVec2 mufVec2Negate(MufVec2 v);
MUF_API MufVec3 mufVec3Negate(MufVec3 v);
MUF_API MufVec4 mufVec4Negate(MufVec4 v);

/**
 * @brief Add two vectors
 * @param[in] v1 The first vector
 * @param[in] v2 The second vector
 * @return The result vector (v1 + v2)
 */
MUF_API MufVec2 mufVec2Add(MufVec2 v1, MufVec2 v2);

/**
 * @brief Add two vectors
 * @param[in] v1 The first vector
 * @param[in] v2 The second vector
 * @return The result vector (v1 + v2)
 */
MUF_API MufVec3 mufVec3Add(MufVec3 v1, MufVec3 v2);

/**
 * @brief Add two vectors
 * @param[in] v1 The first vector
 * @param[in] v2 The second vector
 * @return The result vector (v1 + v2)
 */
MUF_API MufVec4 mufVec4Add(MufVec4 v1, MufVec4 v2);

MUF_API MufVec2 mufVec2Sub(MufVec2 v1, MufVec2 v2);
MUF_API MufVec3 mufVec3Sub(MufVec3 v1, MufVec3 v2);
MUF_API MufVec4 mufVec4Sub(MufVec4 v1, MufVec4 v2);

MUF_API MufVec2 mufVec2Mul(MufVec2 v, muf_f32 s);
MUF_API MufVec3 mufVec3Mul(MufVec3 v, muf_f32 s);
MUF_API MufVec4 mufVec4Mul(MufVec4 v, muf_f32 s);

MUF_API MufVec2 mufVec2Div(MufVec2 v, muf_f32 s);
MUF_API MufVec3 mufVec3Div(MufVec3 v, muf_f32 s);
MUF_API MufVec4 mufVec4Div(MufVec4 v, muf_f32 s);

MUF_API MufVec2 mufVec2Scale(MufVec2 v, muf_f32 s);
MUF_API MufVec3 mufVec3Scale(MufVec3 v, muf_f32 s);
MUF_API MufVec4 mufVec4Scale(MufVec4 v, muf_f32 s);

MUF_API muf_f32 mufVec2Dot(MufVec2 v1, MufVec2 v2);
MUF_API muf_f32 mufVec3Dot(MufVec3 v1, MufVec3 v2);

MUF_API muf_f32 mufVec2Cross(MufVec2 v1, MufVec2 v2);
MUF_API MufVec3 mufVec3Cross(MufVec3 v1, MufVec3 v2);

MUF_API MufVec2 mufVec2Hadamard(MufVec2 v1, MufVec2 v2);
MUF_API MufVec3 mufVec3Hadamard(MufVec3 v1, MufVec3 v2);
MUF_API MufVec4 mufVec4Hadamard(MufVec4 v1, MufVec4 v2);

MUF_API muf_f32 mufVec2Length2(MufVec2 v);
MUF_API muf_f32 mufVec3Length2(MufVec3 v);

MUF_API muf_f32 mufVec2Length(MufVec2 v);
MUF_API muf_f32 mufVec3Length(MufVec3 v);

MUF_API MufVec2 mufVec2Normalize(MufVec2 v);
MUF_API MufVec3 mufVec3Normalize(MufVec3 v);

#define mufVecCast_2_3(V) (MufVec2) { V.x, V.y }
#define mufVecCast_2_4(V) mufVecCast_2_3(V)
#define mufVecCast_3_2(V) (MufVec3) { V.x, V.y, 0.0F }

#define mufVecCast(DstDimension, SrcDimension, Value)

typedef struct MufColor_s {
	muf_f32 r, g, b;
} MufColor;

typedef struct MufColorI_s {
	muf_u8 r, g, b;
} MufColorI;

MUF_API MufColor mufCreateColor(muf_f32 r, muf_f32 g, muf_f32 b);
MUF_API void mufColorLoad(MufColor *color, const muf_f32 *values);
MUF_API void mufColorLoadv(MufColor *color, MufVec3 vec);
MUF_API void mufColorLoadHex(MufColor *color, muf_u32 hexCode);
MUF_API MufColor mufColorAdd(MufColor c1, MufColor c2);
MUF_API MufColor mufColorSub(MufColor c1, MufColor c2);
MUF_API MufColor mufColorMul(MufColor c1, MufColor c2);
MUF_API MufColor mufColorScale(MufColor c, muf_f32 s);

MUF_API muf_u32 mufColorToHex(MufColor c);

typedef struct MufRGBA_s {
	muf_f32 r, g, b, a;
} MufRGBA;

MUF_API MufRGBA mufCreateRGBA(muf_f32 r, muf_f32 g, muf_f32 b, muf_f32 a);
MUF_API void mufRGBALoad(MufRGBA *color, const muf_f32 *values);
MUF_API void mufRGBALoadv(MufRGBA *color, MufVec3 vec);
MUF_API MufRGBA mufRGBAAdd(MufRGBA c1, MufRGBA c2);
MUF_API MufRGBA mufRGBASub(MufRGBA c1, MufRGBA c2);
MUF_API MufRGBA mufRGBAMul(MufRGBA c1, MufRGBA c2);
MUF_API MufRGBA mufRGBAScale(MufRGBA c, muf_f32 s);

typedef struct MufQuat4_s {
	muf_f32 x, y, z, w;
} MufQuat4;

MufQuat4 mufCreateQuat4(muf_f32 x, muf_f32 y, muf_f32 z, muf_f32 w);
MufQuat4 mufCreateQuat4AR();

typedef union MufMat2_u {
	muf_f32 values[2][2];
    struct { MufVec2 v0, v1; } columns;
} MufMat2;

typedef union MufMat3_u {
	muf_f32 values[3][3];
    struct { MufVec3 v0, v1, v2; } columns;
} MufMat3;

typedef union MufMat4_u {
	muf_f32 values[4][4];
    struct { MufVec4 v0, v1, v2, v3; } columns;
} MufMat4;

MUF_API MufMat2 mufCreateMat2(muf_f32 x0, muf_f32 y0, muf_f32 x1, muf_f32 y1);
MUF_API MufMat3 mufCreateMat3(muf_f32 x0, muf_f32 y0, muf_f32 z0, 
	muf_f32 x1, muf_f32 y1, muf_f32 z1,
	muf_f32 x2, muf_f32 y2, muf_f32 z2);
MUF_API MufMat4 mufCreateMat4(muf_f32 x0, muf_f32 y0, muf_f32 z0, muf_f32 w0,
	muf_f32 x1, muf_f32 y1, muf_f32 z1, muf_f32 w1,
	muf_f32 x2, muf_f32 y2, muf_f32 z2, muf_f32 w2,
	muf_f32 x3, muf_f32 y3, muf_f32 z3, muf_f32 w3);

MUF_API void mufMat2Load(MufMat2 *mat, muf_f32 *values, muf_usize count);
MUF_API void mufMat3Load(MufMat3 *mat, muf_f32 *values, muf_usize count);
MUF_API void mufMat4Load(MufMat4 *mat, muf_f32 *values, muf_usize count);
MUF_API void mufMat2Loadv(MufMat2 *mat, MufVec2 v0, MufVec2 v1);
MUF_API void mufMat3Loadv(MufMat3 *mat, MufVec3 v0, MufVec3 v1, MufVec3 v2);
MUF_API void mufMat4Loadv(MufMat4 *mat, MufVec4 v0, MufVec4 v1, MufVec4 v2, MufVec4 v3);

MUF_API muf_bool mufMat2Equal(muf_crawptr m1, muf_crawptr m2);
MUF_API muf_bool mufMat3Equal(muf_crawptr m1, muf_crawptr m2);
MUF_API muf_bool mufMat4Equal(muf_crawptr m1, muf_crawptr m2);

MUF_API MufMat2 mufMat2Identity(void);
MUF_API MufMat3 mufMat3Identity(void);
MUF_API MufMat4 mufMat4Identity(void);

MUF_API MufMat2 mufMat2Zero(void);
MUF_API MufMat3 mufMat3Zero(void);
MUF_API MufMat4 mufMat4Zero(void);

MUF_API MufMat2 mufMat2Add(const MufMat2 *a, const MufMat2 *b);
MUF_API MufMat3 mufMat3Add(const MufMat3 *a, const MufMat3 *b);
MUF_API MufMat4 mufMat4Add(const MufMat4 *a, const MufMat4 *b);

MUF_API MufMat2 *mufMat2AddBy(MufMat2 *base, const MufMat2 *mat);
MUF_API MufMat3 *mufMat3AddBy(MufMat3 *base, const MufMat3 *mat);
MUF_API MufMat4 *mufMat4AddBy(MufMat4 *base, const MufMat4 *mat);

MUF_API MufMat2 mufMat2Sub(const MufMat2 *a, const MufMat2 *b);
MUF_API MufMat3 mufMat3Sub(const MufMat3 *a, const MufMat3 *b);
MUF_API MufMat4 mufMat4Sub(const MufMat4 *a, const MufMat4 *b);

MUF_API MufMat2 *mufMat2SubBy(MufMat2 *base, const MufMat2 *mat);
MUF_API MufMat3 *mufMat3SubBy(MufMat3 *base, const MufMat3 *mat);
MUF_API MufMat4 *mufMat4SubBy(MufMat4 *base, const MufMat4 *mat);

MUF_API MufMat2 mufMat2Mul(const MufMat2 *a, const MufMat2 *b);
MUF_API MufMat3 mufMat3Mul(const MufMat3 *a, const MufMat3 *b);
MUF_API MufMat4 mufMat4Mul(const MufMat4 *a, const MufMat4 *b);

MUF_API MufMat2 *mufMat2MulBy(MufMat2 *base, const MufMat2 *mat);
MUF_API MufMat3 *mufMat3MulBy(MufMat3 *base, const MufMat3 *mat);
MUF_API MufMat4 *mufMat4MulBy(MufMat4 *base, const MufMat4 *mat);

MUF_API MufMat2 mufMat2Scale(const MufMat2 *mat, muf_f32 s);
MUF_API MufMat3 mufMat3Scale(const MufMat3 *mat, muf_f32 s);
MUF_API MufMat4 mufMat4Scale(const MufMat4 *mat, muf_f32 s);

MUF_API MufMat2 mufMat2Inverse(const MufMat2 *mat);
MUF_API MufMat3 mufMat3Inverse(const MufMat3 *mat);
MUF_API MufMat4 mufMat4Inverse(const MufMat4 *mat);

MUF_API MufMat2 mufMat2Transpose(const MufMat2 *mat);
MUF_API MufMat3 mufMat3Transpose(const MufMat3 *mat);
MUF_API MufMat4 mufMat4Transpose(const MufMat4 *mat);

MUF_API muf_f32 mufMat2Determinant(const MufMat2 *mat);
MUF_API muf_f32 mufMat3Determinant(const MufMat3 *mat);
MUF_API muf_f32 mufMat4Determinant(const MufMat4 *mat);

MUF_API MufVec2 mufVec2MulMat(MufVec2 vec, const MufMat2 *mat);
MUF_API MufVec3 mufVec3MulMat(MufVec3 vec, const MufMat3 *mat);
MUF_API MufVec4 mufVec4MulMat(MufVec4 vec, const MufMat4 *mat);

MUF_API MufVec2 mufMat2MulVec(const MufMat2 *mat, MufVec2 vec);
MUF_API MufVec3 mufMat3MulVec(const MufMat3 *mat, MufVec3 vec);
MUF_API MufVec4 mufMat4MulVec(const MufMat4 *mat, MufVec4 vec);

MUF_API MufMat4 *mufRotateX(MufMat4 *base, muf_f32 rotation);
MUF_API MufMat4 *mufRotateY(MufMat4 *base, muf_f32 rotation);
MUF_API MufMat4 *mufRotateZ(MufMat4 *base, muf_f32 rotation);
MUF_API MufMat4 *mufRotate(MufMat4 *base, MufVec3 axis, muf_f32 rotation);
MUF_API MufMat4 *mufScale(MufMat4 *base, MufVec3 scales);
MUF_API MufMat4 *mufScaleAxis(MufMat4 *base, MufVec3 axis, muf_f32 factor);
MUF_API MufMat4 *mufTranslate(MufMat4 *base, MufVec3 v);
MUF_API MufMat4 *mufShearX(MufMat4 *base, muf_f32 s, muf_f32 t);
MUF_API MufMat4 *mufShearY(MufMat4 *base, muf_f32 s, muf_f32 t);
MUF_API MufMat4 *mufShearZ(MufMat4 *base, muf_f32 s, muf_f32 t);
MUF_API MufMat4 *mufMirror(MufMat4 *base, MufVec3 axis);

MUF_API MufVec3 mufVec3Transform(MufVec3 vec, const MufMat4 *transform);
MUF_API MufVec4 mufVec4Transform(MufVec4 vec, const MufMat4 *transform);

typedef struct MufAABB2_s {
	MufVec2 min;
	MufVec2 max;
} MufAABB;

typedef struct MufAABB3_s {
	MufVec3 min;
	MufVec3 max;
} MufAABB3;

typedef struct MufRect2_s {
	muf_f32 x;
	muf_f32 y;
	muf_f32 width;
	muf_f32 height;
} MufRect2;

typedef struct MufRect2i_s {
	muf_i32 x;
	muf_i32 y;
	muf_u32 width;
	muf_u32 height;
} MufRect2i;

typedef struct MufRect2p_s {
	muf_f32 left;
	muf_f32 top;
	muf_f32 right;
	muf_f32 bottom;
} MufRect2p;

typedef struct MufRect2pi_s {
	muf_i32 left;
	muf_i32 top;
	muf_i32 right;
	muf_i32 bottom;
} MufRect2pi;

typedef struct MufCube3_s {
	muf_f32 left;
	muf_f32 top;
	muf_f32 right;
	muf_f32 bottom;
	muf_f32 near;
	muf_f32 far;
} MufCube3;

typedef struct MufCube3i_s {
	muf_i32 left;
	muf_i32 top;
	muf_i32 right;
	muf_i32 bottom;
	muf_i32 near;
	muf_i32 far;
} MufCube3i;

typedef struct MufExtent2i_s {
	muf_u32 width;
	muf_u32 height;
} MufExtent2i;

typedef struct MufExtent3i_s {
	muf_u32 width;
	muf_u32 height;
	muf_u32 depth;
} MufExtent3i;

typedef struct MufExtent2f_s {
	muf_f32 width;
	muf_f32 height;
} MufExtent2f;

typedef struct MufExtent3f_s {
	muf_f32 width;
	muf_f32 height;
	muf_f32 depth;
} MufExtent3f;

typedef MufVec2 MufOffset2f;
typedef MufVec3 MufOffset3f;

typedef struct MufOffset2i_s {
	muf_i32 x;
	muf_i32 y;
} MufOffset2i;

typedef struct MufOffset3i_s {
	muf_i32 x;
	muf_i32 y;
	muf_i32 z;
} MufOffset3i;

#define MUF_VEC2_ZERO (MufVec2) { 0.0F, 0.0F }
#define MUF_VEC3_ZERO (MufVec3) { 0.0F, 0.0F, 0.0F }
#define MUF_VEC4_ZERO (MufVec4) { 0.0F, 0.0F, 0.0F, 0.0F }

#define MUF_VEC2_X_UNIT (MufVec2) { 1.0F, 0.0F };
#define MUF_VEC2_Y_UNIT (MufVec2) { 0.0F, 1.0F };

#define MUF_VEC3_X_UNIT (MufVec3) { 1.0F, 0.0F, 0.0F };
#define MUF_VEC3_Y_UNIT (MufVec3) { 0.0F, 1.0F, 0.0F };
#define MUF_VEC3_Z_UNIT (MufVec3) { 0.0F, 0.0F, 1.0F };

#define MUF_VEC4_X_UNIT (MufVec4) { 1.0F, 0.0F, 0.0F, 0.0F };
#define MUF_VEC4_Y_UNIT (MufVec4) { 0.0F, 1.0F, 0.0F, 0.0F };
#define MUF_VEC4_Z_UNIT (MufVec4) { 0.0F, 0.0F, 1.0F, 0.0F };
#define MUF_VEC4_W_UNIT (MufVec4) { 0.0F, 0.0F, 0.0F, 1.0F };

#endif