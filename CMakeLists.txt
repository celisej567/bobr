cmake_minimum_required( VERSION 4.0 )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project( bobr )

set(BOBR_INSTALL_DIR ${CMAKE_SOURCE_DIR}/_BUILD)

set(CMAKE_INSTALL_PREFIX ${BOBR_INSTALL_DIR})

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory( src/launch )
add_subdirectory( src/mylib )
add_subdirectory( src/inputmanager )

# change this behaviour later
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${BOBR_INSTALL_DIR} OPTIONAL)