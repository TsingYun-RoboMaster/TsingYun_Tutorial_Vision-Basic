cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# TODO
# 1. 寻找依赖包
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp_components REQUIRED)

# 2. 编译发布者组件 (Publisher)
add_library(publisher_component SHARED src/publisher_component.cpp)
target_include_directories(publisher_component PRIVATE include)
ament_target_dependencies(publisher_component rclcpp geometry_msgs rclcpp_components)
rclcpp_components_register_node(publisher_component
  PLUGIN "basic_topic::PublisherComponent"
  EXECUTABLE publisher_node
)

# 3. 编译订阅者组件 (Subscriber)
add_library(subscriber_component SHARED src/subscriber_component.cpp)
target_include_directories(subscriber_component PRIVATE include)
ament_target_dependencies(subscriber_component rclcpp geometry_msgs rclcpp_components)
# 自动生成可执行文件 "subscriber_node"
rclcpp_components_register_node(subscriber_component
  PLUGIN "basic_topic::SubscriberComponent"
  EXECUTABLE subscriber_node
)

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
