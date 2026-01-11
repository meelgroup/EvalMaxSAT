cmake_minimum_required(VERSION 3.12)

get_filename_component(projectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("cmake for ${projectName}")
project(${projectName})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
    ${projectName}
    ${source_files}
)

target_include_directories(${projectName} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/evalmax_cadical>
)

target_link_libraries(${projectName} PUBLIC ${public_libs} PRIVATE ${private_libs})

# Set version properties
set_target_properties(${projectName} PROPERTIES
    VERSION ${EVALMAXSAT_VERSION}
    SOVERSION ${EVALMAXSAT_VERSION_MAJOR}.${EVALMAXSAT_VERSION_MINOR}
)

install(TARGETS ${projectName}
	EXPORT ${EVALMAXSAT_EXPORT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install public API headers
install(FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/cadical.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ccadical.h"
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/evalmax_cadical
)
