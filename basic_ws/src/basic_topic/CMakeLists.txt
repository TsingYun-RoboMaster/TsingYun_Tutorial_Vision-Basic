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

add_library(publisher_component_lib SHARED src/publisher_component.cpp)
target_include_directories(publisher_component_lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
ament_target_dependencies(publisher_component_lib rclcpp rclcpp_components geometry_msgs)
rclcpp_components_register_node(publisher_component_lib PLUGIN "basic_topic::PublisherComponent" EXECUTABLE publisher_component)

add_library(subscriber_component_lib SHARED src/subscriber_component.cpp)
target_include_directories(subscriber_component_lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
ament_target_dependencies(subscriber_component_lib rclcpp rclcpp_components geometry_msgs)
rclcpp_components_register_node(subscriber_component_lib PLUGIN "basic_topic::SubscriberComponent" EXECUTABLE subscriber_component)

# Install
install(TARGETS publisher_component subscriber_component
  publisher_component_lib subscriber_component_lib  # i changed here
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
