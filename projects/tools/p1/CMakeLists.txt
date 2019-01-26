
find_package(BISON)
find_package(FLEX)

BISON_TARGET(Parser p1.y ${CMAKE_CURRENT_BINARY_DIR}/p1.y.c)
FLEX_TARGET(Scanner p1.lex ${CMAKE_CURRENT_BINARY_DIR}/p1.lex.c)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

include_directories(.)
add_executable(p1 main.c p1.lex.c p1.y.c)

# Figure out where the libraries are that we will link against
if ("${LLVM_PACKAGE_VERSION}" VERSION_GREATER "3.4.2")
  llvm_map_components_to_libnames(llvm_libs analysis bitreader bitwriter codegen core asmparser irreader instcombine instrumentation mc objcarcopts scalaropts support ipo target transformutils vectorize)
else()
  llvm_map_components_to_libraries(llvm_libs bitreader bitwriter asmparser irreader instrumentation scalaropts ipo vectorize)
endif()

target_link_libraries(p1 ${llvm_libs} l y)
