
cmake_policy(SET CMP0207 NEW)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(SOURCES
    drazdropermovich.cpp
    FileSystemStuff.cpp
    CShader.cpp
    CTexture.cpp
    CCamera.cpp
    CModel.cpp
    entity/CEntity.cpp
    entity/ECS.cpp
    entity/CModelEntity.cpp
    entity/CRotatingModel.cpp
    entity/CGizmos.cpp
    CCameraController.cpp
    assetcache/assetcache.cpp
    window/SDL3Window.cpp


    filesystem/basicfilesystem.cpp

    ${PROJECT_SOURCE_DIR}/thirdparty/glad/src/glad.c
)

set(INCLUDE_DIRS
    .
    ${PROJECT_SOURCE_DIR}/thirdparty/glad/include
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/src/public
)

set(CMAKE_CXX_STANDARD 23)

if(WIN32)
	set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" ${PROJECT_SOURCE_DIR}/thirdparty/SDL3-VC)
endif()

find_package(SDL3 REQUIRED)
#find_package(OpenGL REQUIRED)

if(LINUX)
#    add_compile_options("-fsanitize=address")
#    add_link_options("-lasan")
endif()
add_executable(launch)

target_link_libraries(launch PRIVATE manyinterfaces)

# i whould like to remove sdl3 entirely later.
target_link_libraries(launch PRIVATE SDL3::SDL3)
#target_link_libraries(launch PRIVATE OpenGL::GL)

target_sources(launch PRIVATE ${SOURCES})
target_include_directories(launch BEFORE PRIVATE ${INCLUDE_DIRS})

install(TARGETS launch RUNTIME DESTINATION .)

if(WIN32)
    install(FILES $<TARGET_PDB_FILE:launch> DESTINATION . OPTIONAL)
endif()

    install(TARGETS launch
        RUNTIME_DEPENDENCIES
			DIRECTORIES $<TARGET_FILE_DIR:SDL3::SDL3>
            PRE_EXCLUDE_REGEXES
                "api-ms-win-.*" "ext-ms-.*"
                "libc\.so\..*" "libgcc_s\.so\..*" "libm\.so\..*" "libstdc\\+\\+\.so\..*"
				 "[Vv][Cc][Rr][Uu][Nn][Tt][Ii][Mm][Ee].*" "[Mm][Ss][Vv][Cc][Pp].*"
            POST_EXCLUDE_REGEXES
                "^\/lib.*" "^\/usr\/lib.*"
                "C:[\\\/][Ww][Ii][Nn][Dd][Oo][Ww][Ss][\\\/].*"
        ARCHIVE DESTINATION .
        LIBRARY DESTINATION .
        RUNTIME DESTINATION .
    )
