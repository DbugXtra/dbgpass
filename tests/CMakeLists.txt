# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Test sources
file(GLOB_RECURSE TEST_SOURCES 
    "unit/*.cpp"
    "integration/*.cpp"
)

# Create test executable
add_executable(password_generator_tests ${TEST_SOURCES})

# Link libraries
target_link_libraries(password_generator_tests
    password_generator_lib
    gtest_main
    gmock
)

# Include directories
target_include_directories(password_generator_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(password_generator_tests)