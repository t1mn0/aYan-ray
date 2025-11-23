add_library(Ayan)

target_link_libraries(Ayan PRIVATE AyanHeaders)

add_executable(examples
    example.cpp
)

target_link_libraries(examples PRIVATE Ayan)

#add_subdirectory(core)
add_subdirectory(math)
#add_subdirectory(sync)
#add_subdirectory(config)
#add_subdirectory(logger)
