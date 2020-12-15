cmake_minimum_required(VERSION 3.0.0)
project(ReccordionC C)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_BUILD_TYPE_STR "Release")
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_BUILD_TYPE_STR "Debug")
else()
    message(FATAL_ERROR "Please set CMAKE_BUILD_TYPE to <Debug|Release>")
endif()

# ----------------------------------------------------------------------------------
# TARGET & THIRDPARTY
# ----------------------------------------------------------------------------------
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(ReccordionC ${HEADERS} ${SOURCES})
target_link_libraries(ReccordionC
    PRIVATE Reccordion
)
# ----------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------
# MACROS
# ----------------------------------------------------------------------------------
set(VIDEO_OUTPUT_DIR "\"${CMAKE_SOURCE_DIR}/output/\"")
target_compile_definitions(ReccordionC
    PRIVATE RECCORDION_C_VIDEO_DIR=${VIDEO_OUTPUT_DIR}
    PRIVATE RECCORDION_IMPORT_DYNAMIC
)
set_target_properties(ReccordionC
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
# ----------------------------------------------------------------------------------