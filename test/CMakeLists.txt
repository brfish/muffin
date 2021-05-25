
macro(muffin_test)
    cmake_parse_arguments(
        M_TEST
        ""
        "NAME"
        "SOURCES"
        ${ARGN}
    )
    add_executable(${M_TEST_NAME} ${M_TEST_SOURCES})
    target_link_libraries(${M_TEST_NAME} PRIVATE muffin::core)
    target_include_directories(${M_TEST_NAME} PRIVATE ${MUFFIN_INCLUDE_DIR})
    
endmacro()

muffin_test(
    NAME muffin_core_array_test
    SOURCES "muffin_core/array_test.c"
)

muffin_test(
    NAME muffin_core_hash_set_test
    SOURCES "muffin_core/hash_set_test.c"
)

muffin_test(
    NAME muffin_core_math_test
    SOURCES "muffin_core/math_test.c"
)