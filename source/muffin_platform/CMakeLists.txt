set(MUFFIN_PLATFORM_SOURCES
    "glfw/monitor.c"
    "glfw/window.c"
    "win32/dlib.c"
    "win32/io.c"
    "win32/time.c"
    "platform.mod.c"
)

add_library(muffin_platform STATIC ${MUFFIN_PLATFORM_SOURCES})
add_library(muffin::platform ALIAS muffin_platform)

target_link_libraries(muffin_platform muffin::common_rules)
target_link_libraries(muffin_platform 
    muffin::core 
    muffin::render 
    glad::glad
    glfw::glfw
)