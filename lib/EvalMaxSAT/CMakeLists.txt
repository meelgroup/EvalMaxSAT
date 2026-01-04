cmake_minimum_required(VERSION 3.12)
get_filename_component(projectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${projectName})
set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

file(GLOB_RECURSE source_files src/*)

add_library(${projectName} ${source_files})

# Set public headers
set_target_properties(${projectName} PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/EvalMaxSAT.h;${CMAKE_CURRENT_SOURCE_DIR}/src/cadicalinterface.h;${CMAKE_CURRENT_SOURCE_DIR}/src/cardincremental.h;${CMAKE_CURRENT_SOURCE_DIR}/src/mcqd.h;${CMAKE_CURRENT_SOURCE_DIR}/src/lazyvariable.h;${CMAKE_CURRENT_SOURCE_DIR}/src/ParseUtils.h;${CMAKE_CURRENT_SOURCE_DIR}/src/utile.h"
)

target_include_directories(${projectName} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${projectName} PRIVATE MaLib evalmax_cadical)

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
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Also install MaLib headers since they're needed by EvalMaxSAT.h
install(FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/../MaLib/src/Chrono.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../MaLib/src/coutUtil.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../MaLib/src/rand.h"
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

