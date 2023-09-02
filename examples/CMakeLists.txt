# libsolaris source files
file(GLOB SOLARIS_EXAMPLES_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/*.h
        ${CMAKE_CURRENT_LIST_DIR}/arena/*.c
        ${CMAKE_CURRENT_LIST_DIR}/arena/*.h
        ${CMAKE_CURRENT_LIST_DIR}/catalog/*.c
        ${CMAKE_CURRENT_LIST_DIR}/catalog/*.h)

add_executable(examples ${SOLARIS_EXAMPLES_SOURCE_LIST})
target_link_libraries(examples PUBLIC "libsolaris")
target_include_directories(examples PUBLIC ${CMAKE_SOURCE_DIR})
