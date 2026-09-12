cmake_minimum_required( VERSION 4.0 )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project( bobr )

set(BOBR_INSTALL_DIR ${CMAKE_SOURCE_DIR}/_BUILD)

set(CMAKE_INSTALL_PREFIX ${BOBR_INSTALL_DIR})

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

option(BOBR_TRACY_PROFILE "Build with Tracy profiler integration (optional)" OFF)

if(BOBR_TRACY_PROFILE)
    message("Tracy enabled!")
endif()

if(MSVC)
    add_compile_options(/MP12)
endif()

add_subdirectory( src/manyinterfaces )
add_subdirectory( src/launch )
if(LINUX)
	add_subdirectory( src/mylib )
endif()
add_subdirectory( src/inputmanager )

# change this behaviour later
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${BOBR_INSTALL_DIR} OPTIONAL)