cmake_minimum_required(VERSION 3.22)
project(solaris VERSION 0.0.1 DESCRIPTION "Solaris Library" LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build as dynamic library" OFF)

# Enable normalized DESTINATION paths (CMake 3.28+)
if (POLICY CMP0177)
    cmake_policy(SET CMP0177 NEW)
endif ()

# #############################################################################
# LIBSOLARIS CONFIGURATION
# #############################################################################

# Source and header files
file(GLOB SOLARIS_SOURCE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB SOLARIS_HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/solaris/*.h")

# Create library
add_library(${PROJECT_NAME} ${SOLARIS_SOURCE_LIST} ${SOLARIS_HEADER_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER "${SOLARIS_HEADER_LIST}"
)

# Math library (UNIX)
find_library(MATH_LIBRARY m)
if (MATH_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${MATH_LIBRARY})
endif ()

# Shared library settings
if (BUILD_SHARED_LIBS)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE SOLARIS_SHARED=1 SOLARIS_BUILD=1)
endif ()

# Platform-specific defines
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif ()

# Compiler warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra -Wpedantic -Werror
            -Wno-gnu-anonymous-struct -Wno-nested-anon-types
    )
endif ()

# Include directories (modern usage)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Installation
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}Targets"
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/solaris
)

# #############################################################################
# DEVELOPMENT CONFIGURATION
# #############################################################################

# Optional: Symlink compile_commands.json if desired
if (DEFINED BUILD_DIRECTORY AND EXISTS "${BUILD_DIRECTORY}/compile_commands.json")
    file(CREATE_LINK "${BUILD_DIRECTORY}/compile_commands.json"
            "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json")
endif ()

include(CTest)
option(BUILD_TESTING "Enable building tests" ON)

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif ()
