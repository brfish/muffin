#include "muffin_app/app.h"

#include "muffin_platform/window.h"

MufWindow *_mufWindow = NULL;

const muf_char *mufAppGetTitle() {
    return mufWindowGetTitle(_mufWindow);
}

MufOffset2i mufAppGetPosition() {
    return mufWindowGetPosition(_mufWindow);
}

MufExtent2i mufAppGetSize() {
    return mufWindowGetSize(_mufWindow);
}

muf_u32 mufAppGetWidth() {
    return mufWindowGetWidth(_mufWindow);
}

muf_u32 mufAppGetHeight() {
    return mufWindowGetHeight(_mufWindow);
}

muf_bool mufAppIsVisible() {
    return mufWindowIsVisible(_mufWindow);
}