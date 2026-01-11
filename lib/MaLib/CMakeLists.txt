cmake_minimum_required(VERSION 3.12)

get_filename_component(projectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("cmake for ${projectName}")
project(${projectName})

set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)
include(CheckCXXCompilerFlag)
include(CheckLinkerFlag)
macro(add_cxx_flag_if_supported flagname)
  check_cxx_compiler_flag("${flagname}" HAVE_FLAG_${flagname})
  if(HAVE_FLAG_${flagname})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flagname}" )
  endif()
endmacro()
add_cxx_flag_if_supported("-fno-omit-frame-pointer")
add_cxx_flag_if_supported("-ggdb3")

add_library (
    ${projectName}
    INTERFACE
)

target_include_directories(${projectName} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${projectName})

# Install the interface library
install(TARGETS ${projectName}
    EXPORT ${EVALMAXSAT_EXPORT_NAME}
)

# Note: Headers are installed by lib/EvalMaxSAT/CMakeLists.txt










