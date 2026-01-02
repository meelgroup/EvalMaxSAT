cmake_minimum_required(VERSION 3.12)
get_filename_component(projectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${projectName})
set(CMAKE_CXX_STANDARD 20)

include_directories(${projectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE source_files src/*)

add_library(${projectName} STATIC ${source_files})
target_link_libraries(${projectName} PRIVATE MaLib cadical)

# Ensure cadical is built before we try to merge it
# add_dependencies(${projectName} cadical)

# Add custom command to merge archives after build with symbol hiding via partial linking
# add_custom_command(TARGET ${projectName} POST_BUILD
#     COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/merge_cadical.sh
#             ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp
#             ${CMAKE_BINARY_DIR}/lib/cadical/libcadical.a
#             $<TARGET_FILE:${projectName}>
#     COMMENT "Merging cadical into libEvalMaxSAT.a with symbol localization"
#     VERBATIM
# )

install(TARGETS ${projectName}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

