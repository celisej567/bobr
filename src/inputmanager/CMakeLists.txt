
cmake_policy(SET CMP0207 NEW)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(SOURCES
    SDL3InputManager.cpp
)

set(INCLUDE_DIRS
    .
    ../public
    ../public/inputmanager
)

set(CMAKE_CXX_STANDARD 23)

if(LINUX)
#    add_compile_options("-fsanitize=address")
#    add_link_options("-lasan")
endif()

add_library(inputmanager SHARED)

target_sources(inputmanager PRIVATE ${SOURCES})
target_include_directories(inputmanager BEFORE PRIVATE ${INCLUDE_DIRS})

install(TARGETS inputmanager LIBRARY DESTINATION .)

if(WIN32)
    install(FILES $<TARGET_PDB_FILE:inputmanager> DESTINATION . OPTIONAL)
endif()
