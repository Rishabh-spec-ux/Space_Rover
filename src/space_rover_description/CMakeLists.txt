cmake_minimum_required(VERSION 3.8)
project(space_rover_description)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

add_executable(wheel_encoder_odometry src/wheel_encoder_odometry.cpp)
ament_target_dependencies(
  wheel_encoder_odometry
  rclcpp
  sensor_msgs
  nav_msgs
  geometry_msgs
  tf2_ros
)

add_executable(ackerman_6_wheels src/ackerman_6_wheels.cpp)
ament_target_dependencies(
  ackerman_6_wheels
  rclcpp
  std_msgs
  geometry_msgs
)

add_executable(arm_and_mast src/arm_and_mast.cpp)
ament_target_dependencies(
  arm_and_mast
  rclcpp
  std_msgs
  std_srvs
)

# Install launch files, URDF/XACRO, meshes, and worlds
install(
  DIRECTORY launch urdf meshes worlds rviz models config scripts
  DESTINATION share/${PROJECT_NAME}
)

install(
  TARGETS
    wheel_encoder_odometry
    ackerman_6_wheels
    arm_and_mast
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
