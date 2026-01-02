cmake_minimum_required(VERSION 3.12)

get_filename_component(projectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("cmake for ${projectName}")
project(${projectName})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(public_libs

)

set(private_libs

)

file(
	GLOB_RECURSE
	source_files
	src/*
)

add_library (
    ${projectName} STATIC
    ${source_files}
)

# Hide all symbols by default to prevent conflicts when merged into libEvalMaxSAT.a
target_compile_options(${projectName} PRIVATE -fvisibility=hidden)

target_include_directories(${projectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${projectName} PUBLIC ${public_libs} PRIVATE ${private_libs})
