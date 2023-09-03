cmake_minimum_required(VERSION 3.22)
project(solaris-lib VERSION 0.0.1 DESCRIPTION "Solaris Library" LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "build as dynamic library" OFF)

add_subdirectory(libsolaris)

if (BUILD_SHARED_LIBS)
    target_link_libraries(solaris PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(solaris PRIVATE SOLARIS_SHARED=1)
    target_compile_definitions(solaris PRIVATE SOLARIS_BUILD=1)
endif ()
