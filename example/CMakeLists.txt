llvm_map_components_to_libnames(llvm_link_libs core support)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} sources)

foreach(file_path ${sources})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" file_name ${file_path})
    add_executable(${file_name} ${file_path})
    target_link_libraries(${file_name} ${llvm_link_libs})
endforeach(file_path ${sources})

# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TARGETS)
