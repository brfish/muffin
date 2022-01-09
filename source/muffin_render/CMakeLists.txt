set(MUFFIN_RENDER_SOURCES
    "backends/gl_backend.c"
    "internal/backend_manager.c"
    "backend.c"
    "commands.c"
    "pipeline.c"
    "render.mod.c"
    "resources.c"
)

add_library(muffin_render STATIC ${MUFFIN_RENDER_SOURCES})
add_library(muffin::render ALIAS muffin_render)

target_link_libraries(muffin_render muffin::common_rules)
target_link_libraries(muffin_render 
    muffin::core 
    glad::glad
)