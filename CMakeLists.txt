cmake_minimum_required(VERSION 3.15)

set(CMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY ON)
set(CMAKE_EXPORT_PACKAGE_REGISTRY OFF)
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

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    if(NOT BUILD_SHARED_LIBS)
        message(STATUS "Compiling statically")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
        endif()
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    else()
        message(STATUS "Compiling for dynamic library use")
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
    TARGETS EvalMaxSAT evalmax_cadical MaLib
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${EVALMAXSAT_TARGETS_FILENAME}"
)

include(CheckCXXCompilerFlag)
include(CheckLinkerFlag)
macro(add_cxx_flag_if_supported flagname)
  check_cxx_compiler_flag("${flagname}" HAVE_FLAG_${flagname})

  if(HAVE_FLAG_${flagname})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flagname}")
  endif()
endmacro()
add_cxx_flag_if_supported("-fno-omit-frame-pointer")

# Build-tree config (absolute paths, for use without installation)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/EvalMaxSATConfig-buildtree.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${EVALMAXSAT_CONFIG_FILENAME}"
    @ONLY
)

# Export this package to the CMake user package registry
export(PACKAGE EvalMaxSAT)

# Installation directory for CMake files
set(DEF_INSTALL_CMAKE_DIR lib/cmake/evalmaxsat)
set(EVALMAXSAT_INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
    "Installation directory for EvalMaxSAT CMake files")

# Install-tree config and version files (relocatable via CMakePackageConfigHelpers)
set(EVALMAXSAT_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(EVALMAXSAT_VERSION_FILENAME "EvalMaxSATConfigVersion.cmake")

include(CMakePackageConfigHelpers)
configure_package_config_file(EvalMaxSATConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${EVALMAXSAT_CONFIG_FILENAME}"
    INSTALL_DESTINATION "${EVALMAXSAT_INSTALL_CMAKE_DIR}"
    PATH_VARS EVALMAXSAT_INSTALL_INCLUDEDIR
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${EVALMAXSAT_VERSION_FILENAME}"
    VERSION ${EVALMAXSAT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${EVALMAXSAT_CONFIG_FILENAME}"
    "${CMAKE_CURRENT_BINARY_DIR}/${EVALMAXSAT_VERSION_FILENAME}"
    DESTINATION "${EVALMAXSAT_INSTALL_CMAKE_DIR}"
)

# Install the export set for use with the install-tree
install(
    EXPORT ${EVALMAXSAT_EXPORT_NAME}
    DESTINATION "${EVALMAXSAT_INSTALL_CMAKE_DIR}"
)

