cmake_minimum_required(VERSION 3.0.0)
project(Reccordion VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_BUILD_TYPE_STR "Release")
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_BUILD_TYPE_STR "Debug")
else()
    message(FATAL_ERROR "Please set CMAKE_BUILD_TYPE to <Debug|Release>")
endif()

# ----------------------------------------------------------------------------------
# OPTIONS
# ----------------------------------------------------------------------------------
set(BUILD_EXAMPLES TRUE)
# ----------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------
# TARGET & THIRDPARTY
# ----------------------------------------------------------------------------------
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(Reccordion SHARED ${HEADERS} ${SOURCES})
target_include_directories(Reccordion
    PRIVATE ${OpenCV_INCLUDE_DIRS}
)
target_link_directories(Reccordion
    PRIVATE ${OpenCV_LIB_DIR}
)
target_link_libraries(Reccordion
    PRIVATE ${OpenCV_LIBS}
)
# ----------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------
# MACROS
# ----------------------------------------------------------------------------------
target_compile_definitions(Reccordion
    PRIVATE RECCORDION_EXPORT_DYNAMIC 
)
set_target_properties(Reccordion
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
# ----------------------------------------------------------------------------------

if(BUILD_EXAMPLES)
    add_subdirectory(examples/c)
    add_subdirectory(examples/java/Project/c)
    add_custom_command(
        TARGET Reccordion
        POST_BUILD
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/examples/rust/project/scripts/build.cmd
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/rust/project
    )
endif(BUILD_EXAMPLES)
