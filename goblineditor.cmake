cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(goblin-editor)

add_subdirectory(src/lib/imgui-docking)

add_executable(${PROJECT_NAME}
        src/editor/GoblinEditor.cpp
        )

target_link_libraries(${PROJECT_NAME} imgui-docking)
target_link_libraries(${PROJECT_NAME} glad)

target_include_directories (${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/lib/imgui-docking>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/lib/gueepo2D/gueepo2D/thirdparty/glad/include>
        )

if(APPLE)
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
endif()