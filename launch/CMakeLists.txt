
set(SOURCES
    drazdropermovich.cpp
    FileSystemStuff.cpp
    CShader.cpp
    CTexture.cpp
    ${PROJECT_SOURCE_DIR}/thirdparty/glad/src/glad.c
)

set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/thirdparty/glad/include
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)


find_package(SDL3 REQUIRED)
#find_package(OpenGL REQUIRED)

add_compile_options("-fsanitize=address")
add_link_options("-lasan")
add_executable(launch)


target_link_libraries(launch PRIVATE SDL3::SDL3)
#target_link_libraries(launch PRIVATE OpenGL::GL)

target_sources(launch PRIVATE ${SOURCES})
target_include_directories(launch PRIVATE ${INCLUDE_DIRS})