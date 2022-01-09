cmake_minimum_required(VERSION 3.11)

project(MuffinEngine VERSION 0.1)

set(CMAKE_C_STANDARD 99)

# Environment variables
set(MUFFIN_ROOT_DIR    "${PROJECT_SOURCE_DIR}")
set(MUFFIN_INCLUDE_DIR "${MUFFIN_ROOT_DIR}/include")
set(MUFFIN_SOURCE_DIR  "${MUFFIN_ROOT_DIR}/source")
set(MUFFIN_EXTERN_DIR  "${MUFFIN_ROOT_DIR}/extern")

set(MUFFIN_COMPILE_OPTIONS "-Wall")

# Build external libraries
add_subdirectory(extern)

# An interface target for providing compilation rules
add_library(muffin_common_rules INTERFACE)
add_library(muffin::common_rules ALIAS muffin_common_rules)
target_include_directories(muffin_common_rules INTERFACE "${MUFFIN_INCLUDE_DIR}")
target_compile_options(muffin_common_rules INTERFACE "${MUFFIN_COMPILE_OPTIONS}")

# Build modules
add_subdirectory(source/muffin_core)
add_subdirectory(source/muffin_data)
add_subdirectory(source/muffin_platform)
add_subdirectory(source/muffin_render)

# Build tests
add_subdirectory(test)