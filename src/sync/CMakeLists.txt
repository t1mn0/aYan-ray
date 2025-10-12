add_library(AyanRaySync STATIC)

target_sources(AyanRaySync PRIVATE
    mutex/Mutex.cpp
    detail/mutex/LockGuard.cpp
    detail/mutex/UniqueLock.cpp
    condvar/CondVar.cpp
    barrier/Barrier.cpp
)

target_include_directories(AyanRaySync PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    $<INSTALL_INTERFACE:include>
)

add_library(AyanRay::Sync ALIAS AyanRaySync)

install(TARGETS AyanRaySync
    EXPORT AyanRayTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../include/ayan/
    DESTINATION include/ayan
)
