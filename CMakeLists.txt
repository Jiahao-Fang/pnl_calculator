cmake_minimum_required(VERSION 3.10)
project(pnl_calculator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(pnl_calculator ${SOURCES})

# Add Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(test_pnl_calculator
    tests/test_pnl_calculator.cpp
)

target_link_libraries(test_pnl_calculator
    GTest::gtest_main
    GTest::gmock_main
)

# Enable testing
enable_testing()
add_test(NAME pnl_calculator_test COMMAND test_pnl_calculator) 