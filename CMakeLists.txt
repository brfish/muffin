cmake_minimum_required(VERSION 3.11)

project(MuffinEngine VERSION 0.1)

set(CMAKE_C_STANDARD 99)

set(MUFFIN_COMPILE_OPTIONS "-Wall")

set(MUFFIN_ROOT_DIR    "${CMAKE_SOURCE_DIR}")
set(MUFFIN_INCLUDE_DIR "${MUFFIN_ROOT_DIR}/include")
set(MUFFIN_SOURCE_DIR  "${MUFFIN_ROOT_DIR}/source")
set(MUFFIN_EXTERN_DIR  "${MUFFIN_ROOT_DIR}/extern")

set(MUFFIN_EXTERN_GLAD_SOURCES
    "extern/glad/src/glad.c"
)
set(MUFFIN_EXTERN_GLAD_INCLUDES
    "extern/glad/include"
)

set(MUFFIN_CORE_SOURCES
    "source/muffin_core/internal/hash_table.c"
    "source/muffin_core/array.c"
    "source/muffin_core/common.c"
    "source/muffin_core/dict.c"
    "source/muffin_core/hash_map.c"
    "source/muffin_core/hash_set.c"
    "source/muffin_core/hash.c"
    "source/muffin_core/log.c"
    "source/muffin_core/math.c"
    "source/muffin_core/module.c"
    "source/muffin_core/string.c"
)

set(MUFFIN_DATA_SOURCES
    "source/muffin_data/image.c"
)

set(MUFFIN_RENDER_SOURCES
    "source/muffin_render/internal/backend_manager.c"
    "source/muffin_render/backend.c"
    "source/muffin_render/commands.c"
    "source/muffin_render/pipeline.c"
    "source/muffin_render/resources.c"
    "source/muffin_render/render.mod.c"
    "source/muffin_render/backends/gl_backend.c"
)

set(MUFFIN_PLATFORM_SOURCES
    "source/muffin_platform/glfw/monitor.c"
    "source/muffin_platform/glfw/window.c"
    "source/muffin_platform/platform.mod.c"
)

set(MUFFIN_TEST_SOURCES
    "source/muffin_test/test.mod.c"
)

add_library(glad STATIC ${MUFFIN_EXTERN_GLAD_SOURCES})
add_library(glad::glad ALIAS glad)
target_include_directories(glad PRIVATE ${MUFFIN_EXTERN_GLAD_INCLUDES})

add_subdirectory(extern/glfw)
set(GLFW_BUILD_EXAMPLES Off)
set(GLFW_BUILD_TESTS Off)
set(GLFW_BUILD_DOCS Off)

add_library(muffin_core STATIC ${MUFFIN_CORE_SOURCES})
add_library(muffin::core ALIAS muffin_core)
target_include_directories(muffin_core PRIVATE ${MUFFIN_INCLUDE_DIR})
target_compile_options(muffin_core PRIVATE ${MUFFIN_COMPILE_OPTIONS})

add_library(muffin_data STATIC ${MUFFIN_DATA_SOURCES})
add_library(muffin::data ALIAS muffin_data)
target_include_directories(muffin_data PRIVATE ${MUFFIN_INCLUDE_DIR})
#target_compile_definitions(muffin_data PRIVATE ${MUFFIN_COMPILE_OPTIONS})
target_link_libraries(muffin_data PRIVATE muffin::core)
target_include_directories(muffin_data PRIVATE "${MUFFIN_EXTERN_DIR}/stb/include")

add_library(muffin_render STATIC ${MUFFIN_RENDER_SOURCES})
add_library(muffin::render ALIAS muffin_render)
target_include_directories(muffin_render PRIVATE ${MUFFIN_INCLUDE_DIR})
target_link_libraries(muffin_render PUBLIC glad::glad)
target_include_directories(muffin_render PRIVATE ${MUFFIN_EXTERN_GLAD_INCLUDES})
target_compile_options(muffin_render PRIVATE ${MUFFIN_COMPILE_OPTIONS})

add_library(muffin_platform STATIC ${MUFFIN_PLATFORM_SOURCES})
add_library(muffin::platform ALIAS muffin_platform)
target_link_libraries(muffin_platform PRIVATE muffin::core muffin::render)
target_link_libraries(muffin_platform PRIVATE glfw)
target_include_directories(muffin_platform PRIVATE ${MUFFIN_INCLUDE_DIR})
target_include_directories(muffin_platform PRIVATE ${MUFFIN_EXTERN_GLAD_INCLUDES})
target_compile_options(muffin_platform PRIVATE ${MUFFIN_COMPILE_OPTIONS})

add_library(muffin_test STATIC ${MUFFIN_TEST_SOURCES})
add_library(muffin::test ALIAS muffin_test)
target_link_libraries(muffin_test PRIVATE  muffin::core)
target_include_directories(muffin_test PRIVATE ${MUFFIN_INCLUDE_DIR})

add_subdirectory(test)