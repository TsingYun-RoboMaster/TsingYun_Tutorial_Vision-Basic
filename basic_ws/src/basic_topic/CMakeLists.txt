cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(geometry_msgs REQUIRED)

include_directories(include)
add_library(publisher_component SHARED src/publisher_component.cpp)
add_library(subscriber_component SHARED src/subscriber_component.cpp)

add_executable(publisher_node src/publisher_node.cpp)
add_executable(subscriber_node src/subscriber_node.cpp)

ament_target_dependencies(publisher_component
  rclcpp
  rclcpp_components
  geometry_msgs
)
ament_target_dependencies(subscriber_component
  rclcpp
  rclcpp_components
  geometry_msgs
)
ament_target_dependencies(publisher_node
  rclcpp
)
ament_target_dependencies(subscriber_node
  rclcpp
)

rclcpp_components_register_nodes(publisher_component "basic_topic::PublisherComponent")
rclcpp_components_register_nodes(subscriber_component "basic_topic::SubscriberComponent")
ament_export_dependencies(rclcpp_components)

target_link_libraries(publisher_node publisher_component)
target_link_libraries(subscriber_node subscriber_component)

# Install
install(TARGETS publisher_component subscriber_component publisher_node subscriber_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()