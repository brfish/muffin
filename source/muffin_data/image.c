#include "muffin_data/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "muffin_core/memory.h"

MufImage *mufCreateImage(muf_u32 width, muf_u32 height) {
    MufImage *image = mufAlloc(MufImage, 1);
    image->width = width;
    image->height = height;
    image->channels = 0;
    image->data = mufAllocBytes(image->width * image->height);
    return image;
}

MufImage *mufCreateImageFromFile(const muf_char *filePath, muf_bool flipVertically) {
    MufImage *image = mufAlloc(MufImage, 1);
    muf_i32 w, h, c;
    stbi_set_flip_vertically_on_load(flipVertically);
    //muf_rawptr data = stbi_load(filePath, &w, &h, &c, 0);
    muf_rawptr data = stbi_loadf(filePath, &w, &h, &c, 0);
    image->width = w;
    image->height = h;
    image->channels = c;
    image->data = data;
    return image;
}

void mufDestroyImage(MufImage *image) {
    stbi_image_free(image->data);
    mufFree(image);
}

muf_u32 mufImageGetWidth(const MufImage *image) {
    return image->width;
}

muf_u32 mufImageGetHeight(const MufImage *image) {
    return image->height;
}

muf_u32 mufImageGetChannels(const MufImage *image) {
    return image->channels;
}

muf_rawptr mufImageGetData(MufImage *image) {
    return image->data;
}