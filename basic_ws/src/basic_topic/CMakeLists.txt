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

add_library(publisher_component SHARED
  src/publisher_component.cpp)

target_include_directories(publisher_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_target_dependencies(publisher_component
  rclcpp
  rclcpp_components
  geometry_msgs)

rclcpp_components_register_node(publisher_component
  PLUGIN "basic_topic::PublisherComponent"
  EXECUTABLE publisher_component_node)

add_library(subscriber_component SHARED
  src/subscriber_component.cpp)

target_include_directories(subscriber_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_target_dependencies(subscriber_component
  rclcpp
  rclcpp_components
  geometry_msgs)

rclcpp_components_register_node(subscriber_component
  PLUGIN "basic_topic::SubscriberComponent"
  EXECUTABLE subscriber_component_node)

# Install
install(TARGETS publisher_component subscriber_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
