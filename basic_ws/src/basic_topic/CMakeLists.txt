cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# TODO
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(geometry_msgs REQUIRED)

include_directories(include)

# Publisher Component
add_library(publisher_component SHARED
  src/publisher_component.cpp
)

add_executable(publisher_node
  src/publisher_main.cpp
)

ament_target_dependencies(
  publisher_node
  rclcpp
  rclcpp_components
  geometry_msgs
)

target_link_libraries(
  publisher_node
  publisher_component
)

target_compile_features(publisher_component PUBLIC c_std_99 cxx_std_17)

ament_target_dependencies(
  publisher_component
  rclcpp
  rclcpp_components
  geometry_msgs
)

rclcpp_components_register_nodes(
  publisher_component
  "basic_topic::PublisherComponent"
)

# Subscriber Component
add_library(subscriber_component SHARED
  src/subscriber_component.cpp
)

add_executable(subscriber_node
  src/subscriber_main.cpp
)

ament_target_dependencies(
  subscriber_node
  rclcpp
  rclcpp_components
  geometry_msgs
)

target_link_libraries(
  subscriber_node
  subscriber_component
)

target_compile_features(subscriber_component PUBLIC c_std_99 cxx_std_17)

ament_target_dependencies(
  subscriber_component
  rclcpp
  rclcpp_components
  geometry_msgs
)

rclcpp_components_register_nodes(
  subscriber_component
  "basic_topic::SubscriberComponent"
)

# Install
install(TARGETS
  publisher_component
  subscriber_component
  publisher_node
  subscriber_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
