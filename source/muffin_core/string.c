#include "muffin_core/string.h"

#define MUF_UTF8_ACCEPT 8
#define MUF_UTF8_REJECT 12

static const muf_u8 _utf8LookupTable1[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,
};

static const muf_u8 _utf8LookupTable2[] = {
    0 ,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
    12,0 ,12,12,12,12,12,0 ,12,0 ,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
    12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
    12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
    12,36,12,12,12,12,12,12,12,12,12,12,
};

MUF_INTERNAL MUF_INLINE muf_u32 _mufUtf8Decode(muf_byte byte, muf_u32 *state, muf_u32 *codepoint) {
    muf_u32 type = _utf8LookupTable1[byte];
    *codepoint = (*state != MUF_UTF8_ACCEPT) ?
        (byte & 0x3F) | (*codepoint << 6) :
        (0xFF >> type) & (byte);
    *state = _utf8LookupTable2[*state + type];
    return *state;
}

muf_i32 mufConvertUTF8ToUTF16(const muf_char *src, muf_usize srcLength, muf_wchar *dst, muf_usize dstMaxSize) {
    muf_byte *s = (muf_byte *) src;
    muf_byte *srcEnd = s + srcLength;

    muf_wchar *d = dst;
    muf_u32 codepoint;
    muf_u32 state = 0;

    while (s < srcEnd) {
        muf_usize dstFree = dstMaxSize - (d - dst);
        muf_byte *sCurrentEnd = s + dstFree  - 1;

        if (srcEnd < sCurrentEnd)
            sCurrentEnd = srcEnd;
        
        if (sCurrentEnd <= s)
            return MUF_UTF8_TOO_SMALL;
        
        while (s < sCurrentEnd) {
            if (_mufUtf8Decode(*s++, &state, &codepoint))
                continue;
            
            if (codepoint > 0xFFFF) {
                *d++ = (muf_u16)(0xD7C0 + (codepoint >> 10));
                *d++ = (muf_u16)(0xDC00 + (codepoint & 0x3FF));
            } else {
                *d++ = (muf_u16)codepoint;
            }
        }

        if (state != MUF_UTF8_ACCEPT)
            return MUF_UTF8_INVALID;

        if ((dstMaxSize - (d - dst)) == 0)
            return MUF_UTF8_TOO_SMALL;
        
        return MUF_UTF8_SUCCESS;
    }
}

muf_usize mufUTF8Length(const muf_char *str) {
    const muf_byte *bytes = (const muf_byte *) str;
    muf_u32 state = 0;
    muf_u32 codepoint = 0;
    muf_u32 count = 0;

    for (muf_index i = 0; bytes[i]; ++i) {
        _mufUtf8Decode(bytes[i], &state, &codepoint);
        if (state == MUF_UTF8_ACCEPT)
            ++count;
    }
    return count;
}