cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# 定义发布者可执行程序
add_executable(basic_topic_publisher src/publisher_component.cpp)
ament_target_dependencies(basic_topic_publisher rclcpp std_msgs)

# 定义订阅者可执行程序
add_executable(basic_topic_subscriber src/subscriber_component.cpp)
ament_target_dependencies(basic_topic_subscriber rclcpp std_msgs)

install(TARGETS
  basic_topic_publisher
  basic_topic_subscriber
  DESTINATION lib/${PROJECT_NAME})

ament_package()
