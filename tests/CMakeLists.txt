# Download GoogleTest if not already available
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prevent gtest from overriding compiler/linker flags
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test Source files
file(GLOB SOLARIS_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create test executable
add_executable(${PROJECT_NAME}_tests
        ${SOLARIS_TEST_SOURCES}
)

# Link C library (written in C) with C++ test runner
target_link_libraries(${PROJECT_NAME}_tests
        PRIVATE
        gtest_main
        ${PROJECT_NAME}
)

# Include both C headers and gtest
target_include_directories(${PROJECT_NAME}_tests PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Register with CTest
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
