#include "muffin_platform/context.h"

#include "glfw/glfw3.h"

void mufPlatformInitContext() {
    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 5);
}