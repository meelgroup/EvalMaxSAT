cmake_minimum_required(VERSION 3.12)
get_filename_component(projectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${projectName})
set(CMAKE_CXX_STANDARD 20)

include_directories(${projectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE source_files src/*)

add_library(${projectName} STATIC ${source_files})
target_link_libraries(${projectName} PRIVATE MaLib)

# Ensure cadical is built before we try to merge it
add_dependencies(${projectName} cadical)

# Add custom command to merge archives after build with symbol localization
add_custom_command(TARGET ${projectName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp/cadical
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp/evalmaxsat
    # Extract cadical objects
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp/cadical && ar x ${CMAKE_BINARY_DIR}/lib/cadical/libcadical.a
    # Localize all symbols in cadical object files to prevent conflicts
    COMMAND find ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp/cadical -name "*.o" -exec objcopy --localize-hidden {} "\;"
    # Extract EvalMaxSAT objects
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp/evalmaxsat && ar x $<TARGET_FILE:${projectName}>
    # Merge everything back - use shell to expand globs
    COMMAND sh -c "ar rcs $<TARGET_FILE:${projectName}> ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp/cadical/*.o ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp/evalmaxsat/*.o"
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/merge_tmp
)

install(TARGETS ${projectName}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

