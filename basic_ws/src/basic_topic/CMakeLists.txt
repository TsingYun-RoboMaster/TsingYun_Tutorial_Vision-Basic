cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
# ament_cmake 是 ROS2 C++ 包的基础构建系统
find_package(ament_cmake REQUIRED)
# geometry_msgs 提供作业要求使用的 geometry_msgs/msg/Quaternion
find_package(geometry_msgs REQUIRED)
# rclcpp 提供 Node、Publisher、Subscription、Timer 等 C++ ROS2 API。
find_package(rclcpp REQUIRED)
# rclcpp_components 配合源码末尾的 RCLCPP_COMPONENTS_REGISTER_NODE 宏使用。
find_package(rclcpp_components REQUIRED)

# publisher_component 是可通过 ros2 run 启动的发布端可执行文件。
add_executable(publisher_component src/publisher_component.cpp)
# 编译时从源码 include/ 找头文件，安装后从 install/include 找头文件。
target_include_directories(publisher_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
# 明确使用 C++17，和 ROS2 Humble 常用编译标准保持一致。
target_compile_features(publisher_component PUBLIC c_std_99 cxx_std_17)
# 把发布端源码里用到的 ROS2 包链接进可执行文件。
ament_target_dependencies(publisher_component
  geometry_msgs
  rclcpp
  rclcpp_components)

# subscriber_component 是可通过 ros2 run 启动的订阅端可执行文件。
add_executable(subscriber_component src/subscriber_component.cpp)
# 订阅端同样需要访问 include/basic_topic 下的头文件。
target_include_directories(subscriber_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
# 订阅端也按 C++17 编译。
target_compile_features(subscriber_component PUBLIC c_std_99 cxx_std_17)
# 把订阅端源码里用到的 ROS2 包链接进可执行文件。
ament_target_dependencies(subscriber_component
  geometry_msgs
  rclcpp
  rclcpp_components)

# Install
# ROS2 的 ros2 run 会在 lib/${PROJECT_NAME} 下查找包内可执行文件。
install(TARGETS publisher_component subscriber_component
  DESTINATION lib/${PROJECT_NAME})

# 安装头文件，方便其他 ROS2 包以后复用这些类。
install(DIRECTORY include/
  DESTINATION include)

# 安装 launch 文件，之后才能执行 ros2 launch basic_topic basic_topic.launch.py。
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

# 生成 ament 包元信息，让 colcon/ros2 能发现这个包。
ament_package()
