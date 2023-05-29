cmake_minimum_required(VERSION 3.0.0)
project(MCRewrite VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(libs/raylib)
add_subdirectory(libs/fmt)
add_subdirectory(libs/glew/build/cmake)

file(GLOB SOURCES
    src/*.cpp
    libs/rlFPCamera/rlFPCamera.cpp
)

if (UNIX)
    find_package(OpenGL REQUIRED)
    set(GL_INCLUDES ${OPENGL_INCLUDE_DIRS})
    set(GL_LIBS ${OPENGL_LIBRARIES})

else()
    set(GL_INCLUDES "")
    set(GL_LIBS opengl32)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} raylib fmt glew ${GL_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC libs/rlFPCamera libs/glew/include ${GL_INCLUDES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)