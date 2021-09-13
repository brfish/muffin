#include "muffin_core/math.h"

#include <string.h>

#include "muffin_core/hash.h"
#include "muffin_core/memory.h"

muf_bool mufFloatEqualEpsilon(muf_f64 a, muf_f64 b, muf_f64 epsilon) {
    return mufAbs(a - b) < epsilon;
}

static const muf_u32 _multiplyDeBruijnBitPosition[] = {
    0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

muf_u32 mufBitsFindFirstSet(muf_u32 bits) {
    return _multiplyDeBruijnBitPosition[((muf_u32)((bits & -bits) * 0x077CB531U)) >> 27];
}

muf_bool mufBitsTest(muf_u32 bits, muf_u32 index) {
    return bits & (1U << index);
}

void mufBitsSet(muf_u32 *bits, muf_u32 index) {
    *bits |= 1U << index;
}

void mufBitsReset(muf_u32 *bits, muf_u32 index) {
    *bits &= ~(1U << index);
}

muf_bool mufBitsFlip(muf_u32 *bits, muf_u32 index) {
    *bits ^= 1 << index;
    return mufBitsTest(*bits, index);
}

muf_f32 mufToRadians(muf_f32 degrees) {
    return degrees / 180.0F * MUF_PI;
}

muf_f32 mufToDegrees(muf_f32 radians) {
    return radians / MUF_PI * 180.0F;
}

muf_u64 mufCeilPower2(muf_u64 value) {
    muf_u64 x = value <= 1 ? 0 : value - 1;

    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return x + 1;
}

MUF_INTERNAL void _mufInitMT19937(MufMT19937 *generator, muf_u32 seed) {
    generator->index = 0;
    generator->seed = seed;
    generator->mt[0] = seed;

    for (muf_index i = 1; i < 624; ++i) {
        generator->mt[i] = (muf_u32) (0xFFFFFFFF & (0x6C078965 * (generator->mt[i - 1] ^ (generator->mt[i - 1] >> 30)) + i));
    }
}

MUF_INTERNAL void _mufMT19937Twist(MufMT19937 *generator) {
    muf_u32 *mt = generator->mt;

    for (muf_index i = 0; i < 624; ++i) {
        muf_u32 y = (mt[i] & 0x80000000) + (mt[(i + 1) % 624] & 0x7FFFFFFF);
        mt[i] = mt[(i + 397) % 624] ^ (y >> 1);
        if (y & 1)
            mt[i] ^= 0x9908B0DF;
    }
}

MufMT19937 *mufCreateMT19937(muf_u32 seed) {
    MufMT19937 *obj = mufAlloc(MufMT19937, 1);
    _mufInitMT19937(obj, seed);
    return obj;
}

void mufDestroyMT19937(MufMT19937 *generator) {
    mufFree(generator);
}

void mufMT19937SetSeed(MufMT19937 *generator, muf_u32 seed) {
    _mufInitMT19937(generator, seed);
}

muf_u32 mufMT19937Next(MufMT19937 *generator) {
    if (generator->index == 0)
        _mufMT19937Twist(generator);
    
    muf_u32 y = generator->mt[generator->index];
    y = y ^ (y >> 11);
	y = y ^ ((y << 7) & (0x9D2C5680));
	y = y ^ ((y << 15) & (0xEFC60000));
	y = y ^ (y >> 18);
	generator->index = (generator->index + 1) % 624;

    return y;
}

muf_i32 mufRandomRange(MufMT19937 *generator, muf_i32 minValue, muf_i32 maxValue) {
    if (minValue < maxValue) {
        mufSwapInt(minValue, maxValue);
    }
    muf_u32 v = mufMT19937Next(generator);
    return (v % (maxValue - minValue + 1)) + minValue;
}

muf_f32 mufRandomRangef(MufMT19937 *generator, muf_f32 minValue, muf_f32 maxValue) {
    if (minValue > maxValue) {
        mufSwap(muf_f32, minValue, maxValue);
    }
    muf_u32 v = mufMT19937Next(generator);
    return minValue + (((muf_f32) v) / UINT32_MAX) * (maxValue - minValue);
}

MufVec2 mufCreateVec2(muf_f32 x, muf_f32 y) {
    return (MufVec2) { x, y };
}

MufVec3 mufCreateVec3(muf_f32 x, muf_f32 y, muf_f32 z) {
    return (MufVec3) { x, y, z };
}

MufVec4 mufCreateVec4(muf_f32 x, muf_f32 y, muf_f32 z, muf_f32 w) {
    return (MufVec4) { x, y, z, w };
}

void mufVec2Load(MufVec2 *v, const muf_f32 *values) {
    v->x = values[0];
    v->y = values[1];
}

void mufVec3Load(MufVec3 *v, const muf_f32 *values) {
    v->x = values[0];
    v->y = values[1];
    v->z = values[2];
}

void mufVec4Load(MufVec4 *v, const muf_f32 *values) {
    v->x = values[0];
    v->y = values[1];
    v->z = values[2];
    v->w = values[3];
}

muf_bool mufVec2Equal(muf_crawptr v1, muf_crawptr v2) {
    const MufVec2 *a = (const MufVec2 *) v1;
    const MufVec2 *b = (const MufVec2 *) v2;
    return a->x == b->x && a->y == b->y;
}

muf_bool mufVec3Equal(muf_crawptr v1, muf_crawptr v2) {
    const MufVec3 *a = (const MufVec3 *) v1;
    const MufVec3 *b = (const MufVec3 *) v2;
    return a->x == b->x && a->y == b->y && a->z == b->z;
}

muf_bool mufVec4Equal(muf_crawptr v1, muf_crawptr v2) {
    const MufVec4 *a = (const MufVec4 *) v1;
    const MufVec4 *b = (const MufVec4 *) v2;
    return a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w;
}

muf_index mufVec2Hash(muf_crawptr v) {
    return mufHashBytes(v, sizeof(MufVec2));
}

muf_index mufVec3Hash(muf_crawptr v) {
    return mufHashBytes(v, sizeof(MufVec3));
}

muf_index mufVec4Hash(muf_crawptr v) {
    return mufHashBytes(v, sizeof(MufVec4));
}

void mufVec2ToArray(MufVec2 vec, muf_f32 *array) {
    array[0] = vec.x;
    array[1] = vec.y;
}

void mufVec3ToArray(MufVec3 vec, muf_f32 *array) {
    array[0] = vec.x;
    array[1] = vec.y;
    array[2] = vec.z;
}

void mufVec4ToArray(MufVec4 vec, muf_f32 *array) {
    array[0] = vec.x;
    array[1] = vec.y;
    array[2] = vec.z;
    array[3] = vec.w;
}

MufVec2 mufVec2Negate(MufVec2 v) {
    return (MufVec2) { -v.x, -v.y };
}

MufVec3 mufVec3Negate(MufVec3 v) {
    return (MufVec3) { -v.x, -v.y, -v.z };
}

MufVec4 mufVec4Negate(MufVec4 v) {
    return (MufVec4) { -v.x, -v.y, -v.z, -v.w };
}

MufVec2 mufVec2Add(MufVec2 v1, MufVec2 v2) {
    return (MufVec2) { v1.x + v2.x, v1.y + v2.y };
}

MufVec3 mufVec3Add(MufVec3 v1, MufVec3 v2) {
    return (MufVec3) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

MufVec4 mufVec4Add(MufVec4 v1, MufVec4 v2) {
    return (MufVec4) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

MufVec2 mufVec2Sub(MufVec2 v1, MufVec2 v2) {
    return (MufVec2) { v1.x - v2.x, v1.y - v2.y };
}

MufVec3 mufVec3Sub(MufVec3 v1, MufVec3 v2) {
    return (MufVec3) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

MufVec4 mufVec4Sub(MufVec4 v1, MufVec4 v2) {
    return (MufVec4) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

MufVec2 mufVec2Mul(MufVec2 v, muf_f32 s) {
    return (MufVec2) { v.x * s, v.y * s };
}

MufVec3 mufVec3Mul(MufVec3 v, muf_f32 s) {
    return (MufVec3) { v.x * s, v.y * s, v.z * s };
}

MufVec4 mufVec4Mul(MufVec4 v, muf_f32 s) {
    return (MufVec4) { v.x * s, v.y * s, v.z * s, v.w * s };
}

MufVec2 mufVec2Div(MufVec2 v, muf_f32 s) {
    MUF_FASSERT(s != 0.0F, "The divisor cannot be zero");
    muf_f32 denom = 1.0F / s;
    return (MufVec2) { v.x * denom, v.y * denom };
}

MufVec3 mufVec3Div(MufVec3 v, muf_f32 s) {
    MUF_FASSERT(s != 0.0F, "The divisor cannot be zero");
    muf_f32 denom = 1.0F / s;
    return (MufVec3) { v.x * denom, v.y * denom, v.z * denom };
}

MufVec4 mufVec4Div(MufVec4 v, muf_f32 s) {
    MUF_FASSERT(s != 0.0F, "The divisor cannot be zero");
    muf_f32 denom = 1.0F / s;
    return (MufVec4) { v.x * denom, v.y * denom, v.z * denom, v.w * denom };
}

MufVec2 mufVec2Scale(MufVec2 v, muf_f32 s) {
    return mufVec2Mul(v, s);
}

MufVec3 mufVec3Scale(MufVec3 v, muf_f32 s) {
    return mufVec3Mul(v, s);
}

MufVec4 mufVec4Scale(MufVec4 v, muf_f32 s) {
    return mufVec4Mul(v, s);
}

muf_f32 mufVec2Dot(MufVec2 v1, MufVec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

muf_f32 mufVec3Dot(MufVec3 v1, MufVec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

muf_f32 mufVec2Cross(MufVec2 v1, MufVec2 v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

MufVec3 mufVec3Cross(MufVec3 v1, MufVec3 v2) {
    return (MufVec3) {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

MufVec2 mufVec2Hadamard(MufVec2 v1, MufVec2 v2) {
    return (MufVec2) { v1.x * v2.x, v1.y * v2.y };
}

MufVec3 mufVec3Hadamard(MufVec3 v1, MufVec3 v2) {
    return (MufVec3) { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

MufVec4 mufVec4Hadamard(MufVec4 v1, MufVec4 v2) {
    return (MufVec4) { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}

muf_f32 mufVec2Length2(MufVec2 v) {
    return v.x * v.x + v.y * v.y;
}

muf_f32 mufVec2Length(MufVec2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

muf_f32 mufVec3Length2(MufVec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

muf_f32 mufVec3Length(MufVec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

MufVec2 mufVec2Normalize(MufVec2 v) {
    return mufVec2Div(v, mufVec2Length(v));
}

MufVec3 mufVec3Normalize(MufVec3 v) {
    return mufVec3Div(v, mufVec3Length(v));
}

MufColor mufCreateColor(muf_f32 r, muf_f32 g, muf_f32 b) {
    return (MufColor) {
        mufClampf(r, 0.0f, 1.0f),
        mufClampf(g, 0.0f, 1.0f),
        mufClampf(b, 0.0f, 1.0f)
    };
}

void mufColorLoad(MufColor *color, const muf_f32 *values) {
    color->r = mufClampf(values[0], 0.0F, 1.0F);
    color->g = mufClampf(values[1], 0.0F, 1.0F);
    color->b = mufClampf(values[2], 0.0F, 1.0F);
}

void mufColorLoadv(MufColor *color, MufVec3 vec) {
    color->r = mufClampf(vec.x, 0.0F, 1.0F);
    color->g = mufClampf(vec.y, 0.0F, 1.0F);
    color->b = mufClampf(vec.z, 0.0F, 1.0F);
}

void mufColorLoadHex(MufColor *color, muf_u32 hexCode) {
    color->r = (muf_f32) (hexCode >> 24) / 255.0F;
    color->g = (muf_f32) ((hexCode >> 16) & 0xF) / 255.0F;
    color->b = (muf_f32) ((hexCode >> 8) & 0xF) / 255.0F;
}

MufColor mufColorAdd(MufColor c1, MufColor c2) {
    return (MufColor) {
        mufClampf(c1.r + c2.r, 0.0F, 1.0F),
        mufClampf(c1.g + c2.g, 0.0F, 1.0F),
        mufClampf(c1.b + c2.b, 0.0F, 1.0F)
    };
}

MufColor mufColorSub(MufColor c1, MufColor c2) {
    return (MufColor) {
        mufClampf(c1.r - c2.r, 0.0F, 1.0F),
        mufClampf(c1.g - c2.g, 0.0F, 1.0F),
        mufClampf(c1.b - c2.b, 0.0F, 1.0F)
    };
}

MufColor mufColorMul(MufColor c1, MufColor c2) {
    return (MufColor) {
        mufClampf(c1.r * c2.r, 0.0F, 1.0F),
        mufClampf(c1.g * c2.g, 0.0F, 1.0F),
        mufClampf(c1.b * c2.b, 0.0F, 1.0F)
    };
}

MufColor mufColorScale(MufColor c, muf_f32 s) {
    return (MufColor) {
        mufClampf(c.r * s, 0.0F, 1.0F),
        mufClampf(c.g * s, 0.0F, 1.0F),
        mufClampf(c.b * s, 0.0F, 1.0F)
    };
}

muf_u32 mufColorToHex(MufColor c) {
    muf_u8 r = (muf_u8) (c.r * 255.0F);
    muf_u8 g = (muf_u8) (c.g * 255.0F);
    muf_u8 b = (muf_u8) (c.b * 255.0F);
    
    return (r << 24) | (g << 16) | (b << 8);
}

MufMat2 mufCreateMat2(muf_f32 x0, muf_f32 x1, muf_f32 y0, muf_f32 y1) {
    return (MufMat2) { { { x0, x1 }, { y0, y1 } } };
}

MufMat3 mufCreateMat3(muf_f32 x0, muf_f32 y0, muf_f32 z0, 
	muf_f32 x1, muf_f32 y1, muf_f32 z1,
	muf_f32 x2, muf_f32 y2, muf_f32 z2) {
    return (MufMat3) { { { x0, y0, z0 }, { x1, y1, z1 }, { x2, y2, z2 } } };    
}

MufMat4 mufCreateMat4(muf_f32 x0, muf_f32 y0, muf_f32 z0, muf_f32 w0,
	muf_f32 x1, muf_f32 y1, muf_f32 z1, muf_f32 w1,
	muf_f32 x2, muf_f32 y2, muf_f32 z2, muf_f32 w2,
	muf_f32 x3, muf_f32 y3, muf_f32 z3, muf_f32 w3) {
    return (MufMat4) { { { x0, y0, z0, w0}, { x1, y1, z1, w1 }, { x2, y2, z2, w2 }, { x3, y3, z3, w3 } } };
}

void mufMat2Load(MufMat2 *mat, muf_f32 *values, muf_usize count) {
    count = mufMin(4, count);
    memset(mat->values, 0, 4 * sizeof(muf_f32));
    memcpy(mat->values, values, count * sizeof(muf_f32));
}

void mufMat3Load(MufMat3 *mat, muf_f32 *values, muf_usize count) {
    count = mufMin(9, count);
    memset(mat->values, 0, 9 * sizeof(muf_f32));
    memcpy(mat->values, values, count * sizeof(muf_f32));
}

void mufMat4Load(MufMat4 *mat, muf_f32 *values, muf_usize count) {
    count = mufMin(16, count);
    memset(mat->values, 0, 16 * sizeof(muf_f32));
    memcpy(mat->values, values, count * sizeof(muf_f32));
}

void mufMat2Loadv(MufMat2 *mat, MufVec2 v0, MufVec2 v1) {
    muf_f32 (*m)[2] = mat->values;

    m[0][0] = v0.x;
    m[0][1] = v0.y;

    m[1][0] = v1.x;
    m[1][1] = v1.y;
}

void mufMat3Loadv(MufMat3 *mat, MufVec3 v0, MufVec3 v1, MufVec3 v2) {
    muf_f32 (*m)[3] = mat->values;
    
    m[0][0] = v0.x;
    m[0][1] = v0.y;
    m[0][2] = v0.z;

    m[1][0] = v1.x;
    m[1][1] = v1.y;
    m[1][2] = v1.z;

    m[2][0] = v2.x;
    m[2][1] = v2.y;
    m[2][2] = v2.z;
}

void mufMat4Loadv(MufMat4 *mat, MufVec4 v0, MufVec4 v1, MufVec4 v2, MufVec4 v3) {
    muf_f32 (*m)[4] = mat->values;
    
    m[0][0] = v0.x;
    m[0][1] = v0.y;
    m[0][2] = v0.z;
    m[0][3] = v0.w;

    m[1][0] = v1.x;
    m[1][1] = v1.y;
    m[1][2] = v1.z;
    m[1][3] = v1.w;

    m[2][0] = v2.x;
    m[2][1] = v2.y;
    m[2][2] = v2.z;
    m[2][3] = v2.w;

    m[3][0] = v3.x;
    m[3][1] = v3.y;
    m[3][2] = v3.z;
    m[3][3] = v3.w;
}

muf_bool mufMat2Equal(muf_crawptr m1, muf_crawptr m2) {
    const MufMat2 *a = (const MufMat2 *) m1;
    const MufMat2 *b = (const MufMat2 *) m2;
    return mufVec2Equal(&a->columns.v0, &b->columns.v0) &&
        mufVec2Equal(&a->columns.v1, &b->columns.v1);
}

muf_bool mufMat3Equal(muf_crawptr m1, muf_crawptr m2) {
    const MufMat3 *a = (const MufMat3 *) m1;
    const MufMat3 *b = (const MufMat3 *) m2;
    return mufVec3Equal(&a->columns.v0, &b->columns.v0) &&
        mufVec3Equal(&a->columns.v1, &b->columns.v1) &&
        mufVec3Equal(&a->columns.v2, &b->columns.v2);
}

muf_bool mufMat4Equal(muf_crawptr m1, muf_crawptr m2) {
    const MufMat4 *a = (const MufMat4 *) m1;
    const MufMat4 *b = (const MufMat4 *) m2;
    return mufVec4Equal(&a->columns.v0, &b->columns.v0) &&
        mufVec4Equal(&a->columns.v1, &b->columns.v1) &&
        mufVec4Equal(&a->columns.v2, &b->columns.v2) &&
        mufVec4Equal(&a->columns.v3, &b->columns.v3);
}

MufMat2 mufMat2Identity() {
    return mufCreateMat2(1.0F, 0.0F, 0.0F, 1.0F);
}

MufMat3 mufMat3Identity() {
    return mufCreateMat3(
        1.0F, 0.0F, 0.0F,
        0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 1.0F
    );
}

MufMat4 mufMat4Identity() {
    return mufCreateMat4(
        1.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 1.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
}

MufMat2 mufMat2Add(const MufMat2 *a, const MufMat2 *b) {
    MufMat2 mat;
    mufMat2Load(&mat, (muf_f32 *)a->values, 4);
    mufMat2AddBy(&mat, b);
    return mat;
}

MufMat3 mufMat3Add(const MufMat3 *a, const MufMat3 *b) {
    MufMat3 mat;
    mufMat3Load(&mat, (muf_f32 *)a->values, 9);
    mufMat3AddBy(&mat, b);
    return mat;
}

MufMat4 mufMat4Add(const MufMat4 *a, const MufMat4 *b) {
    MufMat4 mat; 
    mufMat4Load(&mat, (muf_f32 *)a->values, 16);
    mufMat4AddBy(&mat, b);
    return mat;
}

MufMat2 *mufMat2AddBy(MufMat2 *base, const MufMat2 *m) {
    base->values[0][0] += m->values[0][0];
    base->values[0][1] += m->values[0][1];

    base->values[1][0] += m->values[1][0];
    base->values[1][1] += m->values[1][1];

    return base;
}

MufMat3 *mufMat3AddBy(MufMat3 *base, const MufMat3 *m) {
    base->values[0][0] += m->values[0][0];
    base->values[0][1] += m->values[0][1];
    base->values[0][2] += m->values[0][2];

    base->values[1][0] += m->values[1][0];
    base->values[1][1] += m->values[1][1];
    base->values[1][2] += m->values[1][2];

    base->values[2][0] += m->values[2][0];
    base->values[2][1] += m->values[2][1];
    base->values[2][2] += m->values[2][2];

    return base;
}

MufMat4 *mufMat4AddBy(MufMat4 *base, const MufMat4 *m) {
    base->values[0][0] += m->values[0][0];
    base->values[0][1] += m->values[0][1];
    base->values[0][2] += m->values[0][2];
    base->values[0][3] += m->values[0][3];

    base->values[1][0] += m->values[1][0];
    base->values[1][1] += m->values[1][1];
    base->values[1][2] += m->values[1][2];
    base->values[1][3] += m->values[1][3];

    base->values[2][0] += m->values[2][0];
    base->values[2][1] += m->values[2][1];
    base->values[2][2] += m->values[2][2];
    base->values[2][3] += m->values[2][3];

    base->values[3][0] += m->values[3][0];
    base->values[3][1] += m->values[3][1];
    base->values[3][2] += m->values[3][2];
    base->values[3][3] += m->values[3][3];

    return base;
}

MufMat2 mufMat2Sub(const MufMat2 *m1, const MufMat2 *m2) {
    MufMat2 mat;
    mufMat2Load(&mat, (muf_f32 *)m1->values, 4);
    mufMat2SubBy(&mat, m2);
    return mat;
}

MufMat3 mufMat3Sub(const MufMat3 *m1, const MufMat3 *m2) {
    MufMat3 mat;
    mufMat3Load(&mat, (muf_f32 *)m1->values, 9);
    mufMat3SubBy(&mat, m2);
    return mat;
}

MufMat4 mufMat4Sub(const MufMat4 *m1, const MufMat4 *m2) {
    MufMat4 mat;
    mufMat4Load(&mat, (muf_f32 *)m1->values, 16);
    mufMat4SubBy(&mat, m2);
    return mat;
}

MufMat2 *mufMat2SubBy(MufMat2 *base, const MufMat2 *m) {
    base->values[0][0] -= m->values[0][0];
    base->values[0][1] -= m->values[0][1];

    base->values[1][0] -= m->values[1][0];
    base->values[1][1] -= m->values[1][1];

    return base;
}

MufMat3 *mufMat3SubBy(MufMat3 *base, const MufMat3 *m) {
    base->values[0][0] -= m->values[0][0];
    base->values[0][1] -= m->values[0][1];
    base->values[0][2] -= m->values[0][2];

    base->values[1][0] -= m->values[1][0];
    base->values[1][1] -= m->values[1][1];
    base->values[1][2] -= m->values[1][2];

    base->values[2][0] -= m->values[2][0];
    base->values[2][1] -= m->values[2][1];
    base->values[2][2] -= m->values[2][2];

    return base;
}

MufMat4 *mufMat4SubBy(MufMat4 *base, const MufMat4 *m) {
    base->values[0][0] -= m->values[0][0];
    base->values[0][1] -= m->values[0][1];
    base->values[0][2] -= m->values[0][2];
    base->values[0][3] -= m->values[0][3];

    base->values[1][0] -= m->values[1][0];
    base->values[1][1] -= m->values[1][1];
    base->values[1][2] -= m->values[1][2];
    base->values[1][3] -= m->values[1][3];

    base->values[2][0] -= m->values[2][0];
    base->values[2][1] -= m->values[2][1];
    base->values[2][2] -= m->values[2][2];
    base->values[2][3] -= m->values[2][3];

    base->values[3][0] -= m->values[3][0];
    base->values[3][1] -= m->values[3][1];
    base->values[3][2] -= m->values[3][2];
    base->values[3][3] -= m->values[3][3];

    return base;
}

MufMat2 mufMat2Mul(const MufMat2 *m1, const MufMat2 *m2) {
    const muf_f32 (*a)[2] = m1->values;
    const muf_f32 (*b)[2] = m2->values;

    return mufCreateMat2(
        b[0][0] * a[0][0] + b[0][1] * a[1][0], b[0][0] * a[0][1] + b[0][1] * a[1][1],
        b[1][0] * a[0][0] + b[1][1] * a[1][0], b[1][0] * a[0][1] + b[1][1] * a[1][1]
    );
}

MufMat3 mufMat3Mul(const MufMat3 *m1, const MufMat3 *m2) {
    const muf_f32 (*a)[3] = m1->values;
    const muf_f32 (*b)[3] = m2->values;

    return mufCreateMat3(
        a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2],
        a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2],
        a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2],
        a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2],
        a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2],
        a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2],
        a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2],
        a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2],
        a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2]
    );
}

MufMat4 mufMat4Mul(const MufMat4 *m1, const MufMat4 *m2) {
    const muf_f32 (*b)[4] = m2->values;

    const MufVec4 a0 = m1->columns.v0;
    const MufVec4 a1 = m1->columns.v1;
    const MufVec4 a2 = m1->columns.v2;
    const MufVec4 a3 = m1->columns.v3;

    MufVec4 v0 = mufCreateVec4(
        a0.x * b[0][0] + a1.x * b[0][1] * a2.x * b[0][2] * a3.x * b[0][3],
        a0.y * b[0][0] + a1.y * b[0][1] * a2.y * b[0][2] * a3.y * b[0][3],
        a0.z * b[0][0] + a1.z * b[0][1] * a2.z * b[0][2] * a3.z * b[0][3],
        a0.w * b[0][0] + a1.w * b[0][1] * a2.w * b[0][2] * a3.w * b[0][3]
    );
    MufVec4 v1 = mufCreateVec4(
        a0.x * b[1][0] + a1.x * b[1][1] * a2.x * b[1][2] * a3.x * b[1][3],
        a0.y * b[1][0] + a1.y * b[1][1] * a2.y * b[1][2] * a3.y * b[1][3],
        a0.z * b[1][0] + a1.z * b[1][1] * a2.z * b[1][2] * a3.z * b[1][3],
        a0.w * b[1][0] + a1.w * b[1][1] * a2.w * b[1][2] * a3.w * b[1][3]
    );
    MufVec4 v2 = mufCreateVec4(
        a0.x * b[2][0] + a1.x * b[2][1] * a2.x * b[2][2] * a3.x * b[2][3],
        a0.y * b[2][0] + a1.y * b[2][1] * a2.y * b[2][2] * a3.y * b[2][3],
        a0.z * b[2][0] + a1.z * b[2][1] * a2.z * b[2][2] * a3.z * b[2][3],
        a0.w * b[2][0] + a1.w * b[2][1] * a2.w * b[2][2] * a3.w * b[2][3]
    );
    MufVec4 v3 = mufCreateVec4(
        a0.x * b[3][0] + a1.x * b[3][1] * a2.x * b[3][2] * a3.x * b[3][3],
        a0.y * b[3][0] + a1.y * b[3][1] * a2.y * b[3][2] * a3.y * b[3][3],
        a0.z * b[3][0] + a1.z * b[3][1] * a2.z * b[3][2] * a3.z * b[3][3],
        a0.w * b[3][0] + a1.w * b[3][1] * a2.w * b[3][2] * a3.w * b[3][3]
    );

    MufMat4 result;
    mufMat4Loadv(&result, v0, v1, v2, v3);
    return result;
}

MufMat2 mufMat2Scale(const MufMat2 *mat, muf_f32 s) {
    const muf_f32 (*m)[2] = mat->values;

    return mufCreateMat2(m[0][0] * s, m[0][1] * s, m[1][0] * s, m[1][1] * s);
}

MufMat3 mufMat3Scale(const MufMat3 *mat, muf_f32 s) {
    const muf_f32 (*m)[3] = mat->values;

    return mufCreateMat3(
        m[0][0] * s, m[0][1] * s, m[0][2] * s,
        m[1][0] * s, m[1][1] * s, m[1][2] * s,
        m[2][0] * s, m[2][1] * s, m[2][2] * s
    );
}

MufMat4 mufMat4Scale(const MufMat4 *mat, muf_f32 s) {
    const muf_f32 (*m)[4] = mat->values;

    return mufCreateMat4(
        m[0][0] * s, m[0][1] * s, m[0][2] * s, m[0][3] * s,
        m[1][0] * s, m[1][1] * s, m[1][2] * s, m[1][3] * s,
        m[2][0] * s, m[2][1] * s, m[2][2] * s, m[2][3] * s,
        m[3][0] * s, m[3][1] * s, m[3][2] * s, m[3][3] * s
    );
}

MufMat2 mufMat2Inverse(const MufMat2 *mat) {
    const muf_f32 (*m)[2] = mat->values;
    muf_f32 det = mufMat2Determinant(mat);
    MUF_FASSERT(det != 0.0F, "The matrix is not invertible");
    det = 1.0f / det;
    
    return mufCreateMat2(det * m[1][1], -det * m[0][1], -det * m[1][0], det * m[0][0]);
}

MufMat3 mufMat3Inverse(const MufMat3 *mat) {
    MufMat3 result;
    muf_f32 (*r)[3] = result.values;
    const muf_f32 (*m)[3] = mat->values;
    r[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    r[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
    r[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
    r[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
    r[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
    r[1][2] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
    r[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
    r[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
    r[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

    muf_f32 det = mufMat3Determinant(mat);
    MUF_FASSERT(det != 0.0F, "The matrix is not invertible");
    det = 1.0F / det;

    return mufMat3Scale(&result, det);
}

MufMat4 mufMat4Inverse(const MufMat4 *mat) {
    MufMat4 result;
    muf_f32 (*r)[4] = result.values;
    const muf_f32 (*m)[4] = mat->values;

    r[0][0] = m[1][1] * m[2][2] * m[3][3] -
        m[1][1] * m[2][3] * m[3][2] -
        m[2][1] * m[1][2] * m[3][3] +
        m[2][1] * m[1][3] * m[3][2] +
        m[3][1] * m[1][2] * m[2][3] -
        m[3][1] * m[1][3] * m[2][2];

    r[1][0] = -m[1][0] * m[2][2] * m[3][3] + 
        m[1][0] * m[2][3] * m[3][2] +
        m[2][0] * m[1][2] * m[3][3] -
        m[2][0] * m[1][3] * m[3][2] -
        m[3][0] * m[1][2] * m[2][3] +
        m[3][0] * m[1][3] * m[2][2];

    r[2][0] = m[1][0] * m[2][1] * m[3][3] - 
        m[1][0] * m[2][3] * m[3][1] -
        m[2][0] * m[1][1] * m[3][3] +
        m[2][0] * m[1][3] * m[3][1] +
        m[3][0] * m[1][1] * m[2][3] -
        m[3][0] * m[1][3] * m[2][1];

    r[3][0] = -m[1][0] * m[2][1] * m[3][2] + 
        m[1][0] * m[2][2] * m[3][1] +
        m[2][0] * m[1][1] * m[3][2] -
        m[2][0] * m[1][2] * m[3][1] -
        m[3][0] * m[1][1] * m[2][2] +
        m[3][0] * m[1][2] * m[2][1];

    r[0][1] = -m[0][1] * m[2][2] * m[3][3] + 
        m[0][1] * m[2][3] * m[3][2] +
        m[2][1] * m[0][2] * m[3][3] -
        m[2][1] * m[0][3] * m[3][2] -
        m[3][1] * m[0][2] * m[2][3] +
        m[3][1] * m[0][3] * m[2][2];

    r[1][1] = m[0][0] * m[2][2] * m[3][3] -
        m[0][0] * m[2][3] * m[3][2] -
        m[2][0] * m[0][2] * m[3][3] +
        m[2][0] * m[0][3] * m[3][2] +
        m[3][0] * m[0][2] * m[2][3] -
        m[3][0] * m[0][3] * m[2][2];

    r[2][1] = -m[0][0] * m[2][1] * m[3][3] + 
        m[0][0] * m[2][3] * m[3][1] +
        m[2][0] * m[0][1] * m[3][3] -
        m[2][0] * m[0][3] * m[3][1] -
        m[3][0] * m[0][1] * m[2][3] +
        m[3][0] * m[0][3] * m[2][1];

    r[3][1] = m[0][0] * m[2][1] * m[3][2] - 
        m[0][0] * m[2][2] * m[3][1] -
        m[2][0] * m[0][1] * m[3][2] +
        m[2][0] * m[0][2] * m[3][1] +
        m[3][0] * m[0][1] * m[2][2] -
        m[3][0] * m[0][2] * m[2][1];

    r[0][2] = m[0][1] * m[1][2] * m[3][3] - 
        m[0][1] * m[1][3] * m[3][2] -
        m[1][1] * m[0][2] * m[3][3] +
        m[1][1] * m[0][3] * m[3][2] +
        m[3][1] * m[0][2] * m[1][3] -
        m[3][1] * m[0][3] * m[1][2];

    r[1][2] = -m[0][0] * m[1][2] * m[3][3] + 
        m[0][0] * m[1][3] * m[3][2] +
        m[1][0] * m[0][2] * m[3][3] -
        m[1][0] * m[0][3] * m[3][2] -
        m[3][0] * m[0][2] * m[1][3] +
        m[3][0] * m[0][3] * m[1][2];

    r[2][2] = m[0][0] * m[1][1] * m[3][3] -
        m[0][0] * m[1][3] * m[3][1] -
        m[1][0] * m[0][1] * m[3][3] +
        m[1][0] * m[0][3] * m[3][1] +
        m[3][0] * m[0][1] * m[1][3] -
        m[3][0] * m[0][3] * m[1][1];

    r[3][2] = -m[0][0] * m[1][1] * m[3][2] + 
        m[0][0] * m[1][2] * m[3][1] +
        m[1][0] * m[0][1] * m[3][2] -
        m[1][0] * m[0][2] * m[3][1] -
        m[3][0] * m[0][1] * m[1][2] +
        m[3][0] * m[0][2] * m[1][1];

    r[0][3] = -m[0][1] * m[1][2] * m[2][3] + 
        m[0][1] * m[1][3] * m[2][2] +
        m[1][1] * m[0][2] * m[2][3] -
        m[1][1] * m[0][3] * m[2][2] -
        m[2][1] * m[0][2] * m[1][3] +
        m[2][1] * m[0][3] * m[1][2];

    r[1][3] = m[0][0] * m[1][2] * m[2][3] - 
        m[0][0] * m[1][3] * m[2][2] -
        m[1][0] * m[0][2] * m[2][3] +
        m[1][0] * m[0][3] * m[2][2] +
        m[2][0] * m[0][2] * m[1][3] -
        m[2][0] * m[0][3] * m[1][2];

    r[2][3] = -m[0][0] * m[1][1] * m[2][3] + 
        m[0][0] * m[1][3] * m[2][1] +
        m[1][0] * m[0][1] * m[2][3] -
        m[1][0] * m[0][3] * m[2][1] -
        m[2][0] * m[0][1] * m[1][3] +
        m[2][0] * m[0][3] * m[1][1];

    r[3][3] = m[0][0] * m[1][1] * m[2][2] - 
        m[0][0] * m[1][2] * m[2][1] -
        m[1][0] * m[0][1] * m[2][2] +
        m[1][0] * m[0][2] * m[2][1] +
        m[2][0] * m[0][1] * m[1][2] -
        m[2][0] * m[0][2] * m[1][1];

    muf_f32 det = m[0][0] * r[0][0] + m[0][1] * r[1][0] + m[0][2] * r[2][0] + m[0][3] * r[3][0];
    MUF_FASSERT(det != 0.0F, "The matrix is not invertible");

    det = 1.0F / det;
    return mufMat4Scale(&result, det);
}

MufMat2 mufMat2Transpose(const MufMat2 *mat) {
    const muf_f32 (*m)[2] = mat->values;
    
    return mufCreateMat2(m[0][0], m[1][0], m[0][1], m[1][1]);
}

MufMat3 mufMat3Transpose(const MufMat3 *mat) {
    const muf_f32 (*m)[3] = mat->values;

    return mufCreateMat3(
        m[0][0], m[1][0], m[2][0],
        m[0][1], m[1][1], m[2][1],
        m[0][2], m[1][2], m[2][2]
    );
}

MufMat4 mufMat4Transpose(const MufMat4 *mat) {
    const muf_f32 (*m)[4] = mat->values;

    return mufCreateMat4(
        m[0][0], m[1][0], m[2][0], m[3][0],
        m[0][1], m[1][1], m[2][1], m[3][1],
        m[0][2], m[1][2], m[2][2], m[3][2],
        m[0][3], m[1][3], m[2][3], m[3][3]
    );
}

muf_f32 mufMat2Determinant(const MufMat2 *mat) {
    const muf_f32 (*m)[2] = mat->values;

    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

muf_f32 mufMat3Determinant(const MufMat3 *mat) {
    const muf_f32 (*m)[3] = mat->values;

    return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) - 
        m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) + 
        m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]
    );
}

muf_f32 mufMat4Determinant(const MufMat4 *mat) {
    const muf_f32 (*m)[4] = mat->values;

    muf_f32 f0 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    muf_f32 f1 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    muf_f32 f2 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    muf_f32 f3 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    muf_f32 f4 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    muf_f32 f5 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

    MufVec4 vec = mufCreateVec4(m[1][1] * f0 - m[1][2] * f1 + m[1][3] * f2,
        -(m[1][0] * f0 - m[1][2] * f3 + m[1][3] * f4),
        m[1][0] * f1 - m[1][1] * f3 + m[1][3] * f5,
        m[1][0] * f2 - m[1][1] * f4 + m[1][2] * f5
    );
    const muf_f32 *v = &vec.x;

    return m[0][0] * v[0] + m[0][1] * v[1] +
        m[0][2] * v[2] + m[0][3] * v[3];
}

MufVec2 mufVec2MulMat(MufVec2 vec, const MufMat2 *mat) {
    const muf_f32 (*m)[2] = mat->values;
    const muf_f32 *v = &vec.x;
    return mufCreateVec2(
        v[0] * m[0][0] + v[1] * m[0][1],
        v[0] * m[1][0] + v[1] * m[1][1]
    );
}

MufVec3 mufVec3MulMat(MufVec3 vec, const MufMat3 *mat) {
    const muf_f32 (*m)[3] = mat->values;
    const muf_f32 *v = &vec.x;
    return mufCreateVec3(
        v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2],
        v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2],
        v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2]
    );
}

MufVec4 mufVec4MulMat(MufVec4 vec, const MufMat4 *mat) {
    const muf_f32 (*m)[4] = mat->values;
    const muf_f32 *v = &vec.x;
    return mufCreateVec4(
        v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2] + v[3] * m[0][3],
        v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2] + v[3] * m[1][3],
        v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2] + v[3] * m[2][3],
        v[0] * m[3][0] + v[1] * m[3][1] + v[2] * m[3][2] + v[3] * m[3][3]
    );
}

MufVec2 mufMat2MulVec(const MufMat2 *mat, MufVec2 vec) {
    const muf_f32 (*m)[2] = mat->values;
    const muf_f32 *v = &vec.x;
    return mufCreateVec2(
        m[0][0] * v[0] + m[1][0] * v[1],
        m[0][1] * v[0] + m[1][1] * v[1]
    );
}

MufVec3 mufMat3MulVec(const MufMat3 *mat, MufVec3 vec) {
    const muf_f32 (*m)[3] = mat->values;
    const muf_f32 *v = &vec.x;
    return mufCreateVec3(
        m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2],
        m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2],
        m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2]
    );
}

MufVec4 mufMat4MulVec(const MufMat4 *mat, MufVec4 vec) {
    const muf_f32 (*m)[4] = mat->values;
    const muf_f32 *v = &vec.x;
    return mufCreateVec4(
        m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3],
        m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3],
        m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3],
        m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3]
    );
}

