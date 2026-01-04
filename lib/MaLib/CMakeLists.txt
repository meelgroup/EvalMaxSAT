cmake_minimum_required(VERSION 3.12)

get_filename_component(projectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("cmake for ${projectName}")
project(${projectName})

set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

add_library (
    ${projectName}
    INTERFACE
)

target_include_directories(${projectName} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${projectName})

# Install only the headers needed by EvalMaxSAT.h
# Install to common include directory since EvalMaxSAT headers expect them in same path
install(FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Chrono.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/coutUtil.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/rand.h"
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)










