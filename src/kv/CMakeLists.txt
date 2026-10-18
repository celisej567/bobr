cmake_policy(SET CMP0207 NEW)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(SOURCES
    keyvalue.cpp
    kv_parser.cpp
    kv_string_pool.cpp
    kv_writer.cpp
)

set(CMAKE_CXX_STANDARD 23)

find_package(Threads REQUIRED)

add_library(kv STATIC)

# can be linked into shared modules
set_target_properties(kv PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_sources(kv PRIVATE ${SOURCES})

# CMD::Msg lives there
target_link_libraries(kv PRIVATE manyinterfaces)
target_link_libraries(kv PUBLIC Threads::Threads)

# the string pool is guarded by a mutex
target_compile_definitions(kv PUBLIC KV_THREAD_SAFE)

# users write #include "keyvalue.h"; src/public is only for libkv itself (cmd/cmd.h, mytypes.h)
target_include_directories(kv PUBLIC ${PROJECT_SOURCE_DIR}/src/public/kv)
target_include_directories(kv PRIVATE ${PROJECT_SOURCE_DIR}/src/public)
