
set(SOURCES
    drazdropermovich.cpp
    ${PROJECT_SOURCE_DIR}/thirdparty/glad/src/glad.c
)

set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/thirdparty/glad/include
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_BUILD)


find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(launch)

target_link_libraries(launch PRIVATE SDL3::SDL3)
target_link_libraries(launch PRIVATE OpenGL::GL)

target_sources(launch PRIVATE ${SOURCES})
target_include_directories(launch PRIVATE ${INCLUDE_DIRS})