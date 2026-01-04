cmake_minimum_required(VERSION 3.12)
get_filename_component(projectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${projectName})
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE source_files src/*)

add_library(${projectName} ${source_files})
target_include_directories(${projectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${projectName} PRIVATE MaLib evalmax_cadical)

install(TARGETS ${projectName}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

