cmake_minimum_required(VERSION 3.22)
project(solaris VERSION 0.0.1 DESCRIPTION "Solaris Library" LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "build as dynamic library")

# #############################################################################
# LIBSOLARIS CONFIGURATION
# #############################################################################

# libsolaris source files
file(GLOB SOLARIS_SOURCE_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# libsolaris header files
file(GLOB SOLARIS_HEADER_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/include/solaris/*.h)

# declare the library
add_library(${PROJECT_NAME} ${SOLARIS_SOURCE_LIST} ${SOLARIS_HEADER_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${SOLARIS_HEADER_LIST}")

# handle stupid math library on unix
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${MATH_LIBRARY})
endif()

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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
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



# #############################################################################
# DRIVER CONFIGURATION
# #############################################################################

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(DRIVER "${PROJECT_NAME}-driver")
    add_executable(${DRIVER} "${CMAKE_CURRENT_SOURCE_DIR}/driver.c")
    target_link_libraries(${DRIVER} PRIVATE ${PROJECT_NAME})
endif()

# #############################################################################
# DEVELOPMENT CONFIGURATION
# #############################################################################

# compile commands
if(EXISTS "${BUILD_DIRECTORY}/compile_commands.json")
    file(CREATE_LINK "${BUILD_DIRECTORY}/compile_commands.json" "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json")
endif()
