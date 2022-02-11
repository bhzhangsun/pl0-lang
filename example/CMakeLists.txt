llvm_map_components_to_libnames(llvm_link_libs core support)

add_executable(llvm_example llvm_example.cpp)
target_link_libraries(llvm_example ${llvm_link_libs} )

add_executable(array_initializer array_initializer.cpp)
target_link_libraries(array_initializer ${llvm_link_libs})

add_executable(kaleido kaleido.cpp)
target_link_libraries(kaleido ${llvm_link_libs})

# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TARGETS)
