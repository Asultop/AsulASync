cmake_minimum_required(VERSION 3.16)
project(AsulASync VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Set Qt path for MSVC2022
set(CMAKE_PREFIX_PATH "C:/Qt/6.7.3/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Core Test Widgets)

# Library
add_library(AsulASync SHARED
    src/AsulASync.cpp
    include/AsulASync.h
)

target_include_directories(AsulASync PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(AsulASync PUBLIC Qt6::Core)

target_compile_definitions(AsulASync PRIVATE ASULASYNC_LIBRARY)

set_target_properties(AsulASync PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# Example 1
add_executable(Example1 examples/example1/main.cpp)
target_link_libraries(Example1 PRIVATE AsulASync Qt6::Core)

# Example 2
add_executable(Example2 examples/example2/main.cpp)
target_link_libraries(Example2 PRIVATE AsulASync Qt6::Core)

# Example 3 - GUI with ProgressBar
add_executable(Example3 examples/example3/main.cpp)
target_link_libraries(Example3 PRIVATE AsulASync Qt6::Core Qt6::Widgets)

# Test 1 - Basic functionality
add_executable(Test1 tests/test1/main.cpp)
target_link_libraries(Test1 PRIVATE AsulASync Qt6::Core Qt6::Test)

# Test 2 - Multithreading
add_executable(Test2 tests/test2/main.cpp)
target_link_libraries(Test2 PRIVATE AsulASync Qt6::Core Qt6::Test)

# Enable testing
enable_testing()
add_test(NAME BasicFunctionality COMMAND Test1)
add_test(NAME Multithreading COMMAND Test2)
