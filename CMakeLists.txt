cmake_minimum_required(VERSION 3.12)

set(projectName EvalMaxSAT_bin)

message("cmake for ${projectName}")
project(${projectName})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_language( CXX )
enable_language( C )
include(GenerateExportHeader)
include(GNUInstallDirs)
option(BUILD_SHARED_LIBS "Build the shared library" ON)
option(STATICCOMPILE "Compile to static executable" OFF)
if (STATICCOMPILE)
    set(BUILD_SHARED_LIBS OFF)
endif()

if ((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    if(NOT BUILD_SHARED_LIBS)
        MESSAGE(STATUS "Compiling statically")
        if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
        endif()
        SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    else()
        MESSAGE(STATUS "Compiling for dynamic library use")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# generate JSON file of compile commands -- useful for code extension
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# -----------------------------------------------------------------------------
# Provide an export name to be used by targets that wish to export themselves.
# -----------------------------------------------------------------------------
set(EVALMAXSAT_EXPORT_NAME "EvalMaxSATTargets")

# Version information
set(EVALMAXSAT_VERSION_MAJOR 1)
set(EVALMAXSAT_VERSION_MINOR 0)
set(EVALMAXSAT_VERSION_PATCH 0)
set(EVALMAXSAT_VERSION "${EVALMAXSAT_VERSION_MAJOR}.${EVALMAXSAT_VERSION_MINOR}.${EVALMAXSAT_VERSION_PATCH}")

add_subdirectory(lib/MaLib)
add_subdirectory(lib/evalmax_cadical)
add_subdirectory(lib/EvalMaxSAT)

add_subdirectory(main)

# -----------------------------------------------------------------------------
# Export our targets so that other CMake based projects can interface with
# the build of EvalMaxSAT in the build-tree
# -----------------------------------------------------------------------------
set(EVALMAXSAT_TARGETS_FILENAME "EvalMaxSATTargets.cmake")
set(EVALMAXSAT_CONFIG_FILENAME "EvalMaxSATConfig.cmake")

# Export targets
export(
    TARGETS EvalMaxSAT evalmax_cadical
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${EVALMAXSAT_TARGETS_FILENAME}"
)

# Create EvalMaxSATConfig file for build tree
set(EXPORT_TYPE "Build-tree")
set(CONF_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/include")
configure_file(EvalMaxSATConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${EVALMAXSAT_CONFIG_FILENAME}" @ONLY
)

# Export this package to the CMake user package registry
export(PACKAGE EvalMaxSAT)

# Installation directory for CMake files
set(DEF_INSTALL_CMAKE_DIR lib/cmake/evalmaxsat)
set(EVALMAXSAT_INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
    "Installation directory for EvalMaxSAT CMake files")

# Create EvalMaxSATConfig file for install tree
set(EXPORT_TYPE "installed")
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
configure_file(EvalMaxSATConfig.cmake.in
   "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${EVALMAXSAT_CONFIG_FILENAME}" @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${EVALMAXSAT_CONFIG_FILENAME}"
    DESTINATION "${EVALMAXSAT_INSTALL_CMAKE_DIR}"
)

# Install the export set for use with the install-tree
install(
    EXPORT ${EVALMAXSAT_EXPORT_NAME}
    DESTINATION "${EVALMAXSAT_INSTALL_CMAKE_DIR}"
)

