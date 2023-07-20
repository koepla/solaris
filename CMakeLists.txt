cmake_minimum_required(VERSION 3.22)
project(solaris VERSION 0.0.1 DESCRIPTION "Solaris Library" LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "build as dynamic library" OFF)
option(SOLARIS_BUILD_EXAMPLE "build the example" ON)

add_subdirectory(libsolaris)

if (BUILD_SHARED_LIBS)
    target_link_libraries(libsolaris PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(libsolaris PRIVATE SOLARIS_SHARED=1)
    target_compile_definitions(libsolaris PRIVATE SOLARIS_BUILD=1)
endif ()

if (SOLARIS_BUILD_EXAMPLE)
    add_subdirectory(examples)
endif ()
