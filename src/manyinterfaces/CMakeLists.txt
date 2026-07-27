
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

add_library(manyinterfaces STATIC)

target_sources(manyinterfaces PRIVATE ${SOURCES})
target_include_directories(manyinterfaces BEFORE PRIVATE ${INCLUDE_DIRS})
