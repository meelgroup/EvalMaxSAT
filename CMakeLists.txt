cmake_minimum_required(VERSION 3.12)

set(projectName EvalMaxSAT_bin)

message("cmake for ${projectName}")
project(${projectName})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_language( CXX )
enable_language( C )
option(BUILD_SHARED_LIBS "Build the shared library" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# generate JSON file of compile commands -- useful for code extension
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(lib/MaLib)
add_subdirectory(lib/cadical)
add_subdirectory(lib/EvalMaxSAT)

add_subdirectory(main)

