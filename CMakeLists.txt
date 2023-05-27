cmake_minimum_required(VERSION 3.0.0)
project(MCRewrite VERSION 1.0.0)

add_subdirectory(libs/raylib)

add_library(rlFPCamera STATIC libs/rlFPCamera/rlFPCamera.cpp)
target_include_directories(rlFPCamera PUBLIC libs/rlFPCamera)
target_link_libraries(rlFPCamera raylib)

file(GLOB SOURCES
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} rlFPCamera raylib)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)