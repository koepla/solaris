cmake_minimum_required(VERSION 3.22)
project(solaris VERSION 0.0.1 DESCRIPTION "Solaris Library" LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(SOLARIS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

option(BUILD_SHARED_LIBS "build as dynamic library" OFF)

add_subdirectory(libsolaris)
