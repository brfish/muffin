set(MUFFIN_DATA_SOURCES
    "image.c"
)

add_library(muffin_data STATIC ${MUFFIN_DATA_SOURCES})
add_library(muffin::data ALIAS muffin_data)

target_link_libraries(muffin_data muffin::common_rules)
target_link_libraries(muffin_data stb::image muffin::core)