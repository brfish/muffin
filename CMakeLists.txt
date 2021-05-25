cmake_minimum_required(VERSION 3.11)

project(MuffinEngine VERSION 0.1)

set(CMAKE_C_STANDARD 99)

set(MUFFIN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MUFFIN_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(MUFFIN_EXTERN_DIR ${CMAKE_SOURCE_DIR}/extern)

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
    "source/muffin_core/hash_map.c"
    "source/muffin_core/hash_set.c"
    "source/muffin_core/hash.c"
    "source/muffin_core/math.c"
    "source/muffin_core/string.c"
)
if(WIN32)
    list(APPEND MUFFIN_CORE_SOURCES "source/muffin_core/platform/win32/filesystem.c")
elseif(UNIX)
    list(APPEND MUFFIN_CORE_SOURCES "source/muffin_core/platform/linux/filesystem.c")
endif()

set(MUFFIN_RENDER_SOURCES
    "source/muffin_render/backend/gl_backend.c"
    "source/muffin_render/context.c"
)

add_library(glad STATIC ${MUFFIN_EXTERN_GLAD_SOURCES})
add_library(glad::glad ALIAS glad)

add_library(muffin_core STATIC ${MUFFIN_CORE_SOURCES})
add_library(muffin::core ALIAS muffin_core)
target_include_directories(muffin_core PRIVATE ${MUFFIN_INCLUDE_DIR})

add_subdirectory(test)