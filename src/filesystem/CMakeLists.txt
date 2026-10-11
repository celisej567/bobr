cmake_policy(SET CMP0207 NEW)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(SOURCES
    basicfilesystem.cpp
    loaders/imageloader.cpp
    loaders/objloader.cpp
)

set(INCLUDE_DIRS
    .
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
    ${PROJECT_SOURCE_DIR}/src/public
)

if(LINUX)
#    add_compile_options("-fsanitize=address")
#    add_link_options("-lasan")
endif()

set(CMAKE_CXX_STANDARD 23)

add_library(filesystem SHARED)

# CMD::Msg lives there
target_link_libraries(filesystem PRIVATE manyinterfaces)

target_sources(filesystem PRIVATE ${SOURCES})
target_include_directories(filesystem BEFORE PRIVATE ${INCLUDE_DIRS})

install(TARGETS filesystem
        LIBRARY DESTINATION .
        RUNTIME DESTINATION .)

if(WIN32)
    install(FILES $<TARGET_PDB_FILE:filesystem> DESTINATION . OPTIONAL)
endif()
