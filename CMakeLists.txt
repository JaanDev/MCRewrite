cmake_minimum_required(VERSION 3.5.0)
project(rd-132211)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage("gh:nothings/stb#master")
CPMAddPackage("gh:richgel999/miniz#master")

file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c
    ${CMAKE_SOURCE_DIR}/src/impl/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw glm miniz)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/libs/glad/include        
    ${CMAKE_SOURCE_DIR}/src
    ${stb_SOURCE_DIR}/
)

if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    # file(COPY ${CMAKE_SOURCE_DIR}/assets/terrain.png DESTINATION ${CMAKE_BINARY_DIR}/)
    # file(COPY ${CMAKE_SOURCE_DIR}/assets/terrain.png DESTINATION "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/assets/terrain.png" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/")
else()
    message("-- WARNING: Don't forget to copy resources from .jar file to assets folder!")
endif()