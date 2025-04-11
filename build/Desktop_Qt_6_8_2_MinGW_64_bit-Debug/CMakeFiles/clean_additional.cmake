# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\apppiProject_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\apppiProject_autogen.dir\\ParseCache.txt"
  "apppiProject_autogen"
  )
endif()
