add_library(AyanRayConfig STATIC)

target_sources(AyanRayConfig PRIVATE
    Config.cpp
)

target_include_directories(AyanRayConfig PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../first_party/include>
    $<INSTALL_INTERFACE:include>
)

add_library(AyanRay::Config ALIAS AyanRayConfig)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../first_party/include/
    DESTINATION include
)

install(TARGETS AyanRayConfig
    EXPORT AyanRayTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
