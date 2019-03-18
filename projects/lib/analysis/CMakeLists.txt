
if (use_c)
  # add additional files to end of this list
  add_library(analysis STATIC summary.c dominance.cpp)
elseif (use_c++)
  # add additional files to end of this list
  add_library(analysis STATIC summary.cpp dominance.cpp)
else()
  message("Error configuring library. Either C or C++ must be selected.")
endif()


