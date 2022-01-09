set(MUFFIN_CORE_SOURCES
    "internal/hash_table.c"
    "array.c"
    "common.c"
    "dict.c"
    "hash_map.c"
    "hash_set.c"
    "hash.c"
    "log.c"
    "math.c"
    "memory.c"
    "module.c"
    "string.c"
)

add_library(muffin_core STATIC ${MUFFIN_CORE_SOURCES})
add_library(muffin::core ALIAS muffin_core)
target_link_libraries(muffin_core muffin::common_rules)