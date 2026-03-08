cmake_minimum_required( VERSION 4.0 )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project( govno )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_BUILD)
add_subdirectory( launch )