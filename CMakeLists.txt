cmake_minimum_required(VERSION 3.14)

project(numcpp
    VERSION 1.0
    LANGUAGES CXX
)

# Headers
add_library(numcpp INTERFACE)

target_include_directories(numcpp
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(numcpp
    INTERFACE
        cxx_std_20
)

# Tests
enable_testing()

find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/tests/*.cpp"
)

add_executable(run_tests
    ${TEST_SOURCES}
)

target_link_libraries(run_tests
    PRIVATE
        numcpp
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
