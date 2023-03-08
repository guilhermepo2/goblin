project(imgui-docking)

add_library(${PROJECT_NAME}
        imconfig.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imgui.cpp
        imgui.h
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        backends/imgui_impl_sdl2.h
        backends/imgui_impl_sdl2.cpp
        backends/imgui_impl_opengl3.h
        backends/imgui_impl_opengl3.cpp
        backends/imgui_impl_opengl3_loader.h
        )

target_include_directories (${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}> )
include_directories(${SDL2_INCLUDE_DIR})