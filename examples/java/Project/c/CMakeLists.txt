cmake_minimum_required(VERSION 3.0.0)
project(ReccordionJava VERSION 0.1.0)

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
# TARGET & THIRDPARTY
# ----------------------------------------------------------------------------------
find_package(JNI REQUIRED)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(ReccordionJava SHARED ${HEADERS} ${SOURCES})
target_include_directories(ReccordionJava
    PRIVATE ${JNI_INCLUDE_DIRS}
)
target_link_directories(ReccordionJava
    PRIVATE ${JNI_LIBRARIES}
)
target_link_libraries(ReccordionJava
    PRIVATE Reccordion
)
# ----------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------
# MACROS
# ----------------------------------------------------------------------------------
target_compile_definitions(ReccordionJava
    PRIVATE RECCORDION_IMPORT_DYNAMIC 
)
set_target_properties(ReccordionJava
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
# ----------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------
# JAVA
# ----------------------------------------------------------------------------------
add_custom_command(
    TARGET ReccordionJava
    PRE_BUILD
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/generate.cmd
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/
)
add_custom_command(
    TARGET ReccordionJava
    POST_BUILD
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/build.cmd
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/
)
# ----------------------------------------------------------------------------------