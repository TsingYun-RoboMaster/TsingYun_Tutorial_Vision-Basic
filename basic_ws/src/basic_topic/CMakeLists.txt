cmake_minimum_required(VERSION 3.8)
project(basic_topic)

find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(geometry_msgs REQUIRED)

include_directories(include)

add_executable(publisher_component
  src/publisher_component.cpp
)

ament_target_dependencies(publisher_component
  rclcpp
  rclcpp_components
  geometry_msgs
)

add_executable(subscriber_component
  src/subscriber_component.cpp
)

ament_target_dependencies(subscriber_component
  rclcpp
  rclcpp_components
  geometry_msgs
)

install(TARGETS
  publisher_component
  subscriber_component
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()