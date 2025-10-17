cmake_minimum_required(VERSION 3.15)
project(AyanRay VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(AYANRAY_BUILD_TESTS "Build tests" ON)
option(AYANRAY_BUILD_EXAMPLES "Build examples" ON)
option(AYANRAY_USE_SANITIZERS "Enable sanitizers" OFF)
option(AYANRAY_USE_SSE "Enable SSE optimizations" ON)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "..." FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# TODO : include(CompilerWarnings) and set it

if(AYANRAY_USE_SANITIZERS)
    #include(sanitizers)
    #enable_sanitizers()
endif()

# SSE optimizations for math
if(AYANRAY_USE_SSE AND (CMAKE_SYSTEM_PROCESSOR MATCHES "x86" OR CMAKE_SYSTEM_PROCESSOR MATCHES "amd64"))
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -mfpmath=sse")
    endif()
endif()

add_subdirectory(src)

if(AYANRAY_BUILD_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()
    add_subdirectory(tests)
endif()

if(AYANRAY_BUILD_EXAMPLES)
    #add_subdirectory(examples)
endif()

install(DIRECTORY include/ DESTINATION include)
install(TARGETS AyanRay
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

message(STATUS "AyanRay Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build tests: ${AYANRAY_BUILD_TESTS}")
message(STATUS "  Use sanitizers: ${AYANRAY_USE_SANITIZERS}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
