
cmake_policy(SET CMP0207 NEW)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(SOURCES
    important.cpp
    cmd/cmd.cpp
)

set(INCLUDE_DIRS
    .
    ../public
)

set(CMAKE_CXX_STANDARD 23)

if(LINUX)
#    add_compile_options("-fsanitize=address")
#    add_link_options("-lasan")
endif()

add_library(important STATIC)

target_sources(important PRIVATE ${SOURCES})
target_include_directories(important BEFORE PRIVATE ${INCLUDE_DIRS})
