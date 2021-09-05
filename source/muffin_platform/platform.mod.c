#include "muffin_platform/platform.mod.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

//#include "muffin_core/log.h"
#include "muffin_core/string.h"
#include "muffin_platform/window.h"
#include "muffin_render/backend.h"

void _load(void) {
    glfwInit();

    MufRenderBackend backend = mufGetDefaultRenderBackend();
    
    if (mufIsNullHandle(backend)) {
        //mufError("No available render backend");
        return;
    }

    const muf_char *backendName = mufRenderBackendGetName(backend);

    if (mufCStrCompare(backendName, "opengl-core-4.5") == 0) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
}

void _unload(void) {
    glfwTerminate();
}

MUF_MODULE_REGISTRY_DEF(Platform,
    "MF_platform",
    "",
    mufCreateVersion(1, 0, 0),
    0,
    _load,
    _unload
);