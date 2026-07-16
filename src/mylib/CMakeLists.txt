
cmake_policy(SET CMP0207 NEW)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(SOURCES
    something.cpp
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

add_library(mylib SHARED)

target_sources(mylib PRIVATE ${SOURCES})
target_include_directories(mylib BEFORE PRIVATE ${INCLUDE_DIRS})

install(TARGETS mylib LIBRARY DESTINATION .)

if(WIN32)
    install(FILES $<TARGET_PDB_FILE:mylib> DESTINATION . OPTIONAL)
endif()
