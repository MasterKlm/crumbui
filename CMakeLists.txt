cmake_minimum_required(VERSION 3.10)

# Trim unneeded FreeType dependencies so it builds standalone without hunting for zlib/png/harfbuzz
set(FT_DISABLE_ZLIB TRUE CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 TRUE CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG TRUE CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ TRUE CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI TRUE CACHE BOOL "" FORCE)

add_subdirectory(third_party/freetype)
add_subdirectory(third_party/cglm EXCLUDE_FROM_ALL)



set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")

project(crumbui)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(crumbui ${SOURCES})


target_include_directories(crumbui PRIVATE
    include/

)

target_link_libraries(crumbui PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    freetype
    cglm_headers
    
    opengl32   # Windows
    gdi32      # Windows
    user32     # Windows
)

# Precompiled header
target_precompile_headers(crumbui PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
)