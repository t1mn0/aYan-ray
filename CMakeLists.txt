cmake_minimum_required(VERSION 3.15)
project(Ayan VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#option(AYAN_BUILD_TESTS "Build tests" ON)
#option(AYAN_BUILD_EXAMPLES "Build examples" ON)
#option(AYAN_USE_SANITIZERS "Enable sanitizers" OFF)
#option(DEBUG_MODE "Enable debug mode" OFF)

#if (DEBUG_MODE)
#    add_compile_definitions(Ayan PUBLIC DEBUG)
#    message(STATUS "Debug mode enabled")
#endif()
#

add_library(Ayan)

target_sources(Ayan PRIVATE
    example.cpp
)

# Temp!
add_executable(example
    example.cpp
)
target_link_libraries(example PRIVATE Ayan)

target_include_directories(Ayan PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_compile_definitions(Ayan PUBLIC
    PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}"
)

target_include_directories(Ayan INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(src/math)

target_link_libraries(Ayan PUBLIC AyanMath)