MufMat4 *mufRotateX(MufMat4 *base, muf_f32 rotation) {
    const muf_f32 cost = cosf(rotation);
    const muf_f32 sint = sinf(rotation);
    MufMat4 transform = mufCreateMat4(
        1.0F, 0.0F, 0.0F, 0.0F,
        0.0F, cost, sint, 0.0F,
        0.0F, -sint, cost, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufRotateY(MufMat4 *base, muf_f32 rotation) {
    const muf_f32 cost = cosf(rotation);
    const muf_f32 sint = sinf(rotation);
    MufMat4 transform = mufCreateMat4(
        cost, 0.0F, -sint, 0.0F,
        0.0F, 1.0F, 0.0F, 0.0F,
        sint, 0.0F, cost, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufRotateZ(MufMat4 *base, muf_f32 rotation) {
    const muf_f32 cost = cosf(rotation);
    const muf_f32 sint = sinf(rotation);
    MufMat4 transform = mufCreateMat4(
        cost, sint, 0.0F, 0.0F,
        -sint, cost, 0.0F, 0.0F,
        0.0F, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufRotate(MufMat4 *base, MufVec3 axis, muf_f32 rotation) {
    MufVec3 n = mufVec3Normalize(axis);
    const muf_f32 cost = cosf(rotation);
    const muf_f32 sint = sinf(rotation);
    const muf_f32 fx = n.x * (1.0F - cost);
    const muf_f32 fy = n.y * (1.0F - cost);
    const muf_f32 fz = n.z * (1.0F - cost);
    
    MufMat4 transform;
    muf_f32 (*r)[4] = transform.values;
    r[0][0] = fx * n.x + cost;
    r[0][1] = fx * n.y + sint * n.z;
    r[0][2] = fx * n.z - sint * n.y;
    r[0][3] = 0.0F;

    r[1][0] = fy * n.x - sint * n.z;
    r[1][1] = fy * n.y + cost;
    r[1][2] = fy * n.z + sint * n.x;
    r[1][3] = 0.0F;

    r[2][0] = fz * n.x + sint * n.y;
    r[2][1] = fz * n.y - sint * n.x;
    r[2][2] = fz * n.z + cost;
    r[2][3] = 0.0F;

    r[3][0] = r[3][1] = r[3][2] = 0.0F;
    r[3][3] = 1.0F;

    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufScale(MufMat4 *base, MufVec3 scales) {
    MufMat4 transform = mufCreateMat4(
        scales.x, 0.0F, 0.0F, 0.0F,
        0.0F, scales.y, 0.0F, 0.0F,
        0.0F, 0.0F, scales.z, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufScaleAxis(MufMat4 *base, MufVec3 axis, muf_f32 factor) {
    MufVec3 n = mufVec3Normalize(axis);
    const muf_f32 fx = n.x * (factor - 1.0F);
    const muf_f32 fy = n.y * (factor - 1.0F);
    const muf_f32 fz = n.z * (factor - 1.0F);

    MufMat4 transform;
    muf_f32 (*t)[4] = transform.values;

    t[0][0] = fx * n.x + 1.0F;
    t[0][1] = fy * n.x;
    t[0][2] = fz * n.x;
    t[0][3] = 0.0F;

    t[1][0] = fx * n.y;
    t[1][1] = fy * n.y + 1.0F;
    t[1][2] = fz * n.y;
    t[1][3] = 0.0F;

    t[2][0] = fx * n.z;
    t[2][1] = fz * n.z;
    t[2][2] = fz * n.z + 1.0F;
    t[2][3] = 0.0F;

    t[3][0] = t[3][1] = t[3][2] = 0.0F;
    t[3][3] = 1.0F;

    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufTranslate(MufMat4 *base, MufVec3 v) {
    MufMat4 transform = mufCreateMat4(
        1.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 1.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 1.0F, 0.0F,
        v.x, v.y, v.z, 1.0F
    );
    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufShearX(MufMat4 *base, muf_f32 s, muf_f32 t) {
    MufMat4 transform = mufCreateMat4(
        1.0F, 0.0F, 0.0F, 0.0F,
        s, 1.0F, 0.0F, 0.0F,
        t, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufShearY(MufMat4 *base, muf_f32 s, muf_f32 t) {
    MufMat4 transform = mufCreateMat4(
        1.0F, s, 0.0F, 0.0F,
        0.0F, 1.0F, 0.0F, 0.0F,
        0.0F, t, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufShearZ(MufMat4 *base, muf_f32 s, muf_f32 t) {
    MufMat4 transform = mufCreateMat4(
        1.0F, 0.0F, s, 0.0F,
        0.0F, 1.0F, t, 0.0F,
        0.0F, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
    MufMat4 result = mufMat4Mul(&transform, base);
    mufMat4Load(base, (muf_f32 *) result.values, 16);
    return base;
}

MufMat4 *mufMirror(MufMat4 *base, MufVec3 axis) {
    return mufScaleAxis(base, axis, -1.0F);
}

MufVec3 mufVec3Transform(MufVec3 vec, const MufMat4 *transform) {
    MufVec4 result = mufMat4MulVec(transform, (MufVec4) { vec.x, vec.y, vec.z, 0.0F });
    return mufCreateVec3(result.x, result.y, result.z);
}

MufVec4 mufVec4Transform(MufVec4 vec, const MufMat4 *transform) {
    return mufMat4MulVec(transform, vec);
}