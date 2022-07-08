cmake_minimum_required(VERSION 3.5)
project(my_component)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)

add_library(my_component SHARED
  src/my_component_talker.cpp)
target_include_directories(my_component PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_definitions(my_component 
  PRIVATE "COMPOSITION_BUILDING_DLL")
ament_target_dependencies(my_component
  "rclcpp"
  "rclcpp_components"
  "std_msgs"
  "geometry_msgs"
)
rclcpp_components_register_nodes(my_component PLUGIN "my_composition::my_c_publisher_node" EXECUTABLE my_component_talker_node)
install(TARGETS
my_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
