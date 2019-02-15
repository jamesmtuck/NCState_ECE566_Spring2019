
find_package(BISON)
find_package(FLEX)

BISON_TARGET(Parser minic.y ${CMAKE_CURRENT_BINARY_DIR}/minic.y.cpp)
FLEX_TARGET(Scanner minic.lex ${CMAKE_CURRENT_BINARY_DIR}/minic.lex.cpp)
FLEX_TARGET(Cmdline cmdline.lex ${CMAKE_CURRENT_BINARY_DIR}/Cmdline.cpp COMPILE_FLAGS "-PCmd")
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser Cmdline)

include_directories(.)
add_executable(p2 main.cpp minic.y.cpp minic.lex.cpp list.cpp symbol.cpp Cmdline.cpp)

# Figure out where the libraries are that we will link against
if ("${LLVM_PACKAGE_VERSION}" VERSION_GREATER "3.4.2")
  llvm_map_components_to_libnames(llvm_libs analysis bitreader bitwriter codegen core asmparser irreader instcombine instrumentation mc objcarcopts scalaropts support ipo target transformutils vectorize)
else()
  llvm_map_components_to_libraries(llvm_libs bitreader bitwriter asmparser irreader instrumentation scalaropts ipo vectorize)
endif()

target_link_libraries(p2 ${llvm_libs} l y)
