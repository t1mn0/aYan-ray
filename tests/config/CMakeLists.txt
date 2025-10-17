add_executable(config_test
    ConfigTest.cpp
)

target_link_libraries(config_test
    PRIVATE
    AyanRay::Config
    GTest::gtest
    GTest::gtest_main
)

#target_include_directories(config_test PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
#)

add_test(NAME ConfigTests COMMAND config_test)
