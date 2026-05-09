cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# TODO
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp_components REQUIRED)

include_directories(include)

add_executable(publisher_component src/publisher_node.cpp src/publisher_component.cpp)
ament_target_dependencies(publisher_component rclcpp geometry_msgs rclcpp_components)

add_executable(subscriber_component src/subscriber_node.cpp src/subscriber_component.cpp)
ament_target_dependencies(subscriber_component rclcpp geometry_msgs rclcpp_components)

# Install
install(TARGETS publisher_component subscriber_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
