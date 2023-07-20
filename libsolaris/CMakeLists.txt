# libsolaris source files
file(GLOB SOLARIS_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/*.h
        ${CMAKE_CURRENT_LIST_DIR}/ephemeris/*.c
        ${CMAKE_CURRENT_LIST_DIR}/ephemeris/*.h
        ${CMAKE_CURRENT_LIST_DIR}/utility/*.c
        ${CMAKE_CURRENT_LIST_DIR}/utility/*.h)

add_library(libsolaris ${SOLARIS_SOURCE_LIST})
target_include_directories(libsolaris PUBLIC ${CMAKE_SOURCE_DIR})

if (WIN32)
    target_compile_definitions(libsolaris PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif ()