# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_my_c_topic_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED my_c_topic_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(my_c_topic_FOUND FALSE)
  elseif(NOT my_c_topic_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(my_c_topic_FOUND FALSE)
  endif()
  return()
endif()
set(_my_c_topic_CONFIG_INCLUDED TRUE)

# output package information
if(NOT my_c_topic_FIND_QUIETLY)
  message(STATUS "Found my_c_topic: 0.0.0 (${my_c_topic_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'my_c_topic' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  message(WARNING "${_msg}")
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(my_c_topic_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${my_c_topic_DIR}/${_extra}")
endforeach()
