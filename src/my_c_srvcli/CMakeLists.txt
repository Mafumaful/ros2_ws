cmake_minimum_required(VERSION 3.5)
project(my_c_srvcli)

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
find_package(turtlesim REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(my_c_cli_node src/my_c_cli_node.cpp)
target_include_directories(my_c_cli_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  my_c_cli_node
  "turtlesim"
  "std_msgs"
  "rclcpp"
)

install(TARGETS my_c_cli_node
  DESTINATION lib/${PROJECT_NAME})

  add_executable(my_c_srv_node src/my_c_srv_node.cpp)
  target_include_directories(my_c_srv_node PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
  ament_target_dependencies(
    my_c_srv_node
    "turtlesim"
    "std_msgs"
    "rclcpp"
    "geometry_msgs"
    "std_srvs"
  )
  
install(TARGETS my_c_srv_node
  DESTINATION lib/${PROJECT_NAME})
  

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
