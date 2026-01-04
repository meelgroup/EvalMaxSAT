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

# -----------------------------------------------------------------------------
# Copy public headers into build directory include directory.
# The EvalMaxSATConfig.cmake we generate in the build directory depends on this.
# -----------------------------------------------------------------------------
set(HEADER_DEST "${CMAKE_BINARY_DIR}/include")
add_custom_target(CopyPublicHeaders ALL)

# Copy EvalMaxSAT headers
get_target_property(evalmaxsat_public_headers ${projectName} PUBLIC_HEADER)
foreach(public_header ${evalmaxsat_public_headers})
    get_filename_component(HEADER_NAME ${public_header} NAME)
    add_custom_command(TARGET CopyPublicHeaders PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory "${HEADER_DEST}"
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                           ${public_header}
                           "${HEADER_DEST}"
                      )
endforeach()

# Also copy MaLib headers (needed by EvalMaxSAT.h)
foreach(malib_header Chrono.h coutUtil.h rand.h)
    add_custom_command(TARGET CopyPublicHeaders PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                           "${CMAKE_CURRENT_SOURCE_DIR}/../MaLib/src/${malib_header}"
                           "${HEADER_DEST}"
                      )
endforeach()

# Copy evalmax_cadical public API headers
foreach(cadical_header cadical.hpp ccadical.h)
    add_custom_command(TARGET CopyPublicHeaders PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory "${HEADER_DEST}/evalmax_cadical"
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                           "${CMAKE_CURRENT_SOURCE_DIR}/../evalmax_cadical/src/${cadical_header}"
                           "${HEADER_DEST}/evalmax_cadical"
                      )
endforeach()

