add_library(AyanRayLogger STATIC)

target_sources(AyanRayLogger PRIVATE
    Logger.cpp
)

target_include_directories(AyanRayLogger PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../first_party/include>
    $<INSTALL_INTERFACE:include>
)

add_library(AyanRay::Logger ALIAS AyanRayLogger)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../first_party/include/
    DESTINATION include
)

install(TARGETS AyanRayLogger
    EXPORT AyanRayTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
