# libsolaris source files
file(GLOB SOLARIS_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/ephemeris/*.c
        ${CMAKE_CURRENT_LIST_DIR}/ephemeris/generated/*.h
        ${CMAKE_CURRENT_LIST_DIR}/utility/*.c)

# libsolaris header files
file(GLOB SOLARIS_HEADER_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.h
        ${CMAKE_CURRENT_LIST_DIR}/ephemeris/*.h
        ${CMAKE_CURRENT_LIST_DIR}/utility/*.h)

# declare the library
add_library(${PROJECT_NAME} STATIC ${SOLARIS_SOURCE_LIST} ${SOLARIS_HEADER_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${SOLARIS_HEADER_LIST}")

# handle shared library
if(BUILD_SHARED_LIBS)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE SOLARIS_SHARED=1)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SOLARIS_BUILD=1)
endif()

# we want to use unsecure functions ;)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif()

# make sure that we, and projects that use us have the correct directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${SOLARIS_ROOT}
    PUBLIC
    $<BUILD_INTERFACE:${SOLARIS_ROOT}>
    $<INSTALL_INTERFACE:${SOLARIS_ROOT}>
)

# the 'install'
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT "${PROJECT_NAME}Targets"
)

# in order to preserve the paths, we copy the headers manually
foreach(HEADER ${SOLARIS_HEADER_LIST})
    file(RELATIVE_PATH HEADER_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}" "${HEADER}")
    get_filename_component(HEADER_DIRECTORY_PATH "${HEADER_FILE_PATH}" DIRECTORY)
    install(
            FILES ${HEADER}
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY_PATH}"
    )
endforeach()
