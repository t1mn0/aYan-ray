#add_subdirectory(core)
#add_subdirectory(math)
add_subdirectory(sync)

add_library(AyanRay INTERFACE)

target_link_libraries(AyanRay INTERFACE
#    AyanRay::Core
#    AyanRay::Math
    AyanRay::Sync
)

#target_include_directories(AyanRay INTERFACE
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
#    $<INSTALL_INTERFACE:include>
#)

install(TARGETS AyanRay
    EXPORT AyanRayTargets
)
