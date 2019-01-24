cmake_minimum_required(VERSION 2.8)

find_package(LLVM REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS} ./include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-rtti -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99")

# initialize variables to FALSE
set(use_c FALSE)
set(use_c++ FALSE)

# pick C or C++ source code
if(${USE_CPP})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_CPP=1")		
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_CPP=1")	
  set(use_c++ TRUE)
  message("-- Configured to use C++ source files.")
elseif (${USE_C})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_C=1")		
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_C=1")	
  message("-- Configured to use C source files.")
  set(use_c TRUE)
else()
  set(use_c TRUE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_C=1")		
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_C=1")	
  message("-- Default configuration using C source files.")
  message("-- To select C++ instead, add -DUSE_CPP=TRUE to cmake command.")
endif()

add_subdirectory(lib)
add_subdirectory(tools)
