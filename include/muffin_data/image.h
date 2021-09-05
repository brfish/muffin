#ifndef _MUFFIN_DATA_IMAGE_H_
#define _MUFFIN_DATA_IMAGE_H_

#include "muffin_core/common.h"

typedef struct MufImage_s {
    muf_u32     width;
    muf_u32     height;
    muf_u32     channels;
    muf_rawptr  *data;
} MufImage;

MufImage *mufCreateImage(muf_u32 width, muf_u32 height);
MufImage *mufCreateImageFromFile(const muf_char *filePath, muf_bool flipVertically);
void mufDestroyImage(MufImage *image);

muf_u32 mufImageGetWidth(const MufImage *image);
muf_u32 mufImageGetHeight(const MufImage *image);
muf_u32 mufImageGetChannels(const MufImage *image);
muf_rawptr mufImageGetData(MufImage *image);


#endif