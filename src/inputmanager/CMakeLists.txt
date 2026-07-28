
cmake_policy(SET CMP0207 NEW)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(SOURCES
    SDL3InputManager.cpp
)

set(INCLUDE_DIRS
    .
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/src/public
    ${PROJECT_SOURCE_DIR}/src/public/inputmanager
    ${PROJECT_SOURCE_DIR}/thirdparty/SDL3-VC/include
)

set(CMAKE_CXX_STANDARD 23)

if(WIN32)
	set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" ${PROJECT_SOURCE_DIR}/thirdparty/SDL3-VC)
endif()

find_package(SDL3 REQUIRED)

if(LINUX)
#    add_compile_options("-fsanitize=address")
#    add_link_options("-lasan")
endif()

add_library(inputmanager SHARED)

target_link_libraries(inputmanager PRIVATE SDL3::SDL3)

target_sources(inputmanager PRIVATE ${SOURCES})
target_include_directories(inputmanager BEFORE PRIVATE ${INCLUDE_DIRS})

install(TARGETS inputmanager 
        LIBRARY DESTINATION .
        RUNTIME DESTINATION .)

if(WIN32)
    install(FILES $<TARGET_PDB_FILE:inputmanager> DESTINATION . OPTIONAL)
endif()