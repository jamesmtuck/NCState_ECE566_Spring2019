
if (use_c)
  # add additional files to end of this list
  add_library(transform STATIC CSE_C.c transform.cpp)
elseif (use_c++)
  # add additional files to end of this list
  add_library(transform STATIC CSE_Cpp.cpp transform.cpp)
else()
  message("Error configuring library. Either C or C++ must be selected.")
endif()


