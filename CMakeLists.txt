cmake_minimum_required(VERSION 3.10)
project(dbgpass VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3)
    endif()
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE LIB_SOURCES 
    "src/core/*.cpp"
    "src/strategies/*.cpp"
    "src/providers/*.cpp"
    "src/validators/*.cpp"
    "src/utils/*.cpp"
    "src/cli/*.cpp"
)

# Create library
add_library(password_generator_lib STATIC ${LIB_SOURCES})

# Create executable
add_executable(dbgpass src/main.cpp)
target_link_libraries(dbgpass password_generator_lib)

# Testing
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS dbgpass DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/password_generator)
install(TARGETS password_generator_lib DESTINATION lib)

# Package configuration
include(CPack)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})