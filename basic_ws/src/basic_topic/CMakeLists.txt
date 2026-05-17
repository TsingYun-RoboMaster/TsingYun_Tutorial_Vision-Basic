cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp_components REQUIRED)

# Publisher component
add_library(publisher_component SHARED src/publisher_component.cpp)
ament_target_dependencies(publisher_component rclcpp geometry_msgs rclcpp_components)
rclcpp_components_register_nodes(publisher_component "basic_topic::PublisherComponent")
target_include_directories(publisher_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# Subscriber component
add_library(subscriber_component SHARED src/subscriber_component.cpp)
ament_target_dependencies(subscriber_component rclcpp geometry_msgs rclcpp_components)
rclcpp_components_register_nodes(subscriber_component "basic_topic::SubscriberComponent")
target_include_directories(subscriber_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# Publisher standalone executable（部署模式使用）
add_executable(publisher_node src/publisher_main.cpp)
target_link_libraries(publisher_node publisher_component)
ament_target_dependencies(publisher_node rclcpp geometry_msgs rclcpp_components)
target_include_directories(publisher_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# Subscriber standalone executable（部署模式使用）
add_executable(subscriber_node src/subscriber_main.cpp)
target_link_libraries(subscriber_node subscriber_component)
ament_target_dependencies(subscriber_node rclcpp geometry_msgs rclcpp_components)
target_include_directories(subscriber_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# Install
install(TARGETS
  publisher_component subscriber_component
  publisher_node subscriber_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
