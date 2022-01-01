
add_executable(llvm_example llvm_example.cpp)
target_link_libraries(llvm_example LLVMSupport)

add_executable(array_initializer array_initializer.cpp)
target_link_libraries(array_initializer LLVMSupport)

# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TARGETS)
