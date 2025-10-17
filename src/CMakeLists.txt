#add_subdirectory(core)
#add_subdirectory(math)
add_subdirectory(sync)
add_subdirectory(config)
add_subdirectory(logger)

add_library(AyanRay INTERFACE)

target_link_libraries(AyanRay INTERFACE
    AyanRay::Config
    AyanRay::Logger
#    AyanRay::Core
#    AyanRay::Math
    AyanRay::Sync
)

install(TARGETS AyanRay
    EXPORT AyanRayTargets
)
